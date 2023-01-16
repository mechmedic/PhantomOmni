#include "hapticdevice.h"

//--------------------------------------------------------------
// CKim - Global variable. Device status
static DeviceData gServoDeviceData[2];

HDCallbackCode HDCALLBACK updateDeviceCallback(void *pUserData)
{
    // CKim - Get the pointers of the passed handle list
    HHD* pHandle = (HHD*)pUserData;

    for (int i = 0; i < OMNI_NUM; i++)
    {
        int nButtons = 0;

        hdBeginFrame(pHandle[i]);

        /* Retrieve the current button(s). */
        hdGetIntegerv(HD_CURRENT_BUTTONS, &gServoDeviceData[i].m_buttonState);

        /* Get the current location of the device (HD_GET_CURRENT_POSITION)
        We declare a vector of three doubles since hdGetDoublev returns
        the information in a vector of size 3. */
        hdGetDoublev(HD_CURRENT_POSITION, gServoDeviceData[i].m_devicePosition);

        // CKim - Get the gimbal angles
        hdGetDoublev(HD_CURRENT_GIMBAL_ANGLES, gServoDeviceData[i].m_gimbalAngle);

        // CKim - Get the transformation
        hdGetDoublev(HD_CURRENT_TRANSFORM, gServoDeviceData[i].m_Transform);

        /* Also check the error state of HDAPI. */
        gServoDeviceData[i].m_error = hdGetError();

        // ------------------------------------------------------------------
        /* Copy the position into our device_data tructure. */
        hdEndFrame(pHandle[i]);
    }

    return HD_CALLBACK_CONTINUE;	// CKim - repeat updating
}


HDCallbackCode HDCALLBACK copyDeviceDataCallback(void *pUserData)
{
    DeviceData *pDeviceData = (DeviceData *)pUserData;

    for (int i = 0; i < OMNI_NUM; i++)
    {
        memcpy(&pDeviceData[i], &gServoDeviceData[i], sizeof(DeviceData));
    }

    return HD_CALLBACK_DONE;
}


HapticDevice::HapticDevice()
{
    m_DevName[0] = "L_Arm";
    m_DevName[1] = "R_Arm";
    m_loopFlag = true;
    m_ntwkFlag = false;
}


bool HapticDevice::InitHapticDevice()
{
    // CKim - Initialize the device, must be done before attempting
    // to call any hd functions. We will be initializing two devices

    HDErrorInfo error;
    for(int i=0; i<OMNI_NUM; i++)
    {
        std::ostringstream ostr;
        m_devHandle[i] = hdInitDevice(m_DevName[i].c_str());
        if (HD_DEVICE_ERROR(error = hdGetError()))
        {
            std::cerr << m_DevName[i] << " : ";
            hduPrintError(stderr, &error, "Failed to initialize haptic device");
            ostr << std::cerr.rdbuf();
            m_errMsg = QString::fromUtf8(ostr.str().c_str());
            return false;
        }
        ostr << "Found device model: "<< hdGetString(HD_DEVICE_MODEL_TYPE) << " / serial number: "<< hdGetString(HD_DEVICE_SERIAL_NUMBER) ;
        m_errMsg = QString::fromUtf8(ostr.str().c_str());
    }
    emit DeviceMessage(m_errMsg);
    return true;
}


bool HapticDevice::StartHapticDevice()
{
    // CKim - Should check if initialized.....
    m_loopFlag = true;

    // CKim - Schedule the main scheduler callback that repetedly updates the device state.
    HDErrorInfo error;
    std::ostringstream ostr;
    m_hUpdateHandle = hdScheduleAsynchronous(
        updateDeviceCallback, m_devHandle, HD_MAX_SCHEDULER_PRIORITY);

    // CKim - Start the servo loop scheduler.
    hdStartScheduler();
    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, "Failed to start the scheduler");
        ostr << std::cerr.rdbuf();
        m_errMsg = QString::fromUtf8(ostr.str().c_str());
        return false;
    }

    // CKim - Perform a synchronous call to copy the most current device state.
    hdScheduleSynchronous(copyDeviceDataCallback,
        m_currentData, HD_MIN_SCHEDULER_PRIORITY);

    memcpy(&m_prevData, &m_currentData, OMNI_NUM * sizeof(DeviceData));

    ostr << "Started Haptic Device";
    m_errMsg = QString::fromUtf8(ostr.str().c_str());
    emit DeviceMessage(m_errMsg);
    return true;
}


