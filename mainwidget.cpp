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

#include "mainwidget.h"

#include <QMouseEvent>
#include <QTime>
//#include <GL/glu.h>

#include <math.h>
#include "geometry.h"
#include "cube.h"
#include <QtMath>
#include <QElapsedTimer>
#include "mainwindow.h"
MainWidget::MainWidget(MainWindow *parent, const GameStartInfo & si) :
    QOpenGLWidget(nullptr),
//    glengine(nullptr),
    angularSpeed(0), gscale(0.5f),
    cubeTop(0.2f), cubeBottom(0.1f), cubeTexture(nullptr), bitmapTextTexture(nullptr), buttonsTexture(nullptr),
    mainWindow (parent),gameStartInfo(si),duplicatePending(false),
    rotatePending(false),fillFacePending(false)

{
    buttons = new Buttons();
//    gameStartInfo = si;
    this->resize(480,800);
    QSurfaceFormat format;
        format.setSamples(16);
    this->setFormat(format);
    setAttribute(Qt::WA_AcceptTouchEvents, true);
    connect(buttons,SIGNAL(musicButtonPressed()), this, SLOT(playMusicChanged()));
    prepareSounds();
    saveTimer.setInterval(500);
    connect(&saveTimer, SIGNAL(timeout()), this, SLOT(saveGame()));
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
//    delete glengine;
    delete cubeTexture;
    delete bitmapTextTexture;
    delete buttonsTexture;
    delete cube;
    delete littleCube;
    doneCurrent();
}

void MainWidget::show()
{
 //   qDebug() << "I am MainWidget::show()----------------";
    QOpenGLWidget::show();
//    qDebug() << "I am MainWidget::show()--after QOpenGLWidget::show()--------------";
}

void MainWidget::showEvent(QShowEvent *event)
{
//    qDebug() << "I am showEvent()---------------";

}
void MainWidget::prepareSounds()
{
   soundEffect.addSound("bad.wav", 1.0f);
   soundEffect.addSound("good.wav", 0.5f);
   soundEffect.addSound("Victory.wav", 1.0f);
   music.addSound("wag.wav", 1.0, true);
}

void MainWidget::playMusicChanged()
{
    if (buttons->musicPressed())
            music.playSound(0);
    else music.stop();
}
void vecProd()
{
    QVector3D v1 (0,1,0);
    QVector3D v2 (1,0,0);
    QVector3D p1= QVector3D::crossProduct ( v1, v2) ;
    QVector3D p2= QVector3D::crossProduct ( v2, v1) ;
    qDebug() << "p1=" << p1 << "p2=" << p2;
}
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    //vecProd();
    if (_victory)
        return;
    int x = e->x();
    int y = e->y();
    mousePressPosition = QVector2D(-1,-1);
//     qDebug() << "xy=" << x << y;
    if (_palette.mouseClick(x, y))
    {
        update();
        return;
    }
    if (buttons->mouseClick(x, y))
    {
        update();
        return;
    }
    mousePressPosition = QVector2D(e->localPos());

//     QVector3D glpos = geometries->GetOGLPos(x, y);
 //   qDebug() << glpos.x() << glpos.y() << glpos.z();
    nValidColors += pickPoint(x,y);
    checkValidColors(1);
    if (nValidColors == nTotalColors)
        victory();
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (_victory)
        return;
    if (mousePressPosition.x() <0)
        return;
    QVector2D diff =-( QVector2D(e->localPos()) - mousePressPosition);

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = 5 * diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}
void MainWidget::timerEvent(QTimerEvent *)
{
    angularSpeed *= 0.95;
    bool needsUpdate = true;//false;
    QDateTime now = _victory ?
        endTime : QDateTime::currentDateTime();
    qint64 msecs = startTime.msecsTo(now);
    QDateTime dt;
    dt.setOffsetFromUtc(0);
    dt.setMSecsSinceEpoch(msecs);
    dt.setOffsetFromUtc(0);
    QString sdt = dt.toString("h:mm:ss");
    _bitmapText.setText(sdt + "  "  + QString("%1/%2").arg(nValidColors).arg(nTotalColors));
    if (!isEditMode())
    {
    if (angularSpeed < 0.5)
        angularSpeed = 0.5;
        needsUpdate = true;
    }
    else
    {
        if (angularSpeed < 0.01)
            angularSpeed = 0.0;
        if (cube->needsCellDraw || cube->needsFullDraw || _palette.needsRedraw || angularSpeed>0)
            needsUpdate = true;
    }
    if (needsUpdate)
    {
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
        update();
    }
    if (_victory)
        if (endTime.secsTo(QDateTime::currentDateTime()) >=7)
        {
            timer.stop();
            hide();
            mainWindow->show();
        }
}
void MainWidget::startGame()
{
    _victory = false;
    nTotalColors = gameStartInfo.ncells * gameStartInfo.ncells * 6;
    cube->setNcells(gameStartInfo.ncells);
    cube->needsFullDraw = true;
    littleCube->setNcells(gameStartInfo.ncells);
    littleCube->needsFullDraw = true;
    if (gameStartInfo.data)
    {
        littleCube->setData(gameStartInfo.data);
        if (gameStartInfo.editor)
            cube->setData(gameStartInfo.data);
        else if (gameStartInfo.unfinishedData)
        {
            cube->setData(gameStartInfo.unfinishedData);
            delete[] gameStartInfo.unfinishedData;
        }
        delete[] gameStartInfo.data;
    }
    nValidColors = nValidColorsCount();
    //checkValidColors(2);
    _palette.fillData();
    rotationAxis =  QVector3D(-0.7f,0.7f ,0);
    angularSpeed = 1.0f;
    buttons->setMusic (gameStartInfo.playMusic);
    buttons->setSounds (gameStartInfo.playSounds);
    if (buttons->musicPressed()) music.playSound(0);
    timer.start(12, this);
    startTime = QDateTime::currentDateTime();
}

