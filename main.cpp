#include "qutefan.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QuteFan w;

    w.show();

    return a.exec();
}
