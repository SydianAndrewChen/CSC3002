#include "errexception.h"

Error generateErr(const char *errMessage)
{
    Error err;
    err.errorMessage = errMessage;
    return err;
}

void writeErrFile(Error err)
{
    ofstream of;
    of.open("errLog.txt",ios::app);
    of << "Error message: " << err.errorMessage << endl
       << "Error time: " << err.errTime << endl;
}
