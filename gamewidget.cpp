#include "gamewidget.h"

#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPropertyAnimation>
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
    this->setOrigin(QPointF(0, 0));
    ui->graphicsView->setScene(this->boxScene);
}

GameWidget::~GameWidget() { delete ui; }

bool GameWidget::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->graphicsView) {
        auto eventType = event->type();
        if (eventType == QEvent::MouseButtonPress) {
            auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
            addBox(mapToScene(mouseEvent->pos()));
            return true;
        } else if (eventType == QEvent::Wheel) {
            auto wheelEvent = dynamic_cast<QWheelEvent *>(event);
            auto factor = wheelEvent->angleDelta().y() / 5.0;
            this->smoothScroll(QPointF(0, -factor) + origin(), 500);
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void GameWidget::addBox(const QPointF &pos) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(30, 100);

    static double totalHeight = 0;
    double width = dis(gen), height = dis(gen);
    auto *item = boxScene->createBody(pos.x(), pos.y(), width, height, 1, 0.3);
    item->setBrush(Qt::red);

    totalHeight += height;
    if (totalHeight > 400) {
        smoothScroll(QPointF(0, -totalHeight + 400), 360);
    }
}

QPointF GameWidget::mapToScene(const QPoint &pos) const {
    return ui->graphicsView->mapToScene(pos);
}
void GameWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    ui->graphicsView->resize(this->size());
}

const QPointF &GameWidget::origin() const { return _origin; }
void GameWidget::setOrigin(const QPointF &offset) {
    this->_origin = offset;
    auto delta = ui->graphicsView->transform().m11();
    auto size = ui->graphicsView->size();
    auto pos = offset / delta - QPointF(size.width(), size.height()) / 2;
    boxScene->setSceneRect(QRectF(pos, size));
}

qreal GameWidget::scale() const { return _scale; }
void GameWidget::setScale(qreal scale) {
    this->_scale = scale;
    ui->graphicsView->scale(scale, scale);
}
void GameWidget::smoothScroll(const QPointF &newOrigin, double speed) {
    static QPropertyAnimation *animation = nullptr;
    if (animation == nullptr)
        animation = new QPropertyAnimation(this, "origin");

    auto offset = newOrigin - origin();
    double length = std::sqrt(QPointF::dotProduct(offset, offset));
    animation->setDuration(static_cast<int>(length / speed * 1000));
    animation->setStartValue(origin());
    animation->setEndValue(newOrigin);
    animation->start();
}
