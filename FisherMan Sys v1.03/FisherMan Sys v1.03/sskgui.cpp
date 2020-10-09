#include "sskgui.h"
#include "ui_sskgui.h"
#include <QMessageBox>

/*
 * Current Functions:
 *
 * Administrator:
 * 1. Open student access
 * 2. Show course information
 * 3. Modify course information
 * 4. Add new course
 * 5. Show student information
 * 6. Modify student information
 * 7. Add new student
 * 8. Show user information
 * 9. Modify user information
 * 10. Add new user
 * 11. Show course list information
 * 12. Modify course list information
 * 13. Add new course list
 * --------------------------------------
 *
 * Student:
 * 1. Show course information
 * 2. Show user information
 * 3. Show course list information
 * 4. Add course
 * 5. Modify user information
 */

sskgui::sskgui(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::sskgui)
{
    ui->setupUi(this);
    this->setWindowTitle("Simulated Server K");
    this->setWindowIcon(QIcon(":/Simulated_Server_K.jpg"));
    ui->plainTextEditHelp->setReadOnly(true);

    ui->pushButtonSaveCourse->setVisible(false);
    ui->lineEditTitle->setReadOnly(true);
    ui->lineEditCourseCode->setReadOnly(true);
    ui->plainTextEditCourseDescription->setReadOnly(true);
    ui->lineEditCoursePrerequisite->setReadOnly(true);
    ui->doubleSpinBoxCourseUnit->setReadOnly(true);

    ui->pushButtonSaveCourseList->setVisible(false);
    ui->lineEditTerm->setReadOnly(true);
    ui->lineEditCourse->setReadOnly(true);
    ui->lineEditSpeaker->setReadOnly(true);
    ui->lineEditLocation->setReadOnly(true);
    ui->lineEditTime->setReadOnly(true);
    ui->spinBoxQuota->setReadOnly(true);
    ui->lineEditAssociated->setReadOnly(true);
    ui->lineEditEnrolled->setReadOnly(true);
    ui->pushButtonAddAssociated->setVisible(false);

    ui->lineEditAssociatedTime->setVisible(false);
    ui->lineEditAssociatedTerm->setVisible(false);
    ui->lineEditAssociatedCourse->setVisible(false);
    ui->lineEditAssociatedSpeaker->setVisible(false);
    ui->lineEditAssociatedEnrolled->setVisible(false);
    ui->lineEditAssociatedLocation->setVisible(false);
    ui->spinBoxAssociatedQuota->setVisible(false);

    ui->labelAssociatedTerm->setVisible(false);
    ui->labelAssociatedTime->setVisible(false);
    ui->labelAssociatedEmpty->setVisible(false);
    ui->labelAssociatedEmpty_2->setVisible(false);
    ui->labelAssociatedQuota->setVisible(false);
    ui->labelAssociatedCourse->setVisible(false);
    ui->labelAssociatedSpeaker->setVisible(false);
    ui->labelAssociatedEnrolled->setVisible(false);
    ui->labelAssociatedLocation->setVisible(false);

    ui->pushButtonSaveStudent->setVisible(false);
    ui->lineEditFirstName->setReadOnly(true);
    ui->lineEditLastName->setReadOnly(true);
    ui->comboBoxStatus->setEnabled(false);
    ui->lineEditMajor->setReadOnly(true);
    ui->lineEditSchool->setReadOnly(true);
    ui->lineEditCollege->setReadOnly(true);
    ui->doubleSpinBoxCGPA->setReadOnly(true);
    ui->lineEditStudentID->setReadOnly(true);
    ui->lineEditCourseTaken->setReadOnly(true);
    ui->lineEditCourseInProgress->setReadOnly(true);

    this->myUser = new User();
}

sskgui::~sskgui()
{
    emit closed(pid);
    delete ui;
    delete myUser;
    this->database.writeSettings("Settings.txt");
    this->database.writeFileCourse("Course.txt");
    this->database.writeFileCourseList("CourseList.txt");
    this->database.writeFileStudent("Student.txt");
    this->database.writeFileUser("User.txt");
}

