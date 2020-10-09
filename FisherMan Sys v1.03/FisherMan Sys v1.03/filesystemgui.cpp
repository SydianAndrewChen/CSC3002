#include "filesystemgui.h"
#include "ui_filesystemgui.h"
#include "errexception.h"
#include <QFocusFrame>

FileSystemGUI::FileSystemGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FileSystemGUI)
{
    ui->setupUi(this);
    this->setWindowTitle("File Manager");
    this->setWindowIcon(QIcon(":/case.png"));
    ui->treeWidgetFileSystem->clear();
    ui->treeWidgetFileSystem->setColumnWidth(0,this->width()/2);
    ui->treeWidgetFileSystem->setColumnWidth(1, this->width()/6);
    ui->treeWidgetFileSystem->setColumnWidth(2, this->width()/3);
    ui->treeWidgetFileSystem->setColumnWidth(3, this->width()/6);
    fs = new FileSystem;
    temp = new TempContainer;
    addItm(fs->root);
    init(fs->root);
    connect(ui->treeWidgetFileSystem, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(on_treeWidgetFileSystem_itemClicked(QTreeWidgetItem*, int)));
}

FileSystemGUI::~FileSystemGUI()
{
    delete fs;
    delete temp;
    delete ui;
}

void FileSystemGUI::init(File * root)
{
    for (auto child: root->children) {
        addItm(child, searchItem(root->file_path()));
        init(child);
    }
}

void FileSystemGUI::initMode()
{
    if ((saveMode)||(openMode)) {
        ui->pushButtonCut->deleteLater();
        ui->pushButtonNew->deleteLater();
        ui->pushButtonCopy->deleteLater();
        ui->pushButtonOpen->deleteLater();
        ui->pushButtonPaste->deleteLater();
        ui->pushButtonDelete->deleteLater();
    }else {ui->pushButtonOK->deleteLater();}
}

void FileSystemGUI::addItm(File *file, QTreeWidgetItem * parent)
{
    QTreeWidgetItem * itm;
    if (parent == NULL) {
        itm = new QTreeWidgetItem(ui->treeWidgetFileSystem);
    }else{
        itm = new QTreeWidgetItem();
        if (fileSearch[parent]->type != 0) {
            Error file_status_error = generateErr("File not available!");
            writeErrFile(file_status_error);
            QMessageBox::critical(this, "ERROR", "ERROR: " + QString::fromStdString(file_status_error.errorMessage));
            return;
        }
    }
    itm->setIcon(0, file->icon());
    QString name = QString::fromStdString(file->file_name).section(".", 0, 0);
    qDebug() << "name " << name << endl;
    itm->setText(0, name);
    itm->setText(1, QString::fromStdString(file->fileType())); // TODO: FileType
    QString time = QString::fromStdString(file->created_time);
    QString date = time.split("_")[0];
    QString clock = time.split("_")[1];
    time = date + " " + clock;
    itm->setText(2, time);
    itm->setText(3, QString::number(file->file_size));
    fileSearch[itm] = file;
    if (parent != NULL) parent->addChild(itm);
}

void FileSystemGUI::on_pushButtonNew_clicked()
{
    QString des_path = ui->lineEditPath->text();

    string path = des_path.toStdString();
    QTreeWidgetItem * itm = ui->treeWidgetFileSystem->currentItem();
    QString current_path = QString::fromStdString(fileSearch[itm]->file_path());
    if (fs->locateFile(current_path.toStdString()) == NULL) {
            Error file_existence_error = generateErr("File not found!");
            writeErrFile(file_existence_error);
            QMessageBox::critical(this, "ERROR", "ERROR: " + QString::fromStdString(file_existence_error.errorMessage)
                                  + "\nPlease check if you have entered the right path.");
            return;
    }
    for (int i = 0; i < current_path.size(); i++){
        if (des_path[i] != current_path[i]) return;
    }
    path = checkRepeated(current_path.toStdString(), path);
    fs->createFile(path);
    File * file = fs->locateFile(path);
    cout << "!" << file->file_name << endl;
    string name = QString::fromStdString(file->file_name).split(".")[0].toStdString();
    file->true_path = name + QString::number(file->file_id).toStdString() + file->fileType();
    this->addItm(file, itm);
    QFile new_file(QString::fromStdString(file->true_path));
    new_file.open(QFile::WriteOnly | QFile::Text);
    new_file.close();
}

string FileSystemGUI::checkRepeated(string parent_path, string new_path)
{
    File * parent = fs->locateFile(parent_path);
    for (auto child: parent->children){
        if (child->file_path() == new_path) {
            changeName(new_path);
            checkRepeated(parent_path, new_path);
        }
    }
    return new_path;
}

void FileSystemGUI::changeName(string &new_path)
{
    QStringList filePath = QString::fromStdString(new_path).split("/");
    QString fileName = filePath[filePath.size()-1];
    QString name = fileName.split(".")[0];
    qDebug() << filePath << fileName << name;
    new_path = (QString::fromStdString(new_path).section("/", 0, filePath.size()-2) + "/" +name+"(1)."+fileName.split(".")[1]).toStdString();
}

