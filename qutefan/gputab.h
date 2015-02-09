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

    enum class FanMode {
        Off = 0,
        Quiet,
        Fixed,
        Linear,
        Graph
    };

    void setTempValue(QString);
    void setLevelValue(QString);

    int getFixedLevel();
    int getLinearOffset();
    FanMode getMode();

private:
    Ui::GpuTab *ui;
};

#endif // GPUTAB_H