void sskgui::userInitialize()
{

    this->database.initializeSettings("Settings.txt");

    this->database.readFileUser("User.txt");
    this->database.readFileCourseList("CourseList.txt");
    this->database.readFileCourse("Course.txt");
    this->database.readFileStudent("Student.txt");

    if (myUser->status == false) {
        ui->radioButtonCourseShow->setVisible(false);
        ui->radioButtonCourseAdd->setVisible(false);
        ui->radioButtonCourseModify->setVisible(false);
        ui->lineEditCourseDelete->setVisible(false);
        ui->labelDeleteCourse->setVisible(false);
        ui->pushButtonDeleteCourse->setVisible(false);

        ui->radioButtonCourseListShow->setVisible(false);
        ui->radioButtonCourseListAdd->setVisible(false);
        ui->radioButtonCourseListModify->setVisible(false);
        ui->lineEditDeleteCourseList_term->setVisible(false);
        ui->lineEditDeleteDeleteCourList_course->setVisible(false);
        ui->labelDeleteCourList_course->setVisible(false);
        ui->labelDeleteCourseList_term->setVisible(false);
        ui->pushButtonDeleteCourseList->setVisible(false);
        ui->lineEditAssociated->setVisible(false);

        ui->radioButtonUserAdd->setVisible(false);
        ui->radioButtonUserChangePasswrd->setVisible(false);
        ui->lineEditDeleteUser->setVisible(false);
        ui->pushButtonDeleteUser->setVisible(false);
        ui->checkBox_A_D_available->setVisible(false);
        ui->labelDeleteUser->setVisible(false);

        ui->tabWidget->removeTab(2);

        updateDropCourseList();
        if (!(database._A_D_available)) {
            ui->pushButtonAdd->hide();
            ui->pushButtonDrop->hide();
            ui->pushButtonCancel->hide();
            ui->comboBoxDropCourseList->hide();
        }
    }else if (myUser->status == true) {
        ui->pushButtonAdd->setVisible(false);
        ui->pushButtonDrop->setVisible(false);
        ui->pushButtonCancel->setVisible(false);
        ui->comboBoxAssociated->setVisible(false);
        ui->comboBoxDropCourseList->setVisible(false);
    }
    ui->labelID->setText(QString::fromStdString(myUser->ID));
    ui->labelStatus->setText(QString::fromStdString((myUser->status)?"admin":"common user"));

    ui->comboBoxUserStatus->setEnabled(false);
    ui->lineEditUserID->setEnabled(false);
    ui->lineEditUserID->setText(QString::fromStdString(myUser->ID));
    ui->lineEditUserPasswrd->setText(QString::fromStdString(myUser->password));
    ui->comboBoxUserStatus->setCurrentIndex(myUser->status);
    ui->checkBox_A_D_available->setChecked(database._A_D_available);
}

void sskgui::updateDropCourseList()
{
    Student sdt;
    database.findStudent(myUser->ID, sdt);
    for (auto course:sdt.courseInProgress){
        if (QString::fromStdString(course).trimmed() != "")
            ui->comboBoxDropCourseList->addItem(QString::fromStdString(course));
    }
}

// Course part//
void sskgui::on_radioButtonCourseShow_clicked(bool checked)
{
    ui->lineEditSearchCourse->setVisible(checked);
    ui->lineEditSearchCourse->setEnabled(checked);
    ui->pushButtonSearchCourse->setVisible(checked);
    ui->pushButtonSaveCourse->setVisible(!checked);
    ui->lineEditTitle->setReadOnly(checked);
    ui->lineEditCourseCode->setReadOnly(checked);
    ui->plainTextEditCourseDescription->setReadOnly(checked);
    ui->lineEditCoursePrerequisite->setReadOnly(checked);
    ui->doubleSpinBoxCourseUnit->setReadOnly(checked);
}

