#ifndef HAPTICDEVICEGUI_H
#define HAPTICDEVICEGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

namespace Ui {

// CKim - Hard code your UI here, instead of using QT's GUI Editor
class HapticDeviceGUI
{
public:
//    QLabel *label;
//    QLabel *lblMessage;

//    QMenuBar *menubar;
//    QStatusBar *statusbar;

    QString DeviceBtnStyleStr =  QString::fromUtf8(
                "QPushButton:pressed { background-color: rgb(5, 153, 44); }\n"
                "QPushButton:disabled { color: rgb(33, 33, 33); background-color:gray; font: bold 75 14pt; }\n"
                "QPushButton { color: rgb(255, 255, 255); background-color: rgb(252, 119, 3); font: bold 75 14pt \"Noto Sans\"; }\n" );

    QString LblStyleStatusStr = QString::fromUtf8(
                "color: rgb(0, 0, 0); \n"
                "selection-background-color: rgb(238, 238, 236); \n"
                "selection-color: rgb(238, 238, 236);\n"
                "background-color:  rgb(255, 255, 255);\n"
                "alternate-background-color: rgb(0, 0, 0);\n"
                "font: bold 75 11pt \"Noto Sans\";");

    QWidget *centralwidget;
    QGridLayout* gridLayout;

    QHBoxLayout *device_ui_layout;
    QPushButton *btnInitOmni;
    QPushButton *btnStartOmni;

    QLabel *lblDeviceStatus;

    QHBoxLayout *ntwk_ui_layout;
    QPushButton *btnStartServer;
    QLabel *lblPort;
    QLineEdit *leditPort;

    QLabel *lblNtwkStatus;

