/*
 * File: MyServer.cpp
 * ------------------
 * This file implements the MyServer.h interface.
 */

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

#include "RSAEncipherment.h"
#include "MyDatabase.h"
#include "MyServer.h"
#include "log.h"

using namespace std;

bool islogin = false;
bool status = false;
bool A_D_available = false;
User activeUser;
string currentTerm = "2019-20 Summer";

string instruction = "";

void login(Database &database) {
    while(!islogin) {
        string accountID, accountPsw;
        cout << "Please input your account id: ";
        cin >> accountID;
        cout << "Please input your account password: ";
        cin >> accountPsw;
        for(int i=0; i<database.userSize(); i++)
            if(database.getUser(i).ID == accountID &&
               database.getUser(i).password == accountPsw) {
                cout << "Log in successfully!" << endl;
                activeUser = database.getUser(i);
                writeLogFile("Log In.", "Successfully.", activeUser);
                return ;
            }
        cout << "Log in failed. Please log in again." << endl;
        writeLogFile("Log In.", "Failed.");
    }
}

void logout() {
    islogin = false;
    writeLogFile("Log Out.",
                 "Successfully.",
                 activeUser);
    activeUser = User();
}

void changePassword(Database &database) {
    while(1) {
        string currentPsw, newPsw1, newPsw2;
        cout << "Please input your current account password: ";
        cin >> currentPsw;
        cout << "Please input your new password: ";
        cin >> newPsw1;
        cout << "Please input your new password again: ";
        cin >> newPsw2;
        /*
        for(int i=0; i<database.userSize(); i++)
            if(database.getUser(i).ID == activeUser.ID)
                activeUser = database.getUser(i);
                */
        if(currentPsw != activeUser.password || newPsw1 != newPsw2) {
            cout << "Password change failed. Please try again." << endl;
            writeLogFile("Change Password.",
                         "Failed.",
                         activeUser);
        }
        else {
            cout << "Password change successfully." << endl;
            writeLogFile("Change Password.",
                         "Successfully.",
                         activeUser);

            cout << activeUser.ID << " " << activeUser.password << endl;
            int userI = database.findUser(activeUser.ID, activeUser);
            activeUser.password = newPsw1;
            database.setUser(userI, activeUser);
            break;
        }
    }
}

void openStudentAccess() {
    if(!A_D_available) {
        A_D_available = true;
        writeLogFile("Open Students' Access of Add&Drop.",
                     "Successfully",
                     activeUser);
    }
    else
        writeLogFile("Open Students' Access of Add&Drop.",
                     "Already Opened.",
                     activeUser);
}

void closeStudentAccess() {
    if(A_D_available) {
        A_D_available = false;
        writeLogFile("Close Students' Access of Add&Drop.",
                     "Successfully.",
                     activeUser);
    }
    else
        writeLogFile("Close Students' Access of Add&Drop.",
                     "Already Closed.",
                     activeUser);
}

void showCourseInformation(Database &database) {
    string code;
    Course temp;
    cout << "Please input course code: ";
    cin >> code;
    if(database.findCourse(code, temp) != -1) {
       database.showCourse(temp);
        writeLogFile("Show Course Information of " + code + ".",
                    "Successfully.",
                    activeUser);
    }
    else {
        cout << "No such course" << endl;
        writeLogFile("Show Course Information of " + code + ".",
                     "Course Not Found.",
                     activeUser);
    }
}

void modifyCourseInformation(Database &database) {
    string code;
    Course temp;
    cout << "Please input course code: ";
    cin >> code;
    int courseI = database.findCourse(code, temp);
    if(courseI == -1) {
        cout << "No such course" << endl;
        writeLogFile("Modify Course Information.",
                     "Failed. Reason: Course Not Found.",
                     activeUser);
        return ;
    }
    cout << "Please input which information you want to modify: " << endl;
    cout << "Information contains:" << endl;
    cout << "(1) Code" << "(2) Title" << "(3) Unit" << "(4) Description"
         << "(5) Prerequisite" << endl;
    string info, newStr;
            float newUnit;
    cin >> info;

    if(info == "Code") {
        cout << "New Course Code: ";
        cin >> newStr;
        temp.code = newStr;
        writeLogFile("Modify Course Information of " + info + " to " + newStr,
                     "Successfully",
                     activeUser);
    }
    else if(info == "Title") {
        cout << "New Course Title: ";
        cin >> newStr;
        temp.title = newStr;
        writeLogFile("Modify Course Information of " + info + " to " + newStr,
                     "Successfully",
                     activeUser);
    }
    else if(info == "Unit") {
        cout << "New Course Unit: ";
        cin >> newUnit;
        temp.unit = newUnit;
        writeLogFile("Modify Course Information of " + info + ".",
                     "Successfully",
                     activeUser);
    }
    else if(info == "Description") {
        cout << "New Course Description: ";
        cin >> newStr;
        temp.description = newStr;
        writeLogFile("Modify Course Information of " + info + " to " + newStr,
                     "Successfully",
                     activeUser);
    }
    else if(info == "Prerequisite") {
        cout << "Please input new prerequisites. If finished, please input END" << endl;
        cout << "Explanation of prerequisites: &-co-requisite; |-or; ~exclusive prerequisite" << endl;
        temp.prerequisite.clear();
        while(cin >> newStr) {
            if(newStr == "END")
                break;
            else
                temp.prerequisite.push_back(newStr);
        }
        writeLogFile("Modify Course Information of " + info + ".",
                     "Successfully",
                     activeUser);
    }
    else
        writeLogFile("Modify Course Information of " + info + ".",
                     "Information Not Exist",
                     activeUser);
    database.setCourse(courseI, temp);
}

