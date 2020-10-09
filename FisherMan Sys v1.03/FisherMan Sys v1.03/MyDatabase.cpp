/*
 * File: MyDatabase.cpp
 * --------------------
 * This file implements the MyDatabase.h interface.
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

#include "MyDatabase.h"

using namespace std;


Database::Database() {
    C.clear();
    S.clear();
    U.clear();
    CL.clear();
}

Database::~Database() { }

void Database::initializeSettings(const string &path)
{
    string Line;
    ifstream fin;
    fin.open(path.c_str());
    if (fin) {
        while (getline(fin, Line)) {
            int colonPos = 0;
            for(int i=0; i<(int)Line.length(); i++)
                if(Line[i] == ':') {
                    colonPos = i;
                    break;
                }
            string label = Line.substr(0, colonPos);
            string content = Line.substr(colonPos+2, Line.length()-colonPos-2);
            if (label == "_A_D_availability") {
                cout << content << endl;
                (content == "0") ? _A_D_available = false:_A_D_available = true;
            }
            if (label == "current_term") {
                current_term = content;
                cout << content << endl;
            }
        }
    }
    fin.close();
}

void Database::writeSettings(const string &path)
{
    ofstream fout;
    fout.open(path.c_str());
    if (fout) {
        fout << "_A_D_availability: " << _A_D_available << endl;
        fout << "current_term: " << current_term << endl;
    }
    fout.close();
}

vector<string> Database::relatedCourses(CourseList course)
{
    vector<string> relatedCourse;
    int st = 0, len = 0;
    string t = course.associated;
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
    vector <string> result;
    QString temp;
    for (auto crs: relatedCourse){
        if (QString::fromStdString(crs).trimmed() != "")  result.push_back(crs);
    }
    return result;
}

bool Database::isQuotaAvailable(CourseList course)
{
    return (course.quota >= (int) course.enrolled.size());
}

bool Database::isTimeAvailable(Student stu, CourseList course, CourseList secCourse)
{
    vector<string> time[7];
    stu.courseInProgress.push_back(course.course);
    if(secCourse.course != "N/A")
        stu.courseInProgress.push_back(secCourse.course);
    for(int i=0; i<(int)stu.courseInProgress.size(); i++) {
        CourseList tempCourseList;
        this->findCourseList(stu.courseInProgress[i], current_term, tempCourseList);
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
                    return false;
            }
        }
    }
    return true;
}

bool Database::isPrerequisiteAvailable(Student stu, CourseList firstCourse)
{
    Course c;
    int spacePos = 0;
    for(int i=0 ;i<(int)firstCourse.course.length(); i++)
        if(firstCourse.course[i] == ' ')
            spacePos = i;
    this->findCourse(firstCourse.course.substr(0, spacePos), c);
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
    return (!flg && (c.prerequisite.size() != 0));
}

void Database::addCourseToStudent(Student stu, CourseList course, CourseList secCourse)
{
    Student temp;
    int studentI = this->findStudent(stu.id, temp);
    stu.courseInProgress.push_back(course.course);
    int firstCourseI = this->findCourseList(course.course, current_term, course);
    course.enrolled.push_back(stu.id);
    this->setCourseList(firstCourseI, course);
    if(secCourse.course != "N/A") {
        stu.courseInProgress.push_back(secCourse.course);
        secCourse.enrolled.push_back(stu.id);
        int secondCourseI = this->findCourseList(secCourse.course, current_term, secCourse);
        this->setCourseList(secondCourseI, secCourse);
    }
    this->setStudent(studentI, stu);
    this->showCourseList(course, 1);
    cout << "Add courses successfully" << endl;
}

bool Database::dropCourseFromStudent(Student stu, string course_code)
{
    int studentI = this->findStudent(stu.id, stu);
    cout << "Please choose the following course to drop. (Only input course code, eg. ENG1001)" << endl;
    for(int i=0; i<(int)stu.courseInProgress.size(); i++)
        cout << stu.courseInProgress[i] << endl;

    int dropI = -1, dropJ = -1;
    for(int i=0; i<(int)stu.courseInProgress.size(); i++) {
        int spacePos;
        for(int j=0; j<(int)stu.courseInProgress[i].length(); j++)
            if(stu.courseInProgress[i][j] == ' ')
                spacePos = j;
        if(course_code == stu.courseInProgress[i].substr(0, spacePos) && dropI == -1)
            dropI = i;
        else if(course_code == stu.courseInProgress[i].substr(0, spacePos) && dropI != -1)
            dropJ = i;
    }
    if(dropI == -1 && dropJ == -1) {    // No such course in process
        cout << "Wrong drop operation." << endl;
        return 0;
    }
    else if (dropJ == -1) { // Only one course to drop
        CourseList x;
        int ii = this->findCourseList(stu.courseInProgress[dropI], current_term, x);
        for(int i=0; i<(int)x.enrolled.size(); i++)
            if(x.enrolled[i] == stu.courseInProgress[dropI])
                x.enrolled.erase(x.enrolled.begin()+i);
        this->setCourseList(ii, x);
        stu.courseInProgress.erase(stu.courseInProgress.begin()+dropI);
        this->setStudent(studentI, stu);
    }
    else {  // Both lecture and tutorial
        CourseList x, y;
        int ii = this->findCourseList(stu.courseInProgress[dropI], current_term, x);
        int jj = this->findCourseList(stu.courseInProgress[dropJ], current_term, y);
        for(int i=0; i<(int)x.enrolled.size(); i++) {
            if(x.enrolled[i] == stu.courseInProgress[dropI])
                x.enrolled.erase(x.enrolled.begin()+i);
            if(y.enrolled[i] == stu.courseInProgress[dropJ])
                y.enrolled.erase(y.enrolled.begin()+i);
        }
        this->setCourseList(ii, x);
        this->setCourseList(jj, y);
        if(dropI<dropJ) swap(dropI, dropJ);
        stu.courseInProgress.erase(stu.courseInProgress.begin()+dropI);
        stu.courseInProgress.erase(stu.courseInProgress.begin()+dropJ);
        this->setStudent(studentI, stu);
    }
    return 1;
}

/*
 * Implementation note: readFileCourse
 * -----------------------------------
 * Read file according to the following structure:
 * Course Code: ________
 * Course Title: ________
 * Course Unit: ________
 * Course Description: ________
 * Prerequisite: ________, ________, ________
 * -----------------------------------
 */
