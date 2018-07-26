TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    mix_instructions.cpp \
    mix_computer.cpp \
    mix_compiler.cpp

HEADERS += \
    mix_computer.h \
    mix_instructions.h \
    mix_compiler.h

DISTFILES += \
    code.txt
