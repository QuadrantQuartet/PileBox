#ifndef PILEBOX_STARTMENU_H
#define PILEBOX_STARTMENU_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class StartMenu;
}
QT_END_NAMESPACE

class StartMenu : public QWidget {
    Q_OBJECT

  public:
    explicit StartMenu(QWidget *parent = nullptr);
    ~StartMenu() override;

  signals:
    void startGame();
    void exitGame();
    void helpGame();

  private:
    Ui::StartMenu *ui;
};

#endif  // PILEBOX_STARTMENU_H