void sskgui::on_radioButtonCourseAdd_clicked(bool checked)
{
    ui->lineEditSearchCourse->setVisible(!checked);
    ui->pushButtonSearchCourse->setVisible(!checked);
    ui->pushButtonSaveCourse->setVisible(checked);
    ui->lineEditCourseCode->setText("");
    ui->lineEditTitle->setText("");
    ui->doubleSpinBoxCourseUnit->setValue(0.0);
    ui->plainTextEditCourseDescription->setPlainText("");
    ui->lineEditCoursePrerequisite->setText("");
    ui->lineEditTitle->setReadOnly(!checked);
    ui->lineEditCourseCode->setReadOnly(!checked);
    ui->plainTextEditCourseDescription->setReadOnly(!checked);
    ui->lineEditCoursePrerequisite->setReadOnly(!checked);
    ui->doubleSpinBoxCourseUnit->setReadOnly(!checked);
}

void sskgui::on_radioButtonCourseModify_clicked(bool checked)
{
    ui->lineEditSearchCourse->setVisible(checked);
    ui->lineEditSearchCourse->setEnabled(!checked);
    ui->pushButtonSearchCourse->setVisible(!checked);
    ui->pushButtonSaveCourse->setVisible(checked);
    ui->lineEditTitle->setReadOnly(!checked);
    ui->lineEditCourseCode->setReadOnly(!checked);
    ui->plainTextEditCourseDescription->setReadOnly(!checked);
    ui->lineEditCoursePrerequisite->setReadOnly(!checked);
    ui->doubleSpinBoxCourseUnit->setReadOnly(!checked);
}

void sskgui::on_pushButtonSearchCourse_clicked()
{
    string code = ui->lineEditSearchCourse->text().toStdString();
    if (code == "")  {
        QMessageBox::warning(this, "Not Found", "Please enter the course code!");
        return;
    }
    Course ans;
    if (this->database.findCourse(code, ans) < 0){
        QMessageBox::warning(this, "Not Found", "Sorry, this course is not registered!");
        return;
    }
    ui->lineEditCourseCode->setText(QString::fromStdString(ans.code));
    ui->lineEditTitle->setText(QString::fromStdString(ans.title));
    ui->doubleSpinBoxCourseUnit->setValue(ans.unit);
    ui->plainTextEditCourseDescription->setPlainText(QString::fromStdString(ans.description));
    string course_requisite;
    for (auto requisite: ans.prerequisite){
        course_requisite += (requisite + ";");
    }
    ui->lineEditCoursePrerequisite->setText(QString::fromStdString(course_requisite));
}

void sskgui::on_pushButtonSaveCourse_clicked()
{
    Course crs;
    crs.code = ui->lineEditCourseCode->text().toStdString();
    crs.title = ui->lineEditTitle->text().toStdString();
    crs.unit = ui->doubleSpinBoxCourseUnit->value();
    crs.description = ui->plainTextEditCourseDescription->toPlainText().toStdString();
    QStringList courses_requisite;
    courses_requisite = ui->lineEditCoursePrerequisite->text().split(";");
    for (auto requisite: courses_requisite){
        if (requisite.trimmed() != "") crs.prerequisite.push_back(requisite.toStdString());
    }
    if (ui->radioButtonCourseAdd->isChecked()){
        for (int i = 0; i < database.courseSize(); i++){
            if (database.getCourse(i).code == crs.code) {
                QMessageBox::warning(this, "...", "Course name repeated with the existed course!");
                return;
            }
        }
        database.coursePushBack(crs);
    }else if (ui->radioButtonCourseModify->isChecked()){
        Course temp;
        string code = ui->lineEditCourseCode->text().toStdString();
        int index = database.findCourse(code, temp);
        if (index < 0) {
            QMessageBox::warning(this, "Not Found", "Sorry, this course is not registered!");
            return;
        }
        database.setCourse(index, crs);
    }
}

void sskgui::on_pushButtonDeleteCourse_clicked()
{
    string code = ui->lineEditCourseDelete->text().toStdString();
    Course temp;
    int index = database.findCourse(code, temp);
    if (index == -1) {
        QMessageBox::warning(this, "Not Found", "Sorry, this course is not registered!");
        return;
    }
    database.courseRemove(index);
}