void MainWidget::sound(int is)
{
    if (buttons->soundPressed()) soundEffect.playSound(is);
}

bool MainWidget::isMusicPlaying() const
{
    return buttons->state[1];
}

bool MainWidget::isSoundsPlaying() const
{
    return buttons->state[0];

}

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    initShaders();
    initCubeTexture();
    initBitmapTextTexture();
    initButtonsTexture();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    cube = new Cube(this, false);
    cube->intGL(_program.shaderProgram());
    _palette.intGL(_paletteProgram.shaderProgram());
    _palette.cube = cube;
    buttons->intGL(_buttonsProgram.shaderProgram());
    _bitmapText.intGL(_buttonsProgram.shaderProgram());
    littleCube = new Cube(this, true);
    littleCube->intGL(_program.shaderProgram());
    getFaceCorners();
    calcViewports();
    _palette.onResize(paletteViewport);
    startGame();
}

//! [3]
void MainWidget::initShaders()
{
    if (!_program.init(":/vshader.glsl", ":/fshader.glsl"))
        close();
    if (!_paletteProgram.init(":/staticVshader.glsl", ":/staticFshader.glsl"))
        close();
    if (!_buttonsProgram.init(":/colorVshader.glsl", ":/colorFshader.glsl"))
        close();
}
QVector3D MainWidget::rotatePoint(const QVector3D& v) const
{
    QVector3D res1 = projection * matrix * v;
    return res1;
}
QVector3D MainWidget::winToGl(const QVector3D &v)const
{
    QVector3D res;
    float xx = v.x()- cubeViewport.left();
    float xxx = xx * 2.0 /cubeViewport.width();
    float xxxx = xxx - 1;

    float yy = v.y() - cubeViewport.center().y();
    float yyy = yy * 2.0 /cubeViewport.width();
    float yyyy = - yyy;

    res.setX(xxxx);
    res.setY(yyyy);
    return res;
}

QVector3D MainWidget::winFromGl(const QVector3D &v, int w, float) const
{
    QVector3D res;
    float xx = v.x() * cubeViewport.width() / 2.0;
    float xxx = cubeViewport.center().x() + xx;
    float yy = (2.0-v.y()) * cubeViewport.width() / 2.0;
    float yyy = cubeViewport.center().y() + yy;
    res.setX (xxx);
    res.setY (yyy);
    return res;
}

void MainWidget::calcViewports()
{
    float h = height() * (1- cubeTop - cubeBottom);
    float l, t;
    float w = width();
    if (w>h)
    {
        t = cubeTop * height();
        l = (w-h) * 0.5f;
        w = h;
    }
    else
    {
        t = cubeTop * height();
        w  = w*1.3f;
        l = (width() -w) *0.5f;
    }
    cubeViewport.setRect(l,t,w,w);  //always square
    littleCubeViewport.setRect(0,0, w/3.7,w/3.7);
    float pw = width() * 0.333 * 2;
    paletteViewport.setRect(width() -pw, 0, pw , pw* 0.4);
    _palette.onResize(paletteViewport);
    textViewport.setRect(w/20, w/4, width()*0.7, width()*0.07);
    pw = width() /10.0f;
    buttonsViewport.setRect(textViewport.right(), textViewport.top(), pw*2,  pw);
    buttons->onResize(buttonsViewport);
    //buttonsViewport = textViewport;
}
void MainWidget::getFaceCorners()
{
    cube->getCorners(&faceCorners[0][0]);
}

