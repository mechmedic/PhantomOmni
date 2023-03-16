#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

// CKim - Include GUI header
#include "HapticDeviceGUI.h"

// CKim - OpenHaptics Header
#include "hapticdevice.h"


QT_BEGIN_NAMESPACE
namespace Ui { class HapticDeviceGUI; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnInitOmni_clicked();

    void on_btnStartOmni_clicked();

    void on_btnStartServer_clicked();

private:
    Ui::HapticDeviceGUI *ui;
    HapticDevice m_HapticDevice;
    QTimer* m_DispTimer;


private slots:
    void DisplayStatus();
    void DisplayDeviceMessage(const QString& msg);
    void DisplayNetworkMessage(const QString& msg);


};
#endif // MAINWINDOW_H
