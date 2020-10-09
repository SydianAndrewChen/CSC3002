#include "filesystem.h"

FileSystem::FileSystem() { init(); }

FileSystem::~FileSystem()
{
    writeSysFile("Sys.txt");
    delete root;
}

void FileSystem::init()
{
    root = new File;
    readSysFile("Sys.txt");
}

// Path:path Created_Time:time File_ID:id True_path:path isDel:0/1 isReadable:0/1
void FileSystem::readSysFile(const string & path)
{
    QString line;
    QStringList sysFileInfo;
    string Line;
    ifstream fin;
    fin.open(path.c_str());
    getline(fin,Line);
    line = QString::fromStdString(Line);
    sysFileInfo = line.trimmed().split(" ");
    if (sysFileInfo[0] != ""){
        string file_path    =  sysFileInfo[0].split(":")[1].toStdString();
        string created_time =  sysFileInfo[1].split("^")[1].toStdString();
        int file_id         =  sysFileInfo[2].split(":")[1].toInt();
        string true_path    =  sysFileInfo[3].split(":")[1].toStdString();
        bool isDel          = (sysFileInfo[4].split(":")[1].toStdString() == "1");
        bool isRead         = (sysFileInfo[5].split(":")[1].toStdString() == "1");
        bool isSys          = (sysFileInfo[6].split(":")[1].toStdString() == "1");
        root->file_name     = QString::fromStdString(file_path).split("/")[1].toStdString();
        root->file_path()   = file_path;
        root->created_time  = created_time;
        root->file_id       = file_id;
        root->true_path     = true_path;
        root->isDeletable   = isDel;
        root->isReadable    = isRead;
        root->isSys         = isSys;
    }
    if (fin) {
        while (getline(fin, Line)){
            line = QString::fromStdString(Line);
            sysFileInfo = line.trimmed().split(" ");
            if (sysFileInfo[0] != ""){
                string file_path    =  sysFileInfo[0].split(":")[1].toStdString();
                string created_time =  sysFileInfo[1].split("^")[1].toStdString();
                int file_id         =  sysFileInfo[2].split(":")[1].toInt();
                string true_path    =  sysFileInfo[3].split(":")[1].toStdString();
                bool isDel          = (sysFileInfo[4].split(":")[1].toStdString() == "1");
                bool isRead         = (sysFileInfo[5].split(":")[1].toStdString() == "1");
                bool isSys          = (sysFileInfo[6].split(":")[1].toStdString() == "1");
                createFile(file_path, created_time, file_id, true_path,
                           isDel, isRead, isSys);
            }
        }
    }
    fin.close();
}

void FileSystem::writeSysFile(const string & path)
{
    display();
    ofstream fout;
    fout.open(path.c_str());
    fout.clear();
    vector<File*> search;
    search.push_back(root);
    while (search.size() != 0){
        File * location = search.back();
        if (fout) {
            fout << "Absolute_path:" << location->file_path()
                 << " Created_time^"  << location->created_time
                 << " File_ID:" << location->file_id
                 << " True_path:" << location->true_path
                 << " isDel:" << location->isDeletable
                 << " isRead:" << location->isReadable
                 << " isSys:" << location->isSys << endl << endl;
        }
        search.pop_back();
        for (auto child: location->children){
            search.push_back(child);
        }
    }
    fout.flush();
    fout.close();

}

void FileSystem::createFile(string file_path, string created_time,int file_id, string true_absolute_path,
                            bool del, bool read, bool sys)
{
    createFile(file_path);
    File * file = locateFile(file_path);
    file->created_time = created_time;
    file->true_path = true_absolute_path;
    file->file_id = file_id;
    file->isDeletable = del;
    file->isReadable = read;
    file->isSys = sys;
}

void FileSystem::createFile(string file_path)
{
    int index;
    for (unsigned int i = 0; i < file_path.size(); i++){
        if (file_path[i] == '/') index = i;
    }
    string file_name = file_path.substr(index + 1);
    file_path = file_path.substr(0, index);
    createFile(file_name, file_path);
}

void FileSystem::createFile(string file_name, string file_path)
{
    File * location = locateFile(file_path);
    location->addChild(file_name);
}

void FileSystem::createFile(string file_path, bool del)
{
    createFile(file_path);
    File * location = locateFile(file_path);
    location->isDeletable = del;
}

void FileSystem::createFile(string file_name, string file_path, bool del)
{
    createFile(file_name, file_path);
    File * location = locateFile(file_path + file_name);
    location->isDeletable = del;
}

int FileSystem::getFileID(string file_path)
{
    return locateFile(file_path)->file_id;
}

void FileSystem::deleteFile(string file_path)
{
    File * location = locateFile(file_path);
    location->parent->deleteChild(location->file_name);
}

void FileSystem::deleteFile(int file_id)
{
    File * location = locateFile(file_id);
    location->parent->deleteChild(location->file_name);
}

File * FileSystem::copy(string file_path, string file_des)
{
    File * file = locateFile(file_path);
    File * new_parent = locateFile(file_des);
    File * copy = file->copy();
    new_parent->addChild(copy);
    return copy;
}

File * FileSystem::cut(string file_path, string file_des)
{
    File * cut = locateFile(file_path);
    cut->parent->deleteChild(cut->file_name);
    File * new_parent = locateFile(file_des);
    new_parent->addChild(cut);
    return cut;
}

void FileSystem::showChild(File *file)
{
    cout << "Parent: " << file->file_name << endl;
    if(file->children.size() == 0) cout << "Empty! " << endl;
    for (auto child:file->children){
        cout << "Child: " << child->file_name << endl;
    }
}

void FileSystem::showFile(File *file)
{
    cout << "File name: " << file->file_name << endl;
    cout << "File id: " << file->file_id << endl;
    cout << "Parent: " << file->parent->file_name << endl;
    cout << "Path: " << file->file_path() << endl;
}


File * FileSystem::locateFile(string file_path)
{
    QString filePath = QString::fromStdString(file_path);
    QStringList path = filePath.split("/");
    path.pop_front();
    File * location = root;
    if (path[0] == "Root") {
        path.pop_front();
        return search (location, path);
    } else {
        cout << "Your input is incorrect! Please notice that the path must start with '/Root/...' !" << endl;
        return NULL;
    }
    //TODO: Set up a bin!!!;
}

File *FileSystem::search(File *location, QStringList path)
{
    if (path.size() == 0) {
        // cout << "Final des: " << location->file_name << endl;
        return location;
    }
    for (auto child:location->children){
        if (path[0].toStdString() == child->file_name){
            path.pop_front();
            return search(child, path);
        }
    }
    cout << "Files not found! " << endl;
    return NULL;
}

File * FileSystem::locateFile(int file_id)
{
    vector<File*> search;
    search.push_back(this->root);
    while (search.size() == 0){
        File * location = search.back();
        if (location->file_id == file_id) return location;
        search.pop_back();
        for (auto child: location->children){
            search.push_back(child);
        }
    }
    return NULL;
}

void FileSystem::display()
{
    cout << "Start displaying the file system...\n";
    vector<File*> search;
    search.push_back(this->root);
    while (search.size() != 0){
        File * location = search.back();
        cout << location->file_path() << endl;
        search.pop_back();
        for (auto child: location->children){
            search.push_back(child);
        }
    }
}




// vector<File*> search;
// search.push_back(this->root);
// while (search.size() == 0){
//     File * location = search.back();
//     if (location->file_path() == file_path) return location;
//     search.pop_back();
//     for (auto child: location->children){
//         search.push_back(child);
//     }
// }
// return NULL;
