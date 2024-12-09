QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        DepthAIHandler.cpp \
        ObstacleDetector.cpp \
        main.cpp

HEADERS += \
    DepthAIHandler.h \
    ObstacleDetector.h

# Link OpenCV and DepthAI libraries
LIBS += -L$$OPENCV_LIB_DIR -lopencv_core -lopencv_imgproc -lopencv_highgui  # OpenCV link
LIBS += -L/home/phulinh/depthai-core/build/install/lib -ldepthai  # Link DepthAI

# If using Qt modules, uncomment as necessary
#QT += gui  # Uncomment if using Qt GUI