int MainWidget::pickPoint(int mx, int my)
{
    //qDebug() << "mv=" << mv.x() << mv.y() << "corner=" << corners[2].x() << corners[2].y();
    QVector3D corners[4], saveCorners[4];
    bool saved = false;
    float saveZ;
    bool pinside = false;
    QVector3D res1, saveRes1;
    int saveFace;
    for (int i =0; i< 6; i++)
    {
        corners[0] = rotatePoint(faceCorners[i][0]);
        corners[1] = rotatePoint(faceCorners[i][1]);
        corners[2] = rotatePoint(faceCorners[i][2]);
        corners[3] = rotatePoint(faceCorners[i][3]);
        QVector3D mv = winToGl(QVector3D(mx, my, 0));
        pointInParallelogram(corners[0], corners[1],corners[2], corners[3],mv, res1, &pinside);
        if (pinside)
        {
            if (!saved)
            {
                saveZ = 0;
                for (int j=0; j<4; j++)
                {
                   saveCorners[j] = corners[j];
                   saveZ = saveZ + corners[j].z();
                }
                saveRes1 = res1;
                saveFace = i;
                saved = true;
            }
            else
            {
                float newZ = 0;
                for (int j=0; j<4; j++)
                    newZ = newZ + corners[j].z();
                    if (newZ < saveZ)
                    {
//                        qDebug() << "mv=" << mv.x() << mv.y() <<
//                            "face" << i << "< saveFace" << saveFace << " corner=" <<
//                                corners[2].x() << corners[2].y();
//                            qDebug() << "cubeFace XY" << res1.x() << res1.y()<<
//                                        qFloor(res1.x() * cube->ncells) << qFloor(res1.y()* cube->ncells);
                         int p =cube->pick(i, res1.x(), res1.y(), _palette.selColor);
                         update();
                         return p;
                     }
                     else /*if (corners[j].z() > saveCorners[k].z() +0.1)*/
                     {
//                            qDebug() << "mv=" << mv.x() << mv.y() <<
//                            "saveFace" << saveFace << "<face" << i << " corner=" <<
//                                saveCorners[2].x() << saveCorners[2].y();
//                            qDebug() << "cubeFace XY" << saveRes1.x() << saveRes1.y() <<
//                                        qFloor(saveRes1.x() * cube->ncells) << qFloor(saveRes1.y()* cube->ncells);
                            int p = cube->pick(saveFace, saveRes1.x(), saveRes1.y(), _palette.selColor);
                            update();
                            return p;
                      }
            }
        }
    }
    return 0;
}
void MainWidget::resizeGL(int w, int h)
{
    calcViewports();
}

void MainWidget::paintGL()
{
    QElapsedTimer timer;
        timer.start();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    checkValidColors(3);
    cubeTexture->bind();
    program()->bind();
    drawCube();
    drawPalette();
    _buttonsProgram.shaderProgram()->bind();
    bitmapTextTexture->bind();
    drawBitmapText();
    buttonsTexture->bind();
    drawButtons();
 int ms =  timer.elapsed();
 if (ms> 10000)
     qDebug() << "paintGL" << ms << "milliseconds";
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    event->ignore();
    music.stop();
    mainWindow->show();
    hide();
}
void MainWidget::drawCube()
{
    glViewport(cubeViewport.left(),height() -cubeViewport.bottom(),cubeViewport.width(),cubeViewport.height());
   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    matrix.setToIdentity();
    matrix.rotate(rotation);
    matrix.scale(gscale,gscale,gscale);
    program()->setUniformValue("mvp_matrix", projection * matrix);
    int m_normalMatrixLoc = program()->uniformLocation("normal_matrix");
    QMatrix3x3 normalMatrix = matrix.normalMatrix();
    program()->setUniformValue(m_normalMatrixLoc, normalMatrix);


    cube->draw();
    glViewport(littleCubeViewport.left(),height() -littleCubeViewport.bottom(),
               littleCubeViewport.width(),littleCubeViewport.height());
    littleCube->draw();
}
void MainWidget::drawPalette()
{
    glViewport(paletteViewport.left(),height() -paletteViewport.bottom(),
               paletteViewport.width(),paletteViewport.height());
    _palette.draw();
}
void MainWidget::drawBitmapText()
{
    glViewport(textViewport.left(),height() -textViewport.bottom(),
               textViewport.width(),textViewport.height());
    _bitmapText.draw();
}

void MainWidget::drawButtons()
{
    glViewport(buttonsViewport.left(),height() -buttonsViewport.bottom(),
               buttonsViewport.width(),buttonsViewport.height());
    buttons->draw();
}
void MainWidget::victory()
{
    if (gameStartInfo.editor)
        return;
    endTime = QDateTime::currentDateTime();
    _victory = true;
    gameStartInfo.currGameFile->close();
    gameStartInfo.currGameFile->remove();
    angularSpeed = 100;
    mainWindow->levelDone();
    music.stop();
    sound(2);
}

