#include "mainwindow.h"
#include "mainwidget.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QtMath>
#include <QMessageBox>
#include <QPainter>
#include <QScroller>

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), mainWidget(nullptr), _ncols(5)
{
    ui->setupUi(this);
    getProjectDir();
    getDataFiles();
    downloadNewFiles();
    readSettings();
#ifndef WIN32
    ui->editWidget->hide();
#endif
//    QScroller::ScrollerGestureType gesture = QScroller::LeftMouseButtonGesture;
//    QScroller::grabGesture(ui->dataFilesView, gesture);
//    QScroller::grabGesture(ui->dataFilesView);
    QScroller *scroller = QScroller::scroller(ui->dataFilesView);
    ui->dataFilesView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->dataFilesView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    QScrollerProperties properties = QScroller::scroller(scroller)->scrollerProperties();
    QVariant overshootPolicy = QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
    properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, overshootPolicy);
    scroller->setScrollerProperties(properties);
    properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, overshootPolicy);
    scroller->setScrollerProperties(properties);
    //Scrolling Gesture
    scroller->grabGesture(ui->dataFilesView,QScroller::LeftMouseButtonGesture);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTableView()
{
    dataFilesModel = new DataFilesModel(this);
    ui->dataFilesView->setModel(dataFilesModel);
    ui->dataFilesView->setShowGrid(false);
        ui->dataFilesView->horizontalHeader()->hide();
        ui->dataFilesView->verticalHeader()->hide();
        ui->dataFilesView->horizontalHeader()->setMinimumSectionSize(1);
        ui->dataFilesView->verticalHeader()->setMinimumSectionSize(1);
//        ui->tableView->setModel(new ImageModel(this));
        itemDelegate = new ImageDelegate(this);
        ui->dataFilesView->setItemDelegate((QAbstractItemDelegate*)itemDelegate);
        ui->dataFilesView->resizeColumnsToContents();
        ui->dataFilesView->resizeRowsToContents();
}
int MainWindow::indFromNo(int fno) const
{
    for (int i=0; i< dataFiles.length(); i++)
        if (dataFiles[i].fileNo == fno)
            return i;
    return -1;
}

void MainWindow::loadVertexInfo(GameStartInfo * si)
{
    QString ds;
#ifdef WIN32
    ds = projectDir + "/settings/%1_%2.vert";
#else
    ds  = ":/settings/%1_%2.vert";
#endif
    QString vfs = ds.arg(si->type).arg(si->division);
    QFile f (vfs);
    if (f.exists())
    {
        f.open(QIODevice::ReadOnly);
        si->vertexInfo = new char[f.size()];
        f.read(si->vertexInfo, f.size());
    }
    else
        si->vertexInfo = nullptr;
}

void MainWindow::applicationStateChanged(Qt::ApplicationState state)
{
    qDebug() << "AppState=" << state;
    if (!mainWidget)
        return;
    if (state == Qt::ApplicationInactive || state == Qt::ApplicationSuspended)
        mainWidget->music.stop();
    else if (mainWidget->isVisible())
        mainWidget->playMusicChanged();
}
void MainWindow::readSettings()
{
    QString sdir = projectDir + "/settings";
    QDir dir(sdir);
    if (!dir.exists())
        dir.mkpath(".");
    qDebug() << "Dir =" << sdir;
    QDir::setCurrent(sdir);
    QFile f("settings.json");
    if (!f.exists())
    {
        settings.playMusic = true;
        settings.playSounds = true;
        settings.doneLevels.clear();
        writeSettings();
    }
    else
    {
        if (!f.open(QIODevice::ReadOnly))
            return;
        QTextStream file_text(&f);
        QString json_string;
        json_string = file_text.readAll();
        f.close();
        QByteArray json_bytes = json_string.toLocal8Bit();
        auto json_doc = QJsonDocument::fromJson(json_bytes);

        if (json_doc.isNull()) {
             qDebug() << "Failed to create JSON doc." ;
             return;
         }
        if (!json_doc.isObject()) {
             qDebug()  << "JSON is not an object.";
             return ;
         }

        QJsonObject json_obj = json_doc.object();

        if (json_obj.isEmpty()) {
             qDebug()  << "JSON object is empty.";
             return;}
        settings.playMusic = json_obj["playMusic"].toBool();
        settings.playSounds = json_obj["playSounds"].toBool();
        settings.doneLevels.clear();
        settings.doneInds.clear();
        QJsonArray jdla = json_obj["doneLevels"].toArray();
        for(int i=0; i< jdla.count(); ++i)
        {
            int fno = jdla.at(i).toInt();
            settings.doneLevels.append(fno);
            settings.doneInds.append(indFromNo(fno));
        }

    }
    getCurrMaxLevel();
}

