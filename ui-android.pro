#-------------------------------------------------
#
# Project created by QtCreator 2019-03-25T23:28:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ui-android
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
        $$PWD/ui-android/main_ui-android.cpp \
        $$PWD/ui-android/window-android.cpp

HEADERS += \
        $$PWD/ui-android/window-android.h

CONFIG += mobility
MOBILITY = 


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/./release/ -llib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/./debug/ -llib
else:unix: LIBS += -L$$OUT_PWD/./ -llib

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/./release/liblib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/./debug/liblib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/./release/lib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/./debug/lib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/./liblib.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/./release/ -lui-lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/./debug/ -lui-lib
else:unix: LIBS += -L$$OUT_PWD/./ -lui-lib

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/./release/libui-lib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/./debug/libui-lib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/./release/ui-lib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/./debug/ui-lib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/./libui-lib.a
