#ifndef PILEBOX_HELPWINDOW_H
#define PILEBOX_HELPWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class HelpWindow;
}
QT_END_NAMESPACE

class HelpWindow : public QWidget {
    Q_OBJECT

  public:
    explicit HelpWindow(QWidget *parent = nullptr);
    ~HelpWindow() override;

  signals:
    void backToMenu();

  private:
    Ui::HelpWindow *ui;
};

#endif  // PILEBOX_HELPWINDOW_H