bool HapticDevice::StopHapticDevice()
{
    // CKim - Should check if running.....
    m_loopFlag = false;
    this->wait();

    // CKim - For cleanup, unschedule callbacks and stop the servo loop.
    HDErrorInfo error;
    std::ostringstream ostr;
    hdStopScheduler();
    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, "Failed to stop the scheduler");
        ostr << std::cerr.rdbuf();
        m_errMsg = QString::fromUtf8(ostr.str().c_str());
        return false;
    }
    hdUnschedule(m_hUpdateHandle);
    ostr << "Stopped Haptic Device";
    m_errMsg = QString::fromUtf8(ostr.str().c_str());
    emit DeviceMessage(m_errMsg);
    return true;
}


void HapticDevice::CloseHapticDevice()
{
    for (int i = 0; i < OMNI_NUM; i++)    {
        hdDisableDevice(m_devHandle[i]);
    }
}


void HapticDevice::InitNetwork(int portNum)
{
    // CKim - Code here is based on Qt's Fortune Server and client example
    // https://doc.qt.io/qt-5/qtnetwork-fortuneserver-example.html
    // https://doc.qt.io/qt-5/qtnetwork-blockingfortuneclient-example.html
    // and https://coding-chobo.tistory.com/42
    m_pServer = new QTcpServer(this);

    // CKim - Connect callback that will be executed with the new connection
    connect(m_pServer, SIGNAL(newConnection()), this, SLOT(OnConnect()));

    // CKim - Try to listen for incoming connections
    if (!m_pServer->listen(QHostAddress::Any, portNum))
    {
        m_ntwkMsg = QString("Unable to start the server: %1.").arg(m_pServer->errorString());
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }

    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    //statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n" "Run the Fortune Client example now.")         .arg(ipAddress).arg(tcpServer->serverPort()));

    m_ntwkMsg = QString("The server is running on IP: %1 port: %2").arg(ipAddress).arg(m_pServer->serverPort());
    emit NetworkMessage(m_ntwkMsg);


//    // CKim - Socket variables
//    WSADATA wsaData;
//    SOCKET ServSock, sock;
//    SOCKADDR_IN servAddr, clntAddr;

//    // CKim - Winsocket Setup
//    if (argc != 2){
//        printf("Usage : %s <port>\n", argv[0]);
//        exit(1);
//    }

//    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//        ErrorHandling("WSAStartup() error!");

//    ServSock = socket(PF_INET, SOCK_STREAM, 0);
//    if (ServSock == INVALID_SOCKET)
//        ErrorHandling("socket() error");

//    sock = socket(PF_INET, SOCK_STREAM, 0);
//    if (sock == INVALID_SOCKET)
//        ErrorHandling("socket() error");

//    BOOL bOptVal = TRUE;
//    int bOptLen = sizeof(BOOL);

//    memset(&servAddr, 0, sizeof(servAddr));
//    servAddr.sin_family = AF_INET;
//    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
//    //inet_addr(argv[1]);
//    servAddr.sin_port = htons(atoi(argv[1]));

//    if (bind(ServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
//        ErrorHandling("bind() error!");

//    printf("Listening for connection\n");

//    if (listen(ServSock, 1) == SOCKET_ERROR)
//        ErrorHandling("listen() error!");

//    int szclntAddr = sizeof(clntAddr);
//    sock = accept(ServSock, (SOCKADDR*)&clntAddr, &szclntAddr);
//    if (sock == INVALID_SOCKET)
//        ErrorHandling("accept() error!");

//    printf("Server accepted connection\n");
}


