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
    explicit GpuTab(QWidget* parent = nullptr);
    ~GpuTab();

    enum class FanMode {
        Off = 0,
        Quiet,
        Fixed,
        Linear,
        Graph
    };

    GpuTab::FanMode getMode();
    GpuTab::FanMode last_mode = FanMode::Off;

    Ui::GpuTab* ui;

    virtual void setGPUDefaults() = 0;
    virtual void regulateFan() = 0;
    virtual void displayStatus() = 0;

public slots:
    virtual void resetMax() = 0;
    virtual void showChart() = 0;
};

#endif // GPUTAB_H
