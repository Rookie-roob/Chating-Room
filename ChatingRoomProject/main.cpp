#include "chatingroom.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatingRoom w;
    w.show();
    return a.exec();
}
