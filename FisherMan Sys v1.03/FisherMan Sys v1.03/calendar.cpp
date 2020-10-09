#include "calendar.h"
#include "ui_calendar.h"
#include "errexception.h"
#include <QDebug>

Calendar::Calendar(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calendar)
{
    ui->setupUi(this);
    this->setWindowTitle("Calendar & Memo");
    this->setWindowIcon(QIcon(":/calender.png"));
    readMemoFile("Memo.txt");
}

Calendar::~Calendar()
{
    emit closed(this->pid);
    writeMemoFile("Memo.txt");
    delete ui;
}

void Calendar::readMemoFile(const char * path)
{
    QString text;
    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        Error file_existence_error = generateErr("File not found!");
        writeErrFile(file_existence_error);
        QMessageBox::critical(this, "ERROR", "ERROR: " + QString::fromStdString(file_existence_error.errorMessage));
        return;
    }
    QTextStream in(&file);
    QString date;
    while(in.readLineInto(&text)){
        if (text.startsWith("$Date:")){
            date =  text.section(":",1,1);
            date = date.trimmed();
            memo[date] = "";
        } else { memo[date] += text;}
    }
    file.close();
}

void Calendar::writeMemoFile(const char * path)
{
    QFile file(path);
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        Error file_existence_error = generateErr("File not found!");
        writeErrFile(file_existence_error);
        QMessageBox::critical(this, "ERROR", "ERROR: " + QString::fromStdString(file_existence_error.errorMessage));
        return;
    }
    QTextStream out(&file);
    for (auto date: memo.keys()) {
        out << "$Date: " << date << endl;
        out << memo[date] << endl;
    }
    file.flush();
    file.close();
}

void Calendar::on_calendarWidget_selectionChanged()
{
    QDate dt=ui->calendarWidget->selectedDate();
    QString date=dt.toString("yyyy-MM-dd");
    ui->plainTextEdit->setPlainText(memo[date]);
}

void Calendar::on_pushButtonSave_clicked()
{
    QDate dt=ui->calendarWidget->selectedDate();
    QString date=dt.toString("yyyy-MM-dd");
    QString events = ui->plainTextEdit->toPlainText();
    memo[date] = events;
}
