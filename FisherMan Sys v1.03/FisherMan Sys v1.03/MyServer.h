/*
 * File: MyServer.h
 * ----------------
 * This interface exports execution of MyServer.
 */

#ifndef MYSERVER_H
#define MYSERVER_H

#include <algorithm>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdio>
#include <queue>
#include <ctime>

#include "MyDatabase.h"
#include "Tools.h"
#include "RSAEncipherment.h"
#include "log.h"

using namespace std;

void login(Database &database);

void logout(Database &database);

void changePassword();

void openStudentAccess();

void closeStudentAccess();

void showCourseInformation(Database &database);

void modifyCourseInformation(Database &database);

void addNewCourse(Database &database);

void deleteCourse(Database &database);

void showStudentInformation(Database &database);

void modifyStudentInformation(Database &database);

void addNewStudent(Database &database);

void deleteStudent(Database &database);

void showUserInformation(Database &database);

void addNewUser(Database &database);

void deleteUser(Database &database);

void showCourseListInformation(Database &database);

void modifyCourseListInformation(Database &database);

void addNewCourseList(Database &database);

void deleteCourseList(Database &database);

void showStudentUserInformation(Database &database);

void showSearchResult(int x, bool status, Database &database);

void searchCourseList(Database &database);

void addCourse(Database &database);

void dropCourse(Database &database);

void execution(vector<BigInteger> &BigIntVec, Database &database, EncryptDecrypt &encrypt_decrypt);


#endif // MYSERVER_H
