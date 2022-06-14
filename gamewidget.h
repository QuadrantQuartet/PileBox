#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QGraphicsScene>
#include <QWidget>

#include "box2dWidget/BoxScene.h"

namespace Ui {
class GameWidget;
}

enum class GameState {
    Running,
    Stopped
};

class GameWidget : public QWidget {
    Q_OBJECT
  private:
    QPointF _origin = QPointF(0, 0);
    /// 坐标原点位置偏移
    [[nodiscard]] const QPointF& origin() const;
    void setOrigin(const QPointF& offset);

    qreal _scale = 1.0;
    /// 图形比例缩放
    [[nodiscard]] qreal scale() const;
    void setScale(qreal scale);

  public:
    explicit GameWidget(QWidget* parent = nullptr);
    bool eventFilter(QObject* watched, QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    ~GameWidget() override;

    Q_PROPERTY(QPointF origin READ origin WRITE setOrigin)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)

  private:
    Ui::GameWidget* ui;
    BoxScene* boxScene;
    QTimer* timer;
    /// 最上面的箱子
    b2Body* topBox = nullptr;
    /// 箱子总高度
    double totalHeight = 0;
    /// 除最上面的以外的箱子高度
    double secondTotalHeight = 0;
    /// 游戏状态
    GameState gameState = GameState::Stopped;

    /// 添加箱子
    BoxItem addBox(const QPointF& pos);

    /// 点击添加箱子
    void clickAddBox(const QPointF& pos);

    /// 平滑移动视图
    void smoothScroll(const QPointF& newOrigin, double speed);

    void checkCollision();

    /// 将窗口坐标转换为绘图区域坐标
    [[nodiscard]] QPointF mapToScene(const QPoint& pos) const;
};

#endif  // GAMEWIDGET_H
