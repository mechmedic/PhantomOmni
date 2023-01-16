#ifndef HAPTICDEVICE_H
#define HAPTICDEVICE_H

#include <QThread>
#include <QtNetwork>

// CKim - OpenHaptics Header
#include <HD/hd.h>
#include <HDU/hduVector.h>
#include <HDU/hduError.h>

#include <iostream>
//#include <WinSock2.h>
//#include <conio.h>

#define OMNI_NUM 2
#define NTWK_PORT 9127

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
HDCallbackCode HDCALLBACK updateDeviceCallback(void *pUserData);

// Copy the updated the status of the haptic device to userData
HDCallbackCode HDCALLBACK copyDeviceDataCallback(void *pUserData);

class HapticDevice : public QThread
{
    Q_OBJECT

public:
    void run() override;

public:
    HapticDevice();
    bool InitHapticDevice();
    bool StartHapticDevice();
    bool StopHapticDevice();
    void CloseHapticDevice();
    void GetDeviceData(int idx, DeviceData* devData) {   memcpy(devData, m_currentData+idx, sizeof(DeviceData));    };

    void InitNetwork(int portNum = NTWK_PORT);
    void StopNetwork();

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

    std::string m_DevName[2];
    QString m_errMsg;
    QString m_ntwkMsg;

    QTcpServer* m_pServer = 0;
    QTcpSocket* m_pSocket = 0;

private slots :
    void OnConnect();

signals :
    void DeviceMessage(const QString& msg);
    void NetworkMessage(const QString& msg);

};

#endif // HAPTICDEVICE_H