void MainWindow::writeSettings()
{
    if (mainWidget)
    {
        settings.playMusic = mainWidget->isMusicPlaying();
        settings.playSounds = mainWidget->isSoundsPlaying();
    }
    QString sdir = projectDir + "/settings";
    QDir::setCurrent(sdir);
    QFile jsonFile ("settings.json");
    if (!jsonFile.open(QIODevice::WriteOnly))
        {
            return;
        }
    QJsonObject job;
    job["playMusic"] = settings.playMusic;
    job["playSounds"] = settings.playSounds;
    QJsonArray jdla;
    for (int i=0; i< settings.doneLevels.count(); i++)
        jdla.append(settings.doneLevels[i]);
    job["doneLevels"] = jdla;
    jsonFile.write(QJsonDocument(job).toJson(QJsonDocument::Indented));
    jsonFile.close();
}

bool MainWindow::startUnfinishedGame()
{
//     return false;
    QString ufn = getUnfinishedGame();
    if (ufn == "")
        return false;
   // hide();
    int fno = ufn.chopped(4).toInt();
    currInd = -1;
    for (int i=0; i< dataFiles.length(); i++)
        if (dataFiles[i].fileNo == fno)
        {
            currInd = i;
            break;
        }
    if(currInd < 0)
        return false;
    startDataGame(currInd, ufn);
    return true;
}

QString MainWindow::getUnfinishedGame()
{
    QString sdir = projectDir + "/settings";
    QDir dir(sdir);
    QStringList fnames = dir.entryList(QStringList() << "*.unf" ,QDir::Files);
    if (fnames.length() == 0)
        return "";
    return fnames[0];
}
void MainWindow::getProjectDir()
{
//    QString exedir = QDir(".").absolutePath();
    QDir dir("shapes");
    if (!dir.exists())
        dir.setPath("../Paint-Polyhedron");
//    bool b = dir.exists();
    projectDir = dir.absolutePath();
    qDebug() << "projectDir=" << projectDir;
    shapesDir = projectDir + "/shapes";
    dir.setPath(shapesDir);
    if (!dir.exists())
         dir.mkpath(".");
//    bool b = dir.exists();

}

void MainWindow::levelDone()
{
    for (int i =0; i< settings.doneInds.length(); i++)
        if (currInd == settings.doneInds[i])
            goto l1;
    settings.doneLevels.append(dataFiles[currInd].fileNo);
    settings.doneInds.append(currInd);
//    std::sort(settings.doneLevels.begin(), settings.doneLevels.end());
    l1:
    getCurrMaxLevel();
    writeSettings();
}


