#ifndef SYSTEM_H
#define SYSTEM_H

#include <QWidget>
#include "processmgrGUI.h"
#include "calendar.h"
#include "snake.h"
#include "calculator.h"
#include "txteditor.h"
#include "filesystemgui.h"
#include "LogInGUI.h"
#include "sskgui.h"
#include "main_game_window.h"

QT_BEGIN_NAMESPACE
namespace Ui { class System; }
QT_END_NAMESPACE

class System : public QWidget
{
    Q_OBJECT

public:
    System(QWidget *parent = nullptr);
    ~System();

public slots:
    void showTime();

private slots:
    void updateProMgr(long pid);

    void on_processMgr_clicked();

    void on_calendar_clicked();

    void on_snakeGame_clicked();

    void on_closedown_clicked();

    void on_fileSystem_clicked();

    void on_txtEditor_clicked();

    void fromFileManagerOpenTxtEditor(QString true_path);

    void fromTxtEditorOpenFileManager(QMainWindow * txtEditor, bool mode);

    void on_calculator_clicked();

    void on_simulatedServerK_clicked();

    void updateSSK();

    void on_restart_clicked();

    void on_mineSweep_clicked();

    void on_checkBoxisFullScreen_clicked(bool checked);

private:
    Ui::System *ui;

    QTimer * timer;

    void checkErr();

    ProcessManager * ProMgr;
    ProcessManagerGUI * ProMgrGUI;
    FileSystem * fs;
    FileSystemGUI * fsGUI;

    Calendar * CalendarGUI;
    Snake * SnakeGUI;
    main_game_window *gamewindow; // mineGame
    Calculator * CalculatorGUI;
    TxtEditor * TEditorGUI;

    LogInGUI * logInGUI;
    sskgui * SSKGUI;

};
#endif // SYSTEM_H
