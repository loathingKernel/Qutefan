#ifndef GPUTAB_NVCTRL_H
#define GPUTAB_NVCTRL_H

#include "gputab.h"
#include "qutefan_nvctrl.h"

namespace Ui {
class GpuTabNVCtrl;
}

class GpuTabNVCtrl : public GpuTab
{
public:
    explicit GpuTabNVCtrl(QuteFanNVCtrl*, QuteFanNVCtrl::NvGPU*, QWidget* parent = nullptr);
    ~GpuTabNVCtrl();

    void setGPUDefaults();
    void regulateFan();
    void displayStatus();

private slots:
    void resetMax();

private:
    QuteFanNVCtrl* api;
    QuteFanNVCtrl::NvGPU* gpu;

    int default_level;
    int default_control;

    int max_temp;
    int max_level;

    int temp;
    int level;
};

#endif // GPUTAB_NVCTRL_H