void addNewCourse(Database &database) {
    Course newCourse;
    string t1;
    float t2;
    cout << "Please input new course Code: ";
    cin >> t1;
    newCourse.code = t1;
    cout << "Please input new course Title: ";
    cin >> t1;
    newCourse.title = t1;
    cout << "Please input new course unit: ";
    cin >> t2;
    newCourse.unit = t2;
    cout << "Please input new course description: ";
    cin >> t1;
    newCourse.description = t1;
    cout << "Please input new course prerequisites. If finished, input END." << endl;
    cout << "Explanation: &-co-requisite; |-or; ~-exclusive" << endl;
    while(cin >> t1) {
        if(t1 == "END")
            break;
        else
            newCourse.prerequisite.push_back(t1);
    }
    database.coursePushBack(newCourse);
    writeLogFile("Add New Course. Course Code: " + newCourse.code,
                 "Successfully",
                 activeUser);
}

void deleteCourse(Database &database) {
    string code;
    Course temp;
    cout << "Please input course code which you want to delete: ";
    cin >> code;
    int courseI = database.findCourse(code, temp);
    if(courseI == -1) {
        cout << "Course Not Found. " << endl;
        writeLogFile("Delete Course of " + code,
                     "Failed. Reason: Course Not Found. ",
                     activeUser);
    }
    else {
        database.courseRemove(courseI);
        writeLogFile("Delete Course of " + code,
                     "Successfully. ",
                     activeUser);
    }
}

void showStudentInformation(Database &database) {
    string ID;
    Student stu;
    cout << "Please input student ID: ";
    cin >> ID;
    if(database.findStudent(ID, stu) != -1) {
        database.showStudent(stu);
        writeLogFile("Show Student Information of " + ID + ".",
                     "Successfully",
                     activeUser);
    }
    else {
        cout << "No such student" << endl;
        writeLogFile("Show Student Information of " + ID + ".",
                     "Failed. Reason: Student Not Found.",
                     activeUser);
    }
}

void modifyStudentInformation(Database &database) {
    string ID;
    Student stu;
    cout << "Please input student ID number: ";
    cin >> ID;
    int studentI = database.findStudent(ID, stu);
    if(studentI == -1) {
        cout << "No such student" << endl;
        writeLogFile("Modify Student Information.",
                     "Failed. Reason: Student Not Found.",
                     activeUser);
        return ;
    }
    cout << "Please input which information you want to modify: " << endl;
    cout << "Information contains:" << endl;
    cout << "(1) Name" << "(2) ID Number" << "(3) Telephone Number" << "(4) Major" << "(5) School"
         << "(6) College" << "(7) cGPA" << "(8) Admission Year" << "(9) Status"  << endl;
    string info, newStr;
    int t;
    cin >> info;
    if(info == "Name") {
        cout << "New Name: ";
        cin >> newStr;
        stu.name = newStr;
        writeLogFile("Modify Student Information of " + info + " to " + newStr,
                     "Successfully",
                     activeUser);
    }
    else if(info == "ID Number") {
        cout << "New ID Number: ";
        cin >> newStr;
        stu.id = newStr;
        writeLogFile("Modify Student Information of " + info + " to " + newStr,
                     "Successfully",
                     activeUser);
    }
    else if(info == "Telephone Number") {
        cout << "New Telephone Number: ";
        cin >> newStr;
        stu.telephone = newStr;
        writeLogFile("Modify Student Information of " + info + " to " + newStr,
                     "Successfully",
                     activeUser);
    }
    else if(info == "Major") {
        cout << "New Major: ";
        cin >> newStr;
        stu.major = newStr;
        writeLogFile("Modify Student Information of " + info + " to " + newStr,
                     "Successfully",
                     activeUser);
    }
    else if(info == "School") {
        cout << "New School: ";
        cin >> newStr;
        stu.school = newStr;
        writeLogFile("Modify Student Information of " + info + " to " + newStr,
                     "Successfully",
                     activeUser);
    }
    else if(info == "College") {
        cout << "New Colldge: ";
        cin >> newStr;
        stu.college = newStr;
        writeLogFile("Modify Student Information of " + info + " to " + newStr,
                     "Successfully",
                     activeUser);
    }
    else if(info == "cGPA") {
        cout << "Cannot modify cGPA manually." << endl;
        database.calculateCGPA(stu);
        cout << "cGPA calculation done." << endl;
        writeLogFile("Modify Student Information of " + info,
                     "Failed. Reason: cGPA cannot be modifies manually.",
                     activeUser);
    }
    else if(info == "Admission Year") {
        cout << "New Admission Year: ";
        cin >> t;
        stu.admissionYear = t;
        writeLogFile("Modify Student Information of " + info + " to " + convert(t),
                     "Successfully",
                     activeUser);
    }
    else if(info == "Status") {
        cout << "New Status: ";
        cin >> newStr;
        if(newStr == "Undergraduate") {
            stu.status = false;
            writeLogFile("Modify Student Information of " + info + " to " + newStr,
                         "Successfully",
                         activeUser);
        }

        else if(newStr == "Postgraduate") {
            stu.status = true;
            writeLogFile("Modify Student Information of " + info + " to " + newStr,
                         "Successfully",
                         activeUser);
        }
        else {
            cout << "No such status." << endl;
            writeLogFile("Modify Student Information of " + info + " to " + newStr,
                         "Failed. Reason: Stauts Invalid.",
                         activeUser);
        }
    }
    database.setStudent(studentI, stu);
}

