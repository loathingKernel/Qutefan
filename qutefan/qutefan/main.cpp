#include "qutefan.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("loathingkernel");
    a.setApplicationName("Qutefan");
    a.setApplicationVersion("0.1.0");

    QFont f;
    f.setHintingPreference(QFont::PreferFullHinting);
    a.setFont(f);

    QuteFan w;
    w.show();

    return a.exec();
}
