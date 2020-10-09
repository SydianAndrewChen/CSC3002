/*
 * File: log.h
 * -----------
 * This interface exports writeLogFile function to execute log written
 * containing current user, request information and answer information
 */

#ifndef LOG_H
#define LOG_H

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdio>
#include <ctime>
#include <iomanip>

#include "MyDatabase.h"

using namespace std;

/*
* Type: writeLogFile
* Usage: writeLogFile();
* ------------------------
* This function is used to read data stored in the course file and convert
* into struct Course variables.
*/
void writeLogFile(const string &request,
                  const string &answer,
                  User u,
                  string path = "log.txt");
void writeLogFile(const string &request,
                  const string &answer,
                  string path = "log.txt");

#endif // LOG_H
