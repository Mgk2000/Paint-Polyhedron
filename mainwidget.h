/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include "openglprogram.h"
#include <QOpenGLTexture>
#include <QTime>
#include <QTimer>
#include "palette.h"
#include "bitmaptext.h"
#include "soundeffect.h"
#include "buttons.h"
#include <QTouchEvent>
class RotatingFigure;
//class Cube;
class MainWindow;
struct GameStartInfo
{
    GameStartInfo(int _ncells, uchar* _data, bool _editor);
    ushort type;
    ushort division;
    ushort ncells;
    uchar * data;
    uchar * unfinishedData;
    QFile* currGameFile;
#ifdef WIN32
    bool editor;
    int fileNo;
#endif
    bool playMusic;
    bool playSounds;
    char * vertexInfo;
};
class Buttons;
class QGestureEvent;
class QPinchGesture;
class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit MainWidget(MainWindow *parent, const GameStartInfo & si);
    virtual~MainWidget() override;
    virtual void show();
    QMatrix4x4 projection;
    BitmapText* getBitmapText()  {return & _bitmapText;}
    Buttons* getButtons()  {return  buttons;}
    GameStartInfo gameStartInfo;
#ifdef WIN32
    bool isEditMode() const {return gameStartInfo.editor;}
    bool copySymmetry;

#endif
    void startGame();
    void sound(int is);
    //Cube * cube, *littleCube;
    RotatingFigure * figure, *littleFigure;
#ifdef WIN32
    bool duplicatePending, rotatePending, fillFacePending;
    QTimer saveTimer;
#endif
    bool isMusicPlaying() const;
    bool isSoundsPlaying() const;
    SoundEffect soundEffect, music;
    QVector3D rotatePoint(const QVector3D &v) const;
    QVector3D winToGl(const QVector3D &v) const;
    QVector2D colorSquareTexCoords[7];
    QString projectDir() const;
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;
    bool event(QEvent *event) override;
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    bool gestureEvent(QGestureEvent *event);
    virtual void  closeEvent(QCloseEvent *event) override;
    void initShaders();
private:
    void createFigure();
    QMatrix4x4 matrix;
    Palette* _palette;
    BitmapText _bitmapText;
    QBasicTimer timer;
    QOpenGLShaderProgram* program() {return _program.shaderProgram();}
    OpenGLProgram _program, _paletteProgram , _buttonsProgram;
    QOpenGLTexture *cubeTexture, *bitmapTextTexture, *buttonsTexture;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;
    void calcViewports();
    QRect cubeViewport, littleCubeViewport;
    QRect paletteViewport;
    QRect textViewport, buttonsViewport;
    float gscale;
    float cubeTop, cubeBottom;
    int pickPoint(int mx, int my);
    void drawFigure();
    void drawPalette();
    void touchEvent(QTouchEvent *ev);
    int lastTouchMsec, nextTouchMsec;
    void initCubeTexture();
#ifdef WIN32
    void keyPressEvent(QKeyEvent *event) override;
#endif
    MainWindow* mainWindow;
    void initBitmapTextTexture();
    void initButtonsTexture();
    void drawBitmapText();
    void drawButtons();
    int nValidColors, nTotalColors;
    void victory();
    bool _victory;
    QDateTime startTime,endTime;
    qreal minAngularSpeed;
#ifdef WIN32
    void saveFigureSnap(const QString& fn);
    void rotateToSnap();
    void saveAll();
    void duplicateFace();
    void rotateFace();
#endif
    void checkValidColors(int where);
    void prepareSounds();
    Buttons *buttons;
    void pinchTriggered(QPinchGesture *gesture);
public slots:
    void playMusicChanged();
#ifdef WIN32
    void saveGame();
#endif
};

#endif // MAINWIDGET_H
