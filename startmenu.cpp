//
// Created by DELL on 2022/6/27.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_StartMenu.h" resolved

#include "startmenu.h"

#include "ui_StartMenu.h"

StartMenu::StartMenu(QWidget *parent) : QWidget(parent), ui(new Ui::StartMenu) {
    ui->setupUi(this);

    connect(ui->startButton, &QPushButton::clicked, this,
            [this]() { emit startGame(); });
    connect(ui->exitButton, &QPushButton::clicked, this,
            [this]() { emit exitGame(); });
    connect(ui->helpButton, &QPushButton::clicked, this,
            [this]() { emit helpGame(); });
}

StartMenu::~StartMenu() { delete ui; }