void MainWindow::startDataGame(int ind, QString unfinished)
{
    QString sdir;
#ifdef WIN32
    sdir = dir.path();
#else
    if (dataFiles[ind].resource)
        sdir = dir.path();
    else
        sdir = shapesDir;
#endif
    QString fn =  sdir + '/' + QString("%1.dat").arg(dataFiles[ind].fileNo);
    QFile f (fn);
    f.open(QIODevice::ReadOnly);
    DataFileInfo head;
    int bufSize = f.size() -16;
    char * buf = new char [bufSize];
    f.read((char*)&head, sizeof(head));
    f.read(buf, bufSize);
    GameStartInfo si(head.ncells, (uchar*)buf, ui->editorCheckBox->isChecked());
    si.type = head.type;
    si.division = head.division;
#ifdef WIN32
    ui->typeSpinBox->setValue(si.type);
    ui->divideSpinBox->setValue(si.division);
    ui->ncellsSpinBox->setValue(si.ncells);
#endif
    if (si.type !=0 || si.division != 0)
        loadVertexInfo(&si);
    si.playMusic = settings.playMusic;
    si.playSounds = settings.playSounds;
#ifdef WIN32
    si.fileNo = dataFiles[ind].fileNo;
#endif
    if (unfinished != "")
    {
        currGameFile.setFileName(projectDir + "/settings/" +  unfinished);
        qDebug() << currGameFile.exists() << currGameFile.size() << f.size();
        if (currGameFile.size() == bufSize)
        {
        si.unfinishedData = new uchar[bufSize];
        currGameFile.open(QIODevice::ReadWrite);
        currGameFile.read((char*)si.unfinishedData ,currGameFile.size());
        }
        else
        {
            currGameFile.close();
            currGameFile.remove();
            currGameFile.open(QIODevice::ReadWrite);
            char* ubuf = new char [bufSize];
            for (int i =0; i< bufSize; i++)
                ubuf[i] = 0;
            currGameFile.write(ubuf, bufSize);
            si.unfinishedData = nullptr;
            delete[] ubuf;
        }
    }
    else
    {
        currGameFile.setFileName(QString("%1.unf").arg(dataFiles[ind].fileNo));
        currGameFile.open(QIODevice::ReadWrite);
        char* ubuf = new char [bufSize];
        for (int i =0; i< bufSize; i++)
            ubuf[i] = 0;
        currGameFile.write(ubuf, bufSize);
        si.unfinishedData = nullptr;
        delete[] ubuf;
    }
    si.currGameFile = &currGameFile;
    startGame(si);
    delete[] buf;
}
void MainWindow::startGame(GameStartInfo& si)
{
    if (!mainWidget)
        mainWidget = new MainWidget (this, si);
    else
    {
        mainWidget->gameStartInfo = si;
        //mainWidget->gameStartInfo.data = si.data;
       // mainWidget->gameStartInfo.ncells = si.ncells;
        mainWidget->startGame();
    }
    this->hide();
    mainWidget->show();
    qDebug() << "From MainWindow after widget->show";
}
#ifdef WIN32
void MainWindow::CheckFilesIntegrity()
{
    QDir dir(projectDir + "/shapes");
    QStringList fnames = dir.entryList(QStringList() << "*.dat" ,QDir::Files);
    for (int i = 0; i< fnames.length(); i++)
    {
        //DataFileInfo dfi;
        QFile f(dir.path() + '/' + fnames[i]);
        f.open(QIODevice::ReadOnly);
        f.seek(sizeof(DataFileInfo));
        int bsize = f.size() - sizeof(DataFileInfo);
        char* buf = new char[bsize];
        f.read(buf, bsize);
        for (int j=0; j< bsize; j++)
            if (buf[j] == 0)
            {
                qDebug() << QString("File %1 has gray cell j=%2").arg(fnames[i]).arg(j);
            }
        delete[] buf;
    }
}
void MainWindow::on_ncellsOkButton_clicked()
{
//    CheckFilesIntegrity();
//    return;
    GameStartInfo si(ui->ncellsSpinBox->value(), nullptr,true);
    si.type = ui->typeSpinBox->value();
    si.division = ui->divideSpinBox->value();
    si.playMusic = false;
    si.playSounds = false;
    si.fileNo = 0;
    if (si.type != 0 || si.division !=0)
        loadVertexInfo(&si);
    startGame(si);
    if (si.vertexInfo)
        delete[] si.vertexInfo;
}
#endif

