QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    LogInGUI.cpp \
    MyDatabase.cpp \
    MyServer.cpp \
    RSAEncipherment.cpp \
    Tools.cpp \
    calculator.cpp \
    calendar.cpp \
    errexception.cpp \
    errorexceptionsystem.cpp \
    file.cpp \
    filesystem.cpp \
    filesystemgui.cpp \
    game_model.cpp \
    log.cpp \
    main.cpp \
    main_game_window.cpp \
    memoryallocator.cpp \
    process.cpp \
    processmgr.cpp \
    processmgrGUI.cpp \
    snake.cpp \
    sskgui.cpp \
    system.cpp \
    txteditor.cpp

HEADERS += \
    LogInGUI.h \
    MyDatabase.h \
    MyServer.h \
    RSAEncipherment.h \
    Tools.h \
    calculator.h \
    calendar.h \
    errexception.h \
    file.h \
    filesystem.h \
    filesystemgui.h \
    game_model.h \
    log.h \
    main_game_window.h \
    memoryallocator.h \
    process.h \
    processmgr.h \
    processmgrGUI.h \
    snake.h \
    sskgui.h \
    system.h \
    txteditor.h

FORMS += \
    LogInGUI.ui \
    calculator.ui \
    calendar.ui \
    filesystemgui.ui \
    maingamewindow.ui \
    processmgrGUI.ui \
    snake.ui \
    sskgui.ui \
    system.ui \
    txteditor.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc

DISTFILES += \
    Memo.txt \
    ProcessMgr.png \
    apple.png \
    calender.png \
    case.png \
    clock.png \
    close.png \
    copy.png \
    cut.png \
    dot.png \
    head.png \
    paste.png \
    save.png \
    save_as.png
