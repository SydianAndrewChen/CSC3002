#include "system.h"
#include "ui_system.h"
#include "main_game_window.h"
#include "errexception.h"
#include <QTime>
#include <QTimer>

using namespace std;
System::System(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::System)
{
    ui->setupUi(this);
    this->setWindowTitle("FisherMan Sys ver1.0");
    this->setWindowIcon(QIcon(":/system.png"));
    ui->processMgr->setIcon(QIcon(":/ProcessMgr.png"));
    ui->snakeGame->setIcon(QIcon(":/snake.png"));
    ui->mineSweep->setIcon(QIcon(":/MineSweep.png"));
    ui->txtEditor->setIcon(QIcon(":/text_editor.png"));
    ui->fileSystem->setIcon(QIcon(":/case.png"));
    ui->simulatedServerK->setIcon(QIcon(":/Simulated_Server_K.jpg"));
    ui->calendar->setIcon(QIcon(":/calender.png"));
    ui->calculator->setIcon(QIcon(":/calculator.png"));
    ui->closedown->setIcon(QIcon(":/close.png"));
    ProMgr = new ProcessManager;
    fs = new FileSystem;
    QTimer *time_clock=new QTimer(this);
    connect(time_clock,SIGNAL(timeout()),this,SLOT(showTime()));
    time_clock->start(500);
}

void System::showTime() {
    QTime time = QTime::currentTime();
    QString str = time.toString("hh:mm:ss");
    ui->lcdNumber->display(str);
}

System::~System()
{
    delete ui;
}

void System::updateProMgr(long pid)
{
    this->ProMgr->stopProcess(pid);
}

void System::on_processMgr_clicked()
{
    ProMgrGUI = new ProcessManagerGUI(this);
    ProMgrGUI->ProMgr = this->ProMgr;
    ProMgrGUI->show();
}

void System::on_calculator_clicked()
{
    checkErr();
    CalculatorGUI = new Calculator(this);
    CalculatorGUI->setAttribute(Qt::WA_DeleteOnClose);
    CalculatorGUI->show();
    CalculatorGUI->pid = this->ProMgr->generateProcess("Calculator", true, CalculatorGUI); // Let ProMgr be informed that this process is started.
    connect(CalculatorGUI, SIGNAL(closed(long)), this, SLOT(updateProMgr(long)));
    // QMessageBox::warning(this, "Ah Oh!", "Sorry, this application is not constructed yet, please wait for newer version!");
}

void System::on_calendar_clicked()
{
    checkErr();
    CalendarGUI = new Calendar(this);
    CalendarGUI->setAttribute(Qt::WA_DeleteOnClose);
    CalendarGUI->show();
    CalendarGUI->pid = this->ProMgr->generateProcess("Calendar", true, CalendarGUI); // Let ProMgr be informed that this process is started.
    connect(CalendarGUI, SIGNAL(closed(long)), this, SLOT(updateProMgr(long)));
}

void System::on_fileSystem_clicked()
{
    fsGUI = new FileSystemGUI(this);
    fsGUI->setAttribute(Qt::WA_DeleteOnClose);
    fsGUI->fs->init();
    fsGUI->initMode();
    fsGUI->show();
    connect(fsGUI, SIGNAL(openTxtFile(QString)), this, SLOT(fromFileManagerOpenTxtEditor(QString)));
}

void System::on_txtEditor_clicked()
{
    checkErr();
    TEditorGUI = new TxtEditor(this);
    TEditorGUI->setAttribute(Qt::WA_DeleteOnClose);
    TEditorGUI->show();
    TEditorGUI->pid = this->ProMgr->generateProcess("TEditor", true, TEditorGUI); // Let ProMgr be informed that this process is started.
    connect(TEditorGUI, SIGNAL(closed(long)), this, SLOT(updateProMgr(long)));
    connect(TEditorGUI, SIGNAL(openFile(QMainWindow*, bool)), this, SLOT(fromTxtEditorOpenFileManager(QMainWindow*, bool)));
    connect(TEditorGUI, SIGNAL(saveAs(QMainWindow*, bool)), this, SLOT(fromTxtEditorOpenFileManager(QMainWindow*, bool)));
}