bool dataFileLess (const DataFile& df1, const DataFile& df2)
{
    return df1.size < df2.size;
}
int fileNameToNo(const QString& fn)
{
    QString snum = fn.chopped(4);
    return snum.toInt();
}
void MainWindow::getDataFiles()
{
#ifdef WIN32
    dir.setPath(projectDir + "/shapes");
#else
    dir.setPath(":/shapes");
#endif
    if (!dir.exists())
        dir.setPath("../shapes");
    if (!dir.exists())
    {
        QMessageBox msgBox;
        msgBox.setText("Directory " + dir.absolutePath() +  " does not exist");
        msgBox.exec();
        return;
    }
    maxLevel = 3;
    maxFileNo = 0;
    QStringList fnames = dir.entryList(QStringList() << "*.dat" ,QDir::Files);
    for (int i=0; i< fnames.length(); i++)
    {
        DataFile df;
#ifndef WIN32
        df.resource = true;
#else
        df.resource = false;
#endif
        df.fileNo = fileNameToNo (fnames[i]);
        if (df.fileNo > maxFileNo)
            maxFileNo = df.fileNo;
        QFileInfo f (dir.path() + '/' + fnames[i]);
        df.size = f.size();
        dataFiles.append(df);
        if (df.ncells() > maxLevel)
            maxLevel = df.ncells();
    }
#ifndef WIN32
    QDir shdir(shapesDir);
    QStringList shfnames =  shdir.entryList(QStringList() << "*.dat" ,QDir::Files);
    for (int i=0; i< shfnames.length(); i++)
    {
        DataFile df;
        df.resource = false;
        df.fileNo = fileNameToNo (shfnames[i]);
        if (df.fileNo > maxFileNo)
            maxFileNo = df.fileNo;
        QFileInfo f (shdir.path() + '/' + shfnames[i]);
        df.size = f.size();
        dataFiles.append(df);
        if (df.ncells() > maxLevel)
            maxLevel = df.ncells();
    }
#endif

    std::sort(dataFiles.begin(), dataFiles.end(), dataFileLess);
}

void MainWindow::getCurrMaxLevel()
{
    currMaxLevel = 33;
    return;
    for (int i =0; i< settings.doneInds.length(); i++)
    {
        int nc = dataFiles[settings.doneInds[i]].ncells();
        if (nc > currMaxLevel)
            currMaxLevel = nc;
    }
    int nn = 0;
    for (int i=0; i<dataFiles.length(); i++)
        if (dataFiles[i].ncells() == currMaxLevel)
            nn++;
    for (int i =0; i< settings.doneInds.length(); i++)
        if (dataFiles[settings.doneInds[i]].ncells() == currMaxLevel)
        {
            nn--;
            if (nn == 0)
            {
                currMaxLevel ++;
                return;
            }
        }
}

bool MainWindow::levelIsLocked(int i) const
{
    return dataFiles[i].ncells() > currMaxLevel  && false;
}

bool MainWindow::levelIsDone(int ind) const
{
    for (int j =0; j< settings.doneInds.length(); j++)
        if (ind == settings.doneInds[j])
            return true;
    return false;
}

void MainWindow::downloadNewFiles()
{
    QNetworkAccessManager manager;
    bool newfile = false;
    for (int fno = maxFileNo+1; fno<100000000; fno++)
        if (!downloadShape(&manager, fno))
                break;
    else
         newfile = true;
    if (newfile)
         std::sort(dataFiles.begin(), dataFiles.end(), dataFileLess);
}
bool MainWindow::downloadShape(QNetworkAccessManager *manager, int fno)
{
    const QString shapesURL = "http://games-mgk.h1n.ru/polyhedron/";
    QString datUrl(QString(shapesURL + "%1.dat").arg(fno));
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(datUrl)));
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();
    qDebug() << reply->error();
    if (reply->error() != QNetworkReply::NoError)
    {
        delete reply;
        return false;
    }
    QByteArray bts = reply->readAll();
    delete reply;

    QString pngUrl(QString(shapesURL + "%1.png").arg(fno));
    reply = manager->get(QNetworkRequest(QUrl(pngUrl)));
//    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();
    qDebug() << reply->error();
    if (reply->error() != QNetworkReply::NoError)
    {
        delete reply;
        return false;
    }
    QByteArray pngbts = reply->readAll();
    maxFileNo = fno;
    QFile dataf(QString(shapesDir + "/%1.dat").arg(fno));
    dataf.open(QIODevice::WriteOnly);
    dataf.write(bts);
    QFile pf(QString(shapesDir + "/%1.png").arg(fno));
    pf.open(QIODevice::WriteOnly);
    pf.write(pngbts);
    ////

    DataFile df;
    df.resource = false;
    df.fileNo = fno;
    if (df.fileNo > maxFileNo)
        maxFileNo = df.fileNo;
    df.size = dataf.size();
    dataFiles.append(df);
    if (df.ncells() > maxLevel)
        maxLevel = df.ncells();
    ////

    delete reply;
    return true;

}
int DataFilesModel::rowCount(const QModelIndex &) const
{
    return (mainWindow->dataFiles.length()  + mainWindow->ncols()-1 )
            / mainWindow->ncols();
}

