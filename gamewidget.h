#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QGraphicsScene>
#include <QWidget>

#include "box2dWidget/BoxScene.h"

namespace Ui {
class GameWidget;
}

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

    /// 在绘图区域中添加一个正方形
    void addBox(const QPointF& pos);

    void smoothScroll(const QPointF& newOrigin, double speed);

    /// 将窗口坐标转换为绘图区域坐标
    [[nodiscard]] QPointF mapToScene(const QPoint& pos) const;
};

#endif  // GAMEWIDGET_H
