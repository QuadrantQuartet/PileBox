#include "mainwindow.h"

#include <QFile>
#include <QMessageBox>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->gameMenu();

    auto style = QFile(":/gameStyle.qss");
    style.open(QFile::ReadOnly);
    setStyleSheet(style.readAll());
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::gameStart() {
    auto* gameWidget = new GameWidget(this);
    connect(gameWidget, &GameWidget::backToMenu, this, &MainWindow::gameMenu);
    this->setCentralWidget(gameWidget);
}

void MainWindow::gameMenu() {
    auto* startMenu = new StartMenu(this);
    connect(startMenu, &StartMenu::startGame, this, &MainWindow::gameStart);
    connect(startMenu, &StartMenu::exitGame, this, &MainWindow::gameExit);
    connect(startMenu, &StartMenu::helpGame, this, &MainWindow::gameHelp);
    this->setCentralWidget(startMenu);
}

void MainWindow::gameHelp() {
    auto* helpWindow = new HelpWindow(this);
    connect(helpWindow, &HelpWindow::backToMenu, this, &MainWindow::gameMenu);
    this->setCentralWidget(helpWindow);
}

void MainWindow::gameExit() { this->close(); }
