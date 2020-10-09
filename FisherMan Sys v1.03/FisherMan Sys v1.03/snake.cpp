#include <QPainter>
#include <QTime>
#include <QDebug>
#include "snake.h"

Snake::Snake(QWidget *parent) : QWidget(parent) {
    this->setFixedSize(2 * B_WIDTH, 1* B_HEIGHT);
    this->setWindowTitle("Snake game!");
    verLine.setLine(B_WIDTH, 0, B_WIDTH, B_HEIGHT);
    horLine.setLine(0, B_HEIGHT, B_WIDTH, B_HEIGHT);
    setStyleSheet("background-color:white;");
    leftDirection = false;
    rightDirection = true;
    upDirection = false;
    downDirection = false;
    inGame = true;
    resize(B_WIDTH, B_HEIGHT);
    loadImages();
    initGame();
}

void Snake::loadImages() {
    dot.load(":/Snake/dot.png");
    head.load(":/Snake/head.png");
    apple.load(":/Snake/apple.png");
    head.scaled(DOT_SIZE,DOT_SIZE);
    apple.scaled(DOT_SIZE,DOT_SIZE);
}

void Snake::initGame() {
    dots = 3;
    for (int z = 0; z < dots; z++) {
        x[z] = 50 - z * 2* DOT_SIZE;
        y[z] = 50;
    }
    locateApple();
    timerId = startTimer(DELAY);
}

void Snake::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    doDrawing();
}

void Snake::doDrawing() {
    QPainter qp(this);
    QString message = "Score: ";
    QString score = QString::number(dots-3);
    message += score;
    if (inGame) {
        qp.drawText(5* B_WIDTH / 4, B_HEIGHT/2 , message);
        QLine checkLine1;
        checkLine1.setLine(0, y[0] , B_WIDTH, y[0]);
        QLine checkLine2;
        checkLine2.setLine(0 ,y[0] + 39, B_WIDTH, y[0] + 39);
        qp.drawLine(checkLine2);
        qp.drawLine(checkLine1);
        qp.drawLine(horLine);
        qp.drawLine(verLine);
        qp.drawImage(apple_x, apple_y, apple);
        for (int z = 0; z < dots; z++) {
            if (z == 0) {
                qp.drawImage(x[z], y[z], head);
            } else {
                qp.drawImage(x[z], y[z], dot);
            }
        }
    } else {
        gameOver(qp);
    }
}

void Snake::gameOver(QPainter &qp) {
    QString message = "Game over";
    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);
    int textWidth = fm.width(message);
    qp.setFont(font);
    int h = height();
    int w = width();
    qp.translate(QPoint(w/2, h/2));
    qp.drawText(-textWidth/2, 0, message);
}

void Snake::checkApple() {
    if ((abs(x[0] - apple_x) < 40) && (abs(y[0] - apple_y) < 40)) {
        dots++;
        locateApple();
    }
}

void Snake::move() {
    for (int z = dots; z > 0; z--) {
        x[z] = x[(z - 1)];
        y[z] = y[(z - 1)];
    }
    if (leftDirection) {
        x[0] -= DOT_SIZE;
    }
    if (rightDirection) {
        x[0] += DOT_SIZE;
    }
    if (upDirection) {
        y[0] -= DOT_SIZE;
    }
    if (downDirection) {
        y[0] += DOT_SIZE;
    }
}

void Snake::checkCollision() {
    for (int z = dots; z > 0; z--) {
        if ((z > 4) && (x[0] == x[z]) && (y[0] == y[z])) {
            inGame = false;
        }
    }
    if (y[0] >= B_HEIGHT - 49) {
        inGame = false;
    }
    if (y[0] < 0) {
        inGame = false;
    }
    if (x[0] >= B_WIDTH - 49) {
        inGame = false;
    }
    if (x[0] < 0) {
        inGame = false;
    }
    if(!inGame) {
        killTimer(timerId);
    }
}

void Snake::locateApple() {

    QTime time = QTime::currentTime();
    qsrand((uint) time.msec());

    int r = qrand() % RAND_POS;
    apple_x = (r * DOT_SIZE);

    r = qrand() % RAND_POS;
    apple_y = (r * DOT_SIZE);
    if ((apple_x > B_WIDTH - 39)||(apple_y > B_HEIGHT - 39)) locateApple();
}

void Snake::timerEvent(QTimerEvent *e) {
    Q_UNUSED(e);
    if (inGame) {
        checkApple();
        checkCollision();
        move();
    }
    repaint();
}

void Snake::keyPressEvent(QKeyEvent *e) {
    int key = e->key();
    if ((key == Qt::Key_Left) && (!rightDirection)) {
        leftDirection = true;
        upDirection = false;
        downDirection = false;
    }
    if ((key == Qt::Key_Right) && (!leftDirection)) {
        rightDirection = true;
        upDirection = false;
        downDirection = false;
    }
    if ((key == Qt::Key_Up) && (!downDirection)) {
        upDirection = true;
        rightDirection = false;
        leftDirection = false;
    }
    if ((key == Qt::Key_Down) && (!upDirection)) {
        downDirection = true;
        rightDirection = false;
        leftDirection = false;
    }
    QWidget::keyPressEvent(e);
}

void Snake::closeEvent(QCloseEvent * e)
{
    this->ProMgr->stopProcess(pid);
    QWidget::closeEvent(e);
}

