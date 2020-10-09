#include "processmgr.h"

ProcessManager::ProcessManager()
{
    this->generateProcess("ProcessManager", false);
    this->generateProcess("File System", false);
    this->generateProcess("Clock", false);
    // Other background apps;
}

ProcessManager::~ProcessManager() { }

long ProcessManager::generateProcess()
{
    Process pro;
    this->start(pro);
    return pro.pid;
}

long ProcessManager::generateProcess(const char* str)
{
    Process pro(str);
    this->start(pro);
    return pro.pid;
}

long ProcessManager::generateProcess(const char* str, bool del)
{
    Process pro(str, del);
    this->start(pro);
    return pro.pid;
}

long ProcessManager::generateProcess(const char *str, unsigned int storage)
{
    Process pro(str);
    pro.storage = storage;
    this->mAlctor.bestAlloc(pro.pid, pro.storage);
    this->start(pro);
    return pro.pid;
}

long ProcessManager::generateProcess(const char *str, bool del, unsigned int storage)
{
    Process pro(str, del);
    pro.storage = storage;
    this->mAlctor.bestAlloc(pro.pid, pro.storage);
    this->start(pro);
    return pro.pid;
}

long ProcessManager::generateProcess(const char* str, QObject * GUI)
{
    Process pro(str);
    pro.GUIptr = GUI;
    this->start(pro);
    return pro.pid;
}

long ProcessManager::generateProcess(const char* str, bool del, QObject * GUI)
{
    Process pro(str, del);
    pro.GUIptr = GUI;
    this->start(pro);
    return pro.pid;
}

long ProcessManager::generateProcess(const char *str, QObject *GUI, unsigned int storage)
{
    Process pro(str);
    pro.GUIptr = GUI;
    pro.storage = storage;
    this->mAlctor.bestAlloc(pro.pid, pro.storage);
    this->start(pro);
    return pro.pid;
}

long ProcessManager::generateProcess(const char *str, bool del, QObject *GUI, unsigned int storage)
{
    Process pro(str, del);
    pro.GUIptr = GUI;
    pro.storage = storage;
    this->mAlctor.bestAlloc(pro.pid, pro.storage);
    this->start(pro);
    return pro.pid;
}

void ProcessManager::stopProcess(int pid)
{
    this->stop(pid);
}

void ProcessManager::update()
{
    sign ++;
}

void ProcessManager::start(Process & pro)
{
    if ((count == MAXIMUM_PRO) || (current_storage > MAXSIZE)){
        pro.GUIptr->deleteLater();
    };
    for (auto &proEl: ProList)
    {
        if (proEl.first == pro.name) proEl.second.push_back(pro);
    }
    vector<Process> Pros;
    Pros.push_back(pro);
    ProList.insert(make_pair(pro.name, Pros));
    count++;
    current_storage += pro.storage;
    update();
}

void ProcessManager::stop(int pid)
{
    for (auto & proEl:ProList)
    {
        for (unsigned int i = 0; i < proEl.second.size(); i++){
            if (proEl.second[i].pid == pid)
            {
                current_storage -= proEl.second[i].storage;
                if (!proEl.second[i].del) {return;}
                time(&proEl.second[i].clock.stopTime);
                strftime(proEl.second[i].clock.stopTm,sizeof(proEl.second[i].clock.stopTm), "%H:%M:%S",localtime(&proEl.second[i].clock.stopTime));
                proEl.second[i].clock.durationTime = difftime(proEl.second[i].clock.stopTime, proEl.second[i].clock.startTime);
                addToHistory(proEl.second[i]);
                proEl.second.erase(proEl.second.begin()+i);
            }
        }
        if (proEl.second.size() == 0) ProList.erase(proEl.first);
    }
    count--;
    update();
}

Process * ProcessManager::search(int pid)
{
    for (auto & proEl: ProList){for (auto & pro: proEl.second){if (pro.pid == pid) return & pro;}}
    return NULL;
}

void ProcessManager::addToHistory(Process pro)
{
    for (auto &proEl: History) {
        if (proEl.first == pro.name) proEl.second.push_back(pro);
    } // Check if there are repeated process in the list
    vector<Process> Pros;
    Pros.push_back(pro);
    History.insert(make_pair(pro.name, Pros));
}

void ProcessManager::displayProcess()
{
    cout << "Showing Process: " << endl;
    for (auto & el:ProList)
    {
        cout << "name: " << el.first << endl;
        for (auto & el: el.second)
        {
            cout << "pid: " << el.pid << " Starting time: " << el.clock.startTm << endl;
        }
        cout << endl;
    }
}

void ProcessManager::displayHistory()
{
    cout << "Showing History: " << endl;
    for (auto & el:History)
    {
        cout << "name: " << el.first << endl;
        for (auto & el: el.second)
        {
            cout << "pid: " << el.pid
                 << " Starting time: " << el.clock.startTm
                 << " Stopping time: " << el.clock.stopTm
                 << " Duration time: " << el.clock.durationTime << endl;
        }
        cout << endl;
    }
}
