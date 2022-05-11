#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    bool eventFilter(QObject *watched, QEvent *event) override;

  private:
    Ui::MainWindow *ui;
    QGraphicsScene *graphicsScene;
    bool graphicDragging = false;
    QPoint graphicDragStartPoint;
    QPoint graphicDragOffset = {0, 0};
};

#endif  // MAINWINDOW_H