void System::fromFileManagerOpenTxtEditor(QString true_path)
{
    TEditorGUI = new TxtEditor(this);
    TEditorGUI->filePath = true_path;
    TEditorGUI->openFile(true_path);
    TEditorGUI->show();
    TEditorGUI->pid = this->ProMgr->generateProcess("TEditor", true, TEditorGUI);
    connect(TEditorGUI, SIGNAL(closed(long)), this, SLOT(updateProMgr(long)));
    connect(TEditorGUI, SIGNAL(saveAs(QMainWindow*)), this, SLOT(fromTxtEditorOpenFileManager(QMainWindow*)));
}

void System::fromTxtEditorOpenFileManager(QMainWindow * txtEditor, bool mode)
{
    fsGUI = new FileSystemGUI(txtEditor);
    fsGUI->setAttribute(Qt::WA_DeleteOnClose);
    fsGUI->saveMode = !mode;
    fsGUI->openMode = mode;
    fsGUI->initMode();
    fsGUI->fs->init();
    fsGUI->show();
    if (!mode) connect(fsGUI, SIGNAL(transmitTruePath(QString)), txtEditor, SLOT(saveFile(QString)));
    if (mode) connect(fsGUI, SIGNAL(transmitTruePath(QString)), txtEditor, SLOT(openFile(QString)));
}

void System::on_simulatedServerK_clicked()
{
    checkErr();
    for (auto pro: this->ProMgr->ProList){
        QString s = pro.first;
        if (s == "Simulated Server K") return;
    }
    logInGUI = new LogInGUI(this);
    logInGUI->setAttribute(Qt::WA_DeleteOnClose);
    logInGUI->sskGUI = NULL;
    logInGUI->show();
    logInGUI->pid = this->ProMgr->generateProcess("Simulated Server K Log In", true, logInGUI); // Let ProMgr be informed that this process is started.
    connect(logInGUI, SIGNAL(closed(long)), this, SLOT(updateProMgr(long)));
    connect(logInGUI, SIGNAL(isLogin(bool)), this, SLOT(updateSSK()));
}

void System::updateSSK()
{
    SSKGUI = logInGUI->sskGUI;
    SSKGUI->setAttribute(Qt::WA_DeleteOnClose);
    SSKGUI->show();
    SSKGUI->pid = this->ProMgr->generateProcess("Simulated Server K", true, SSKGUI);
    connect(SSKGUI, SIGNAL(closed(long)), this, SLOT(updateProMgr(long)));
}

void System::on_snakeGame_clicked()
{
    checkErr();
    SnakeGUI = new Snake();
    SnakeGUI->setAttribute(Qt::WA_DeleteOnClose);
    SnakeGUI->show();
    SnakeGUI->pid = this->ProMgr->generateProcess("Snake", true, SnakeGUI);
    SnakeGUI->ProMgr = this->ProMgr;
}

void System::on_mineSweep_clicked()
{
    checkErr();
    gamewindow = new main_game_window(this);
    gamewindow->setAttribute(Qt::WA_DeleteOnClose);
    gamewindow->setWindowFlag(Qt::Window, true);
    gamewindow->show();
    gamewindow->pid = this->ProMgr->generateProcess("Mine Sweep", true, gamewindow);
    connect(gamewindow, SIGNAL(closed(long)), this, SLOT(updateProMgr(long)));
}

void System::checkErr()
{
    if (this->ProMgr->count == this->ProMgr->MAXIMUM_PRO) {
        Error process_maximum_error = generateErr("Process exceeded maximum limit.");
        writeErrFile(process_maximum_error);
        QMessageBox::critical(this, "ERROR", "ERROR: " + QString::fromStdString(process_maximum_error.errorMessage));
        return;
    }
    if (this->ProMgr->current_storage > this->ProMgr->MAXSIZE) {
        Error storage_maximum_error = generateErr("Storage exceeded maximum limit.");
        writeErrFile(storage_maximum_error);
        QMessageBox::critical(this, "ERROR", "ERROR: " + QString::fromStdString(storage_maximum_error.errorMessage));
        return;
    }
}

void System::on_restart_clicked()
{
    QMessageBox::warning(this, "Ah Oh!", "Sorry, this function is not constructed yet, please wait for newer version!");
}

void System::on_closedown_clicked()
{
    this->close();
}

void System::on_checkBoxisFullScreen_clicked(bool checked)
{
    (checked == true) ? this->showFullScreen(): this->showNormal();
}