void Database::readFileCourse(const string &path) {
    string Line;
    ifstream fin;
    fin.open(path.c_str());
    if(fin) {
        while(getline(fin, Line)) {
            if(Line == "--------------------------------")
                C.push_back(Course());
            else {
                int lastid = C.size()-1;
                int colonPos = 0;
                for(int i=0; i<(int)Line.length(); i++)
                    if(Line[i] == ':') {
                        colonPos = i;
                        break;
                    }
                string label = Line.substr(0, colonPos);
                string content = Line.substr(colonPos+2, Line.length()-colonPos-2);
                if(label == "Course Code")	C[lastid].code = content;
                if(label == "Course Title")	C[lastid].title = content;
                if(label == "Course Unit")	C[lastid].unit = floatConvert(content);
                if(label == "Course Description") C[lastid].description = content;
                if(label == "Prerequisite")	{
                    int st=0, len=0;
                    for(int i=0; i<(int)content.length(); i++) {
                        if(content[i] == ',') {
                            C[lastid].prerequisite.push_back(content.substr(st, len));
                            i++;
                            st = i+1;
                            len = 0;
                        }
                        else if(content[i]!=' ')
                            len++;
                    }
                    if(content.length()!=0)
                        C[lastid].prerequisite.push_back(content.substr(st, len));
                }
            }
        }
        C.pop_back();
    }
    fin.close();
}

/*
 * Implementation note: writeFileCourse
 * -----------------------------------
 * Write file according to the following structure:
 * Course Code: ________
 * Course Title: ________
 * Course Unit: ________
 * Course Description: ________
 * Prerequisite: ________, ________, ________
 * -----------------------------------
 */
