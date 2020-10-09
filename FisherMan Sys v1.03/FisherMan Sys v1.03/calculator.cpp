#include "calculator.h"
#include "ui_calculator.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRect>
#include <QMenuBar>
#include <QMessageBox>
#include <QClipboard>
#include <QApplication>

Calculator::Calculator(QWidget *parent) :
    QMainWindow(parent)
{
    QWidget *centerWindow = new QWidget;
    this->setWindowTitle("Calculator");
    this->setWindowIcon(QIcon(" "));
    this->setCentralWidget(centerWindow);

    creatAction();
    creatMenu();

    //label
    operationLabel = new QLabel("Operation:");
    resultLabel = new QLabel("Result:");
    //display column
    showTextEdit = new QLineEdit;
    showTextEdit->setReadOnly(true);    //Sst to read only
    resultTextEdit = new QLineEdit;
    showTextEdit->setReadOnly(true);

    //Keyboard button
    backBtn = new QPushButton("<-");
    ceBtn = new QPushButton("CE");
    clearBtn = new QPushButton("CLS");
    oneBtn = new QPushButton("1");
    twoBtn = new QPushButton("2");
    threeBtn = new QPushButton("3");
    fourBtn = new QPushButton("4");
    fiveBtn = new QPushButton("5");
    sixBtn = new QPushButton("6");
    sevenBtn = new QPushButton("7");
    eightBtn = new QPushButton("8");
    nineBtn = new QPushButton("9");
    zeroBtn = new QPushButton("0");
    pointBtn = new QPushButton(".");
    isBtn = new QPushButton("=");
    isBtn->setStyleSheet("background-color: rgb(255,80,20)");
    addBtn = new QPushButton("+");
    subBtn = new QPushButton("-");
    mulBtn = new QPushButton("*");
    divideBtn = new QPushButton("/");
    signBtn = new QPushButton("+/-");
    //Set the layout
    QHBoxLayout *H0 = new QHBoxLayout;
    H0->addWidget(operationLabel);
    H0->addWidget(resultLabel);
    H0->setStretch(0,2);
    H0->setStretch(1,1);

    QHBoxLayout *H1 = new QHBoxLayout;
    H1->addWidget(showTextEdit);
    H1->addWidget(resultTextEdit);
    H1->setStretch(0,2);
    H1->setStretch(1,1);

    QHBoxLayout *H2 = new QHBoxLayout;
    H2->addWidget(backBtn);
    H2->addWidget(ceBtn);
    H2->addWidget(clearBtn);
    H2->addWidget(addBtn);

    QHBoxLayout *H3 = new QHBoxLayout;
    H3->addWidget(sevenBtn);
    H3->addWidget(eightBtn);
    H3->addWidget(nineBtn);
    H3->addWidget(subBtn);

    QHBoxLayout *H4 = new QHBoxLayout;
    H4->addWidget(fourBtn);
    H4->addWidget(fiveBtn);
    H4->addWidget(sixBtn);
    H4->addWidget(mulBtn);

    QHBoxLayout *H5 = new QHBoxLayout;
    H5->addWidget(oneBtn);
    H5->addWidget(twoBtn);
    H5->addWidget(threeBtn);
    H5->addWidget(divideBtn);

    QHBoxLayout *H6 = new QHBoxLayout;
    H6->addWidget(signBtn);
    H6->addWidget(zeroBtn);
    H6->addWidget(pointBtn);
    H6->addWidget(isBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(H0);
    mainLayout->addLayout(H1);
    mainLayout->addLayout(H2);
    mainLayout->addLayout(H3);
    mainLayout->addLayout(H4);
    mainLayout->addLayout(H5);
    mainLayout->addLayout(H6);

    connect(oneBtn, SIGNAL(clicked()), this, SLOT(button_1_click()));
    connect(twoBtn, SIGNAL(clicked()), this, SLOT(button_2_click()));
    connect(threeBtn, SIGNAL(clicked()), this, SLOT(button_3_click()));
    connect(fourBtn, SIGNAL(clicked()), this, SLOT(button_4_click()));
    connect(fiveBtn, SIGNAL(clicked()), this, SLOT(button_5_click()));
    connect(sixBtn, SIGNAL(clicked()), this, SLOT(button_6_click()));
    connect(sevenBtn, SIGNAL(clicked()), this, SLOT(button_7_click()));
    connect(eightBtn, SIGNAL(clicked()), this, SLOT(button_8_click()));
    connect(nineBtn, SIGNAL(clicked()), this, SLOT(button_9_click()));
    connect(zeroBtn, SIGNAL(clicked()), this, SLOT(button_0_click()));

    connect(addBtn, SIGNAL(clicked()), this, SLOT(button_add_click()));
    connect(subBtn, SIGNAL(clicked()), this, SLOT(button_sub_click()));
    connect(mulBtn, SIGNAL(clicked()), this, SLOT(button_mul_click()));
    connect(divideBtn, SIGNAL(clicked()), this, SLOT(button_divide_click()));
    connect(isBtn, SIGNAL(clicked()), this, SLOT(button_is_click()));
    connect(signBtn, SIGNAL(clicked()), this, SLOT(button_sign_click()));

    connect(clearBtn, SIGNAL(clicked()), this, SLOT(button_cls_click()));
    connect(ceBtn, SIGNAL(clicked()), this, SLOT(button_ce_click()));
    connect(backBtn, SIGNAL(clicked()), this, SLOT(button_back_click()));
    connect(pointBtn, SIGNAL(clicked()), this, SLOT(button_point_click()));

    centerWindow->setLayout(mainLayout);
}

Calculator::~Calculator()
{
    emit closed(pid);
}
void Calculator::creatAction()
{

    copyAction = new QAction("&Copy",this);
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setStatusTip("Copy the result");
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copyResult()));

    pasteAction = new QAction("&Paste",this);
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setStatusTip("Paste");
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(pasteNumber()));

    helpAction = new QAction("&Help",this);
    helpAction->setShortcut(tr("F1"));
    helpAction->setStatusTip("Help");
    connect(helpAction, SIGNAL(triggered()), this, SLOT(helpMessageBox()));

    aboutAction = new QAction("&About",this);
    aboutAction->setShortcut(tr("F2"));
    aboutAction->setStatusTip("About");
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutMessageBox()));

}