void MainWidget::rotateToSnap()
{
    rotation = QQuaternion(0.9262, -0.28875, -0.24218,0.00065);
    gameStartInfo.editor = true;
    update();
}

bool MainWidget::event1(QEvent *event)
{
    QEvent::Type et =event->type();
    switch (et)
    {
    case QEvent::TouchBegin:
    case QEvent::TouchCancel:
    case QEvent::TouchEnd:
    case QEvent::TouchUpdate:
        touchEvent((QTouchEvent *)event);
        return true;
    default:  break;
    }
    return QOpenGLWidget::event(event);
}

void MainWidget::touchEvent(QTouchEvent *ev)
{
    int n = ev->touchPoints().size();
    if (n)
    {
    if (ev->touchPointStates() & (Qt::TouchPointPressed | Qt::TouchPointMoved))
    {
        int touchInterval = 100;
        QPointF p = ev->touchPoints()[0].pos();
        Qt::TouchPointStates state = ev->touchPointStates();
        bool processTouch = false;
        int currMsec = QTime::currentTime().msecsSinceStartOfDay();
        if (state & Qt::TouchPointPressed)
        {
            processTouch = true;
            nextTouchMsec = currMsec + touchInterval*2;
        }
        else if (state & Qt::TouchPointMoved)
            if (currMsec >= nextTouchMsec)
            {
                processTouch = true;
                nextTouchMsec = currMsec + touchInterval;
            }

        if (processTouch)
        {
//        lastTouchMsec = currMsec;
        qDebug() << "Touches = " << p.x() << p.y() << ev->touchPointStates() ;
//        float fx, fy;
//        winToGL(p.x(), p.y(), &fx, &fy);
//        touch(fx, fy);
        pickPoint(p.x(), p.y());

        }
     }
    }

    ev->accept();
}
void MainWidget::initCubeTexture()
{
   cubeTexture = new QOpenGLTexture(QImage(":/images/cube768.png").mirrored());
    cubeTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
//    cubeTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    cubeTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    cubeTexture->setWrapMode(QOpenGLTexture::Repeat);
}
void MainWidget::initBitmapTextTexture()
{
//    bitmapTextTexture = new QOpenGLTexture(QImage(":/images/abcenru.png").mirrored());
    bitmapTextTexture = new QOpenGLTexture(QImage(":/images/Times New Roman,120,-1,5,75,0,0,0,0,0.png").mirrored());
    bitmapTextTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    bitmapTextTexture->generateMipMaps();
    bitmapTextTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    bitmapTextTexture->setWrapMode(QOpenGLTexture::Repeat);
}

void MainWidget::initButtonsTexture()
{
    buttonsTexture = new QOpenGLTexture(QImage(":/images/Buttons.png").mirrored());
    buttonsTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    buttonsTexture->generateMipMaps();
    buttonsTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    buttonsTexture->setWrapMode(QOpenGLTexture::Repeat);
}
void MainWidget::keyPressEvent(QKeyEvent *event)
{
    if (_victory)
        return;
    int k = event->key();
//    qDebug() << "Pressed=" << k;
    switch(k)
    {
        case 16777264:  //F1
            rotateToSnap(); break;
        case 16777265:    //F2
            this->saveCubeSnap("shapes/cube.png"); break;
        case  16777266:   //F3
            {fillFacePending = true; break;}
        case  16777267:   //F4
            {duplicateFace(); break;}
        case  16777268:   //F5
            {rotateFace(); break;}
        case  16777269:   //F6
            {saveAll(); break;}
        case  16777270:   //F7
            {saveGame(); break;}
        case 16777313 : // android back button
        {
        hide();
        mainWindow->show();
        break;
        }
    }
}
void MainWidget::duplicateFace()
{
    duplicatePending = true;
}
void MainWidget::rotateFace()
{
    rotatePending = true;
}

int MainWidget::nValidColorsCount() const
{
    int vc =0;
    for (int i=0; i< 6; i ++)
        for (int j =0; j< cube->ncells; j++)
            for (int k =0; k< cube->ncells; k++)
                if (cube->faces[i].cells[j][k].colorInd == littleCube->faces[i].cells[j][k].colorInd)
                    vc ++;
    return vc;
}

void MainWidget::checkValidColors(int where)
{
    return;
    int vc = nValidColorsCount();
    if (vc!= nValidColors)
        qDebug() << "checkValidColors error where=" << where << " vc=" << vc << " nValidColors=" << nValidColors;
    nValidColors = vc;
}
void MainWidget::saveAll()
{
    rotateToSnap();
    saveTimer.start();
}

GameStartInfo::GameStartInfo(int _ncells, uchar *_data, bool _editor) : ncells(_ncells),
    data(_data), editor(_editor)
{

}
