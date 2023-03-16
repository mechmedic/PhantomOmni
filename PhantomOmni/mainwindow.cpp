#include "mainwindow.h"
#include "HapticDeviceGUI.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HapticDeviceGUI)
{
    ui->setupUi(this);
    ui->btnStartOmni->setEnabled(false);
//    ui->btnStopOmni->setEnabled(false);
//    ui->btnStopServer->setEnabled(false);

    m_DispTimer = new QTimer(this);
    m_DispTimer->setInterval(40);

    connect(&m_HapticDevice,&HapticDevice::DeviceMessage,this,&MainWindow::DisplayDeviceMessage);
    connect(&m_HapticDevice,&HapticDevice::NtwkMessage,this,&MainWindow::DisplayNetworkMessage);
    connect(m_DispTimer, &QTimer::timeout, this, &MainWindow::DisplayStatus);
}

MainWindow::~MainWindow()
{
    m_HapticDevice.StopNetwork();
    m_HapticDevice.StopHapticDevice();
    delete ui;
}


void MainWindow::on_btnInitOmni_clicked()
{
    if(!m_HapticDevice.InitHapticDevice())
    {
        return;
    }
    ui->btnInitOmni->setEnabled(false);
    ui->btnStartOmni->setEnabled(true);
}


void MainWindow::on_btnStartOmni_clicked()
{
    if(!m_HapticDevice.StartHapticDevice())
    {
       return;
    }
    m_DispTimer->start(50);

    ui->btnStartOmni->setEnabled(false);
    //ui->btnStopOmni->setEnabled(true);
}


//void MainWindow::on_btnStopOmni_clicked()
//{
//    m_DispTimer->stop();
//    if(!m_HapticDevice.StopHapticDevice())
//    {
//        // CKim - Handle Error
//    }
//    ui->btnStartOmni->setEnabled(true);
//    ui->btnStopOmni->setEnabled(false);
//}


void MainWindow::DisplayStatus()
{
    DeviceData devData;
    QString qstr;

    for(int n=0; n<OMNI_NUM; n++)
    {
        m_HapticDevice.GetDeviceData(n, &devData);
        for(int i=0; i<16; i++)
        {
            QTextStream(&qstr) << devData.m_Transform[i];
            if(n==0)    ui->lblLeftTf[i]->setText(qstr);
            if(n==1)    ui->lblRightTf[i]->setText(qstr);
            qstr.clear();
        }
    }
}


void MainWindow::DisplayNetworkMessage(const QString& msg)
{
   ui->lblNtwkStatus->setText(msg);
}


void MainWindow::DisplayDeviceMessage(const QString& msg)
{
   ui->lblDeviceStatus->setText(msg);
}


void MainWindow::on_btnStartServer_clicked()
{
    m_HapticDevice.StartListen();
    ui->btnStartServer->setEnabled(false);
//    ui->btnStopServer->setEnabled(true);
}


//void MainWindow::on_btnStopServer_clicked()
//{
//    m_HapticDevice.StopNetwork();
//    ui->btnStartServer->setEnabled(true);
//    ui->btnStopServer->setEnabled(false);
//}

