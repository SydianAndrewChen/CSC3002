
#pragma once

#include <QWidget>
#include <QKeyEvent>
#include "processmgr.h"

class Snake : public QWidget {

public:
      Snake(QWidget *parent = 0);
      void sendSignal();

protected:
      void paintEvent(QPaintEvent *);
      void timerEvent(QTimerEvent *);
      void keyPressEvent(QKeyEvent *);
      void closeEvent(QCloseEvent *);

private:
      QImage dot;
      QImage head;
      QImage apple;
      QLine verLine;
      QLine horLine;

      static const int B_WIDTH = 300;
      static const int B_HEIGHT = 300;
      static const int DOT_SIZE = 10;
      static const int ALL_DOTS = 900;
      static const int RAND_POS = 29;
      static const int DELAY = 140;

      int timerId;
      int dots;
      int apple_x;
      int apple_y;

      int x[ALL_DOTS];
      int y[ALL_DOTS];

      bool leftDirection;
      bool rightDirection;
      bool upDirection;
      bool downDirection;
      bool inGame;

      void loadImages();
      void initGame();
      void locateApple();
      void checkApple();
      void checkCollision();
      void move();
      void doDrawing();
      void gameOver(QPainter &);

      ProcessManager * ProMgr;
      long pid;

      friend class System;
};
