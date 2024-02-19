QT -= gui

CONFIG += c++11 console
CONFIG += app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/ffmpeg/include
INCLUDEPATH += $$PWD/SDL2/include
INCLUDEPATH += $$PWD/portaudio/include

LIBS += $$PWD/ffmpeg/lib/avformat.lib   \
        $$PWD/ffmpeg/lib/avcodec.lib    \
        $$PWD/ffmpeg/lib/avdevice.lib   \
        $$PWD/ffmpeg/lib/avfilter.lib   \
        $$PWD/ffmpeg/lib/avutil.lib     \
        $$PWD/ffmpeg/lib/postproc.lib   \
        $$PWD/ffmpeg/lib/swresample.lib \
        $$PWD/ffmpeg/lib/swscale.lib    \
        $$PWD/SDL2/lib/x86/SDL2.lib     \
        $$PWD/portaudio/lib/portaudio_x86.lib


SOURCES += main.cpp \
    log.cpp \
    audio.cpp \
    video.cpp \
    avpackets_queue.cpp \
    av_clock.cpp

HEADERS += \
    log.h \
    audio.h \
    video.h \
    avpackets_queue.h \
    av_clock.h
