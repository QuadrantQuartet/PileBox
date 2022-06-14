#include "gamewidget.h"

#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QTime>
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
    this->setOrigin(QPointF(0, -200));
    ui->graphicsView->setScene(this->boxScene);

    ui->lblGameOver->hide();
    ui->lblScore->setAttribute(Qt::WA_TransparentForMouseEvents);

    // 初始化第一个箱子
    auto baseBox = this->addBox(QPointF(0, 0));
    baseBox.item->setBrush(Qt::blue);
    topBox = nullptr;  // 第一个箱子不参与位置结算

    // 初始化定时器
    this->timer = new QTimer(this);
    timer->setInterval(100);
    connect(this->timer, &QTimer::timeout, this, &GameWidget::checkCollision);
    timer->start();

    // 初始化游戏状态
    this->gameState = GameState::Running;
}

GameWidget::~GameWidget() { delete ui; }

bool GameWidget::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->graphicsView) {
        auto eventType = event->type();
        if (eventType == QEvent::MouseButtonPress) {
            auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
            auto pos = mapToScene(mouseEvent->pos());
            clickAddBox(pos);
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

void GameWidget::clickAddBox(const QPointF &pos) {
    static QTime lastTime = QTime(0, 0);
    constexpr int coolDownMsecs = 500;

    QTime currentTime = QTime::currentTime();
    if (lastTime.msecsTo(currentTime) < coolDownMsecs) return;

    if (-pos.y() > totalHeight &&
        gameState == GameState::Running) {  // 只允许在上方添加箱子
        auto box = addBox(pos);
        box.item->setBrush(Qt::red);
    }
    lastTime = currentTime;
}

BoxItem GameWidget::addBox(const QPointF &pos) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(30, 100);

    double width = dis(gen), height = dis(gen);
    auto box = boxScene->createBody(pos.x(), pos.y(), width, height, 1, 0.3);

    secondTotalHeight = totalHeight;

    // 根据箱子高度调整视口
    totalHeight += height;
    if (totalHeight > 350 && height > 50) {
        smoothScroll(QPointF(0, -(totalHeight - 350) - 200), 360);
    }
    ui->lblScore->setText(QString("高度: %1m").arg(toMeter(totalHeight)));

    topBox = box.body;

    return box;
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
    animation->setDuration(
        std::min(static_cast<int>(length / speed * 1000), 2000));
    animation->setEasingCurve(QEasingCurve::InOutCubic);
    animation->setStartValue(origin());
    animation->setEndValue(newOrigin);
    animation->start();
}

void GameWidget::checkCollision() {
    if (gameState != GameState::Running) return;
    if (topBox == nullptr) return;
    auto topBoxPos = topBox->GetPosition();
    auto posY = toPixel(topBoxPos.y);
    if (-posY < secondTotalHeight) {
        ui->lblGameOver->show();
        smoothScroll(QPointF(0, -200), 360);
        gameState = GameState::Stopped;
    }
}
