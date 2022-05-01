#ifndef CHATINGWIDGET_H
#define CHATINGWIDGET_H

#include <QWidget>

namespace Ui {
class ChatingWidget;
}

class ChatingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatingWidget(QWidget *parent = nullptr);
    ~ChatingWidget();

private:
    Ui::ChatingWidget *ui;
};

#endif // CHATINGWIDGET_H
