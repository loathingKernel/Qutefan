#include "qutefan.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("loathingkernel");
    a.setApplicationName("Qutefan");
    a.setApplicationVersion("0.0.1");

    QFont f;
    f.setHintingPreference(QFont::PreferFullHinting);
    a.setFont(f);

    QuteFan w;
    w.setWindowTitle(a.applicationName());
    w.show();

    return a.exec();
}
