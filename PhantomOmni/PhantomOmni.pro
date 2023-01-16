QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    hapticdevice.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    MyGUI.h \
    hapticdevice.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += \
  "C:\OpenHaptics\Developer\3.5.0\include" \
  "C:\OpenHaptics\Developer\3.5.0\utilities\include"



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../OpenHaptics/Developer/3.5.0/utilities/lib/x64/release/ -lhdu
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../OpenHaptics/Developer/3.5.0/utilities/lib/x64/debug/ -lhdu
else:unix: LIBS += -L$$PWD/../../OpenHaptics/Developer/3.5.0/utilities/lib/x64/ -lhdu

INCLUDEPATH += $$PWD/../../OpenHaptics/Developer/3.5.0/utilities/include
DEPENDPATH += $$PWD/../../OpenHaptics/Developer/3.5.0/utilities/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../OpenHaptics/Developer/3.5.0/lib/x64/release/ -lhd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../OpenHaptics/Developer/3.5.0/lib/x64/debug/ -lhd
else:unix: LIBS += -L$$PWD/../../OpenHaptics/Developer/3.5.0/lib/x64/ -lhd

INCLUDEPATH += $$PWD/../../OpenHaptics/Developer/3.5.0/include
DEPENDPATH += $$PWD/../../OpenHaptics/Developer/3.5.0/include