/*
 **Create menu
 */
void Calculator::creatMenu()
{

    //compile menu
    editMenu=menuBar()->addMenu("&Edit(E)");
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);

    //help menu
    helpMenu=menuBar()->addMenu("&Help(H)");
    helpMenu->addAction(helpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);
}

/*
** Use the message box to create an about window
*/
void Calculator::aboutMessageBox()
{
    QMessageBox::about(NULL, "About", "Calculator V1.0");
}

void Calculator::helpMessageBox()
{
    QMessageBox::about(NULL, "Help","No need for help!");
}

void Calculator::copyResult()
{
    QClipboard *copy = QGuiApplication::clipboard();    //Clipboard operation
    temp = QString::number(result);
    copy->setText(temp);                    //Copy to the clipboard
}

void Calculator::pasteNumber()
{
    QClipboard *paste = QGuiApplication::clipboard();
    temp = paste->text();
    showTextEdit->setText(temp);            //Paste it onto the clipboard
}

/*
**Press the button and save to temp and display on the label
*/
void Calculator::button_1_click()
{
    temp += "1";
    showTextEdit->setText(temp);
}

void Calculator::button_2_click()
{
    temp += "2";
    showTextEdit->setText(temp);
}

void Calculator::button_3_click()
{
    temp += "3";
    showTextEdit->setText(temp);
}

void Calculator::button_4_click()
{
    temp += "4";
    showTextEdit->setText(temp);
}

void Calculator::button_5_click()
{
    temp += "5";
    showTextEdit->setText(temp);
}

void Calculator::button_6_click()
{
    temp += "6";
    showTextEdit->setText(temp);
}

void Calculator::button_7_click()
{
    temp += "7";
    showTextEdit->setText(temp);
}

void Calculator::button_8_click()
{
    temp += "8";
    showTextEdit->setText(temp);
}

void Calculator::button_9_click()
{
    temp += "9";
    showTextEdit->setText(temp);
}