// CourseList part//
void sskgui::on_radioButtonCourseListShow_clicked(bool checked)
{
    ui->labelCode->setVisible(checked);
    ui->labelTerm->setVisible(checked);
    ui->lineEditSearchCode->setVisible(checked);
    ui->lineEditSearchTerm->setVisible(checked);
    ui->lineEditSearchCode->setEnabled(checked);
    ui->lineEditSearchTerm->setEnabled(checked);
    ui->pushButtonSearchCourseList->setVisible(checked);
    ui->pushButtonSaveCourseList->setVisible(!checked);
    ui->lineEditTerm->setReadOnly(checked);
    ui->lineEditCourse->setReadOnly(checked);
    ui->lineEditSpeaker->setReadOnly(checked);
    ui->lineEditLocation->setReadOnly(checked);
    ui->lineEditTime->setReadOnly(checked);
    ui->spinBoxQuota->setReadOnly(checked);
    ui->lineEditAssociated->setReadOnly(checked);
    ui->lineEditEnrolled->setReadOnly(checked);
}

void sskgui::on_radioButtonCourseListModify_clicked(bool checked)
{
    ui->labelCode->setVisible(checked);
    ui->labelTerm->setVisible(checked);
    ui->lineEditSearchCode->setVisible(checked);
    ui->lineEditSearchTerm->setVisible(checked);
    ui->lineEditSearchCode->setEnabled(!checked);
    ui->lineEditSearchTerm->setEnabled(!checked);
    ui->pushButtonSearchCourseList->setVisible(!checked);
    ui->pushButtonSaveCourseList->setVisible(checked);

    ui->lineEditTerm->setReadOnly(!checked);
    ui->lineEditCourse->setReadOnly(!checked);
    ui->lineEditSpeaker->setReadOnly(!checked);
    ui->lineEditLocation->setReadOnly(!checked);
    ui->lineEditTime->setReadOnly(!checked);
    ui->spinBoxQuota->setReadOnly(!checked);
    ui->lineEditAssociated->setReadOnly(!checked);
    ui->lineEditEnrolled->setReadOnly(!checked);
}

void sskgui::on_radioButtonCourseListAdd_clicked(bool checked)
{
    ui->labelCode->setVisible(!checked);
    ui->labelTerm->setVisible(!checked);
    ui->lineEditSearchCode->setVisible(!checked);
    ui->lineEditSearchTerm->setVisible(!checked);
    ui->pushButtonSearchCourseList->setVisible(!checked);
    ui->pushButtonSaveCourseList->setVisible(checked);

    ui->lineEditTerm->setReadOnly(!checked);
    ui->lineEditCourse->setReadOnly(!checked);
    ui->lineEditSpeaker->setReadOnly(!checked);
    ui->lineEditLocation->setReadOnly(!checked);
    ui->lineEditTime->setReadOnly(!checked);
    ui->spinBoxQuota->setReadOnly(!checked);
    ui->lineEditAssociated->setReadOnly(!checked);
    ui->lineEditEnrolled->setReadOnly(!checked);

    ui->lineEditTerm->setText("");
    ui->lineEditCourse->setText("");
    ui->lineEditSpeaker->setText("");
    ui->lineEditLocation->setText("");
    ui->lineEditTime->setText("");
    ui->spinBoxQuota->setValue(0);
    ui->lineEditAssociated->setText("");
    ui->lineEditEnrolled->setText("");
}

void sskgui::on_pushButtonSearchCourseList_clicked()
{
    string code = ui->lineEditSearchCode->text().toStdString();
    string term = ui->lineEditSearchTerm->text().toStdString();
    CourseList ans;
    if (ui->lineEditSearchCode->text().trimmed() == ""){
        QMessageBox::warning(this, "Not Found", "Please enter the course code!");
        return;
    }
    if (database.findCourseList(code, term, ans) < 0){
        QMessageBox::warning(this, "Not Found", "Sorry, this course list is not registered!");
        return;
    }
    ui->lineEditTerm->setText(QString::fromStdString(ans.term));
    ui->lineEditCourse->setText(QString::fromStdString(ans.course));
    ui->lineEditSpeaker->setText(QString::fromStdString(ans.speaker));
    ui->lineEditLocation->setText(QString::fromStdString(ans.location));
    ui->lineEditTime->setText(QString::fromStdString(ans.time));
    ui->spinBoxQuota->setValue(ans.quota);
    if (myUser->status == 0) {
        QStringList associated;
        for (auto relatedCourse: database.relatedCourses(ans)){
            associated << QString::fromStdString(relatedCourse);
        }
        ui->comboBoxAssociated->clear();
        ui->comboBoxAssociated->addItems(associated);
    }
    ui->lineEditAssociated->setText(QString::fromStdString(ans.associated));
    QString enrolled;
    for (auto enrolled_student: ans.enrolled){
        QString temp = QString::fromStdString(enrolled_student);
        if (temp.trimmed() != "") enrolled += (temp.trimmed() + ";");
    }
    ui->lineEditEnrolled->setText(enrolled);
}

