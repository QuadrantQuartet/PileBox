#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QGraphicsScene>

namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    bool eventFilter(QObject* watched, QEvent* event) override;
    ~GameWidget();

private:
    Ui::GameWidget *ui;
    QGraphicsScene* graphicsScene;
    bool graphicDragging = false;
    QPoint graphicDragStartPoint;
    QPoint graphicDragOffset = {0, 0};

    /// 在绘图区域中添加一个正方形
    void addRect(const QPointF& pos);
    /// 开始拖拽平移
    void startDrag(const QPoint& pos);
    /// 更新拖拽平移过程
    void updateDrag(const QPoint& pos);
    /// 结束拖拽平移
    void endDrag();

    /// 缩放绘图区域
    void graphicsScale(double factor);
    /// 平移绘图区域
    void graphicsTranslate(const QPointF& offset);

    /// 将窗口坐标转换为绘图区域坐标
    [[nodiscard]] QPointF mapToScene(const QPoint& pos) const;
};

#endif // GAMEWIDGET_H