void addNewStudent(Database &database) {
    Student newStudent;
    string t1;
    int t3;
    getline(cin, t1);
    cout << "Please input new student Name: ";
    getline(cin, t1);
    newStudent.name = t1;
    cout << "Please input new student ID Number: ";
    getline(cin, t1);
    newStudent.id = t1;
    cout << "Please input new student telephone number: ";
    getline(cin, t1);
    newStudent.telephone = t1;
    cout << "Please input new student major: ";
    getline(cin, t1);
    newStudent.major = t1;
    cout << "Please input new student school: ";
    getline(cin, t1);
    newStudent.school = t1;
    cout << "Please input new student college: ";
    getline(cin, t1);
    newStudent.college = t1;
    //no course taken yet
    newStudent.cGPA = 0.0;
    cout << "Please input new student admission year: ";
    cin >> t3;
    newStudent.admissionYear = t3;
    getline(cin, t1);
    cout << "Please input new student status: ";
    getline(cin, t1);
    if(t1 == "Undergraduate")
        newStudent.status = false;
    else if (t1 == "Postgraduate")
        newStudent.status = true;
    else {
        cout << "No such status. " << endl;
        writeLogFile("Add New Student of " + newStudent.id + ".",
                     "Failed. Reason: Status Invalid.",
                     activeUser);
        return ;
    }
    database.studentPushBack(newStudent);
    writeLogFile("Add New Student of " + newStudent.id + ".",
                 "Successfully.",
                 activeUser);
}

void deleteStudent(Database &database) {
    string id;
    Student temp;
    cout << "Please input student ID which you want to delete: ";
    cin >> id;
    int studentI = database.findStudent(id, temp);
    if(studentI == -1) {
        cout << "Student Not Found. " << endl;
        writeLogFile("Delete Student of " + id,
                     "Failed. Reason: Student Not Found. ",
                     activeUser);
    }
    else {
        database.studentRemove(studentI);
        writeLogFile("Delete Student of " + id,
                     "Successfully. ",
                     activeUser);
    }
}

void showUserInformation(Database &database) {
    // can only show personal information
    database.showUser(activeUser);
    writeLogFile("Show User Information",
                 "Successfully",
                 activeUser);
}

void addNewUser(Database &database) {
    User newUser;
    string t1;
    cout << "Please input new account ID: ";
    cin >> t1;
    newUser.ID = t1;
    cout << "Please input new account Password: ";
    cin >> t1;
    newUser.password = t1;
    cout << "Please input new account status: ";
    cin >> t1;
    if(t1 == "Common users")
        newUser.status = false;
    else if(t1 == "Administrator")
        newUser.status = true;
    else {
        cout << "No such status." << endl;
        writeLogFile("Add New User of " + newUser.ID,
                     "Failed. Reason: Status Invalid.",
                     activeUser);
        return ;
    }
    database.userPushBack(newUser);
    writeLogFile("Add New User of " + newUser.ID,
                 "Successfully.",
                 activeUser);
}

void deleteUser(Database &database) {
    string id;
    User temp;
    cout << "Please input user ID which you want to delete: ";
    cin >> id;
    int userI = database.findUser(id, temp);
    if(temp == activeUser) {    // cannot delete himself or herself
        writeLogFile("Delete User of " + id,
                     "Failed. Reason: Cannot Delete Current User.",
                     activeUser);
        return ;
    }
    if(userI == -1) {
        cout << "User Not Found. " << endl;
        writeLogFile("Delete User of " + id,
                     "Failed. Reason: User Not Found. ",
                     activeUser);
    }
    else {
        database.userRemove(userI);
        writeLogFile("Delete User of " + id,
                     "Successfully. ",
                     activeUser);
    }
}

void showCourseListInformation(Database &database) {
    string code, term;
    CourseList temp;
    getline(cin, term);
    cout << "Please input term: ";
    getline(cin, term);
    cout << "Please input course code: ";
    getline(cin, code);
    if(database.findCourseList(code, term, temp) != -1) {
        database.showCourseList(temp, true);
        writeLogFile("Show Course List Information of " + code + " in " + term,
                     "Successfully.",
                     activeUser);
    }
    else {
        cout << "No such course" << endl;
        writeLogFile("Show Course List Information of " + code + " in " + term,
                     "Failed. Reason: Course List Not Found. ",
                     activeUser);
    }
}

