/*
 * File: MyDatabase.h
 * ------------------
 * This interface exports struct variable type and simulate a database
 * by reading and writing ".txt" file in a formal syntax.
 */

#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <vector>
#include <QString>
#include <QStringList>

#include "Tools.h"

using namespace std;

struct Course {
    string code;
    string title;
    float unit;
    string description;
    vector<string> prerequisite;
    Course() {
        code = "N/A";
        title = "N/A";
        unit = 0;
        description = "N/A";
    };
};

struct Student {
    string name;
    string id;
    string telephone;
    string major;
    string school;
    string college;
    float cGPA;
    int admissionYear;
    bool status;	//0-undergraduate, 1-postgraduate
    vector<string> courseTaken;
    vector<float> courseGrade;
    vector<string> courseInProgress;
    Student() {
        name = "N/A";
        id = "000000000";
        telephone = "00000000000";
        major = "N/A";
        school = "N/A";
        college = "N/A";
        cGPA = 0.0;
        admissionYear = 2000;
        status = false;
    }
    bool operator == (const Student b) const {
        return this->id == b.id;
    }
};

struct User {
    string ID;
    string password;
    bool status;	//0-common users, 1-administrators
    User() {
        ID = "N/A";
        password = "N/A";
        status = false;
    }
    bool operator == (const User b) const {
        return this->ID == b.ID;
    }
};

struct CourseList {
    string term;
    string course;
    string speaker;
    string location;
    string time;
    string associated;
    int quota;
    vector<string> enrolled;
    CourseList() {
        term = "N/A";
        course = "N/A";
        speaker = "N/A";
        location = "N/A";
        time = "N/A";
        quota = 100;
        associated = "";
    }
    bool operator == (const CourseList b) const {
        return this->term == b.term && this->course == b.course;
    }
};

class Database {
public:
    Database();
    ~Database();

    void initializeSettings(const string &path);
    void writeSettings(const string &path);

    vector<string> relatedCourses(CourseList course);
    bool isQuotaAvailable(CourseList course);
    bool isTimeAvailable(Student stu, CourseList course, CourseList secCourse = CourseList());
    bool isPrerequisiteAvailable(Student stu, CourseList course);
    void addCourseToStudent(Student stu, CourseList course, CourseList secCourse = CourseList());
    bool dropCourseFromStudent(Student stu, string course_code);

    /*
    * Type: readFileCourse
    * Usage: readFileCourse();
    * ------------------------
    * This function is used to read data stored in the course file and convert
    * into struct Course variables.
    */
    void readFileCourse(const string &path);

    /*
    * Type: writeFileCourse
    * Usage: writeFileCourse();
    * -------------------------
    * This function is used to write file according to struct Course list.
    */
    void writeFileCourse(const string &path);

    /*
    * Type: readFileStudent
    * Usage: readFileStudent();
    * -------------------------
    * This function is used to read data stored in the course file and convert
    * into struct Student variables.
    */
    void readFileStudent(const string &path);

    /*
    * Type: writeFileStudent
    * Usage: writeFileStudent();
    * --------------------------
    * This function is used to write file according to struct Student list.
    */
    void writeFileStudent(const string &path);

    /*
    * Type: readFileUser
    * Usage: readFileUser();
    * ----------------------
    * This function is used to read data stored in the course file and convert
    * into struct User variables.
    */
    void readFileUser(const string &path);

    /*
    * Type: writeFileUser
    * Usage: writeFileUser();
    * -----------------------
    * This function is used to write file according to struct User list.
    */
    void writeFileUser(const string &path);

    /*
    * Type: readFileCourseList
    * Usage: readFileCourseList();
    * ----------------------------
    * This function is used to read data stored in the course file and convert
    * into struct CourseList variables.
    */
    void readFileCourseList(const string &path);

    /*
    * Type: writeFileCourseList
    * Usage: writeFileCourseList();
    * -----------------------------
    * This function is used to write file according to struct CourseList list.
    */
    void writeFileCourseList(const string &path);

    /*
    * Type: findCourse
    * Usage: id = findCourse(code, c);
    * --------------------------------
    * This function is used to find out whether such a course with exact course code
    * exists or not.
    */
    int findCourse(string code, Course &ans);

    /*
    * Type: findStudent
    * Usage: id = findStudent(code, s);
    * ---------------------------------
    * This function is used to find out whether such a student with exact student ID
    * exists or not.
    */
    int findStudent(string ID, Student &ans);

    /*
    * Type: findUser
    * Usage: id = findUser(code, u);
    * ------------------------------
    * This function is used to find out whether such a user with exact account ID
    * exists or not.
    */
    int findUser(string ID, User &ans);

    /*
    * Type: findCourseList
    * Usage: id = findCourseList(code, term, cl);
    * -------------------------------------------
    * This function is used to find out whether such a course with exact courselist
    * code exists or not.
    */
    int findCourseList(string courseCode, string term, CourseList &ans);

