#ifndef LOGINGUI_H
#define LOGINGUI_H

#include <QMainWindow>
#include <string>
#include <fstream>
#include <QMessageBox>
#include "sskgui.h"
using namespace std;


namespace Ui {
class LogInGUI;
}

class LogInGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit LogInGUI(QWidget *parent = nullptr);
    ~LogInGUI();

    int pid;

signals:
    void closed(long pid);
    void isLogin(bool isLogin);

private slots:
    void on_pushButtonLogin_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::LogInGUI *ui;

    QList<User> * userInfo;

    sskgui * sskGUI = NULL;

    void readFileUser();
    User findMyUser(string ID);

    friend class System;
};


#endif // LOGINGUI_H

