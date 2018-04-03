TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread

SOURCES += \
    ../lib/csv.cpp \
    ../lib/variable.cpp \
    ../recuitSimule/main.cpp \
    ../evaluateur/evaluateur.cpp \
    ../../evaluateCSV/evaluateCSV.cpp

HEADERS += \
    ../lib/csv.hpp \
    ../lib/environnement.hpp \
    ../lib/variable.hpp \
    ../evaluateur/evaluateur.hpp \
    ../../evaluateCSV/evaluateCSV.hpp
