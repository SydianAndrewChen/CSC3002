#include "file.h"

File::File() { }

File::~File()
{
    for (auto child: this->children){
        delete(child);
    }
}

File::File(string name)
{
    this->file_name = name;
    this->type = checkSuffix(name);
    this->file_id = Hash(name) + rand()%131;
    this->file_id = Hash(name) + rand()%131;
    this->created_time = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss").toStdString();
}

void File::addChild(string file_name)
{
    File * file;
    file = new File(file_name);
    this->addChild(file);
}

void File::addChild(File *file) {
    if (this->haveRepeatedFile(file->file_name)){
        if (file->type != 0){
            int length = file->file_name.size();
            string suffix = file->file_name.substr(length - 4);
            string name = file->file_name.substr(0, length - 4);
            name += "*";
            file->file_name = name + suffix;
        }else file->file_name += "*";
    }
    this->children.push_back(file);
    file->setParent(this);
}

void File::deleteChild(string file_name)
{
    int index = 0;
    for (auto child: this->children){
        if (child->file_name == file_name) {
            if (!(child->isDeletable)) return;
            children.erase(children.begin()+index);
        }
        index ++;
    }
    return;
}

void File::setParent(File *file) { this->parent = file; }

string File::file_path()
{
    string file_path = "/" + this->file_name;
    if (parent != NULL){
        file_path = this->parent->file_path() + file_path;
    }
    return file_path;
}

bool File::haveRepeatedFile(string file_name)
{
    for (auto child:this->children){ if (child->file_name == file_name) return true;}
    return false;
}

int File::Hash(string name)
{
    unsigned int hash = 0;
    unsigned int i = 0;
    while (i < name.size())
    {
        hash = (name[i]) + (hash << 6) + (hash << 16) - hash;
        i++;
    }
    return (hash & 0x7FFFFFFF);
}

Type File::checkSuffix(string file_name)
{
    QString name = QString::fromStdString(file_name);
    name = name.trimmed();
    QString suffix = name.section(".", -1);
    if (suffix == "txt"){
        return txt;
    }else if (suffix == "exe"){
        return exe;
    }else {return dir;}
}

string File::fileType()
{
    switch (type){
        case 0: return ".dir"; break;
        case 1: return ".txt"; break;
        case 2: return ".exe"; break;
    default: return "???"; break;
    }
    return "???";
}

QIcon File::icon()
{
    switch(type){
    case 0: return QIcon(":/case.png"); break;
    case 1: return QIcon(":/file.png"); break;
    case 2:
        if (file_name == "Calendar.exe")           return QIcon(":/calender.png");
        if (file_name == "ProcessMgr.exe")         return QIcon(":/ProcessMgr.png");
        if (file_name == "Simulated_Server_K.exe") return QIcon(":/Simulated_Server_K.jpg");
        if (file_name == "Snake.exe")              return QIcon(":/snake.png");
        if (file_name == "MineSweep.exe")          return QIcon(":/MineSweep.png");
        if (file_name == "Calculator.exe")         return QIcon(":/calculator.png");
        if (file_name == "TextEditor.exe")         return QIcon(":/text_editor.png");
        else return QIcon(":/file.png");
    default: return QIcon(":/file.png");
    }
    return QIcon(":/file.png");
}

File *File::copy()
{
    File * copy;
    copy = new File(this->file_name);
    copy->file_size = this->file_size;
    copy->isReadable = this->isReadable;
    copy->isDeletable = this->isDeletable;
    copy->file_id = this->file_id + rand()%131;
    return copy;
}
