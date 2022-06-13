#include "gamewidget.h"

#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QMouseEvent>

#include "box2dWidget/BoxScene.h"
#include "ui_gamewidget.h"

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameWidget) {
    ui->setupUi(this);
    //    ui->graphicsView->grabMouse();
    ui->graphicsView->installEventFilter(this);
    // 初始化图形界面
    this->boxScene = new BoxScene(this);
    auto width = static_cast<qreal>(ui->graphicsView->width());
    auto height = static_cast<qreal>(ui->graphicsView->height());
    this->boxScene->setSceneRect(-width / 2, -height / 2, width, height);
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
            auto factor = exp(wheelEvent->angleDelta().y() / 1000.0);
            graphicsScale(factor);
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void GameWidget::addRect(const QPointF &pos) {
    auto *item = boxScene->createBody(pos.x(), pos.y(), 50, 50, 1, 0.3);
    item->setBrush(Qt::red);
}

void GameWidget::graphicsScale(double factor) {
    ui->graphicsView->scale(factor, factor);
}

void GameWidget::graphicsTranslate(const QPointF &offset) {
    auto delta = ui->graphicsView->transform().m11();
    boxScene->setSceneRect(QRectF(offset / delta, ui->graphicsView->size()));
}

QPointF GameWidget::mapToScene(const QPoint &pos) const {
    return ui->graphicsView->mapToScene(pos);
}
void GameWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    ui->graphicsView->resize(this->size());
}