void modifyCourseListInformation(Database &database) {
    string course, term;
    CourseList temp;
    getline(cin, term);
    cout << "Please input term: ";
    getline(cin, term);
    cout << "Please input course code (including Lecture and Tutorial number): ";
    getline(cin, course);
    int courseListI = database.findCourseList(course, term, temp);
    if(courseListI == -1) {
        cout << "No such course" << endl;
        writeLogFile("Modify Course List Information.",
                     "Failed. Reason: Course List Not Found.",
                     activeUser);
        return ;
    }
    cout << "Please input which information you want to modify: " << endl;
    cout << "Information contains:" << endl;
    cout << "(1) Term" << "(2) Course" << "(3) Speaker" << "(4) Location" << "(5) Time"
         << "(6) Associated" << "(7) Quota" << "(8)Enrolled" << endl;
    string info, newStr;
    int newQuota;
    cin >> info;
    if(info == "Term") {
        cout << "New Course Term ";
        cin >> newStr;
        temp.term = newStr;
        writeLogFile("Modify Couse List Information of " + info + " to " + newStr,
                     "Successfully",
                     activeUser);
    }
    else if(info == "Course") {
        cout << "New Course Code: ";
        cin >> newStr;
        temp.course = newStr;
        writeLogFile("Modify Couse List Information of " + info + " to " + newStr,
                     "Successfully",
                     activeUser);
    }
    else if(info == "Speaker") {
        cout << "New Course Speaker: ";
        cin >> newStr;
        temp.speaker = newStr;
        writeLogFile("Modify Couse List Information of " + info + " to " + newStr,
                     "Successfully",
                     activeUser);
    }
    else if(info == "Location") {
        cout << "New Course Location: ";
        cin >> newStr;
        temp.location = newStr;
        writeLogFile("Modify Couse List Information of " + info + " to " + newStr,
                     "Successfully",
                     activeUser);
    }
    else if(info == "Time") {
        cout << "New Course Time: ";
        cin >> newStr;
        temp.time = newStr;
        writeLogFile("Modify Couse List Information of " + info + " to " + newStr,
                     "Successfully",
                     activeUser);
    }
    else if(info == "Quota") {
        cout << "New Course Quota: ";
        cin >> newQuota;
        temp.quota = newQuota;
        writeLogFile("Modify Couse List Information of " + info + " to " + convert(newQuota),
                     "Successfully",
                     activeUser);
    }
    else if(info == "Enrolled") {
        cout << "Please input new Enrolled student list. If finished, please input END" << endl;
        temp.enrolled.clear();
        while(cin >> newStr) {
            if(newStr == "END")
                break;
            else
                temp.enrolled.push_back(newStr);
        }
        writeLogFile("Modify Couse List Information of " + info,
                     "Successfully",
                     activeUser);
    }
    else
        writeLogFile("Modify Course List Information of " + info + ".",
                     "Information Not Exist",
                     activeUser);
    database.setCourseList(courseListI, temp);
}

void addNewCourseList(Database &database) {
    CourseList newCourseList;
    string t1;
    int t2;
    getline(cin, t1);
    cout << "Please input new course Term: ";
    getline(cin, t1);
    newCourseList.term = t1;
    cout << "Please input new course Code: ";
    getline(cin, t1);
    newCourseList.course= t1;
    cout << "Please input new course Speaker: ";
    getline(cin, t1);
    newCourseList.speaker = t1;
    cout << "Please input new course location: ";
    getline(cin, t1);
    newCourseList.location = t1;
    cout << "Please input new course time: ";
    getline(cin, t1);
    newCourseList.time = t1;
    cout << "Please input new course associated: ";
    getline(cin, t1);
    newCourseList.associated = t1;
    cout << "Please input new course quota: ";
    cin >> t2;
    newCourseList.quota = t2;
    database.courseListPushBack(newCourseList);
    writeLogFile("Add New Course List of " + newCourseList.course + " in " + newCourseList.term,
                 "Successfully",
                 activeUser);
}

void deleteCourseList(Database &database) {
    string code, term;
    CourseList temp;
    getline(cin, term);
    cout << "Please input course term which you want to delete: ";
    getline(cin, term);
    cout << "Please input course code which you want to delete: ";
    getline(cin, code);
    int courseListI = database.findCourseList(code, term, temp);
    if(courseListI == -1) {
        cout << "Couse List Not Found. " << endl;
        writeLogFile("Delete Course List of " + code + " in " + term,
                     "Failed. Reason: Course List Not Found. ",
                     activeUser);
    }
    else {
        database.courseListRemove(courseListI);
        writeLogFile("Delete Course List of " + code + " in " + term,
                     "Successfully. ",
                     activeUser);
    }
}

void showStudentUserInformation(Database &database) {
    Student stu;
    database.showUser(activeUser);
    database.findStudent(activeUser.ID, stu);
    database.showStudent(stu);
    writeLogFile("Show Student's User Information",
                 "Successfully",
                 activeUser);
}

