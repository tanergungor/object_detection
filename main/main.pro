TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

INCLUDEPATH += "C:/Library/opencv-3.1/build/include/opencv"
INCLUDEPATH += "C:/Library/opencv-3.1/build/include/opencv2"
INCLUDEPATH += "C:/Library/opencv-3.1/build/include/"

LIBS += "C:/Library/opencv-3.1/build/x64/vc14/lib/opencv_world310.lib"
LIBS += "C:/Library/opencv-3.1/build/x64/vc14/lib/opencv_world310d.lib"
