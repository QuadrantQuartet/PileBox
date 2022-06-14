#include "gamewidget.h"

#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QMouseEvent>
#include <random>

#include "box2dWidget/BoxScene.h"
#include "ui_gamewidget.h"

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameWidget) {
    ui->setupUi(this);
    //    ui->graphicsView->grabMouse();
    ui->graphicsView->installEventFilter(this);
    // 初始化图形界面
    this->boxScene = new BoxScene(this);
    this->graphicsTranslate();
    ui->graphicsView->setScene(this->boxScene);
}

GameWidget::~GameWidget() { delete ui; }

bool GameWidget::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->graphicsView) {
        auto eventType = event->type();
        if (eventType == QEvent::MouseButtonPress) {
            auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
            addRect(mapToScene(mouseEvent->pos()));
            return true;
        } else if (eventType == QEvent::Wheel) {
            auto wheelEvent = dynamic_cast<QWheelEvent *>(event);
            auto factor = wheelEvent->angleDelta().y() / 5.0;
            offset.setY(offset.y() + factor);
            graphicsTranslate();
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void GameWidget::addRect(const QPointF &pos) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(30, 100);
    double width = dis(gen), height = dis(gen);
    auto *item = boxScene->createBody(pos.x(), pos.y(), width, height, 1, 0.3);
    item->setBrush(Qt::red);
}

void GameWidget::graphicsScale(double factor) {
    ui->graphicsView->scale(factor, factor);
}

void GameWidget::graphicsTranslate() {
    auto delta = ui->graphicsView->transform().m11();
    auto size = ui->graphicsView->size();
    auto pos = offset / delta - QPointF(size.width(), size.height()) / 2;
    boxScene->setSceneRect(QRectF(pos, size));
}

QPointF GameWidget::mapToScene(const QPoint &pos) const {
    return ui->graphicsView->mapToScene(pos);
}
void GameWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    ui->graphicsView->resize(this->size());
}
