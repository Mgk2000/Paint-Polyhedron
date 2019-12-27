#include <QImage>
#include <QFile>
#include <QDir>
#include <QColor>
#include "mainwidget.h"
#include "mainwindow.h"
#include "cube.h"
void MainWidget::saveGame()
{
    saveTimer.stop();
    QDir dir(mainWindow->projectDir + "/shapes");
    if (!dir.exists())
        return;
    DataFileInfo dfi;
    dfi.type = gameStartInfo.type;
    dfi.division = gameStartInfo.division;
    dfi.ncells = gameStartInfo.ncells;
    int fsize = figure->getNCells();
    char* fileBuf = new  char [fsize];
    figure->getCellsData(fileBuf);
    QString fn;
    for (int i=1; i< 1000000; i++)
    {
       fn = dir.path() + QString("/%1").arg(i);
       if (!QFile(fn + ".dat").exists() && !QFile(fn + ".png").exists())
           break;
    }

   QFile shapeFile(fn + ".dat");
   shapeFile.open(QIODevice::WriteOnly);
   shapeFile.write((const char *)&dfi, sizeof(dfi));
   shapeFile.write((const char *)fileBuf, fsize);
//   shapeFile.write(&level,1);
   QString es = shapeFile.errorString();
   delete[] fileBuf;
   shapeFile.close();
   saveFigureSnap(fn + ".png");
}

void MainWidget::saveFigureSnap(const QString& fn )
{
    int w = width();
    int h = w;
    QImage image(w,h,QImage::Format_RGB888);
    glReadPixels(0, height() -  (cubeViewport.center().y() + w/2) ,
                 w , h, GL_RGB,  GL_UNSIGNED_BYTE,image.bits());
    image = image.mirrored(false, true);
    QImage im = image.scaled(w/4, w/4 ,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    im.save(fn);

}

