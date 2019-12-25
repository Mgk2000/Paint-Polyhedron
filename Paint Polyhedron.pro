QT       += core gui widgets opengl multimedia
CONFIG     += debug

CONFIG += qt warn_on rtti exceptions
TARGET = Paint Polyhedron
TEMPLATE = app
# LIBS    += -lqjson
SOURCES += main.cpp \
    bitmaptext.cpp \
    buttons.cpp \
    cube.cpp \
    drawingobject.cpp \
    geometry.cpp \
    mainwindow.cpp \
    octahedron.cpp \
    openglprogram.cpp \
    palette.cpp \
    polyhedron.cpp \
    rotatingfigure.cpp \
    saver.cpp \
    soundeffect.cpp

SOURCES += \
    mainwidget.cpp

HEADERS += \
    bitmaptext.h \
    buttons.h \
    cube.h \
    drawingobject.h \
    geometry.h \
    mainwindow.h \
    octahedron.h \
    openglprogram.h \
    palette.h \
    mainwidget.h \
    polyhedron.h \
    rotatingfigure.h \
    soundeffect.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target


FORMS += \
    mainwindow.ui


contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android-sources
}

DISTFILES += \
    android-sources/AndroidManifest.xml \
    android-sources/build.gradle \
    android-sources/gradle/wrapper/gradle-wrapper.jar \
    android-sources/gradle/wrapper/gradle-wrapper.properties \
    android-sources/gradlew \
    android-sources/gradlew.bat \
    android-sources/res/values/libs.xml
