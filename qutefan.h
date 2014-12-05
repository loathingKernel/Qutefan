#ifndef QUTEFAN_H
#define QUTEFAN_H

#include <QMainWindow>

namespace Ui {
class QuteFan;
}

class QuteFan : public QMainWindow
{
    Q_OBJECT

public:
    explicit QuteFan(QWidget *parent = 0);
    ~QuteFan();

private:
    Ui::QuteFan *ui;
};

#endif // QUTEFAN_H
