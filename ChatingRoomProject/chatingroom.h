#ifndef CHATINGROOM_H
#define CHATINGROOM_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatingRoom; }
QT_END_NAMESPACE

class ChatingRoom : public QMainWindow
{
    Q_OBJECT

public:
    ChatingRoom(QWidget *parent = nullptr);
    ~ChatingRoom();

private:
    Ui::ChatingRoom *ui;
};
#endif // CHATINGROOM_H