void Database::writeFileCourse(const string &path) {
    ofstream fout;
    fout.open(path.c_str());
    fout.clear();
    if(fout) {
        fout << "--------------------------------" << endl;
        for(int i=0; i<(int)C.size(); i++) {
            fout << "Course Code: " << C[i].code << endl;
            fout << "Course Title: " << C[i].title << endl;
            fout << "Course Unit: " << C[i].unit << endl;
            fout << "Course Description: " << C[i].description << endl;
            fout << "Prerequisite: " ;
            for(int j=0; j<(int)C[i].prerequisite.size(); j++) {
                fout << C[i].prerequisite[j];
                if(j != (int)C[i].prerequisite.size()-1)
                    fout << ", ";
            }
            fout << endl;
            fout << "--------------------------------" << endl;
        }
    }
    fout.close();
}

/*
 * Implementation note: readFileStudent
 * -----------------------------------
 * Read file according to the following structure:
 * Name: ________
 * ID Number: ________
 * Telephone Number: ________
 * Major: ________
 * School: ________
 * College: ________
 * cGPA: ________
 * Admission Year: ________
 * Status: ________
 * Course Taken: ________
 * Course Grade: ________
 * Course in Progress: ________
 * Shopping Cart: ________
 * -----------------------------------
 */
void Database::readFileStudent(const string &path) {
    string Line;
    ifstream fin;
    fin.open(path.c_str());
    if(fin) {
        while(getline(fin, Line)) {
            if(Line == "--------------------------------")
                S.push_back(Student());
            else {
                int lastid = S.size()-1;
                int colonPos = 0;
                for(int i=0; i<(int)Line.length(); i++)
                    if(Line[i] == ':') {
                        colonPos = i;
                        break;
                    }
                string label = Line.substr(0, colonPos);
                string content = Line.substr(colonPos+2, Line.length()-colonPos-2);
                if(label == "Name")
                    S[lastid].name = content;
                if(label == "ID Number")
                    S[lastid].id = content;
                if(label == "Telephone Number")
                    S[lastid].telephone = content;
                if(label == "Major")
                    S[lastid].major = content;
                if(label == "School")
                    S[lastid].school = content;
                if(label == "College")
                    S[lastid].college = content;
                if(label == "cGPA")
                    S[lastid].cGPA = floatConvert(content);
                if(label == "Admission Year")
                    S[lastid].admissionYear = intConvert(content);
                if(label == "Status")
                    S[lastid].status = (content == "Undergraduate"?false:true);
                if(label == "Course Taken")	{
                    int st=0, len=0;
                    for(int i=0; i<(int)content.length(); i++) {
                        if(content[i] == ',') {
                            S[lastid].courseTaken.push_back(content.substr(st, len));
                            i++;
                            st = i+1;
                            len = 0;
                        }
                        else if(content[i]!=' ')
                            len++;
                    }
                    if(content.length()!=0)
                        S[lastid].courseTaken.push_back(content.substr(st, len));
                }
                if(label == "Course Grade")	{
                    int st=0, len=0;
                    for(int i=0; i<(int)content.length(); i++) {
                        if(content[i] == ',') {
                            S[lastid].courseGrade.push_back(floatConvert(content.substr(st, len)));
                            i++;
                            st = i+1;
                            len = 0;
                        }
                        else if(content[i]!=' ')
                            len++;
                    }
                    if(content.length()!=0)
                        S[lastid].courseGrade.push_back(floatConvert(content.substr(st, len)));
                }
                if(label == "Course in Progress") {
                    int st=0, len=0;
                    for(int i=0; i<(int)content.length(); i++) {
                        if(content[i] == ',') {
                            S[lastid].courseInProgress.push_back(content.substr(st, len));
                            i++;
                            st = i+1;
                            len = 0;
                        }
                        else if(content[i]!=' ')
                            len++;
                    }
                    if(content.length()!=0)
                        S[lastid].courseInProgress.push_back(content.substr(st, len));
                }
            }

        }
        S.pop_back();
    }
    fin.close();
}