void showSearchResult(int x, bool status, Database &database) {
    string key;
    if(x==0)    key = "ACT";   // Accouting
    if(x==1)    key = "PSY";   // Applied Psychology
    if(x==2)    key = "ART";   // Art
    if(x==3)    key = "BIM";   // Bioinformatics
    if(x==4)    key = "BSC";   // Biological Sciences
    if(x==5)    key = "BIO";   // Biology
    if(x==6)    key = "BME";   // Biomedical Engineering
    if(x==7)    key = "BGE";   // Business Genreral Education
    if(x==8)    key = "MBM";   // Business Management
    if(x==9)    key = "CHM";   // Chemistry
    if(x==10)   key = "CHI";   // Chinese
    if(x==11)   key = "CLC";   // Chinese Language and Culture
    if(x==12)   key = "CSC";   // Computer Science
    if(x==13)   key = "CIE";   // Computer and Information Eng
    if(x==14)   key = "MDS";   // Data Science
    if(x==15)   key = "DDA";   // Data and Decision Analysis
    if(x==16)   key = "DMS";   // Decision & Management Sciences
    if(x==17)   key = "DMF";   // Design and Manufacturing
    if(x==18)   key = "DSJ";   // Dummy Subject
    if(x==19)   key = "ECO";   // Economics
    if(x==20)   key = "EIE";   // Electronic Info Engineering
    if(x==21)   key = "ENE";   // Energy Engineering
    if(x==22)   key = "ENG";   // English
    if(x==23)   key = "FIN";   // Finance
    if(x==24)   key = "MFE";   // Financial Engineering
    if(x==25)   key = "FMA";   // Financial Mathematics
    if(x==26)   key = "FRN";   // French
    if(x==27)   key = "GEA";   // GE Areas: Area A
    if(x==28)   key = "GEB";   // GE Areas: Area B
    if(x==29)   key = "GEC";   // GE Areas: Area C
    if(x==30)   key = "GED";   // GE Areas: Area D
    if(x==31)   key = "GFH";   // GE Foundation: Humanity
    if(x==32)   key = "GFN";   // GE Foundation: Nature
    if(x==33)   key = "ERG";   // General Engineering Courses
    if(x==34)   key = "GNB";   // Genomics and Bioinformatics
    if(x==35)   key = "GER";   // German
    if(x==36)   key = "GBS";   // Global Studies
    if(x==37)   key = "HSS";   // Humanities and Social Science
    if(x==38)   key = "IEG";   // Information Engineering
    if(x==39)   key = "ITE";   // Information Techonology
    if(x==40)   key = "IDE";   // Innovation Design & Entrepreneur
    if(x==41)   key = "MGT";   // Management
    if(x==42)   key = "MDR";   // Mandarin
    if(x==43)   key = "MKT";   // Marketing
    if(x==44)   key = "MSE";   // Material Science Engineering
    if(x==45)   key = "MAT";   // Mathematics
    if(x==46)   key = "PHI";   // Philosophy
    if(x==47)   key = "PED";   // Physical Education
    if(x==48)   key = "PHY";   // Physics
    if(x==49)   key = "RMS";   // Risk Management Science
    if(x==50)   key = "SPN";   // Spanish
    if(x==51)   key = "STA";   // Statistics
    if(x==52)   key = "LSC";   // Supply Chain and Logistics Mgt
    if(x==53) key = "TRA";   // Translation
    for(int i=0; i<database.courseListSize(); i++)
        if(database.getCourseList(i).course.substr(0,3) == key)
            database.showCourseList(database.getCourseList(i), status);
}

void searchCourseList(Database &database) {
    string term, subject;
    string subjectList[54] = {"Accouting","Applied Psychology","Art","Bioinformatics","Biological Sciences","Biology",
                              "Biomedical Engineering","Business Genreral Education","Business Management","Chemistry",
                              "Chinese","Chinese Language and Culture","Computer Science","Computer and Information Eng",
                              "Data Science","Data and Decision Analysis","Decision & Management Sciences",
                              "Design and Manufacturing","Dummy Subject","Economics","Electronic Info Engineering",
                              "Energy Engineering","English","Finance","Financial Engineering","Financial Mathematics",
                              "French","GE Areas: Area A","GE Areas: Area B","GE Areas: Area C","GE Areas: Area D",
                              "GE Foundation: Humanity","GE Foundation: Nature","General Engineering Courses",
                              "Genomics and Bioinformatics","German","Global Studies","Humanities and Social Science",
                              "Information Engineering","Information Techonology","Innovation Design & Entrepreneur",
                              "Management","Mandarin","Marketing","Material Science Engineering","Mathematics",
                              "Philosophy","Physical Education","Physics","Risk Management Science","Spanish",
                              "Statistics","Supply Chain and Logistics Mgt","Translation"};
    cout << "Please input which term you want to search: ";
    cin >> term;
    cout << "Please input which subject you want to search. Subject lists below.";
    for(int i=0; i<54; i++)
        cout << subjectList[i] << endl;
    cin >> subject;
    int subjectI = -1;
    for(int i=0; i<54; i++)
        if(subject == subjectList[i])
            subjectI = i;
    if(subjectI == -1) {
        cout << "No such subject." << endl;
        writeLogFile("Search Course List. Subject: " + subject,
                     "Failed. Reason: Subject Not Found.",
                     activeUser);
        return ;
    }
    showSearchResult(subjectI, false, database);
    writeLogFile("Search Course List. Subject: " + subject,
                 "Successfully.",
                 activeUser);
}

