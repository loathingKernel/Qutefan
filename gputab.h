#ifndef GPUTAB_H
#define GPUTAB_H

#include <QWidget>

namespace Ui {
class GpuTab;
}

class GpuTab : public QWidget
{
    Q_OBJECT

public:
    explicit GpuTab(QWidget *parent = 0);
    ~GpuTab();

    void setTempValue(QString);
    void setLevelValue(QString);

private:
    Ui::GpuTab *ui;
};

#endif // GPUTAB_H
