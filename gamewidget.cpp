#include "gamewidget.h"

#include <QGraphicsRectItem>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QTime>
#include <random>

#include "NinePatch/NinePatch.h"
#include "box2dWidget/BoxScene.h"
#include "ui_gamewidget.h"

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameWidget) {
    ui->setupUi(this);
    ui->graphicsView->installEventFilter(this);

    // 初始化图形界面
    ui->graphicsOverlay->setAttribute(Qt::WA_TransparentForMouseEvents);
    this->overlay = new QGraphicsScene(this);
    ui->graphicsOverlay->setScene(this->overlay);
    ui->graphicsOverlay->setSceneRect(-this->width() / 2., 0, this->width(),
                                      this->height());
    auto gameOverWidth = this->width() * 0.8;
    gameOver = new QGraphicsPixmapItem(QPixmap(":/GameOver.png"));
    gameOver->setScale(gameOverWidth / gameOver->pixmap().width());
    gameOver->setPos(-gameOverWidth / 2, 100);

    gameOver->setZValue(100);
    overlay->addItem(gameOver);

    ui->lblScore->setAttribute(Qt::WA_TransparentForMouseEvents);
    connect(ui->btnRestart, &QPushButton::clicked, this, &GameWidget::restart);

    // 初始化定时器
    this->timer = new QTimer(this);
    timer->setInterval(100);
    connect(this->timer, &QTimer::timeout, this, &GameWidget::checkCollision);

    // 启动游戏
    restart();
}

GameWidget::~GameWidget() {
    delete boxScene;
    delete ui;
}

void GameWidget::restart() {
    totalHeight = 0;
    secondTotalHeight = 0;

    auto *oldScene = this->boxScene;
    this->boxScene = new BoxScene(nullptr);
    this->setOrigin(QPointF(0, -200));
    ui->graphicsView->setScene(this->boxScene);

    // 背景图层
    auto bgWidth = this->width() * 1.05;
    auto bgHeight = bgWidth * 8000. / 512.;
    auto *background = new QGraphicsPixmapItem(QPixmap(":/sky.png"));
    background->setScale(bgWidth / background->pixmap().width());
    background->setPos(-bgWidth / 2, -bgHeight * 0.95);
    background->setZValue(1);
    this->boxScene->addItem(background);

    // 初始化第一个箱子
    this->addBox(QPointF(0, -200));
    topBox = nullptr;  // 第一个箱子不参与位置结算

    delete oldScene;

    gameOver->setVisible(false);
    ui->widget->hide();
    timer->start();
    // 初始化游戏状态
    this->gameState = GameState::Running;
}

bool GameWidget::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->graphicsView) {
        auto eventType = event->type();
        if (eventType == QEvent::MouseButtonPress) {
            auto mouseEvent = dynamic_cast<QMouseEvent *>(event);
            auto pos = mapToScene(mouseEvent->pos());
            clickAddBox(pos);
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
        addBox(pos);
    }
    lastTime = currentTime;
}

BoxItem GameWidget::addBox(const QPointF &pos) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(75, 150);

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

    // 加载图片
    QPixmap pixmap = createPixmapFromNinePatchImage(QImage(":/box.9.png"),
                                                    static_cast<int>(width),
                                                    static_cast<int>(height));
    auto brush = QBrush(pixmap);
    auto brush_transform =
        brush.transform().translate(-width / 2, -height / 2);  // 偏移原点
    brush.setTransform(brush_transform);
    box.item->setBrush(brush);

    box.item->setZValue(3);

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
        gameOver->setVisible(true);
        ui->widget->show();
        smoothScroll(QPointF(0, -200), 360);
        gameState = GameState::Stopped;
    }
}
