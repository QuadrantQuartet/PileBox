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

  public:
    explicit GameWidget(QWidget* parent = nullptr);
    bool eventFilter(QObject* watched, QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    ~GameWidget() override;

  private:
    Ui::GameWidget* ui;
    BoxScene* boxScene;
    QPointF offset = QPointF(0, 0);

    /// 在绘图区域中添加一个正方形
    void addRect(const QPointF& pos);
    /// 缩放绘图区域
    void graphicsScale(double factor);
    /// 平移绘图区域
    void graphicsTranslate();

    /// 将窗口坐标转换为绘图区域坐标
    [[nodiscard]] QPointF mapToScene(const QPoint& pos) const;
};

#endif  // GAMEWIDGET_H