/*
 * Implementation note: writeFileStudent
 * -----------------------------------
 * Write file according to the following structure:
 * Name: ________
 * ID Number: ________
 * Telephone Number: ________
 * Major: ________
 * School: ________
 * College: ________
 * cGPA: ________
 * Admission Year: ________
 * Status: ________
 * Course Taken: ________
 * Course Grade: ________
 * Course in Progress: ________
 * Shopping Cart: ________
 * -----------------------------------
 */
void Database::writeFileStudent(const string &path) {
    ofstream fout;
    fout.open(path.c_str());
    fout.clear();
    if(fout) {
        fout << "--------------------------------" << endl;
        for(int i=0; i<(int)S.size(); i++) {
            fout << "Name: " << S[i].name << endl;
            fout << "ID Number: " << S[i].id << endl;
            fout << "Telephone Number: " << S[i].telephone << endl;
            fout << "Major: " << S[i].major << endl;
            fout << "School: " << S[i].school << endl;
            fout << "College: " << S[i].college << endl;
            fout << "cGPA: " << S[i].cGPA << endl;
            fout << "Admission Year: " << S[i].admissionYear << endl;
            fout << "Status: ";
            if(S[i].status)
                fout << "Postgraduate" << endl;
            else
                fout << "Undergraduate" << endl;
            fout << "Course Taken: " ;
            for(int j=0; j<(int)S[i].courseTaken.size(); j++) {
                fout << S[i].courseTaken[j];
                if(j != (int)S[i].courseTaken.size()-1)
                    fout << ", ";
            }
            fout << endl;
            fout << "Course Grade: " ;
            for(int j=0; j<(int)S[i].courseGrade.size(); j++) {
                fout << setprecision(4) << S[i].courseGrade[j];
                if(j != (int)S[i].courseTaken.size()-1)
                    fout << ", ";
            }
            fout << endl;
            fout << "Course in Progress: " ;
            for(int j=0; j<(int)S[i].courseInProgress.size(); j++) {
                fout << S[i].courseInProgress[j];
                if(j != (int)S[i].courseInProgress.size()-1)
                    fout << ", ";
            }
            fout << endl;
            fout << "--------------------------------" << endl;
        }
    }
    fout.close();
}

/*
 * Implementation note: readFileUser
 * ---------------------------------
 * Read file according to the following structure:
 * ID: ________
 * Password: ________
 * Status: ________
 * -----------------------------------
 */
void Database::readFileUser(const string &path) {
    string Line;
    ifstream fin;
    fin.open(path.c_str());
    if(fin) {
        while(getline(fin, Line)) {
            if(Line == "--------------------------------")
                U.push_back(User());
            else {
                int lastid = U.size()-1;
                int colonPos = 0;
                for(int i=0; i<(int)Line.length(); i++)
                    if(Line[i] == ':') {
                        colonPos = i;
                        break;
                    }
                string label = Line.substr(0, colonPos);
                string content = Line.substr(colonPos+2, Line.length()-colonPos-2);
                if(label == "ID")
                    U[lastid].ID = content;
                if(label == "Password")
                    U[lastid].password = content;
                if(label == "Status")
                    U[lastid].status = (content=="admin"?true:false);
            }
        }
        U.pop_back();
    }
    fin.close();
}

/*
 * Implementation note: writeFileUser
 * ---------------------------------
 * Write file according to the following structure:
 * ID: ________
 * Password: ________
 * Status: ________
 * -----------------------------------
 */
void Database::writeFileUser(const string &path) {
    ofstream fout;
    fout.open(path.c_str());
    fout.clear();
    if(fout) {
        fout << "--------------------------------" << endl;
        for(int i=0; i<(int)U.size(); i++) {
            fout << "ID: " << U[i].ID << endl;
            fout << "Password: " << U[i].password << endl;
            fout << "Status: ";
            if(U[i].status)
                fout << "admin" << endl;
            else
                fout << "Common User" << endl;
            fout << "--------------------------------" << endl;
        }
    }
    fout.close();
}

