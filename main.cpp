#include "qutefan.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("loathingkernel");
    a.setApplicationName("QuteFan");
    a.setApplicationVersion("0.0.1");
    a.setStyle("GTK+");

    QFont f;
    f.setHintingPreference(QFont::PreferFullHinting);
    a.setFont(f);

    QuteFan w;
    w.setWindowTitle(QString("%1").arg(a.applicationName()));
    w.show();

    return a.exec();
}