void addCourse(Database &database) {
    // Add%Drop accessible
    if(!A_D_available) {
        cout << "Not Access to Add&Drop" << endl;
        writeLogFile("Add Course",
                     "Failed. Reason: No Access to Add&Drop Period.",
                     activeUser);
        return ;
    }
    string courseCode;
    // pick one course
    getline(cin, courseCode);
    cout << "Please input course code (including lecture and tutorial number, eg. ENG1001 L01-LEC: ";
    getline(cin, courseCode);
    CourseList firstCourse, secondCourse;
    Student stu;
    int courseListI = database.findCourseList(courseCode, currentTerm, firstCourse),
        studentI = database.findStudent(activeUser.ID, stu);
    if(courseListI == -1) {
        cout << "No such course. " << endl;
        writeLogFile("Add Course " + courseCode + " in " + currentTerm,
                     "Failed. Reason: Course Not Found.",
                     activeUser);
        return ;
    }
    // fetch associated course
    vector<string> relatedCourse;
    int st = 0, len = 0;
    string t = database.getCourseList(courseListI).associated;
    for(int i=0; i<(int)t.length(); i++) {
        if(t[i]=='|') {
            relatedCourse.push_back(t.substr(st, len));
            st = i+1;
            len = 0;
        }
        else
            len++;
    }
    relatedCourse.push_back(t.substr(st, len));
    // associated courses exist - pick one
    if(relatedCourse.size() != 0) {

        string associatedCourseCode;
        cout << "You must choose one of these associated courses: ";
        for(int i=0; i<(int)relatedCourse.size(); i++)
            cout << relatedCourse[i] << ",";
        cout << endl;
        getline(cin, associatedCourseCode);
        //getline(cin, associatedCourseCode);
        int k = -1;
        for(int i=0; i<(int)relatedCourse.size(); i++)
            if(relatedCourse[i] == associatedCourseCode)
                k = i;
        if(k == -1) { //  no such course
            cout << "No such course. " << endl;
            writeLogFile("Add Course " + courseCode + " in " + currentTerm,
                         "Failed. Reason: Course Not Found.",
                         activeUser);
            return ;
        }
        database.findCourseList(associatedCourseCode, currentTerm, secondCourse);
    }
    // judge quota availability
    if(firstCourse.quota == (int)firstCourse.enrolled.size() ||
       secondCourse.quota == (int)secondCourse.enrolled.size()) {
        cout << "Your expected course is full." << endl;
        writeLogFile("Add Course: " + firstCourse.course + ", " + secondCourse.course,
                     "Failed. Reason: Cannot Enroll Full Courses",
                     activeUser);
        return ;
    }
    // judge time availability
    vector<string> time[7];
    stu.courseInProgress.push_back(firstCourse.course);
    if(secondCourse.course != "N/A")
        stu.courseInProgress.push_back(secondCourse.course);
    for(int i=0; i<(int)stu.courseInProgress.size(); i++) {
        CourseList tempCourseList;
        database.findCourseList(stu.courseInProgress[i], currentTerm, tempCourseList);
        int st = 0, len = 0;
        tempCourseList.time+= ',';
        for(int j=0; j<(int)tempCourseList.time.length(); j++) {
            if(tempCourseList.time[i] == ',') {
                string tempTime = tempCourseList.time.substr(st, len);
                if(tempTime.substr(0,2) == "Mo")    time[0].push_back(tempTime.substr(3, tempTime.length()-3));
                if(tempTime.substr(0,2) == "Tu")    time[1].push_back(tempTime.substr(3, tempTime.length()-3));
                if(tempTime.substr(0,2) == "We")    time[2].push_back(tempTime.substr(3, tempTime.length()-3));
                if(tempTime.substr(0,2) == "Th")    time[3].push_back(tempTime.substr(3, tempTime.length()-3));
                if(tempTime.substr(0,2) == "Fr")    time[4].push_back(tempTime.substr(3, tempTime.length()-3));
                if(tempTime.substr(0,2) == "Sa")    time[5].push_back(tempTime.substr(3, tempTime.length()-3));
                if(tempTime.substr(0,2) == "Su")    time[6].push_back(tempTime.substr(3, tempTime.length()-3));
                st = i+2;
                len = 0;
            }
            else
                    len++;
        }
    }
    for(int i=0; i<7; i++) {
        sort(time[i].begin(),time[i].end());
        for(int j=0; j<(int)time[i].size()-1; j++) {
            int h1 = intConvert(time[i][j].substr(6, 2)),
                m1 = intConvert(time[i][j].substr(9, 2)),
                h2 = intConvert(time[i][j+1].substr(0, 2)),
                m2 = intConvert(time[i][j+1].substr(3, 2));
            if(h1 > h2 ||
               (h1 == h2 && m1 > m2)) {// previous course ends after the following course begins
                cout << "Time Conflicts." << endl;
                writeLogFile("Add Course: " + firstCourse.course + ", " + secondCourse.course,
                             "Failed. Reason: Time Conflicts",
                             activeUser);
                return ;
            }
        }
    }
    // judge prerequisite
    Course c;
    int spacePos = 0;
    for(int i=0 ;i<(int)firstCourse.course.length(); i++)
        if(firstCourse.course[i] == ' ')
            spacePos = i;
    database.findCourse(firstCourse.course.substr(0, spacePos), c);
    bool flg = false;
    for(int i=0; i<(int)c.prerequisite.size(); i++) {
        if(c.prerequisite[i][0] == '&') {   // co-requisite
            for(int j=0; j<(int)stu.courseTaken.size(); j++)
                if(c.prerequisite[i] == stu.courseTaken[j])
                    flg = true;
            for(int j=0; j<(int)stu.courseInProgress.size(); j++) {
                spacePos = 0;
                for(int k=0; k<(int)stu.courseInProgress[j].length(); k++)
                    if(firstCourse.course[k] == ' ')
                        spacePos = k;
                if(c.prerequisite[i] == stu.courseInProgress[j].substr(0, spacePos))
                    flg = true;
            }
        }
        else if(c.prerequisite[i][0] == '~') {  // exclusive-requisite
            flg = true;
            for(int j=0; j<(int)stu.courseTaken.size(); j++)
                if(c.prerequisite[i] == stu.courseTaken[j])
                    flg = false;
            for(int j=0; j<(int)stu.courseInProgress.size(); j++) {
                spacePos = 0;
                for(int k=0; k<(int)stu.courseInProgress[j].length(); k++)
                    if(firstCourse.course[k] == ' ')
                        spacePos = k;
                if(c.prerequisite[i] == stu.courseInProgress[j].substr(0, spacePos))
                    flg = false;
            }
        }
        else if(c.prerequisite[i].length() <= 8) {  // one-prerequisite
            for(int j=0; j<(int)stu.courseTaken.size(); j++)
                if(c.prerequisite[i] == stu.courseTaken[j])
                    flg = true;
        }
        else {  // optional-prerequisite
            c.prerequisite[i] += '|';
            int st = 0, len = 0;
            for(int k=0; k<(int)c.prerequisite[i].length(); k++) {
                if(c.prerequisite[i][k] == '|') {
                    string pre = c.prerequisite[i].substr(st, len);
                    for(int j=0; j<(int)stu.courseTaken.size(); j++)
                        if(pre == stu.courseTaken[j])
                            flg = true;
                    st = k+1;
                    len = 0;
                }
                else
                    len++;
            }
        }
    }
    if(!flg && c.prerequisite.size() != 0) {
        cout << "Not Satisfy Prerequisites." << endl;
        writeLogFile("Add Course: " + firstCourse.course + ", " + secondCourse.course,
                     "Failed. Reason: Prerequisite Dissatisfaction",
                     activeUser);
        return ;
    }
    // add successfully
    database.setStudent(studentI, stu);
    firstCourse.enrolled.push_back(stu.id);
    int firstCourseI = database.findCourseList(firstCourse.course, currentTerm, firstCourse);
    database.setCourseList(firstCourseI, firstCourse);
    if(secondCourse.course != "N/A") {
        secondCourse.enrolled.push_back(stu.id);
        int secondCourseI = database.findCourseList(secondCourse.course, currentTerm, secondCourse);
        database.setCourseList(secondCourseI, secondCourse);
    }
    cout << "Add courses successfully" << endl;
    writeLogFile("Add Course: " + firstCourse.course + ", " + secondCourse.course,
                 "Successfully.",
                 activeUser);
}

