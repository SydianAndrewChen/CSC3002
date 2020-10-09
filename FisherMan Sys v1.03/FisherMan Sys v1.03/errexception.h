#include <time.h>
#include <QDateTime>
#include <string>
#include <fstream>
using namespace std;

struct Error {
    const char * errorMessage = "???";
    QDateTime errT = QDateTime::currentDateTime();
    string errTime = errT.toString("yyyy-MM-dd hh:mm:ss").toStdString();
};


Error generateErr(const char * errMessage);
void writeErrFile(Error err);