void sskgui::on_pushButtonAdd_clicked()
{
    if (ui->lineEditTerm->text().trimmed() == ""){
        QMessageBox::warning(this, "Not Found", "Please enter the course code!");
        return;
    }
    ui->pushButtonDrop->setEnabled(false);
    ui->lineEditTerm->setEnabled(false);
    ui->lineEditCourse->setEnabled(false);
    ui->lineEditTime->setEnabled(false);
    ui->lineEditEnrolled->setEnabled(false);
    ui->lineEditLocation->setEnabled(false);
    ui->lineEditSpeaker->setEnabled(false);
    ui->spinBoxQuota->setEnabled(false);

    string course = ui->lineEditSearchCode->text().toStdString();
    string term = ui->lineEditSearchTerm->text().toStdString();
    Student stu;
    CourseList crs;
    database.findStudent(myUser->ID, stu);
    database.findCourseList(course, term, crs);
    if (database.relatedCourses(crs).size() == 0){
        if (!(database.isQuotaAvailable(crs))){
            QMessageBox::warning(this, "...", "Quota not enough!");
            return;
        }
        if (!(database.isTimeAvailable(stu, crs))) {
            QMessageBox::warning(this, "...", "Time not available!");
            return;
        }
        if (database.isPrerequisiteAvailable(stu, crs)) {
            QMessageBox::warning(this, "...", "Prerequisite disqualified!");
            return;
        }
        database.addCourseToStudent(stu,crs);
        QMessageBox::information(this, "...", "Succeeded!");
        updateDropCourseList();
        return;
    }
    temp = crs;
    ui->pushButtonAdd->setVisible(false);
    ui->pushButtonAddAssociated->setVisible(true);
}

void sskgui::on_comboBoxAssociated_currentIndexChanged(const QString &arg1)
{
    ui->lineEditAssociatedTime->setVisible(true);
    ui->lineEditAssociatedTerm->setVisible(true);
    ui->lineEditAssociatedCourse->setVisible(true);
    ui->lineEditAssociatedSpeaker->setVisible(true);
    ui->lineEditAssociatedEnrolled->setVisible(true);
    ui->lineEditAssociatedLocation->setVisible(true);
    ui->spinBoxAssociatedQuota->setVisible(true);

    ui->labelAssociatedTerm->setVisible(true);
    ui->labelAssociatedTime->setVisible(true);
    ui->labelAssociatedEmpty->setVisible(true);
    ui->labelAssociatedEmpty_2->setVisible(true);
    ui->labelAssociatedQuota->setVisible(true);
    ui->labelAssociatedCourse->setVisible(true);
    ui->labelAssociatedSpeaker->setVisible(true);
    ui->labelAssociatedEnrolled->setVisible(true);
    ui->labelAssociatedLocation->setVisible(true);

    CourseList ans;
    string course = arg1.toStdString();
    database.findCourseList(course, database.current_term, ans);
    ui->lineEditAssociatedTerm->setText(QString::fromStdString(ans.term));
    ui->lineEditAssociatedCourse->setText(QString::fromStdString(ans.course));
    ui->lineEditAssociatedSpeaker->setText(QString::fromStdString(ans.speaker));
    ui->lineEditAssociatedLocation->setText(QString::fromStdString(ans.location));
    ui->lineEditAssociatedTime->setText(QString::fromStdString(ans.time));
    ui->spinBoxAssociatedQuota->setValue(ans.quota);
    QString enrolled;
    for (auto enrolled_student: ans.enrolled){
        QString temp = QString::fromStdString(enrolled_student);
        if (temp.trimmed() != "") enrolled += (temp.trimmed() + ";");
    }
    ui->lineEditAssociatedEnrolled->setText(enrolled);
}


