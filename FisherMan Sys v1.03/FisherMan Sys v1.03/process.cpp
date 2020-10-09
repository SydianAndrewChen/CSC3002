#include "process.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

Process::Process()
{
    this->name = "NULL";
    this->pid = time(NULL) + rand()/131;
    time(&clock.startTime);
    strftime(clock.startTm,sizeof(clock.startTm), "%H:%M:%S",localtime(&clock.startTime));
}

Process::Process(const char* name)
{
    this->name = name;
    this->pid = Hash(name) + rand()%131;
    time(&clock.startTime);
    strftime(clock.startTm,sizeof(clock.startTm), "%H:%M:%S",localtime(&clock.startTime));
}

Process::Process(const char* name, bool del)
{
    this->name = name;
    this->pid = Hash(name) + rand()%131;
    this->del = del;
    time(&clock.startTime);
    strftime(clock.startTm,sizeof(clock.startTm), "%H:%M:%S",localtime(&clock.startTime));
}

Process::~Process() { }

int Process::Hash(const char* str)
{
    unsigned int hash = 0;
    unsigned int i = 0;
    while (i < sizeof(str)/sizeof(char))
    {
        hash = (str[i]) + (hash << 6) + (hash << 16) - hash;
        i++;
    }
    return (hash & 0x7FFFFFFF);
}

QIcon Process::icon()
{
    QString temp = name;
    if  (temp == "Calendar") return QIcon(":/calender.png");
    if  (temp == "ProcessManager") return QIcon(":/ProcessMgr.png");
    if ((temp == "Simulated Server K")||(temp == "Simulated Server K Log In")) return QIcon(":/Simulated_Server_K.jpg");
    if  (temp == "Clock") return QIcon(":/clock.png");
    if  (temp == "TEditor") return QIcon(":/text_editor.png");
    if  (temp == "Snake") return QIcon(":/snake.png");
    if  (temp == "Mine Sweep") return QIcon(":/MineSweep.png");
    if  (temp == "Calculator") return QIcon(":/calculator.png");
    if  (temp == "File System") return QIcon(":/case.png");
    else return QIcon(":/file.png");
}