    QHBoxLayout *tfLayout;
    QGridLayout* leftTfLayout;
    QLabel *lblLeftTf[16];
    QGridLayout* rightTfLayout;
    QLabel *lblRightTf[16];
    QLabel *lblLeftName;
    QLabel *lblRightName;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 300);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));

        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        // ----------------------------------------------------------------------
        // CKim - Set up Button layout for device control
        device_ui_layout = new QHBoxLayout();
        device_ui_layout->setObjectName(QString::fromUtf8("device_ui_layout"));

        btnInitOmni = new QPushButton(centralwidget);
        btnInitOmni->setObjectName(QString::fromUtf8("btnInitOmni"));
        sizePolicy.setHeightForWidth(btnInitOmni->sizePolicy().hasHeightForWidth());
        btnInitOmni->setSizePolicy(sizePolicy);
        btnInitOmni->setStyleSheet(DeviceBtnStyleStr);
        device_ui_layout->addWidget(btnInitOmni);

        btnStartOmni = new QPushButton(centralwidget);
        btnStartOmni->setObjectName(QString::fromUtf8("btnStartOmni"));
        sizePolicy.setHeightForWidth(btnStartOmni->sizePolicy().hasHeightForWidth());
        btnStartOmni->setSizePolicy(sizePolicy);
        btnStartOmni->setStyleSheet(DeviceBtnStyleStr);
        device_ui_layout->addWidget(btnStartOmni);

        gridLayout->addLayout(device_ui_layout,0,0);

        // ----------------------------------------------------------------------
        // CKim - Setup UI for device status message
        lblDeviceStatus = new QLabel(centralwidget);
        lblDeviceStatus->setObjectName(QString::fromUtf8("lblDeviceStatus"));
        lblDeviceStatus->setStyleSheet(LblStyleStatusStr);
        lblDeviceStatus->setAlignment(Qt::AlignLeft);
        gridLayout->addWidget(lblDeviceStatus,1,0);

        // ----------------------------------------------------------------------
        // CKim - Setup UI for tfMat
        tfLayout = new QHBoxLayout();
        tfLayout->setObjectName(QString::fromUtf8("tfLayout"));
        leftTfLayout = new QGridLayout();
        leftTfLayout->setObjectName(QString::fromUtf8("leftTfLayout"));
        rightTfLayout = new QGridLayout();
        rightTfLayout->setObjectName(QString::fromUtf8("rightTfLayout"));
        lblLeftName = new QLabel(centralwidget);
        lblRightName = new QLabel(centralwidget);
        lblLeftName->setStyleSheet(LblStyleStatusStr);
        lblRightName->setStyleSheet(LblStyleStatusStr);

        for(int i=0; i<16; i++)
        {
            lblLeftTf[i] = new QLabel(centralwidget);
            leftTfLayout->addWidget(lblLeftTf[i],i%4, i/4);
            lblRightTf[i] = new QLabel(centralwidget);
            rightTfLayout->addWidget(lblRightTf[i], i%4, i/4);
        }
        leftTfLayout->addWidget(lblLeftName,4,0,1,4);
        rightTfLayout->addWidget(lblRightName,4,0,1,4);
        tfLayout->addLayout(leftTfLayout);
        tfLayout->addLayout(rightTfLayout);

        gridLayout->addLayout(tfLayout,2,0);


        // ----------------------------------------------------------------------
        // CKim - Set up Button layout for network control
        ntwk_ui_layout = new QHBoxLayout();
        ntwk_ui_layout->setObjectName(QString::fromUtf8("ntwk_button_layout"));

        btnStartServer = new QPushButton(centralwidget);
        btnStartServer->setObjectName(QString::fromUtf8("btnStartServer"));
        sizePolicy.setHeightForWidth(btnStartServer->sizePolicy().hasHeightForWidth());
        btnStartServer->setSizePolicy(sizePolicy);
        btnStartServer->setStyleSheet(DeviceBtnStyleStr);
        ntwk_ui_layout->addWidget(btnStartServer);

        lblPort = new QLabel(centralwidget);
        lblPort->setObjectName(QString::fromUtf8("lblPort"));
        lblPort->setStyleSheet(LblStyleStatusStr);
        lblPort->setAlignment(Qt::AlignRight);
        lblPort->setAlignment(Qt::AlignCenter);
        ntwk_ui_layout->addWidget(lblPort);

        leditPort = new QLineEdit(centralwidget);
        leditPort->setObjectName(QString::fromUtf8("leditPort"));
        leditPort->setInputMask("0000;_");
        leditPort->setText(QString::number(7272));
        ntwk_ui_layout->addWidget(leditPort);

        gridLayout->addLayout(ntwk_ui_layout,3,0);


        lblNtwkStatus = new QLabel(centralwidget);
        lblNtwkStatus->setObjectName(QString::fromUtf8("lblNtwkStatus"));
        lblNtwkStatus->setStyleSheet(LblStyleStatusStr);
        lblNtwkStatus->setAlignment(Qt::AlignLeft);
        gridLayout->addWidget(lblNtwkStatus,4,0);


        MainWindow->setCentralWidget(centralwidget);

//        menubar = new QMenuBar(MainWindow);
//        menubar->setObjectName(QString::fromUtf8("menubar"));
//        menubar->setGeometry(QRect(0, 0, 800, 22));
//        MainWindow->setMenuBar(menubar);
//        statusbar = new QStatusBar(MainWindow);
//        statusbar->setObjectName(QString::fromUtf8("statusbar"));
//        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
//        label->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p>Msg :</p></body></html>", nullptr));
//        lblMessage->setText(QCoreApplication::translate("MainWindow", "Device Status", nullptr));

        btnInitOmni->setText(QCoreApplication::translate("MainWindow", "Init Omni", nullptr));
        btnStartOmni->setText(QCoreApplication::translate("MainWindow", "Start Omni", nullptr));
        lblDeviceStatus->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));

        btnStartServer->setText(QCoreApplication::translate("MainWindow", "Start Server", nullptr));
        lblNtwkStatus->setText(QCoreApplication::translate("MainWindow", "NtwkStatus", nullptr));

        lblPort->setText(QApplication::translate("MainWindow", "PORT : ", nullptr));

        lblLeftName->setText(QApplication::translate("MainWindow", "L_Arm", nullptr));
        lblRightName->setText(QApplication::translate("MainWindow", "R_Arm", nullptr));
    } // retranslateUi


};

} // namespace Ui

QT_END_NAMESPACE

#endif // HAPTICDEVICEGUI_H
