#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QMessageBox>
#include "game_model.h"
#include "main_game_window.h"
#include "ui_maingamewindow.h"

// ---------Global Variable------------ //
const int blockSize = 20;
const int offsetX = 5; // Block Width
const int offsetY = 5; // Block Heigt
const int spaceY = 70; // Space on the top
// ------------------------------//


main_game_window::main_game_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::main_game_window)
{
    ui->setupUi(this);
    // Create a time label
    timeLabel = new QLabel(this);
    // Connect the signal and slot
    connect(ui->actionstart, SIGNAL(triggered(bool)), this, SLOT(onStartGameClicked()));
    connect(ui->actionbasic, SIGNAL(triggered(bool)), this, SLOT(onLevelChooseClicked()));
    connect(ui->actionmedium, SIGNAL(triggered(bool)), this, SLOT(onLevelChooseClicked()));
    connect(ui->actionhard, SIGNAL(triggered(bool)), this, SLOT(onLevelChooseClicked()));
    connect(ui->actionquit, SIGNAL(triggered(bool)), this, SLOT(onQuitClicked()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));

    // Create and initialize the game, set all variables.
    // Default level is medium.
    // Define the windows size after creating the game, and start the timer.
    game = new GameModel;
    game->createGame();
    setFixedSize(game->mCol * blockSize + offsetX * 2, game->mRow * blockSize + offsetY * 2 + spaceY);
    timeLabel->setGeometry(game->mCol * blockSize + offsetX * 2 - 80, spaceY / 2, 80, 20);
    timeLabel->setText("Time: " + QString::number(game->timerSeconds) + " s");
    timer->start(1000);
}

main_game_window::~main_game_window()
{
    emit closed(pid);
    delete game;
    game = NULL;
    delete ui;
}

void main_game_window::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap bmpBlocks(":/res/blocks.bmp");
    QPixmap bmpFaces(":/res/faces.bmp");
    QPixmap bmpFrame(":/res/frame.bmp");
    QPixmap bmpNumber(":/res/timenumber.bmp");

    // Draw the emoji
    switch(game->gameState)
    {
    case OVER:
        painter.drawPixmap((game->mCol * blockSize + offsetX * 2) / 2 - 12, spaceY / 2, bmpFaces, 0 * 24, 0, 24, 24); // 24是笑脸的边长,锚点在左上，因为工具栏占了些，所以看起来不再中间
        break;
    case PLAYING:
        painter.drawPixmap((game->mCol * blockSize + offsetX * 2) / 2 - 12, spaceY / 2, bmpFaces, 1 * 24, 0, 24, 24);
        break;
    case WIN:
        painter.drawPixmap((game->mCol * blockSize + offsetX * 2) / 2 - 12, spaceY / 2, bmpFaces, 2 * 24, 0, 24, 24);
        break;
    default:
        painter.drawPixmap((game->mCol * blockSize + offsetX * 2) / 2 - 12, spaceY / 2, bmpFaces, 1 * 24, 0, 24, 24);
        break;
    }

    // Draw the mines number
    int n = game->curMineNumber;
    int posX = (game->mCol * blockSize + offsetX * 2) / 2 - 50; // The position where the last number should be put
    if(n <= 0) // Redraw when n <= o
    {
        painter.drawPixmap(posX, spaceY / 2, bmpNumber, n * 20, 0, 20, 28); // size of the number is 20*28
    }
    while(n > 0) // for digits
    {
        painter.drawPixmap(posX - 20, spaceY / 2, bmpNumber, n % 10 * 20, 0, 20, 28); // start from the first digit
        n /= 10;
        posX -= 20;
    }

    // Draw the mines
    for(int i = 0; i < game->mRow; i++)
    {
        for(int j = 0; j < game->mCol; j++)
        {
            switch(game->gameMap[i][j].curState)
            {
            // According to different situations of a block
            case UN_DIG:
                painter.drawPixmap(j * blockSize + offsetX, i * blockSize + offsetY + spaceY , bmpBlocks, blockSize * 10, 0, blockSize, blockSize);
                break;
            case DIGGED:
                painter.drawPixmap(j * blockSize + offsetX, i * blockSize + offsetY + spaceY, bmpBlocks, blockSize * game->gameMap[i][j].valueFlag, 0, blockSize, blockSize);
                break;
            case MARKED:
                painter.drawPixmap(j * blockSize + offsetX, i * blockSize + offsetY + spaceY, bmpBlocks, blockSize * 11, 0, blockSize, blockSize);
                break;
            case BOMB:
                painter.drawPixmap(j * blockSize + offsetX, i * blockSize + offsetY + spaceY, bmpBlocks, blockSize * 9, 0, blockSize, blockSize);
                break;
            case WRONG_BOMB:
                if(game->gameState == PLAYING || game->gameState == FAULT)
                {
                    // Show the flags when during the game
                    painter.drawPixmap(j * blockSize + offsetX, i * blockSize + offsetY + spaceY, bmpBlocks, blockSize * 11, 0, blockSize, blockSize);
                }
                else if(game->gameState == OVER)
                {
                    // Show the mines when game is end.
                    painter.drawPixmap(j * blockSize + offsetX, i * blockSize + offsetY + spaceY, bmpBlocks, blockSize * 12, 0, blockSize, blockSize);
                }
                break;
            default:
                break;
            }
        }
    }
    handleGameState(game);
}

