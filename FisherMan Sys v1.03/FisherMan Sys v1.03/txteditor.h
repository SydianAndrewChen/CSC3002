#ifndef TXTEDITOR_H
#define TXTEDITOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class TxtEditor; }
QT_END_NAMESPACE

class TxtEditor : public QMainWindow
{
    Q_OBJECT

public:
    TxtEditor(QWidget *parent = nullptr);
    ~TxtEditor();
    int pid;

signals:
    void closed(long pid);

    void saveAs(QMainWindow *, bool mode);

    void openFile(QMainWindow *, bool mode);

private slots:
    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionFont_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_2_triggered();

    void on_actionSave_as_triggered();

    void openFile(QString path);

    void saveFile(QString path);

private:
    Ui::TxtEditor *ui;
    QString filePath;
    QString fileName;

    friend class System;
};
#endif // TXTEDITOR_H
