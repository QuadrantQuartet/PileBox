#include "mainwindow.h"

#include <QFile>

#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    gamewidget = new GameWidget;
    auto style = QFile(":/gameStyle.qss");
    style.open(QFile::ReadOnly);
    setStyleSheet(style.readAll());
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_startButton_clicked() {
    this->setCentralWidget(gamewidget);
}

void MainWindow::on_exitButton_clicked() { this->close(); }

void MainWindow::on_helpButton_clicked() {
    //    QMessageBox::information(this, tr("Dynamic Layouts Help"),
    //                                   tr("This example shows how to change
    //                                   layouts "
    //                                      "dynamically."));
}