/*
 * Implementation note: readFileCourseList
 * ---------------------------------------
 * Read file according to the following structure:
 * Term: ________
 * Course: ________
 * Speaker: ________
 * Time: ________
 * Location: ________
 * Associated: _________
 * Quota: ________
 * Students Enrolled: ________, ________, ________
 * -----------------------------------
 */
void Database::readFileCourseList(const string &path) {

    string Line;
    ifstream fin;
    fin.open(path.c_str());
    if(fin) {
        while(getline(fin, Line)) {
            if(Line == "--------------------------------")
                CL.push_back(CourseList());
            else {
                int lastid = CL.size()-1;
                int colonPos = 0;
                for(int i=0; i<(int)Line.length(); i++)
                    if(Line[i] == ':') {
                        colonPos = i;
                        break;
                    }
                string label = Line.substr(0, colonPos);
                string content = Line.substr(colonPos+2, Line.length()-colonPos-2);
                if(label == "Term")
                    CL[lastid].term = content;
                if(label == "Course")
                    CL[lastid].course = content;
                if(label == "Speaker")
                    CL[lastid].speaker = content;
                if(label == "Location")
                    CL[lastid].location = content;
                if(label == "Time")
                    CL[lastid].time = content;
                if(label == "Quota")
                    CL[lastid].quota = intConvert(content);
                if(label == "Associated")
                    CL[lastid].associated = content;
                if(label == "Students Enrolled") {
                    int st=0, len=0;
                    for(int i=0; i<(int)content.length(); i++) {
                        if(content[i] == ',') {
                            CL[lastid].enrolled.push_back(content.substr(st, len));
                            i++;
                            st = i+1;
                            len = 0;
                        }
                        else
                            len++;
                    }
                    if(content.length()!=0)
                        CL[lastid].enrolled.push_back(content.substr(st, len));
                }

            }
        }
        CL.pop_back();
    }
    fin.close();
}

/*
 * Implementation note: writeFileCourseList
 * ---------------------------------------
 * Write file according to the following structure:
 * Term: ________
 * Course: ________
 * Speaker: ________
 * Time: ________
 * Location: ________
 * Associated: _________
 * Quota: ________
 * Students Enrolled: ________, ________, ________
 * -----------------------------------
 */
void Database::writeFileCourseList(const string &path) {
    ofstream fout;
    fout.open(path.c_str());
    fout.clear();
    if(fout) {
        fout << "--------------------------------" << endl;
        for(int i=0; i<(int)CL.size(); i++) {
            fout << "Term: " << CL[i].term << endl;
            fout << "Course: " << CL[i].course << endl;
            fout << "Speaker: " << CL[i].speaker << endl;
            fout << "Time: " << CL[i].time << endl;
            fout << "Location: " << CL[i].location << endl;
            fout << "Associated: " << CL[i].associated << endl;
            fout << "Quota: " << CL[i].quota << endl;
            fout << "Students Enrolled: ";
            for(int j=0; j<(int)CL[i].enrolled.size(); j++) {
                fout << CL[i].enrolled[j];
                if(j != (int)CL[i].enrolled.size()-1)
                    fout << ", ";
            }
            fout << endl;
            fout << "--------------------------------" << endl;
        }
    }
    fout.close();
}

/*
* Implementation note: findCourse
* -------------------------------
* Search the whole database to find out whether such a course with exact course
* code exists and change the reference value;
*/
int Database::findCourse(string code, Course &ans) {
    for(int i=0; i<(int)C.size(); i++)
        if(C[i].code == code) {
            ans = C[i];
            return i;
        }
    return -1;
}

/*
* Implementation note: findStudent
* --------------------------------
* Search the whole database to find out whether such a student with exact student
* ID exists and change the reference value;
*/
int Database::findStudent(string ID, Student &ans) {
    for(int i=0; i<(int)S.size(); i++)
        if(S[i].id == ID) {
            ans = S[i];
            return i;
        }
    return -1;
}

