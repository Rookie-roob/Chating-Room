#include "chatingwidget.h"
#include "ui_chatingwidget.h"

ChatingWidget::ChatingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatingWidget)
{
    ui->setupUi(this);
}

ChatingWidget::~ChatingWidget()
{
    delete ui;
}