void FileSystemGUI::on_pushButtonOK_clicked()
{
    if (saveMode){
        QString des_path = ui->lineEditPath->text();
        string path = des_path.toStdString();
        QTreeWidgetItem * itm = ui->treeWidgetFileSystem->currentItem();
        QString current_path = QString::fromStdString(fileSearch[itm]->file_path());
        if (fileSearch[itm]->file_path() == path) return;
        fs->createFile(path);
        File * file = fs->locateFile(path);
        string name = QString::fromStdString(file->file_name).split(".")[0].toStdString();
        file->true_path = name + QString::number(file->file_id).toStdString() + file->fileType();
        this->addItm(file, itm);
        emit transmitTruePath(QString::fromStdString(file->true_path));
    } else if (openMode){
        QTreeWidgetItem * itm = searchItem(ui->lineEditPath->text().toStdString());
        File * file = fileSearch[itm];
        emit transmitTruePath(QString::fromStdString(file->true_path));
    }
    this->close();
}


void FileSystemGUI::on_pushButtonDelete_clicked()
{
    string path = ui->lineEditPath->text().toStdString();
    if (fs->locateFile(path) == NULL) {
            Error file_existence_error = generateErr("File not found!");
            writeErrFile(file_existence_error);
            QMessageBox::critical(this, "ERROR", "ERROR: " + QString::fromStdString(file_existence_error.errorMessage)
                                  + "\nPlease check if you have entered the right path.");
            return;
    }else if (!(fs->locateFile(path)->isDeletable)){
        Error file_priority_error = generateErr("File not deletable!");
        writeErrFile(file_priority_error);
        QMessageBox::critical(this, "ERROR", "ERROR: " + QString::fromStdString(file_priority_error.errorMessage));
        return;
    }
    QTreeWidgetItem * itm = searchItem(path);
    fs->deleteFile(path);
    itm->parent()->removeChild(itm);
}

void FileSystemGUI::on_treeWidgetFileSystem_itemClicked(QTreeWidgetItem *item)
{
    ui->lineEditPath->setText(QString::fromStdString(fileSearch[item]->file_path()));
    if ((!saveMode)&&(!openMode)){
        if (fileSearch[item]->isSys == true) {
            ui->pushButtonCopy->setVisible(false);
            ui->pushButtonCut->setVisible(false);
            ui->pushButtonNew->setVisible(false);
            ui->pushButtonPaste->setVisible(false);
            ui->pushButtonDelete->setVisible(false);
            ui->pushButtonOpen->setVisible(false);
        }else {
            ui->pushButtonCopy->setVisible(true);
            ui->pushButtonCut->setVisible(true);
            ui->pushButtonNew->setVisible(true);
            ui->pushButtonPaste->setVisible(true);
            ui->pushButtonDelete->setVisible(true);
            ui->pushButtonOpen->setVisible(true);
        }
    }
}

void FileSystemGUI::on_pushButtonCopy_clicked()
{
    temp->temp_itm = ui->treeWidgetFileSystem->currentItem();
    temp->home_path = fileSearch[temp->temp_itm]->file_path();
    temp->isCopy = true;
}

void FileSystemGUI::on_pushButtonCut_clicked()
{
    temp->temp_itm = ui->treeWidgetFileSystem->currentItem();
    temp->home_path = fileSearch[temp->temp_itm]->file_path();
    temp->isCopy = false;
}

void FileSystemGUI::on_pushButtonPaste_clicked()
{
    if (temp->home_path == " ") return;
    QTreeWidgetItem * itm = ui->treeWidgetFileSystem->currentItem();
    string des_path = fileSearch[itm]->file_path();
    if (temp->isCopy) {
        File * file = fs->copy(temp->home_path, des_path);
        addItm(file, itm);
    }else {
        QTreeWidgetItem * hom = searchItem(temp->home_path);
        hom->parent()->removeChild(hom);
        File * file = fs->cut(temp->home_path, des_path);
        addItm(file, itm);
    }
}

void FileSystemGUI::on_pushButtonOpen_clicked()
{
    QTreeWidgetItem * itm = ui->treeWidgetFileSystem->currentItem();
    QString true_path = QString::fromStdString(fileSearch[itm]->true_path);
    if (fileSearch[itm]->type == 1) {
        emit openTxtFile(true_path);
    }else{
        Error file_status_error = generateErr("File not available!");
        writeErrFile(file_status_error);
        QMessageBox::critical(this, "ERROR", "ERROR: " + QString::fromStdString(file_status_error.errorMessage));
        return;
    }
}


QTreeWidgetItem *FileSystemGUI::searchItem(string file_path)
{
    for (auto item: fileSearch.keys()){
        if (fileSearch[item]->file_path() == file_path) return item;
    }
    return NULL;
}

