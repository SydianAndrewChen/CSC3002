#ifndef CALENDAR_H
#define CALENDAR_H

#include <QMainWindow>
#include <QMap>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class Calendar; }
QT_END_NAMESPACE

class Calendar : public QMainWindow
{
    Q_OBJECT

public:
    Calendar(QWidget *parent = nullptr);
    ~Calendar();

    int pid;

private slots:
    void on_calendarWidget_selectionChanged();

    void on_pushButtonSave_clicked();

signals:
    void closed(long pid);

private:
    Ui::Calendar *ui;
    QMap<QString, QString> memo;



    void readMemoFile(const char * path);
    void writeMemoFile(const char * path);
};
#endif // CALENDAR_H
