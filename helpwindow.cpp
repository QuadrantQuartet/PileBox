#include "helpwindow.h"

#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::HelpWindow) {
    ui->setupUi(this);

    connect(ui->btnClose, &QPushButton::clicked, this,
            [this]() { emit backToMenu(); });
}

HelpWindow::~HelpWindow() { delete ui; }