void HapticDevice::run()
{
    // -----------------------------------------------------------
    // CKim - Prepare the loop
    char btnL = 0;              char btnR = 0;
    double deltaL[6] = { 0 };	double deltaR[6] = { 0 };
    double TfMatL[16];          double TfMatR[16];
    double prevOrt[3][3];		double currOrt[3][3];		double deltaOrt[3][3];
    int cnt = 0;

    // -----------------------------------------------------------
    // CKim - run the main loop
    while (m_loopFlag)
    {
        // CKim - Perform a synchronous call to copy the most current device state.
        // This synchronous scheduler call ensures that the device state is obtained in a thread-safe manner.
        hdScheduleSynchronous(copyDeviceDataCallback,
            m_currentData, HD_MIN_SCHEDULER_PRIORITY);

        // CKim - Check if an error occurred.
        for (int i = 0; i < OMNI_NUM; i++)
        {
            if (HD_DEVICE_ERROR(m_currentData[i].m_error))
            {
                std::ostringstream ostr;
                std::cerr << m_DevName[i] << " : ";
                hduPrintError(stderr, &m_currentData[i].m_error, "Device error detected");
                ostr << std::cerr.rdbuf();
                m_errMsg = QString::fromUtf8(ostr.str().c_str());
                emit DeviceMessage(m_errMsg);
                break;
            }

            if (hduIsSchedulerError(&m_currentData[i].m_error))
            {
                std::ostringstream ostr;
                std::cerr << m_DevName[i] << " : ";
                hduPrintError(stderr, &m_currentData[i].m_error, "Scheduler error detected");
                ostr << std::cerr.rdbuf();
                m_errMsg = QString::fromUtf8(ostr.str().c_str());
                emit DeviceMessage(m_errMsg);
                break;
            }
        }

        // CKim - Process Data

        // CKim - Send data over network
        if(m_ntwkFlag)
        {
            m_pSocket->write((char*)deltaL, 6 * sizeof(double));

            //            // CKim - Send data over network
//            send(sock, (char*)deltaL, 6 * sizeof(double), 0);
//            send(sock, &btnL, sizeof(char), 0);
//            send(sock, (char*)deltaR, 6 * sizeof(double), 0);
//            send(sock, &btnR, sizeof(char), 0);



//            //! [5]
//                QByteArray block;
//                QDataStream out(&block, QIODevice::WriteOnly);
//                out.setVersion(QDataStream::Qt_5_10);

//                out << fortunes[QRandomGenerator::global()->bounded(fortunes.size())];

//                clientConnection->write(block);
//                clientConnection->disconnectFromHost();
//            //! [5]



        }

        // CKim - Store off the current data for the next loop.
        memcpy(&m_prevData, &m_currentData, OMNI_NUM * sizeof(DeviceData));
    }

    return;
}


void HapticDevice::OnConnect()
{
    // We then call QTcpServer::nextPendingConnection(), which returns the QTcpSocket representing the server side of the connection.
    // By connecting QTcpSocket::disconnected() to QObject::deleteLater(), we ensure that the socket will be deleted after disconnecting.
    m_pSocket = m_pServer->nextPendingConnection();
    connect(m_pSocket, &QAbstractSocket::disconnected, m_pSocket, &QObject::deleteLater);

    std::ostringstream ostr;
    ostr << "Server accepted connection from ";
    m_ntwkMsg = QString::fromUtf8(ostr.str().c_str());
    emit NetworkMessage(m_ntwkMsg);
    m_ntwkFlag = true;
}


void HapticDevice::StopNetwork()
{
    if(m_pSocket)
    {
        m_pSocket->disconnectFromHost();
    }
}


//    btnL = btnR = 0;
//    isPedalDown = 1;

//    if (isPedalDown)
//    {
//        // CKim - Calculate position (mm) difference.
//        for (int i = 0; i < 3; i++)	{
//            deltaL[i] = currentData[0].m_devicePosition[i] - prevData[0].m_devicePosition[i];
//            deltaR[i] = currentData[1].m_devicePosition[i] - prevData[1].m_devicePosition[i];
//        }

//        // CKim - Just send current gimbal angles in radian.
//        for (int i = 0; i < 3; i++) {
//            deltaL[i + 3] = currentData[0].m_gimbalAngle[i] - prevData[0].m_gimbalAngle[i];
//            deltaR[i + 3] = currentData[1].m_gimbalAngle[i] - prevData[1].m_gimbalAngle[i];
//        }

//        //// CKim - Copy matrix. Stored in column major, that is [a,b; c,d] is stored as [a,c,b,d]
//        //for (int i = 0; i < 16; i++)
//        //{
//        //	TfMatL[i] = currentData[0].m_Transform[i];
//        //	TfMatR[i] = currentData[1].m_Transform[i];
//        //}

//        //// CKim - Copy previous and current orientation
//        //for (int i = 0; i < 3; i++)	{
//        //	for (int j = 0; j < 3; j++)	{
//        //		prevOrt[j][i] = prevData[0].m_Transform[4 * i + j];
//        //		currOrt[j][i] = currentData[0].m_Transform[4 * i + j];
//        //	}
//        //}

