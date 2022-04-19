#include "chatingroom.h"
#include "ui_chatingroom.h"

ChatingRoom::ChatingRoom(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChatingRoom)
{
    ui->setupUi(this);
}

ChatingRoom::~ChatingRoom()
{
    delete ui;
}

