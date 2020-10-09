#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "file.h"
#include <fstream>
#include <QStringList>
#include <QFile>

// Standard file path format:
/*   /App/Hello.txt    */

class FileSystem
{
public:
    FileSystem();
    ~FileSystem();

    void init();
    void readSysFile(const string & path);
    void writeSysFile(const string & path);
    void settings(string file_path, string createdTime);

    void createFile(string file_path);
    void createFile(string file_name, string file_path);
    void createFile(string file_path, bool del);
    void createFile(string file_name, string file_path, bool del);

    void createFile(string file_path, string created_time,  int file_id, string true_path,
                    bool del, bool read, bool sys);

    int getFileID(string file_path);

    void deleteFile(string file_path);
    void deleteFile(int file_id);

    ifstream readFile(string file_path);
    ofstream writeFile(string file_path);
    QFile * getAccess(string file_path);

    File * copy(string file_path, string file_des);
    File * cut(string file_path, string file_des);

    // Testing methods
    void showChild(File *file);
    void showFile(File * file);
    void display();
    File *search(File * location, QStringList path);

private:
    File *locateFile(string file_path);
    File *locateFile(int file_id);

    File * root = NULL;

    friend class FileSystemGUI;
    friend class System;
};

#endif // FILESYSTEM_H