int DataFilesModel::columnCount(const QModelIndex &) const
{
    return mainWindow->ncols();
}

QVariant DataFilesModel::data(const QModelIndex &, int/* role*/) const
{
        return QVariant();
}

void MainWindow::on_dataFilesView_clicked(const QModelIndex &index)
{
//    downloadNewFiles();
   int ind = index.row()* _ncols + index.column();
   if (ind >= dataFiles.length())
       return ;
#ifndef WIN32
//   if (levelIsLocked(ind))
//       return;
#endif
   if (currGameFile.isOpen())
       currGameFile.close();
   QString sdir = projectDir + "/settings";
   QDir dir(sdir);
   QStringList fnames = dir.entryList(QStringList() << "*.unf" ,QDir::Files);
   for (auto fn = fnames.begin(); fn != fnames.end(); fn++)
   {
       QFile f (*fn);
       f.remove();
   }
   currInd = ind;
   startDataGame(ind);
   qDebug() << "File=" << dataFiles[ind].fileNo;
}

ImageDelegate::ImageDelegate(QObject *parent)
{
    mainWindow = (MainWindow* ) parent;
}

void ImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int r = index.row();
    int c = index.column();
    int ind = r* mainWindow->ncols() +c;
    if (ind >= mainWindow->dataFiles.length())
        return ;
    int pixelSize, lockSize;;
    QString s = QString("%1").arg(mainWindow->dataFiles[ind].fileNo);
#ifdef WIN32
    s=mainWindow->projectDir+ "/shapes/" +s;
    pixelSize = 24;
    lockSize =32;
#else
    if (mainWindow->dataFiles[ind].resource)
        s = ":shapes/" +s;
    else
        s = mainWindow->shapesDir + "/" + s;
    pixelSize = 72;
    lockSize = 64;
#endif
    s= s + ".png";
    QPixmap pixmap(s);
    painter->drawPixmap(option.rect, pixmap);
    if (mainWindow->levelIsLocked(ind))
    {
        QPixmap lpixmap(":/images/lock-48.png");
        QRect r(option.rect.x(), option.rect.y() +option.rect.height() -lockSize, lockSize,lockSize);
        painter->drawPixmap(r, lpixmap);
    }
    else if (mainWindow->levelIsDone(ind))
    {
        QFont font;
        font.setPixelSize(pixelSize);
        //font.setBold(true);
        painter->setFont(font);
        QPen pen;
        pen.setColor(QColor(0,255,0));
        painter->setPen(pen);
        QString s1 = "✔";
        int x = option.rect.x();
        int y = option.rect.y() +option.rect.height();
        painter->drawText(x,y,s1);
    }
}

QSize ImageDelegate::sizeHint(const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    int w = (mainWindow->ui->dataFilesView->width()-20) /mainWindow->ncols();
    return QSize(w,w);
}
#ifdef WIN32
void MainWindow::on_serviceButton_clicked()
{
    QString ssaveDir = "c:/Projects/Qt/SaveShapes";
    QString sshapesDir = "c:/Projects/Qt/Paint-Polyhedron/shapes";
    QDir shapesDir(sshapesDir);
    QStringList fnames = shapesDir.entryList(QStringList() << "*.dat" ,QDir::Files);
    for (int i=0; i< fnames.length(); i++)
    {
        QString s1 = sshapesDir + '/' + fnames[i];
        QString s2 = ssaveDir + '/' + fnames[i];
        if (QFileInfo(s2).exists())
        {
            shapesDir.remove(fnames[i]);
            QString spng = fnames[i].chopped(3) + "png";
            shapesDir.remove(spng);
        }
    }
}
#endif
void MainWindow::networkFinished()
{
    qDebug() << "networkFinished";
}

void MainWindow::networkError()
{
    qDebug() << "networkError";

}
