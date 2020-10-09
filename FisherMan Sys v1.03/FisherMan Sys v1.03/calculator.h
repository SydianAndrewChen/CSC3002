#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QString>
#include <QVector>
#include <QKeyEvent>
#include <QLabel>
#include <QMenu>
#include <QAction>

namespace Ui {
class Calculator;
}

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = nullptr);
    ~Calculator();

    QString temp;

    double number;
    void operation();           //calculation
    void keyPressEvent(QKeyEvent *);       //the expression of keyboard
    void creatAction();         //create actions
    void creatMenu();           //create menu

    int pid;

signals:
    void closed(long pid);

private:
    QMenu *editMenu;            //edit menu
    QMenu *helpMenu;            //help mune

    QAction *copyAction;        //copy
    QAction *pasteAction;       //paste
    QAction *helpAction;        //see help
    QAction *aboutAction;       //about

    //Calculator related controls
    QLabel *operationLabel;
    QLabel *resultLabel;

    QLineEdit *showTextEdit;    //Expression display bar
    QLineEdit *resultTextEdit;  //Result display bar
    QPushButton *backBtn;       //back space
    QPushButton *ceBtn;         //delete
    QPushButton *clearBtn;      //delete all
    QPushButton *oneBtn;
    QPushButton *twoBtn;
    QPushButton *threeBtn;
    QPushButton *fourBtn;
    QPushButton *fiveBtn;
    QPushButton *sixBtn;
    QPushButton *sevenBtn;
    QPushButton *eightBtn;
    QPushButton *nineBtn;
    QPushButton *zeroBtn;
    QPushButton *pointBtn;      //.
    QPushButton *signBtn;       //+/-

    //arithmetic
    QPushButton *addBtn;
    QPushButton *subBtn;
    QPushButton *mulBtn;
    QPushButton *divideBtn;
    QPushButton *isBtn;

public slots:
    void button_1_click();
    void button_2_click();
    void button_3_click();
    void button_4_click();
    void button_5_click();
    void button_6_click();
    void button_7_click();
    void button_8_click();
    void button_9_click();
    void button_0_click();

    void button_add_click();
    void button_sub_click();
    void button_mul_click();
    void button_divide_click();
    void button_is_click();

    void button_cls_click();
    void button_ce_click();
    void button_back_click();
    void button_point_click();
    void button_sign_click();

    void helpMessageBox();
    void aboutMessageBox();

    void copyResult();
    void pasteNumber();

private:
    bool num2 = false;
    int operator1 = 0;
    double result = 0;
    bool flag = false;
};

#endif // CALCULATOR_H
