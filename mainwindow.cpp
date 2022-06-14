#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gamewidget=new GameWidget;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    this->setCentralWidget(gamewidget);
}


void MainWindow::on_exitButton_clicked()
{
    this->close();
}


void MainWindow::on_helpButton_clicked()
{
    QMessageBox msg;
    msg.setText("The rule is very simple.\nJust pile the boxes as higher as you can in order to get higher scores.\nEnjoy you self and challange your best!");
    msg.setStyleSheet("font :12pt");
    msg.setWindowTitle("HelpPage");
    msg.exec();
}

