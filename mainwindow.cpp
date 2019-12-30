#include "mainwindow.h"
#include "mainwidget.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QtMath>
#include <QMessageBox>
#include <QPainter>
#include <QScroller>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), mainWidget(nullptr), _ncols(5)
{
    ui->setupUi(this);
    getProjectDir();
    getDataFiles();
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
    QString exedir = QDir(".").absolutePath();
    QDir dir("shapes");
    if (!dir.exists())
        dir.setPath("../Paint-Polyhedron");
    bool b = dir.exists();
    projectDir = dir.absolutePath();
    qDebug() << "projectDir=" << projectDir;
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
    QString fn = dir.path() + '/' + QString("%1.dat").arg(dataFiles[ind].fileNo);
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
    if (si.type !=0 || si.division != 0)
        loadVertexInfo(&si);
    si.playMusic = settings.playMusic;
    si.playSounds = settings.playSounds;
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

void MainWindow::on_ncellsOkButton_clicked()
{
 /*DataFileInfo head;
    for (int i=0; i<10; i++)
        head.c[i] = 'X';
    QString sdir("c:/Projects/Qt/Paint Polyhedron/shapes");
    QDir dir (sdir);
    if (!dir.exists())
        return;
    QStringList fnames = dir.entryList(QStringList() << "*.dat" ,QDir::Files);
    head.w[0] = head.w[1] = 0;
    for (int i =0 ; i<fnames.length(); i++)
    {
        QFile f (sdir + "/" + fnames[i]);
        if (!f.exists())
            return;
        if (!f.open(QIODevice::ReadWrite))
            return;
        f.read(head.buf, f.size());
        int nc = qSqrt(f.size()/6);
        head.w[2] = nc;
        f.seek(0);
        f.write((char*)&head, f.size()+16);
    }
    return;*/
    GameStartInfo si(ui->ncellsSpinBox->value(), nullptr,true);
    si.type = ui->typeSpinBox->value();
    si.division = ui->divideSpinBox->value();
    si.playMusic = false;
    si.playSounds = false;
    if (si.type != 0 || si.division !=0)
        loadVertexInfo(&si);
    startGame(si);
    if (si.vertexInfo)
        delete[] si.vertexInfo;
}
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
    QStringList fnames = dir.entryList(QStringList() << "*.dat" ,QDir::Files);
    for (int i=0; i< fnames.length(); i++)
    {
        DataFile df;
        df.fileNo = fileNameToNo (fnames[i]);
        QFileInfo f (dir.path() + '/' + fnames[i]);
        df.size = f.size();
        dataFiles.append(df);
        if (df.ncells() > maxLevel)
            maxLevel = df.ncells();
    }
    std::sort(dataFiles.begin(), dataFiles.end(), dataFileLess);
}

void MainWindow::getCurrMaxLevel()
{
    currMaxLevel = 3;
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
int DataFilesModel::rowCount(const QModelIndex &parent) const
{
    return (mainWindow->dataFiles.length()  + mainWindow->ncols()-1 )
            / mainWindow->ncols();
}

int DataFilesModel::columnCount(const QModelIndex &parent) const
{
    return mainWindow->ncols();
}

QVariant DataFilesModel::data(const QModelIndex &index, int role) const
{
        return QVariant();
}

void MainWindow::on_dataFilesView_clicked(const QModelIndex &index)
{
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
    s = ":shapes/" +s;
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
        QRect r =  option.rect;
        int x = option.rect.x();
        int y = option.rect.y() +option.rect.height();
        painter->drawText(x,y,s1);
    }
}

QSize ImageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int w = (mainWindow->ui->dataFilesView->width()-20) /mainWindow->ncols();
    return QSize(w,w);
}