void main_game_window::handleGameState(GameModel *game)
{
    if(game->gameState == OVER)
    {
        timer->stop();
        qDebug() << "you lose!";
    }
    else if(game->gameState == WIN)
    {
        timer->stop();
        qDebug() << "you win!";
    }
}

void main_game_window::mousePressEvent(QMouseEvent *event)
{
    if(event->y() < spaceY + offsetY)
    {
        int x = event->x();
        int y = event->y();
        // Check whether the emoji was clicked
        if(x >= (game->mCol * blockSize + offsetX * 2) / 2 - 12
        && x <= (game->mCol * blockSize + offsetX * 2) / 2 + 12
        && y >= spaceY / 2
        && y <= spaceY / 2 + 24)
        {
            game->restartGame(); // Restart
            timer->start(1000);
            timeLabel->setText("Time: " + QString::number(game->timerSeconds) + " s"); // Reset the time label
            update();
        }
    }
    else if(game->gameState != OVER && game->gameState != WIN)
    {
        // Blocks can be clicked when the game is not end.
        // Check the position that was clicked
        int px = event->x() - offsetX;
        int py = event->y() - offsetY - spaceY;
        // Find out the block that was clicked
        int row = py / blockSize;
        int col = px / blockSize;
        // Reply to the click
        switch(event->button())
        {
        case Qt::LeftButton:
            game->digMine(row, col);
            update(); // Redraw after every click
            break;
        case Qt::RightButton:
            game->markMine(row, col);
            update();
            break;
        default:
            break;
        }
    }

}

void main_game_window::onStartGameClicked()
{
    qDebug()<<"game started";
    game->restartGame(); // Restart
    timeLabel->setText("Time: " + QString::number(game->timerSeconds) + " s");
    timer->start(1000);
    update();
}

void main_game_window::onLevelChooseClicked()
{
    QAction *actionSender = (QAction *)dynamic_cast<QAction *>(sender());
    if(actionSender == ui->actionbasic)
    {
        qDebug() << "basic";
        // Ser the game first
        game->createGame(8, 10, 15, BASIC);
    }
    else if(actionSender == ui->actionmedium)
    {
        qDebug() << "medium";
        game->createGame(15, 20, 50, MEDIUM);
    }
    else if(actionSender == ui->actionhard)
    {
        qDebug() << "hard";
        game->createGame(20, 30, 100, HARD);
    }
    // Restart the timer
    timer->start(1000);
    // Redraw the UI
    timeLabel->setText("Time: " + QString::number(game->timerSeconds) + " s");
    timeLabel->setGeometry(game->mCol * blockSize + offsetX * 2 - 80, spaceY / 2, 80, 20);
    setFixedSize(game->mCol * blockSize + offsetX * 2, game->mRow * blockSize + offsetY * 2 + spaceY);
}

void main_game_window::updateTimer()
{
    game->timerSeconds++;
    timeLabel->setText("Time: " + QString::number(game->timerSeconds) + " s");
    qDebug() << game->timerSeconds;
}

void main_game_window::onQuitClicked()
{
    this->close();
}