    /*
    * Type: showCourse
    * Usage: showCourse(c);
    * ---------------------
    * This function is used to print out course information.
    */
    void showCourse(Course c);

    /*
    * Type: showStudent
    * Usage: showStudent(s);
    * ----------------------
    * This function is used to print out student information.
    */
    void showStudent(Student s);

    /*
    * Type: showUser
    * Usage: showUser(u);
    * -------------------
    * This function is used to print out user information.
    */
    void showUser(User u);

    /*
    * Type: showCourseList
    * Usage: showCourseList(cl);
    * --------------------------
    * This function is used to print out course list information.
    */
    void showCourseList(CourseList cl, bool status);

    /*
    * Type: calculateCGPA
    * Usage: calgulateCGPA(s);
    * --------------------------
    * This function is used to calculate student's cGPA.
    */
    void calculateCGPA(Student &s);

    /*
    * Type: getCourse
    * Usage: c = getCourse(i);
    * ------------------------
    * This function is used to realize fetch of private data field.
    */
    Course getCourse(int i);

    /*
    * Type: getStudent
    * Usage: s = getStudent(i);
    * -------------------------
    * This function is used to realize fetch of private data field.
    */
    Student getStudent(int i);

    /*
    * Type: getUser
    * Usage: u = getUser(i);
    * ----------------------
    * This function is used to realize fetch of private data field.
    */
    User getUser(int i);

    /*
    * Type: getCourseList
    * Usage: cl = getCourseList(i);
    * -----------------------------
    * This function is used to realize fetch of private data field.
    */
    CourseList getCourseList(int i);

    /*
    * Type: setCourse
    * Usage: setCourse(i, c);
    * -----------------------
    * This function is used to realize modification of private data field.
    */
    void setCourse(int i, Course &temp);

    /*
    * Type: setStudent
    * Usage: setStudent(i, s);
    * ------------------------
    * This function is used to realize modification of private data field.
    */
    void setStudent(int i, Student &temp);

    /*
    * Type: setUser
    * Usage: setUser(i, u);
    * ------------------------
    * This function is used to realize modification of private data field.
    */
    void setUser(int i, User &temp);

    /*
    * Type: setCourseList
    * Usage: setCourseList(i, cl);
    * ----------------------------
    * This function is used to realize modification of private data field.
    */
    void setCourseList(int i, CourseList &temp);

    /*
    * Type: courseSize
    * Usage: n = courseSize();
    * ------------------------
    * This function is used to realize query of private data field.
    */
    int courseSize();

    /*
    * Type: studentSize
    * Usage: n = studentSize();
    * -------------------------
    * This function is used to realize query of private data field.
    */
    int studentSize();

    /*
    * Type: userSize
    * Usage: n = userSize();
    * ----------------------
    * This function is used to realize query of private data field.
    */
    int userSize();

    /*
    * Type: courseListSize
    * Usage: n = courseListSize();
    * ----------------------------
    * This function is used to realize query of private data field.
    */
    int courseListSize();

    /*
    * Type: coursePushBack
    * Usage: coursePushBack(c);
    * -------------------------
    * This function is used to realize push_back method of vector C.
    */
    void coursePushBack(Course temp);

    /*
    * Type: studentPushBack
    * Usage: studentPushBack(s);
    * --------------------------
    * This function is used to realize push_back method of vector S.
    */
    void studentPushBack(Student temp);

    /*
    * Type: userPushBack
    * Usage: userPushBack(s);
    * -----------------------
    * This function is used to realize push_back method of vector U.
    */
    void userPushBack(User temp);

    /*
    * Type: userPushBack
    * Usage: userPushBack(s);
    * -----------------------
    * This function is used to realize push_back method of vector CL.
    */
    void courseListPushBack(CourseList temp);

    /*
    * Type: courseRemove
    * Usage: courseRemove(i);
    * -----------------------
    * This function is used to realize remove method of vector C.
    */
    void courseRemove(int i);

    /*
    * Type: studentRemove
    * Usage: courseRemove(i);
    * -----------------------
    * This function is used to realize remove method of vector S.
    */
    void studentRemove(int i);

    /*
    * Type: userRemove
    * Usage: userRemove(i);
    * ---------------------
    * This function is used to realize remove method of vector U.
    */
    void userRemove(int i);

    /*
    * Type: courseListRemove
    * Usage: courseListRemove(i);
    * ---------------------------
    * This function is used to realize remove method of vector CL.
    */
    void courseListRemove(int i);

    string current_term = "2019-20 Summer";
    bool _A_D_available = false;

private:
    vector<Course> C;
    vector<Student> S;
    vector<User> U;
    vector<CourseList> CL;
};


#endif // MYDATABASE_H
