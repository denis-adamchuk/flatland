TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES = \
   $$PWD/console2/main_console2.cpp

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
