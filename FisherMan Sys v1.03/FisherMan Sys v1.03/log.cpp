/*
 * File: log.cpp
 * -------------
 * This file implements the log.h interface.
 */

#include "log.h"

using namespace std;

void writeLogFile(const string &request,
                  const string &answer,
                  User u,
                  string path) {
    int y = getLocalTime("year");
    int m = getLocalTime("month");
    int md = getLocalTime("monthday");
    int h = getLocalTime("hour");
    int mnt = getLocalTime("minute");
    int sec = getLocalTime("second");
    ofstream fout;
    fout.open(path.c_str(),ios::app);
    fout << setw(4) << setfill('0') << y << "-"
         << setw(2) << setfill('0') << m << "-"
         << setw(2) << setfill('0') << md << " "
         << setw(2) << setfill('0') << h << ":"
         << setw(2) << setfill('0') <<mnt<< ":"
         << setw(2) << setfill('0') << sec << endl;
    fout << "Account ID:" << u.ID << "\t";
    fout << "Account status:";
    if(u.status)
        fout << "administrator" << endl;
    else
        fout << "common user" << endl;
    fout << "Request: " << request << endl << "Answer: " << answer << endl;
    fout << "--------------------------------" << endl;
    fout.close();
}

void writeLogFile(const string &request,
                  const string &answer,
                  string path) {
    int y = getLocalTime("year");
    int m = getLocalTime("month");
    int md = getLocalTime("monthday");
    int h = getLocalTime("hour");
    int mnt = getLocalTime("minute");
    int sec = getLocalTime("second");
    ofstream fout;
    fout.open(path.c_str(),ios::app);
    fout << setw(4) << setfill('0') << y << "-"
         << setw(2) << setfill('0') << m << "-"
         << setw(2) << setfill('0') << md << " "
         << setw(2) << setfill('0') << h << ":"
         << setw(2) << setfill('0') <<mnt<< ":"
         << setw(2) << setfill('0') << sec << endl;
    fout << "Request: " << request << endl << "Answer: " << answer << endl;
    fout << "--------------------------------" << endl;
    fout.close();
}
