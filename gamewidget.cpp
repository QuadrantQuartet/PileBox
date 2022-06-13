#include "gamewidget.h"
#include "ui_gamewidget.h"

#include <box2d/box2d.h>

#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QMouseEvent>

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);
//    ui->graphicsView->grabMouse();
    ui->graphicsView->installEventFilter(this);
    // 初始化图形界面
    this->graphicsScene = new QGraphicsScene(this);
    this->graphicsScene->setSceneRect(0, 0, ui->graphicsView->width(),
                                      ui->graphicsView->height());
    ui->graphicsView->setScene(this->graphicsScene);
}

GameWidget::~GameWidget()
{
    delete ui;
}

bool GameWidget::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->graphicsView) {
        auto eventType = event->type();
        qDebug() << eventType;
        if (eventType == QEvent::MouseButtonPress) {
            auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::RightButton) {
                addRect(mapToScene(mouseEvent->pos()));
                return true;
            } else if (mouseEvent->button() == Qt::LeftButton) {
                startDrag(mouseEvent->pos());
                return true;
            }
        } else if (eventType == QEvent::MouseButtonRelease) {
            endDrag();
            return true;
        } else if (eventType == QEvent::MouseMove && graphicDragging) {
            auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
            updateDrag(mouseEvent->pos());
            graphicsTranslate(graphicDragOffset);
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
    auto *item = new QGraphicsRectItem(pos.x() - 50, pos.y() - 50, 100, 100);
    item->setBrush(Qt::red);
    graphicsScene->addItem(item);
}

void GameWidget::startDrag(const QPoint &pos) {
    graphicDragging = true;
    QCursor cursor;
    cursor.setShape(Qt::ClosedHandCursor);
    ui->graphicsView->setCursor(cursor);
    graphicDragStartPoint = pos;
}

void GameWidget::updateDrag(const QPoint &pos) {
    graphicDragOffset -= pos - graphicDragStartPoint;
    graphicDragStartPoint = pos;
}

void GameWidget::endDrag() {
    graphicDragging = false;
    QCursor cursor;
    cursor.setShape(Qt::ArrowCursor);
    ui->graphicsView->setCursor(cursor);
}

void GameWidget::graphicsScale(double factor) {
    ui->graphicsView->scale(factor, factor);
}

void GameWidget::graphicsTranslate(const QPointF &offset) {
    auto delta = ui->graphicsView->transform().m11();
    graphicsScene->setSceneRect(
        QRectF(offset / delta, ui->graphicsView->size()));
}

QPointF GameWidget::mapToScene(const QPoint &pos) const {
    return ui->graphicsView->mapToScene(pos);
}



