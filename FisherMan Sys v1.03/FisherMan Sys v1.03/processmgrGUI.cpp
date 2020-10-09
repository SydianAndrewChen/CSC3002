#include "processmgr.h"
#include "processmgrGUI.h"
#include "ui_processmgrGUI.h"
#include "errexception.h"
#include <QMessageBox>
#include <QDebug>

ProcessManagerGUI::ProcessManagerGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProcessManagerGUI)
{
    ui->setupUi(this);
    this->setWindowTitle("ProMgr");
    this->setWindowIcon(QIcon(":/ProcessMgr.png"));

    ui->tableWidgetHistory->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetHistory->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetHistory->setFocusPolicy(Qt::NoFocus);
    ui->tableWidgetHistory->setColumnWidth(0,500);
    ui->tableWidgetHistory->setColumnWidth(1,200);
    ui->tableWidgetHistory->setColumnWidth(2,200);

    ui->tableWidgetRunning->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetRunning->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetRunning->setFocusPolicy(Qt::NoFocus);
    ui->tableWidgetRunning->setColumnWidth(0,500);
    ui->tableWidgetRunning->setColumnWidth(1,200);
    ui->tableWidgetRunning->setColumnWidth(2,200);
    ui->tableWidgetRunning->setColumnWidth(3,200);
    ui->tabWidget->setCurrentIndex(0);

    ProMgr = new ProcessManager;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);
}

ProcessManagerGUI::~ProcessManagerGUI()
{
    delete ProMgr;
    delete ui;
    delete timer;
}

void ProcessManagerGUI::update()
{
    if (this->flag != this->ProMgr->sign){
        constructRunningTable();
        constructHistoryTable();
        flag = this->ProMgr->sign;
    }
}

void ProcessManagerGUI::constructRunningTable()
{
    QTableWidget *table = this->ui->tableWidgetRunning;
    int i = table->rowCount();
    do
    {
        table->removeRow(i);
        i--;
    }while (i >= 0);
    for (auto & el: this->ProMgr->ProList)
    {
        QString qs = el.first;
        for (auto & proEl: el.second)
        {
            createItemsARowRunning(table->rowCount(),
                            proEl.icon(), qs, proEl.storage, proEl.pid, proEl.clock.startTm);
        }
    }
}

/* Put all the information of a process into the tableWidget. */
void ProcessManagerGUI::createItemsARowRunning(int rowNo, QIcon icon, QString Name, unsigned int storage, long pid, const char* start)
{
    QTableWidgetItem *item;
    QString str;
    if (Name.length() > 0) {
        item = new QTableWidgetItem(Name);
    } else {
        item = new QTableWidgetItem("Null");
    }
    item->setIcon(icon);
    ui->tableWidgetRunning->insertRow(ui->tableWidgetRunning->rowCount());
    ui->tableWidgetRunning->setItem(rowNo, 0, item);
    QString sto = QString::number(storage);
    item = new QTableWidgetItem(sto);
    ui->tableWidgetRunning->setItem(rowNo, 1, item);
    QString id = QString::number(pid,10);
    item = new QTableWidgetItem(id);
    ui->tableWidgetRunning->setItem(rowNo, 2, item);
    QString s = start;
    item = new QTableWidgetItem(s);
    ui->tableWidgetRunning->setItem(rowNo, 3, item);
}

void ProcessManagerGUI::constructHistoryTable()
{
    QTableWidget *table = this->ui->tableWidgetHistory;
    int i = table->rowCount();
    do
    {
        table->removeRow(i);
        i--;
    }while (i >= 0);
    this->ProMgr->displayHistory();
    for (auto & el: this->ProMgr->History)
    {
        QString qs = el.first;
        for (auto & proEl: el.second)
        {
            createItemsARowHistory(table->rowCount(),
                            proEl.icon(), qs, proEl.clock.startTm, proEl.clock.stopTm, proEl.clock.durationTime);
        }
    }
}

void ProcessManagerGUI::createItemsARowHistory(int rowNo, QIcon icon, QString Name, const char *start, const char *stop, double duration)
{
    QTableWidgetItem *item;
    QString str;
    if (Name.length() > 0) {
        item = new QTableWidgetItem(Name);
    } else {
        item = new QTableWidgetItem("Null");
    }
    item->setIcon(icon);
    ui->tableWidgetHistory->insertRow(ui->tableWidgetHistory->rowCount());
    ui->tableWidgetHistory->setItem(rowNo, 0, item);
    QString sta = start;
    item = new QTableWidgetItem(sta);
    ui->tableWidgetHistory->setItem(rowNo, 1, item);
    QString sto = stop;
    item = new QTableWidgetItem(sto);
    ui->tableWidgetHistory->setItem(rowNo, 2, item);
    QString dur = QString::number(duration);
    item = new QTableWidgetItem(dur);
    ui->tableWidgetHistory->setItem(rowNo, 3, item);
}


/* Stop a process. */
void ProcessManagerGUI::on_pushButtonStop_clicked()
{
    int rowNo = ui->tableWidgetRunning->currentRow();
    if (rowNo != -1)
    {
        long pid = ui->tableWidgetRunning->item(rowNo, 2)->text().toLong();
        if (ProMgr->search(pid)->del == false)
        {
            Error process_del_error = generateErr("Process not deletable!");
            writeErrFile(process_del_error);
            QMessageBox::critical(this, "ERROR", "ERROR: " + QString::fromStdString(process_del_error.errorMessage));
            return;
        }
        if (strncmp(ProMgr->search(pid)->name, "NULL", 4)) ProMgr->search(pid)->GUIptr->deleteLater();
        ui->tableWidgetRunning->removeRow(rowNo);
        ProMgr->stopProcess(pid);
    }
}
