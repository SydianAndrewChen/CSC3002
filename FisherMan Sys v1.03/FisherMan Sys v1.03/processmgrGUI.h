#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <processmgr.h>
#include <QTimer>
#include <QStandardItemModel>
#include <QTableView>
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class ProcessManagerGUI; }
QT_END_NAMESPACE

class ProcessManagerGUI : public QMainWindow
{
    Q_OBJECT

public:
    ProcessManagerGUI(QWidget *parent = nullptr);
    ~ProcessManagerGUI();

    void constructRunningTable();
    void createItemsARowRunning(int rowNo, QIcon icon, QString Name, unsigned int storage, long pid, const char* start);
    void constructHistoryTable();
    void createItemsARowHistory(int rowNo, QIcon icon,QString Name, const char* start, const char* stop, double duration);

public slots:
    void update();

private slots:
    void on_pushButtonStop_clicked();

private:
    Ui::ProcessManagerGUI *ui;
    ProcessManager * ProMgr;

    QTimer * timer;
    int flag = 0;
    friend class System;

};
#endif // MAINWINDOW_H
