# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = ut

QT       -= gui

TEMPLATE = app

HEADERS = \
   $$PWD/catch2/catch.hpp \
   $$PWD/catch2/catch_reporter_automake.hpp \
   $$PWD/catch2/catch_reporter_tap.hpp \
   $$PWD/catch2/catch_reporter_teamcity.hpp \
   $$PWD/lib/SimpleFlatland.h \
   $$PWD/lib/SimpleMapCreator.h

SOURCES = \
   $$PWD/ut/main_ut.cpp \
   $$PWD/ut/Test_SimpleFlatland.cpp \
   $$PWD/ut/Test_SimpleMapCreator.cpp \
   $$PWD/lib/SimpleFlatland.cpp \
   $$PWD/lib/SimpleMapCreator.cpp

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
