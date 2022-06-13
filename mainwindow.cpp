#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewidget.h"
#include "ui_gamewidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    GameWidget*gamewidget=new GameWidget;
    this->close();
    gamewidget->show();
}


void MainWindow::on_exitButton_clicked()
{
    this->close();
}

