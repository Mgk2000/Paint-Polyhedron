#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAbstractTableModel>
#include <QDir>
#include <QStyleOptionViewItem>
#include <QMainWindow>
#include <QAbstractItemDelegate>
#include "mainwidget.h"
#include<QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QtMath>
struct DataFileInfo{
    ushort type, division, ncells;
    char c[10];
};

class QTableView;
namespace Ui {
class MainWindow;
}
class MainWindow;
struct DataFile
{
    int fileNo;
    int size;
    int ncells() const {return (int)qSqrt((size-16)/6);}
};

class DataFilesModel : public QAbstractTableModel
{
public:
    DataFilesModel(MainWindow * mw): mainWindow(mw) {}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    MainWindow* mainWindow;
};
class ImageDelegate : public QAbstractItemDelegate
{
public:
    ImageDelegate(QObject * parent = nullptr);

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
    MainWindow* mainWindow;
};
struct Settings
{
    Settings() : playMusic(true), playSounds(true) {}
    bool playMusic;
    bool playSounds;
    QList <int> doneLevels;
    QList <int> doneInds;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QList<DataFile> dataFiles;
    void startDataGame(int ind, QString unfinished = "");
    void startGame(GameStartInfo& si);
    QDir dir;
    int ncols () const {return _ncols;}
    Ui::MainWindow *ui;
    void setTableView();
    void readSettings();
    void writeSettings();
    bool startUnfinishedGame();
    QString getUnfinishedGame();
    QFile currGameFile;
    Settings settings;
    QString projectDir;
    void getProjectDir();
    void levelDone();
    bool levelIsLocked(int i) const;
    bool levelIsDone(int i) const;
private slots:
    void on_ncellsOkButton_clicked();
    void on_dataFilesView_clicked(const QModelIndex &index);

private:
    void getDataFiles();
    DataFilesModel * dataFilesModel;
    MainWidget* mainWidget;
    ImageDelegate* itemDelegate;
    int _ncols;
    int currInd;
    void getCurrMaxLevel();
    int maxLevel, currMaxLevel;
    int indFromNo(int fno) const;
public slots:
    void applicationStateChanged(Qt::ApplicationState state);
};

#endif // MAINWINDOW_H
