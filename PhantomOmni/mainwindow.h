#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

// CKim - OpenHaptics Header
#include "hapticdevice.h"

QT_BEGIN_NAMESPACE
namespace MyUi { class MainWindow; }
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

    void on_btnStopOmni_clicked();

    void on_btnStartServer_clicked();

private:
    MyUi::MainWindow *ui;
    HapticDevice m_HapticDevice;
    QTimer* m_DispTimer;

private slots:
    void DisplayStatus();
    void DisplayDeviceMessage(const QString& msg);
    void DisplayNtwkMessage(const QString& msg);

    void on_btnStopServer_clicked();
};
#endif // MAINWINDOW_H
