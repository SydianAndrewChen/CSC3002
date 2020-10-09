#ifndef SSKGUI_H
#define SSKGUI_H

#include <QMainWindow>
#include <QTimer>
#include <fstream>
#include "Tools.h"
#include "MyDatabase.h"

QT_BEGIN_NAMESPACE
namespace Ui { class sskgui; }
QT_END_NAMESPACE

class sskgui : public QMainWindow
{
    Q_OBJECT

public:
    sskgui(QWidget *parent = nullptr);
    ~sskgui();
    void userInitialize();
    void updateDropCourseList();
    int pid;

signals:
    void closed(long pid);

private slots:
    void on_pushButtonSearchCourse_clicked();

    void on_pushButtonSearchStudent_clicked();

    void on_pushButtonSearchCourseList_clicked();

    void on_radioButtonCourseListModify_clicked(bool checked);

    void on_radioButtonCourseListAdd_clicked(bool checked);

    void on_radioButtonCourseModify_clicked(bool checked);

    void on_radioButtonCourseShow_clicked(bool checked);

    void on_radioButtonCourseAdd_clicked(bool checked);

    void on_radioButtonStudentShow_clicked(bool checked);

    void on_radioButtonStudentModify_clicked(bool checked);

    void on_radioButtonStudentAdd_clicked(bool checked);

    void on_radioButtonCourseListShow_clicked(bool checked);

    void on_pushButtonSaveCourse_clicked();

    // void on_pushButtonDeleteStudent_clicked();

    void on_pushButtonDeleteCourse_clicked();

    void on_pushButtonSaveCourseList_clicked();

    void on_pushButtonDeleteCourseList_clicked();

    void on_pushButtonSaveStudent_clicked();

    void on_pushButtonDeleteStudent_clicked();

    void on_pushButtonUserSave_clicked();

    void on_radioButtonUserChangePasswrd_clicked(bool checked);

    void on_radioButtonUserAdd_clicked(bool checked);

    void on_pushButtonAdd_clicked();

    void on_pushButtonDrop_clicked();

    void on_checkBox_A_D_available_stateChanged(int arg1);


    void on_pushButtonCancel_clicked();

    void on_pushButtonAddAssociated_clicked();

    void on_comboBoxAssociated_currentIndexChanged(const QString &arg1);

private:
    Ui::sskgui *ui;

    friend class LogInGUI;
    User * myUser;
    Database database;
    CourseList temp; // Used in constructing the adding system

    void readFileCourse(const string & path);
    void writeFileCourse(const string & path);
    bool findCourse(string code, Course & ans);
};


#endif // SSKGUI_H
