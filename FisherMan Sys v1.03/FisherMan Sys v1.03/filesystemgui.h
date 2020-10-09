#ifndef FILESYSTEMGUI_H
#define FILESYSTEMGUI_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QMap>
#include "filesystem.h"
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class FileSystemGUI; }
QT_END_NAMESPACE

struct TempContainer{
    QTreeWidgetItem * temp_itm = NULL;
    string home_path = " ";
    bool isCopy = true;
};

class FileSystemGUI : public QMainWindow
{
    Q_OBJECT

public:
    FileSystemGUI(QWidget *parent = nullptr);
    ~FileSystemGUI();

    void addItm(File * file, QTreeWidgetItem * parent = NULL);
    void init(File * root);
    void initMode();

private slots:

    void on_pushButtonNew_clicked();

    void on_pushButtonDelete_clicked();

    void on_treeWidgetFileSystem_itemClicked(QTreeWidgetItem *item);

    void on_pushButtonCopy_clicked();

    void on_pushButtonCut_clicked();

    void on_pushButtonPaste_clicked();

    void on_pushButtonOpen_clicked();

    void on_pushButtonOK_clicked();

signals:
    void itemClicked(QTreeWidgetItem*, int);
    void openTxtFile(QString);
    void transmitTruePath(QString);

private:
    string checkRepeated(string parent_path, string new_path);
    void changeName(string & new_path);

    Ui::FileSystemGUI *ui;

    FileSystem *  fs;
    QMap<QTreeWidgetItem *, File *> fileSearch;
    QTreeWidgetItem *searchItem(string file_path);

    TempContainer * temp;

    friend class System;
    bool openMode = false;
    bool saveMode = false;
};
#endif // FILESYSTEMGUI_H
