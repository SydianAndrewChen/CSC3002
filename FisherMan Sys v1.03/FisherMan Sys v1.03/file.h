#ifndef FILE_H
#define FILE_H

#include <string>
#include <vector>
#include <QString>
#include <iostream>
#include <QDebug>
#include <QIcon>
#include <QDateTime>
using namespace std;

enum Type {dir, txt, exe};

class File
{
public:
    File();
    ~File();
    File(string name);

private:
    void addChild(string file_name);
    void addChild(File * file);
    void deleteChild(string file_name);
    void setParent(File * file);
    string file_path();

    bool haveRepeatedFile(string file_name);

    int file_id;
    string file_name = "Empty File";
    string created_time;
    string deleted_time;
    int file_size = 0;
    string true_path = "";

    bool isSys = false;
    bool isReadable = true;
    bool isDeletable = true;

    File* parent = NULL;
    Type type = dir;
    vector<File*> children;

    int Hash(string name);
    Type checkSuffix(string file_name);
    string fileType();
    QIcon icon();

    friend class FileSystem;
    friend class FileSystemGUI;
    File *copy();
};

#endif // FILE_H
