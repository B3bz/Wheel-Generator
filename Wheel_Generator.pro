SOURCES += \
    main.cpp \
    mafenetre.cpp

QT += widgets

HEADERS += \
    mafenetre.h

CONFIG += c++14

RESOURCES += \
    Wheel_Generator.qrc

win32:RC_ICONS += wheel-icon.ico