void Calculator::button_0_click()
{
    bool ok;
    double tem = temp.toDouble(&ok);

    //Useless zeros cannot be input
    if(1 == temp.contains(".") || 0 != tem || NULL == temp)
    {
        temp += "0";
    }
    showTextEdit->setText(temp);
}

void Calculator::button_add_click()
{
    if(false == num2)
    {
        num2 = true;
    }

    operation();
    operator1 = 1;          //Operator1 is used to store operators
    showTextEdit->setText("+");
}

void Calculator::button_sub_click()
{

    if(false == num2)
    {
        num2 = true;
    }

    operation();
    operator1 = 2;
    showTextEdit->setText("-");
}

void Calculator::button_mul_click()
{
    if(false == num2)
    {
        num2 = true;
    }

    operation();
    operator1 = 3;
    showTextEdit->setText("*");
}

void Calculator::button_divide_click()
{
    if(false == num2)
    {
        num2 = true;
    }

    operation();
    operator1 = 4;
    showTextEdit->setText("/");
}

void Calculator::operation()
{
    bool ok;
    if(NULL == temp)
    {
        number = 0;
    }
    else
    {
        number = temp.toDouble(&ok);
    }

    //Operator1 is used to select the corresponding operation
    switch(operator1)
    {
    case 0:result = number;break;
    case 1:result += number;break;
    case 2:result -= number;break;
    case 3:result *= number;break;
    case 4:
        if(number == 0)
        {
            flag = true;
            break;
        }
        result /= number;
        break;
    default:
        break;
    }
    temp = "";
}

void Calculator::button_is_click()
{
    bool ok;

    //Single number, no result
    if(num2 == false)
    {
        number = temp.toDouble(&ok);
        QString str1 = QString::number(number);
        resultTextEdit->setText(str1);
        return;
    }
    operation();

    num2 = false;
    operator1 = 0;

    if(true == flag)
    {
        resultTextEdit->setText("Numerator cannot be 0!");
        return;
    }
    temp = QString::number(result);         //double turn to qstring
    resultTextEdit->setText(temp);
}

void Calculator::button_cls_click()
{
    temp = "";
    number = 0;
    operator1 = 0;
    result = 0;
    showTextEdit->setText(temp);
    resultTextEdit->setText(temp);
}

void Calculator::button_ce_click()
{
    temp = "";
    showTextEdit->setText(temp);
}

void Calculator::button_back_click()
{
    temp.chop(1);                           //Chop removes 1 character from the end of the string
    showTextEdit->setText(temp);
}

void Calculator::button_point_click()
{
    if(0 == temp.contains("."))
    {
        temp += ".";
    }
    showTextEdit->setText(temp);
}

void Calculator::button_sign_click()
{
    //see if it's a negative number
    if(temp.startsWith("-"))
    {
        temp = temp.mid(1);
        showTextEdit->setText(temp);
        return;
    }
    temp = temp.insert(0, "-");
    showTextEdit->setText(temp);
}

void Calculator::keyPressEvent(QKeyEvent *k)
{
    //int b = k->key();
    switch(k->key())
    {
    case Qt::Key_0:button_0_click();break;
    case Qt::Key_1:button_1_click();break;
    case Qt::Key_2:button_2_click();break;
    case Qt::Key_3:button_3_click();break;
    case Qt::Key_4:button_4_click();break;
    case Qt::Key_5:button_5_click();break;
    case Qt::Key_6:button_6_click();break;
    case Qt::Key_7:button_7_click();break;
    case Qt::Key_8:button_8_click();break;
    case Qt::Key_9:button_9_click();break;
    case Qt::Key_Plus:button_add_click();break;         // +
    case Qt::Key_Minus:button_sub_click();break;        // -
    case Qt::Key_Asterisk:button_mul_click();break;     // *
    case Qt::Key_Slash:button_divide_click();break;     // /
    case Qt::Key_Backspace:button_back_click();break;   // ¡û
    case Qt::Key_Enter:button_is_click();break;         // =
    case Qt::Key_Period:button_point_click();break;     // .
    default:break;
    }
}
