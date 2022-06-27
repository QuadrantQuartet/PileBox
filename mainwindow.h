#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "gamewidget.h"
#include "helpwindow.h"
#include "startmenu.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

  private slots:
    void gameStart();
    void gameMenu();
    void gameExit();
    void gameHelp();

  private:
    Ui::MainWindow *ui;
};

#endif  // MAINWINDOW_H