/*
* Implementation note: findUser
* -----------------------------
* Search the whole database to find out whether such a user with exact account
* ID exists and change the reference value;
*/
int Database::findUser(string ID, User &ans) {
    for(int i=0; i<(int)U.size(); i++)
        if(U[i].ID == ID) {
            ans = U[i];
            return i;
        }
    return -1;
}

/*
* Implementation note: findCourseList
* -----------------------------------
* Search the whole database to find out whether such a course with exact course
* code exists and change the reference value;
*/
int Database::findCourseList(string code, string term, CourseList &ans) {
    for(int i=0; i<(int)CL.size(); i++)
        if(CL[i].course == code &&
           CL[i].term == term) {
            ans = CL[i];
            return i;
        }
    return -1;
}

/*
* Implementation note: showCourse
* -------------------------------
* Print out relevant course information.
*/
void Database::showCourse(Course c) {
    cout << "Course Code: " << c.code << endl;
    cout << "Course Title: " << c.title << endl;
    cout << "Course Unit: " << c.unit << endl;
    cout << "Course Description: " << c.description << endl;
    cout << "Prerequisite: " ;
    for(int j=0; j<(int)c.prerequisite.size(); j++) {
        cout << c.prerequisite[j];
        if(j != (int)c.prerequisite.size()-1)
            cout << ", ";
    }
    cout << endl;
}

/*
* Implementation note: showStudent
* --------------------------------
* Print out relevant student information.
*/
void Database::showStudent(Student s) {
    cout << "Name: " << s.name << endl;
    cout << "ID Number: " << s.id << endl;
    cout << "Telephone Number: " << s.telephone << endl;
    cout << "Major: " << s.major << endl;
    cout << "School: " << s.school << endl;
    cout << "College: " << s.college << endl;
    cout << "cGPA: " << s.cGPA << endl;
    cout << "Adimission Year: " << s.admissionYear << endl;
    cout << "Status: ";
    if(s.status)
        cout << "Postgraduate" << endl;
    else
        cout << "Undergraduate" << endl;
    cout << "Course Taken: " ;
    for(int j=0; j<(int)s.courseTaken.size(); j++) {
        cout << setw(8) << s.courseTaken[j];
        if(j != (int)s.courseTaken.size()-1)
            cout << ", ";
    }
    cout << endl;
    cout << "Course Grade: " ;
    for(int j=0; j<(int)s.courseGrade.size(); j++) {
        cout << setw(8) << setprecision(4) << s.courseGrade[j];
        if(j != (int)s.courseTaken.size()-1)
            cout << ", ";
    }
    cout << endl;
    cout << "Course in Progress: " ;
    for(int j=0; j<(int)s.courseInProgress.size(); j++) {
        cout << s.courseInProgress[j];
        if(j != (int)s.courseInProgress.size()-1)
            cout << ", ";
    }
    cout << endl;
}

/*
* Implementation note: showUser
* -----------------------------
* Print out relevant user information.
*/
void Database::showUser(User u) {
    cout << "ID: " << u.ID << endl;
    cout << "Password: " << u.password << endl;
    cout << "Status: ";
    if(u.status)
        cout << "admin" << endl;
    else
        cout << "Common User" << endl;
}

/*
* Implementation note: showCourseList
* -----------------------------------
* Print out relevant course list information.
*/
void Database::showCourseList(CourseList cl, bool status) {
    cout << "Term: " << cl.term << endl;
    cout << "Course: " << cl.course << endl;
    cout << "Speaker: " << cl.speaker << endl;
    cout << "Time: " << cl.time << endl;
    cout << "Location: " << cl.location << endl;
    cout << "Associated: " << cl.associated << endl;
    cout << "Quota: " << cl.quota << endl;
    if(status)  { // admin accessible
        cout << "Students Enrolled: ";
        for(int j=0; j<(int)cl.enrolled.size(); j++) {
            cout << cl.enrolled[j];
            if(j != (int)cl.enrolled.size()-1)
                cout << ", ";
        }
        cout << endl;
    }
}

