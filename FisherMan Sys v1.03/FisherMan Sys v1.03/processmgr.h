#ifndef PROCESSMGR_H
#define PROCESSMGR_H

#include <map>
#include <vector>
#include <iostream>
#include "process.h"
#include "memoryallocator.h"

using namespace std;

class ProcessManager
{
public:
    /* Constructor: ProcessManager
     * Usage: ProcessManager ProMgr;
     * ------------------------------
     * MUST BE INVOKED when the os simulation started!!!
     * Initialize one process manager. Main functions of the manager:
     * 1. Display all processes running and their memory allocation
     * 2. Launch and stop the process
     * 3. Display the memory allocation in disks and districts(*)
     */
    ProcessManager();

    /* Destructor: ~ProcessManager
     * ---------------------------
     * Stops every process and frees any storage occupied by processes.
     * Invoked when the os simulation stopped.
     */
    ~ProcessManager();

    /******************************/
    /* Methods used in other apps */
    /******************************/

    /*
     * Method: generateProcess
     * Usage: ProMgr.generateProcess();
     *        ProMgr.generateProcess(str);
     *        ProMgr.generateProcess(str, del);
     * -------------------------
     * Invoked inside one app.
     * Empty process.
     * Process with string name.
     * Process with string name and bool value representing deletablility(background process or not).
     * Methods will return one pid corresponding to the process, later utilized in the stop process.
     */
    long generateProcess();
    long generateProcess(const char* str);
    long generateProcess(const char* str, bool del);
    long generateProcess(const char* str, unsigned int storage);
    long generateProcess(const char* str, bool del, unsigned int storage);
    long generateProcess(const char* str,QObject * GUI);
    long generateProcess(const char* str, bool del, QObject *GUI);
    long generateProcess(const char* str,QObject * GUI, unsigned int storage);
    long generateProcess(const char* str, bool del, QObject *GUI, unsigned int storage);

    /*
     * Method: generateProcess
     * Usage: ProMgr.stopProcess(pid)
     * -------------------------
     * Stop the process.
     */
    void stopProcess(int pid);

    /* Testing function */
    void displayProcess();
    void displayHistory();

/*--------No need to check if not developer of ProMgr app.--------*/
private:
    /**********************************/
    /* Methods used inside promgr app */
    /**********************************/

    /*
     * Method: update
     * Usage: ProMgr.update();
     * ----------------------------
     * Invoked whenever one process is launched or stopped. Update() currently
     * will only contain generateGUI() method.
     */
    void update();

    /*
     * Method: start
     * Usage: ProMgr.start(pro)
     * ----------------------------
     * Start a process.
     */
    void start(Process & pro);

    /*
     * Method: stop
     * Usage: ProMgr.stop(pro)
     * -------------------------
     * Stopped a process.
     */
    void stop(int pid);

    /*
     * Method: search
     * Usage: Process pro = ProMgr.search(pid)
     * -------------------------
     * Search a process according to the pid.
     */
    Process * search(int pid);

    /*
     * Method: addToHistory
     * Usage: ProMgr.addToHistory(pro);
     * --------------------------------
     * Invoked when a process is stopped
     */
    void addToHistory(Process pro);

    /* Private variables*/
    static const unsigned int MAXIMUM_PRO = 20; // Maximum will be set as 20 temporarily

    unsigned int count = 0;

    map<const char*,vector<Process>> ProList;
    map<const char*,vector<Process>> History;

    friend class ProcessManagerGUI;
    friend class System;

    // Synchronize
    int sign = 0;

    // Memory Allocation
    MemoryAllocator mAlctor;
    unsigned int current_storage = 0;
    static const unsigned int MAXSIZE = 640;
};

#endif // PROCESSMGR_H
