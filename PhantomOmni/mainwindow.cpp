#include "mainwindow.h"
#include "MyGUI.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new MyUi::MainWindow)
{
    ui->setupUi(this);
    ui->btnStartOmni->setEnabled(false);
    ui->btnStopOmni->setEnabled(false);
    ui->btnStopServer->setEnabled(false);

    m_DispTimer = new QTimer(this);

    connect(&m_HapticDevice,&HapticDevice::DeviceMessage,this,&MainWindow::DisplayDeviceMessage);
    connect(&m_HapticDevice,&HapticDevice::NetworkMessage,this,&MainWindow::DisplayNtwkMessage);
    connect(m_DispTimer, &QTimer::timeout, this, &MainWindow::DisplayStatus);
}

MainWindow::~MainWindow()
{
    m_HapticDevice.CloseHapticDevice();
    delete ui;
}


void MainWindow::on_btnInitOmni_clicked()
{
    if(!m_HapticDevice.InitHapticDevice())
    {
        // CKim - Handle error
    }
    ui->btnInitOmni->setEnabled(false);
    ui->btnStartOmni->setEnabled(true);
}


void MainWindow::on_btnStartOmni_clicked()
{
    if(!m_HapticDevice.StartHapticDevice())
    {
        // CKim - Handle Error
    }
    m_HapticDevice.start();
    m_DispTimer->start(50);

    ui->btnStartOmni->setEnabled(false);
    ui->btnStopOmni->setEnabled(true);
}


void MainWindow::on_btnStopOmni_clicked()
{
    m_DispTimer->stop();
    if(!m_HapticDevice.StopHapticDevice())
    {
        // CKim - Handle Error
    }
    ui->btnStartOmni->setEnabled(true);
    ui->btnStopOmni->setEnabled(false);
}


void MainWindow::DisplayStatus()
{
    DeviceData devData;
    m_HapticDevice.GetDeviceData(0, &devData);

    QString qstr;
    for(int i=0; i<16; i++)
    {
        QTextStream(&qstr) << devData.m_Transform[i];
        ui->lblLeftTf[i]->setText(qstr);
        qstr.clear();
    }
}


void MainWindow::DisplayDeviceMessage(const QString& msg)
{
   ui->lblDeviceStatus->setText(msg);
}


void MainWindow::DisplayNtwkMessage(const QString& msg)
{
   ui->lblNtwkStatus->setText(msg);
}


void MainWindow::on_btnStartServer_clicked()
{
    m_HapticDevice.InitNetwork(ui->leditPort->text().toInt());
    ui->btnStartServer->setEnabled(false);
    ui->btnStopServer->setEnabled(true);
}


void MainWindow::on_btnStopServer_clicked()
{
    m_HapticDevice.StopNetwork();
    ui->btnStartServer->setEnabled(true);
    ui->btnStopServer->setEnabled(false);
}

