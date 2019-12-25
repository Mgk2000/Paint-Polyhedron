#include <QImage>
#include <QFile>
#include <QDir>
#include <QColor>
#include "mainwidget.h"
#include "mainwindow.h"
#include "cube.h"
void MainWidget::saveGame()
{
    Cube * cube = (Cube*) figure;
    saveTimer.stop();
    QDir dir(mainWindow->projectDir + "/shapes");
    if (!dir.exists())
        return;
    int nc = cube->ncells;
    char* fileBuf = new  char [nc*nc*6];
    for (int i =0; i <6; i++)
        for (int j=0; j< nc; j++)
            for (int k=0; k < nc; k++)
            {
                int ind = i*nc * nc + j* nc + k;
                char colorInd = cube->faces[i].cells[j][k].colorInd;
                fileBuf[ind] = colorInd;
            }
   QString fn;
    for (int i=1; i< 1000000; i++)
    {
       fn = dir.path() + QString("/%1").arg(i);
       if (!QFile(fn + ".dat").exists() && !QFile(fn + ".png").exists())
           break;
    }

   QFile shapeFile(fn + ".dat");
   shapeFile.open(QIODevice::WriteOnly);
   shapeFile.write((const char *)fileBuf, nc*nc*6);
//   shapeFile.write(&level,1);
   QString es = shapeFile.errorString();
   delete[] fileBuf;
   shapeFile.close();
   saveCubeSnap(fn + ".png");
}

void MainWidget::saveCubeSnap(const QString& fn )
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

