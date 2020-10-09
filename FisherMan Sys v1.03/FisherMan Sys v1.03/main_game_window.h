#ifndef MAIN_GAME_WINDOW_H
#define MAIN_GAME_WINDOW_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class main_game_window;
}

class GameModel; // Declare the game model calss

class main_game_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit main_game_window(QWidget *parent = nullptr);
    ~main_game_window();
    int pid;

signals:
    void closed(long);

protected:
    virtual void paintEvent(QPaintEvent *event);       // Redraw the UI
    virtual void mousePressEvent(QMouseEvent *event);  // Get the click of mouse
private:
    Ui::main_game_window *ui;

    GameModel *game; // game
    QTimer *timer;     // timer
    QLabel *timeLabel; // time number

    void handleGameState(GameModel *game); // Deal with the game situation
private slots:
    void onStartGameClicked();    // Begin the game
    void onLevelChooseClicked();  // Choose the level
    void onQuitClicked();         // Quit the game
    void updateTimer();           // Record the time
};

#endif // MAIN_GAME_WINDOW_H
