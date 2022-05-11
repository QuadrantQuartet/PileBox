#include "mainwindow.h"

#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QMouseEvent>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // 监听鼠标事件
    ui->graphicsView->grabMouse();
    ui->graphicsView->installEventFilter(this);
    // 初始化图形界面
    this->graphicsScene = new QGraphicsScene(this);
    this->graphicsScene->setSceneRect(0, 0, ui->graphicsView->width(),
                                      ui->graphicsView->height());
    ui->graphicsView->setScene(this->graphicsScene);
}

MainWindow::~MainWindow() { delete ui; }
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    auto eventType = event->type();
    qDebug() << eventType;
    if (watched == ui->graphicsView) {
        if (eventType == QEvent::MouseButtonPress) {
            auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::RightButton) {
                auto [x, y] = ui->graphicsView->mapToScene(mouseEvent->pos());
                auto *item = new QGraphicsRectItem(x - 50, y - 50, 100, 100);
                item->setBrush(Qt::red);
                this->graphicsScene->addItem(item);
                return true;
            } else if (mouseEvent->button() == Qt::LeftButton) {
                graphicDragging = true;
                QCursor cursor;
                cursor.setShape(Qt::ClosedHandCursor);
                ui->graphicsView->setCursor(cursor);
                graphicDragStartPoint = mouseEvent->pos();
                return true;
            }
        } else if (eventType == QEvent::MouseButtonRelease) {
            graphicDragging = false;
            QCursor cursor;
            cursor.setShape(Qt::ArrowCursor);
            ui->graphicsView->setCursor(cursor);
            return true;
        } else if (eventType == QEvent::MouseMove) {
            if (graphicDragging) {
                auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
                graphicDragOffset -= mouseEvent->pos() - graphicDragStartPoint;
                graphicDragStartPoint = mouseEvent->pos();
                auto delta = ui->graphicsView->transform().m11();
                ui->graphicsView->setSceneRect(QRectF(
                    graphicDragOffset / delta, ui->graphicsView->size()));
                return true;
            }
        } else if (eventType == QEvent::Wheel) {
            auto wheelEvent = dynamic_cast<QWheelEvent *>(event);
            auto factor = exp(wheelEvent->angleDelta().y() / 1000.0);
            ui->graphicsView->scale(factor, factor);
        }
    }
    return QWidget::eventFilter(watched, event);
}