void sskgui::on_pushButtonAddAssociated_clicked()
{
    Student stu;
    database.findStudent(myUser->ID, stu);
    CourseList tut;
    string tutName = ui->lineEditAssociatedCourse->text().toStdString();
    database.findCourseList(tutName, database.current_term, tut);
    if ((!(database.isQuotaAvailable(temp)))||(!database.isQuotaAvailable(tut))){
        QMessageBox::warning(this, "...", "Quota not enough!");
        return;
    }
    if (!(database.isTimeAvailable(stu, temp, tut))) {
        QMessageBox::warning(this, "...", "Time not available!");
        return;
    }
    if (!database.isPrerequisiteAvailable(stu, temp)) {
        QMessageBox::warning(this, "...", "Prerequisite disqualified!");
        return;
    }
    database.addCourseToStudent(stu, temp, tut);
    updateDropCourseList();
    return;
}


void sskgui::on_pushButtonDrop_clicked()
{
    Student stu;
    database.findStudent(myUser->ID, stu);
    QString course_code = ui->comboBoxDropCourseList->currentText().split(" ")[0];
    if (database.dropCourseFromStudent(stu, course_code.toStdString())){
        QMessageBox::information(this, "...", "Drop succeeded!");
        return;
    }else {
        QMessageBox::critical(this, "...", "Drop failed!");
        return;
    }
}

void sskgui::on_pushButtonCancel_clicked()
{
    ui->pushButtonDrop->setEnabled(true);
    ui->pushButtonDrop->setEnabled(true);
    ui->lineEditTerm->setEnabled(true);
    ui->lineEditCourse->setEnabled(true);
    ui->lineEditTime->setEnabled(true);
    ui->lineEditEnrolled->setEnabled(true);
    ui->lineEditLocation->setEnabled(true);
    ui->lineEditSpeaker->setEnabled(true);
    ui->spinBoxQuota->setEnabled(true);

        ui->pushButtonAdd->setVisible(true);
        ui->pushButtonAddAssociated->setVisible(false);
}

void sskgui::on_pushButtonSaveCourseList_clicked()
{
    CourseList lst;
    lst.term = ui->lineEditTerm->text().toStdString();
    lst.course = ui->lineEditCourse->text().toStdString();
    lst.speaker = ui->lineEditSpeaker->text().toStdString();
    lst.location = ui->lineEditLocation->text().toStdString();
    lst.time = ui->lineEditTime->text().toStdString();
    lst.quota = ui->spinBoxQuota->value();
    lst.associated = ui->lineEditAssociated->text().toStdString();
    if (ui->radioButtonCourseListAdd->isChecked()){
        for (int i = 0; i < database.courseListSize(); i++){
            if ((database.getCourseList(i).course == lst.course)
              &&(database.getCourseList(i).term == lst.term)){
                QMessageBox::warning(this, "...", "Course list name and term repeated with the existed course list!");
                return;
            }
        }
        database.courseListPushBack(lst);
    }else if (ui->radioButtonCourseListModify->isChecked()){
        CourseList temp;
        string term = ui->lineEditTerm->text().toStdString();
        string course = ui->lineEditCourse->text().toStdString();
        int index = database.findCourseList(course, term, temp);
        if (index < 0) {
            QMessageBox::warning(this, "Not Found", "Sorry, this course list is not registered!");
            return;
        }
        database.setCourseList(index, lst);
    }
}

void sskgui::on_pushButtonDeleteCourseList_clicked()
{
    CourseList temp;
    string term = ui->lineEditDeleteCourseList_term->text().toStdString();
    string code = ui->lineEditDeleteDeleteCourList_course->text().toStdString();
    int index = database.findCourseList(code, term, temp);
    if (index < 0) {
        QMessageBox::warning(this, "Not Found", "Sorry, this course list is not registered!");
        return;
    }
    database.courseListRemove(index);
}