//        //// CKim - Compensate for the Z rotation
//        //double dz = -prevData[0].m_gimbalAngle[2];
//        //double rotz[3][3];		double tmpMat[3][3];
//        //rotz[0][0] = cos(-dz);		rotz[0][1] = -sin(-dz);		rotz[0][2] = 0;
//        //rotz[1][0] = sin(-dz);		rotz[1][1] = cos(-dz);		rotz[1][2] = 0;
//        //rotz[2][0] = 0.0;			rotz[2][1] = 0.0;			rotz[2][2] = 1.0;
//        //for (int i = 0; i < 3; i++) {
//        //	for (int j = 0; j < 3; j++)	{
//        //		tmpMat[i][j] = 0;
//        //		for (int k = 0; k < 3; k++)	{
//        //			tmpMat[i][j] += (prevOrt[i][k] * rotz[k][j]);
//        //		}
//        //	}
//        //}
//        //for (int i = 0; i < 3; i++) {
//        //	for (int j = 0; j < 3; j++)	{
//        //		prevOrt[i][j] = tmpMat[i][j];
//        //	}
//        //}

//        //dz = -currentData[0].m_gimbalAngle[2];
//        //rotz[0][0] = cos(-dz);		rotz[0][1] = -sin(-dz);		rotz[0][2] = 0;
//        //rotz[1][0] = sin(-dz);		rotz[1][1] = cos(-dz);		rotz[1][2] = 0;
//        //rotz[2][0] = 0.0;			rotz[2][1] = 0.0;			rotz[2][2] = 1.0;
//        //for (int i = 0; i < 3; i++) {
//        //	for (int j = 0; j < 3; j++)	{
//        //		tmpMat[i][j] = 0;
//        //		for (int k = 0; k < 3; k++)	{
//        //			tmpMat[i][j] += (currOrt[i][k] * rotz[k][j]);
//        //		}
//        //	}
//        //}
//        //for (int i = 0; i < 3; i++) {
//        //	for (int j = 0; j < 3; j++)	{
//        //		currOrt[i][j] = tmpMat[i][j];
//        //	}
//        //}


//        //// CKim - Calculate prevOrt.transpose * currOrt;
//        //for (int i = 0; i < 3; i++){
//        //	for (int j = 0; j < 3; j++){
//        //		deltaOrt[i][j] = 0;
//        //		for (int k = 0; k < 3; k++) {
//        //			deltaOrt[i][j] += (prevOrt[k][i] * currOrt[k][j]);
//        //		}
//        //	}
//        //}

//        //// CKim - Convert deltaOrt into axis angle
//        //double tmp = ((deltaOrt[0][0] + deltaOrt[1][1] + deltaOrt[2][2]) - 1.0) / 2.0;
//        //if (fabs(tmp) >= 1.0)
//        //{
//        //	deltaL[3] = deltaL[4] = deltaL[5] = 0;
//        //}
//        //else
//        //{
//        //	double theta = acos(tmp);
//        //	deltaL[3] = (deltaOrt[2][1] - deltaOrt[1][2]) / (2 * sin(theta)) * theta;
//        //	deltaL[4] = (deltaOrt[0][2] - deltaOrt[2][0]) / (2 * sin(theta)) * theta;
//        //	deltaL[5] = (deltaOrt[1][0] - deltaOrt[0][1]) / (2 * sin(theta)) * theta;
//        //}

//        //printf("Pos : %.3f %.3f %.3f\n", currentData[0].m_devicePosition[0], currentData[0].m_devicePosition[1], currentData[0].m_devicePosition[2]);
//        //printf("Ort : %.3f %.3f %.3f\n", currentData[0].m_gimbalAngle[0], currentData[0].m_gimbalAngle[1], currentData[0].m_gimbalAngle[2]);

//        // CKim - Encode Button State
//        btnL |= (1 << 3);
//        btnR |= (1 << 3);

//        if (currentData[0].m_buttonState & HD_DEVICE_BUTTON_1)	{ btnL |= (1 << 1); }
//        if (currentData[0].m_buttonState & HD_DEVICE_BUTTON_2)	{ btnL |= (1 << 0); }
//        if (currentData[1].m_buttonState & HD_DEVICE_BUTTON_1)	{ btnR |= (1 << 1); }
//        if (currentData[1].m_buttonState & HD_DEVICE_BUTTON_2)	{ btnR |= (1 << 0); }
//    }
//    else
//    {
//        for (int i = 0; i < 3; i++)	{
//            deltaL[i] = 0;
//            deltaL[i + 3] = 0;
//            deltaR[i] = 0;
//            deltaR[i + 3] = 0;
//        }
//    }

//    //

