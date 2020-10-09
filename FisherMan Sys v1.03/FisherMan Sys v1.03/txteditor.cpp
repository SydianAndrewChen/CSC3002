#include "txteditor.h"
#include "ui_txteditor.h"
#include "errexception.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QFont>
#include <QFontDialog>
#include <QMessageBox>

TxtEditor::TxtEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TxtEditor)
{
    ui->setupUi(this);
    ui->actionCut->setIcon(QIcon(":/cut.png"));
    ui->actionCopy->setIcon(QIcon(":/copy.png"));
    ui->actionPaste->setIcon(QIcon(":/paste.png"));
    ui->actionSave_2->setIcon(QIcon(":/save.png"));
    ui->actionSave_as->setIcon(QIcon(":/save_as.png"));
    ui->actionUndo->setIcon(QIcon(":/drawback.png"));
    this->setCentralWidget(ui->textEdit);
    this->setWindowIcon(QIcon(":/text_editor.png"));
}

TxtEditor::~TxtEditor()
{
    delete ui;
}

void TxtEditor::on_actionNew_triggered()
{
    filePath = "";
    ui->textEdit->setPlainText("");
}

void TxtEditor::on_actionOpen_triggered()
{
    emit openFile(this, 1);
}

void TxtEditor::on_actionSave_2_triggered()
{
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        Error file_existence_error = generateErr("File not found!");
        writeErrFile(file_existence_error);
        QMessageBox::critical(this, "ERROR", "ERROR: " + QString::fromStdString(file_existence_error.errorMessage));
        return;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    ui->textEdit->setPlainText(text);
    file.close();
}

void TxtEditor::on_actionSave_as_triggered()
{
    emit saveAs(this, 0);
}

void TxtEditor::openFile(QString filePath)
{
    this->filePath = filePath;
    QFile file(filePath);
    // filePath = fileName;
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "..","File not open!");
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setPlainText(text);
    file.close();
}

void TxtEditor::saveFile(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "..","File not open!");
        return;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    ui->textEdit->setPlainText(text);
    file.close();
}

void TxtEditor::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void TxtEditor::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void TxtEditor::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void TxtEditor::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void TxtEditor::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void TxtEditor::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    if (ok){
        ui->textEdit->setFont(font);
    } else return;
}