void dropCourse(Database &database) {
    Student stu;
    int studentI = database.findStudent(activeUser.ID, stu);
    cout << "Please choose the following course to drop. (Only input course code, eg. ENG1001)" << endl;
    for(int i=0; i<(int)stu.courseInProgress.size(); i++)
        cout << stu.courseInProgress[i] << endl;
    string temp;

    int dropI = -1, dropJ = -1;
    cin >> temp;
    for(int i=0; i<(int)stu.courseInProgress.size(); i++) {
        int spacePos;
        for(int j=0; j<(int)stu.courseInProgress[i].length(); j++)
            if(stu.courseInProgress[i][j] == ' ')
                spacePos = j;
        if(temp == stu.courseInProgress[i].substr(0, spacePos) && dropI == -1)
            dropI = i;
        else if(temp == stu.courseInProgress[i].substr(0, spacePos) && dropI != -1)
            dropJ = i;
    }
    if(dropI == -1 && dropJ == -1) {    // No such course in process
        cout << "Wrong drop operation." << endl;
        writeLogFile("Drop Course: " + temp,
                     "Failed. Reason: Course in Process Not Found,",
                     activeUser);
        return ;
    }
    else if (dropJ == -1) { // Only one course to drop
        CourseList x;
        int ii = database.findCourseList(stu.courseInProgress[dropI], currentTerm, x);
        for(int i=0; i<(int)x.enrolled.size(); i++)
            if(x.enrolled[i] == stu.courseInProgress[dropI])
                x.enrolled.erase(x.enrolled.begin()+i);
        database.setCourseList(ii, x);
        stu.courseInProgress.erase(stu.courseInProgress.begin()+dropI);
        database.setStudent(studentI, stu);
    }
    else {  // Both lecture and tutorial
        CourseList x, y;
        int ii = database.findCourseList(stu.courseInProgress[dropI], currentTerm, x);
        int jj = database.findCourseList(stu.courseInProgress[dropJ], currentTerm, y);
        for(int i=0; i<(int)x.enrolled.size(); i++) {
            if(x.enrolled[i] == stu.courseInProgress[dropI])
                x.enrolled.erase(x.enrolled.begin()+i);
            if(y.enrolled[i] == stu.courseInProgress[dropJ])
                y.enrolled.erase(y.enrolled.begin()+i);
        }
        database.setCourseList(ii, x);
        database.setCourseList(jj, y);
        if(dropI<dropJ) swap(dropI, dropJ);
        stu.courseInProgress.erase(stu.courseInProgress.begin()+dropI);
        stu.courseInProgress.erase(stu.courseInProgress.begin()+dropJ);
        database.setStudent(studentI, stu);
    }
    writeLogFile("Drop Course: " + temp,
                 "Successfully",
                 activeUser);

}