/*
* Implementation note: calculateCGPA
* ----------------------------------
* Calculate student's cGPA.
*/
void Database::calculateCGPA(Student &s) {
    float totUnit = 0, totGrade = 0;
    for(int i=0; i<(int)s.courseTaken.size(); i++) {
        Course temp;
        if(findCourse(s.courseTaken[i], temp) == -1) {
            cout << "No such course." << endl;
            return ;
        }
        totUnit += temp.unit;
        totGrade += s.courseGrade[i]*temp.unit;
    }
    s.cGPA = totGrade/totUnit;
}

/*
* Implementation note: getCourse
* ------------------------------
* Fetch i-th course in the database.
*/
Course Database::getCourse(int i) {
    return C[i];
}

/*
* Implementation note: getStudent
* -------------------------------
* Fetch i-th student in the database.
*/
Student Database::getStudent(int i) {
    return S[i];
}

/*
* Implementation note: getUser
* ----------------------------
* Fetch i-th user in the database.
*/
User Database::getUser(int i) {
    return U[i];
}

/*
* Implementation note: getCourseList
* ----------------------------------
* Fetch i-th courseList in the database.
*/
CourseList Database::getCourseList(int i) {
    return CL[i];
}

/*
* Implementation note: setCourse
* ------------------------------
* Modify i-th course in the database.
*/
void Database::setCourse(int i, Course &temp) {
    C[i] = temp;
}

/*
* Implementation note: setStudent
* -------------------------------
* Modify i-th student in the database.
*/
void Database::setStudent(int i, Student &temp) {
    S[i] = temp;
}

/*
* Implementation note: setUser
* -------------------------------
* Modify i-th user in the database.
*/
void Database::setUser(int i, User &temp) {
    U[i] = temp;
}

/*
* Implementation note: setCourseList
* -------------------------------
* Modify i-th courseList in the database.
*/
void Database::setCourseList(int i, CourseList &temp) {
    CL[i] = temp;
}

/*
* Implementation note: courseSize
* -------------------------------
* Returns size of course database.
*/
int Database::courseSize() {
    return C.size();
}

/*
* Implementation note: studentSize
* --------------------------------
* Returns size of student database.
*/
int Database::studentSize() {
    return S.size();
}

/*
* Implementation note: userSize
* -----------------------------
* Returns size of user database.
*/
int Database::userSize() {
    return U.size();
}

/*
* Implementation note: courseListSize
* -----------------------------------
* Returns size of courseList database.
*/
int Database::courseListSize() {
    return CL.size();
}

/*
* Implementation note: coursePushBack
* -----------------------------------
* Realizes push_back method of private course vector.
*/
void Database::coursePushBack(Course temp) {
    C.push_back(temp);
}

/*
* Implementation note: studentPushBack
* ------------------------------------
* Realizes push_back method of private student vector.
*/
void Database::studentPushBack(Student temp) {
    S.push_back(temp);
}

/*
* Implementation note: userPushBack
* ---------------------------------
* Realizes push_back method of private user vector.
*/
void Database::userPushBack(User temp) {
    U.push_back(temp);
}

/*
* Implementation note: courseListPushBack
* ---------------------------------------
* Realizes push_back method of private courseList vector.
*/
void Database::courseListPushBack(CourseList temp) {
    CL.push_back(temp);
}

/*
* Implementation note: courseRemove
* ---------------------------------
* Removes i-th element in the private course vector.
*/
void Database::courseRemove(int i) {
    C.erase(C.begin()+i);
}

/*
* Implementation note: studentRemove
* ----------------------------------
* Removes i-th element in the private student vector.
*/
void Database::studentRemove(int i) {
    S.erase(S.begin()+i);
}

/*
* Implementation note: userRemove
* -------------------------------
* Removes i-th element in the private user vector.
*/
void Database::userRemove(int i) {
    U.erase(U.begin()+i);
}

/*
* Implementation note: courseListRemove
* -------------------------------------
* Removes i-th element in the private courseList vector.
*/
void Database::courseListRemove(int i) {
    CL.erase(CL.begin()+i);
}
