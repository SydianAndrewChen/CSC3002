#include "LogInGUI.h"
#include "ui_LogInGUI.h"
#include <iostream>

using namespace std;

LogInGUI::LogInGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogInGUI)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/Simulated_Server_K.jpg"));
    this->setWindowTitle("SSK Log In");
    this->setFixedSize(700, 250);
    userInfo = new QList<User>;
    readFileUser();
}

LogInGUI::~LogInGUI()
{
    emit closed(pid);
    if (sskGUI != NULL) emit isLogin(true);
    delete userInfo;
    delete ui;
}

void LogInGUI::on_pushButtonLogin_clicked()
{
    string user = ui->lineEditUser->text().toStdString();
    string password = ui->lineEditPassword->text().toStdString();
    if (user == "") {
        QMessageBox::warning(this, "Incorrect", "Please enter user id!");
        return;
    }
    if (password == "") {
        QMessageBox::warning(this, "Incorrect", "Please enter password!");
        return;
    }
    if (findMyUser(user) == User()) {
        QMessageBox::warning(this, "Incorrect", "This user does not exist!");
        return;
    }
    if (findMyUser(user).password == password){
        sskGUI = new sskgui();
        emit closed(pid);
        User temp = findMyUser(user);
        sskGUI->myUser->ID = temp.ID;
        sskGUI->myUser->status = temp.status;
        sskGUI->myUser->password = temp.password;
        sskGUI->userInitialize();

        sskGUI->show();
        sskGUI->setAttribute(Qt::WA_DeleteOnClose);
        this->close();
    } else {
        QMessageBox::warning(this, "Incorrect", "Your password does not fit the user name.");
    }
}

void LogInGUI::on_pushButtonCancel_clicked()
{
    this->close();
}

void LogInGUI::readFileUser()
{
    string Line;
    ifstream fin;
    fin.open("User.txt");
    if(fin) {
        while(getline(fin, Line)) {
            if(Line == "--------------------------------")
                userInfo->push_back(User());
            else {
                int lastid = userInfo->size()-1;
                int colonPos = 0;
                for(int i=0; i<(int)Line.length(); i++)
                    if(Line[i] == ':') {
                        colonPos = i;
                        break;
                    }
                string label = Line.substr(0, colonPos);
                string content = Line.substr(colonPos+2, Line.length()-colonPos-2);
                if(label == "ID")
                    (*userInfo)[lastid].ID = content;
                if(label == "Password")
                    (*userInfo)[lastid].password = content;
                if(label == "Status")
                    (*userInfo)[lastid].status = (content=="admin"?true:false);
            }
        }
        userInfo->pop_back();
    }
    fin.close();
}

User LogInGUI::findMyUser(string ID)
{
    for(int i=0; i<(int)userInfo->size(); i++)
        if((*userInfo)[i].ID == ID) {
            return (*userInfo)[i];
        }
    return User();
}


