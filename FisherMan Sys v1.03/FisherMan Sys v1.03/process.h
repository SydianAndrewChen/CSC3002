/*
 * File: process.h
 */
#ifndef PROCESS_H
#define PROCESS_H

#include <time.h>
#include <QObject>
#include <QIcon>

/************************************************************/
/* Any one need to use ProMgr please check the processmgr.h */
/************************************************************/

struct Time{
    time_t startTime;
    time_t stopTime;
    char startTm[16];
    char stopTm[16];
    double durationTime;
};

class Process
{
public:
    Process();
    Process(const char* proName);
    Process(const char* proName,bool del);
    ~Process();

private:
    unsigned int storage = 10;

    int pid;
    const char* name;
    bool del = true;

    Time clock;

    QObject * GUIptr;
    QIcon icon();

    friend class ProcessManager;
    friend class ProcessManagerGUI;
    friend class System;

    int Hash(const char* str);
};

#endif // PROCESS_H
