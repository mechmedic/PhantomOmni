#ifndef HAPTICDEVICE_H
#define HAPTICDEVICE_H

#include <QThread>
#include <QtNetwork>

// CKim - OpenHaptics Header
#include <HD/hd.h>
#include <HDU/hduVector.h>
#include <HDU/hduError.h>

#include <iostream>

#include <WinSock2.h>       // Windows
//#include <sys/socket.h>   // Linux

//#include <conio.h>

#define OMNI_NUM 2
#define NTWK_PORT 7272

//--------------------------------------------------------------
/* Holds data retrieved from HDAPI. */
typedef struct
{
    HDint m_buttonState;			/* Has the device button has been pressed. */
    hduVector3Dd m_devicePosition;	/* Current device coordinates. */
    hduVector3Dd m_gimbalAngle;		/* Current device coordinates. */
    HDdouble m_Transform[16];		// CKim - Transform
    HDErrorInfo m_error;
} DeviceData;

//--------------------------------------------------------------
// CKim - Callback functions to be added to the scheduler
// Read and update the status of the haptic device

/// Callback function for updating the status of the haptic device
/// Added to the scheduler and periodically executed in the
/// control loop of the device
HDCallbackCode HDCALLBACK updateDeviceCallback(void *pUserData);

/// Callback function for copying the updated the status of the
/// haptic device to userData. Added to the scheduler to retrieve
/// the haptic device data
HDCallbackCode HDCALLBACK copyDeviceDataCallback(void *pUserData);


class HapticDevice : public QThread
{
    Q_OBJECT

public:
    /// Constructor
    HapticDevice();

    /// Initializes Haptic Device
    bool InitHapticDevice();

    /// Starts scheduler of the haptic device and thread for
    /// retrieving data and sending over the network.
    bool StartHapticDevice();

    /// Copy Device data. Called in GUI timer
    void GetDeviceData(int idx, DeviceData* devData) {   memcpy(devData, m_currentData+idx, sizeof(DeviceData));    };

    /// Start server for incoming connection from the teleoprated robot
    void StartListen(int portNum = NTWK_PORT);

    /// Stop scheduler, Close haptic device
    bool StopHapticDevice();

    /// Close connection
    void StopNetwork();

    /// Function running on separate thread.
    /// Data reading and sending is implemented in this function
    void run() override;


private :
    bool m_loopFlag;
    bool m_ntwkFlag;

    // CKim - Haptic Device Handle
    HHD m_devHandle[OMNI_NUM] = { 0 };

    // CKim - Handle to Scheduler
    HDSchedulerHandle m_hUpdateHandle = 0;

    // CKim - Structure used to capture data from the device.
    DeviceData m_currentData[OMNI_NUM];
    DeviceData m_prevData[OMNI_NUM];

    // CKim - Haptic device name
    std::string m_DevName[2];

    /// Port for listening. Handle to native socket. QTcpSocket and Server objects
    int m_portNum;
    qintptr m_hdSock = 0;
    QTcpSocket* m_pSocket = 0;
    QTcpServer* m_pServer = 0;

    /// Device status message
    QString m_deviceMsg;
    QString m_ntwkMsg;


private slots :
    /// Executed when server accepted conection
    void OnConnect();

    //
    void OnDisconnect();

signals :
    void DeviceMessage(const QString& msg);
    void NtwkMessage(const QString& msg);

};

#endif // HAPTICDEVICE_H