// Student part//
void sskgui::on_radioButtonStudentShow_clicked(bool checked)
{
    ui->lineEditSearchStudent->setVisible(checked);
    ui->lineEditSearchStudent->setEnabled(checked);
    ui->pushButtonSearchStudent->setVisible(checked);
    ui->pushButtonSaveStudent->setVisible(!checked);

    ui->lineEditFirstName->setReadOnly(checked);
    ui->lineEditLastName->setReadOnly(checked);
    ui->comboBoxStatus->setEnabled(!checked);
    ui->lineEditMajor->setReadOnly(checked);
    ui->lineEditSchool->setReadOnly(checked);
    ui->lineEditCollege->setReadOnly(checked);
    ui->doubleSpinBoxCGPA->setReadOnly(checked);
    ui->lineEditStudentID->setReadOnly(checked);
}

void sskgui::on_radioButtonStudentModify_clicked(bool checked)
{
    ui->lineEditSearchStudent->setVisible(checked);
    ui->lineEditSearchStudent->setEnabled(!checked);
    ui->pushButtonSearchStudent->setVisible(!checked);
    ui->pushButtonSaveStudent->setVisible(checked);

    ui->lineEditFirstName->setReadOnly(!checked);
    ui->lineEditLastName->setReadOnly(!checked);
    ui->comboBoxStatus->setEnabled(checked);
    ui->lineEditMajor->setReadOnly(!checked);
    ui->lineEditSchool->setReadOnly(!checked);
    ui->lineEditCollege->setReadOnly(!checked);
    ui->doubleSpinBoxCGPA->setReadOnly(!checked);
    ui->lineEditStudentID->setReadOnly(!checked);
}

void sskgui::on_radioButtonStudentAdd_clicked(bool checked)
{
    ui->lineEditSearchStudent->setVisible(!checked);
    ui->pushButtonSearchStudent->setVisible(!checked);
    ui->pushButtonSaveStudent->setVisible(checked);

    ui->lineEditFirstName->setReadOnly(!checked);
    ui->lineEditLastName->setReadOnly(!checked);
    ui->comboBoxStatus->setEnabled(checked);
    ui->lineEditMajor->setReadOnly(!checked);
    ui->lineEditSchool->setReadOnly(!checked);
    ui->lineEditCollege->setReadOnly(!checked);
    ui->lineEditStudentID->setReadOnly(!checked);
    ui->lineEditFirstName->setText("");
    ui->lineEditLastName->setText("");
    ui->comboBoxStatus->setCurrentIndex(0);
    ui->lineEditMajor->setText("");
    ui->lineEditSchool->setText("");
    ui->lineEditCollege->setText("");
    ui->lineEditStudentID->setText("");

    ui->doubleSpinBoxCGPA->setReadOnly(checked);
    ui->doubleSpinBoxCGPA->setValue(0);
}

void sskgui::on_pushButtonSearchStudent_clicked() {
    string ID = ui->lineEditSearchStudent->text().toStdString();
    if (ID == " ") {
        QMessageBox::warning(this, "Not Found", "Please enter the student ID!");
        return;
    }
    Student ans;
    if (this->database.findStudent(ID, ans) < 0) {
        QMessageBox::warning(this, "Not Found", "Sorry, this student is not registered!");
        return;
    }
    QStringList name = QString::fromStdString(ans.name).split(" ");
    ui->lineEditFirstName->setText(name[1]);
    ui->lineEditLastName->setText(name[0]);
    ui->lineEditStudentID->setText(QString::fromStdString(ans.id));
    ui->comboBoxStatus->setCurrentIndex(ans.status);
    ui->lineEditMajor->setText(QString::fromStdString(ans.major));
    ui->lineEditSchool->setText(QString::fromStdString(ans.school));
    ui->lineEditCollege->setText(QString::fromStdString(ans.college));
    ui->doubleSpinBoxCGPA->setValue(ans.cGPA);
    QString courseTaken;
    for (auto course: ans.courseTaken){
        QString temp = QString::fromStdString(course);
        if (temp.trimmed() != "") courseTaken += (temp.trimmed() + ";");
    }
    ui->lineEditCourseTaken->setText(courseTaken);
    QString courseInProgress;
    for (auto course: ans.courseInProgress){
        QString temp = QString::fromStdString(course);
        if (temp.trimmed() != "") courseInProgress += (temp.trimmed() + ";");
    }
    ui->lineEditCourseInProgress->setText(courseInProgress);
}