void execution(vector<BigInteger> &BigIntVec, Database &database, EncryptDecrypt &encrypt_decrypt) {
    string instruction = "";
    for(int i=0; i<(int)BigIntVec.size(); i++)
        instruction = instruction + encrypt_decrypt.decrypt(BigIntVec[i]);
    bool instructionValidity = false;
    islogin = activeUser.ID=="N/A" ? false : true;
    if(instruction == "$login")
        login(database), instructionValidity = true;
    if(instruction == "$logout")
        logout(), instructionValidity = true;
    if(islogin) {
        if(activeUser.status) {  // adiministrator
            if(instruction == "$openstudentaccess") // Open Add&Drop Access
                openStudentAccess(), instructionValidity = true;
            if(instruction == "$closestudentaccess") // Close Add&Drop Access
                closeStudentAccess(), instructionValidity = true;
            if(instruction == "$showcourseinformation") // Show Course Information
                showCourseInformation(database), instructionValidity = true;
            if(instruction == "$modifycourseinformation") //Modify Course Information
                modifyCourseInformation(database), instructionValidity = true;
            if(instruction == "$addnewcourse") // Add New Course
                addNewCourse(database), instructionValidity = true;
            if(instruction == "$deletecourse") // Delete Course
                deleteCourse(database), instructionValidity = true;
            if(instruction == "$showstudentinformation") // Show Student Information
                showStudentInformation(database), instructionValidity = true;
            if(instruction == "$modifystudentinformation") // Modify Student Information
                modifyStudentInformation(database), instructionValidity = true;
            if(instruction == "$addnewstudent") // Add New Student
                addNewStudent(database), instructionValidity = true;
            if(instruction == "$deletestudent") // Delete Student
                deleteStudent(database), instructionValidity = true;
            if(instruction == "$showuserinformation") // Show Current User Information
                showUserInformation(database), instructionValidity = true;
            if(instruction == "$changepassword") // Modify Current User Information (Password only)
                changePassword(database), instructionValidity = true;
            if(instruction == "$addnewuser") // Add New User
                addNewUser(database), instructionValidity = true;
            if(instruction == "$deleteuser") // Delete User
                deleteUser(database), instructionValidity = true;
            if(instruction == "$showcourselistinformation") // Show Course List Information
                showCourseListInformation(database), instructionValidity = true;
            if(instruction == "$modifycourselistinformation") // Modify Course List Information
                modifyCourseListInformation(database), instructionValidity = true;
            if(instruction == "$addnewcourselist") // Add New Course List
                addNewCourseList(database), instructionValidity = true;
            if(instruction == "$deletecourselist") // Delete Course List
                deleteCourseList(database), instructionValidity = true;
        }
        else {  // common user - student
            if(instruction == "$showcourseinformation") // Show Course Information
                showCourseInformation(database), instructionValidity = true;
            if(instruction == "$showuserinformation") // Show Current User Information
                showStudentUserInformation(database), instructionValidity = true;
            if(instruction == "$searchcourselist") // Search Course List
                searchCourseList(database), instructionValidity = true;
            if(instruction == "$changepassword") // Modify Current Student Information
                changePassword(database), instructionValidity = true;
            if(instruction == "$addcourse") // Add Course
                addCourse(database), instructionValidity = true;
            if(instruction == "$dropcourse") //Drop Course
                dropCourse(database), instructionValidity = true;
        }
    }
    if(!instructionValidity) {
        cout << "Invalid Instruction." << endl;
        writeLogFile(instruction,
                     "Failed. Reason: No such Instruction.",
                     activeUser);
    }
}
