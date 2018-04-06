TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    config.cpp

HEADERS += \
    config.h
#INCLUDEPATH += /opencv3/opencv_in/include \
#                /opencv3/opencv_in/include/opencv \
#                /opencv3/opencv_in/include/opencv2

#LIBS += /opencv3/opencv_in/lib/libopencv_core.so.3.1    \
#        /opencv3/opencv_in/lib/libopencv_imgproc.so\
# /opencv3/opencv_in/lib/libopencv_imgcodecs.so.3.1 \
# /opencv3/opencv_in/lib/libopencv_highgui.so.3.1
INCLUDEPATH += -I /usr/local/opencv/340/include \
                /usr/local/opencv/340/include/opencv \
                /usr/local/opencv/340/include/opencv2

LIBS += -L /usr/local/opencv/340/lib/ -lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_imgcodecs

DISTFILES += \
    camera.bmp