void sskgui::on_pushButtonSaveStudent_clicked()
{
    Student sdt;
    sdt.name = (ui->lineEditLastName->text() + " " + ui->lineEditFirstName->text()).toStdString();
    sdt.status = ui->comboBoxStatus->currentIndex();
    sdt.id = ui->lineEditStudentID->text().toStdString();
    sdt.major = ui->lineEditMajor->text().toStdString();
    sdt.school = ui->lineEditSchool->text().toStdString();
    sdt.college = ui->lineEditCollege->text().toStdString();
    sdt.cGPA = ui->doubleSpinBoxCGPA->value();
    if (ui->radioButtonStudentAdd->isChecked()) {
        for (int i = 0; i < database.studentSize(); i++) {
            if (database.getStudent(i).id == sdt.id) {
                QMessageBox::warning(this, "...", "Student ID repeated with the existed student!");
                return;
            }
        }
        database.studentPushBack(sdt);
    }else if (ui->radioButtonStudentModify->isChecked()){
        Student temp;
        string id = ui->lineEditStudentID->text().toStdString();
        int index = database.findStudent(id, temp);
        if (index < 0) {
            QMessageBox::warning(this, "Not Found", "Sorry, this student is not registered!");
            return;
        }
        sdt.courseTaken = temp.courseTaken;
        sdt.courseInProgress = temp.courseInProgress;
        database.setStudent(index, sdt);
    }
}
void sskgui::on_pushButtonDeleteStudent_clicked()
{
    Student temp;
    string id = ui->lineEditStudentID->text().toStdString();
    int index = database.findStudent(id, temp);
    if (index < 0) {
        QMessageBox::warning(this, "Not Found", "Sorry, this student is not registered!");
        return;
    }
    database.courseRemove(index);
}


// User part//

void sskgui::on_radioButtonUserChangePasswrd_clicked(bool checked)
{
    ui->lineEditUserID->setText(QString::fromStdString(myUser->ID));
    ui->lineEditUserID->setEnabled(!checked);
    ui->lineEditUserPasswrd->setText(QString::fromStdString(myUser->password));
    ui->comboBoxUserStatus->setCurrentIndex(myUser->status);
    ui->comboBoxUserStatus->setEnabled(!checked);
}

void sskgui::on_radioButtonUserAdd_clicked(bool checked)
{
    ui->lineEditUserID->clear();
    ui->lineEditUserID->setEnabled(checked);
    ui->lineEditUserPasswrd->clear();
    ui->comboBoxUserStatus->setCurrentIndex(0);
    ui->comboBoxUserStatus->setEnabled(checked);
}

void sskgui::on_pushButtonUserSave_clicked()
{
    User usr;
    usr.ID = ui->lineEditUserID->text().toStdString();
    usr.password = ui->lineEditUserPasswrd->text().toStdString();
    usr.status = ui->comboBoxUserStatus->currentIndex();
    if (ui->radioButtonUserChangePasswrd->isChecked()){
        User temp;
        int index = database.findUser(usr.ID, temp);
        if (index < 0) {
            QMessageBox::warning(this, "UNKNOWN ERROR", "This error would only happen if system file is manually modified!");
            return;
        }
        database.setUser(index, usr);
        QMessageBox::information(this, "...", "Password successfully changed!");
    } else if (ui->radioButtonUserAdd->isChecked()) {
        for (int i = 0; i < database.userSize(); i++) {
            if (database.getUser(i).ID == usr.ID) {
                QMessageBox::warning(this, "...", "User ID repeated with the existed user!");
                return;
            }
        }
        database.userPushBack(usr);
    }
}

void sskgui::on_checkBox_A_D_available_stateChanged(int arg1)
{
    database._A_D_available = arg1;
}
