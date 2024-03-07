#ifndef GPUTAB_NVCTRL_H
#define GPUTAB_NVCTRL_H

#include <QDateTime>

#include "gputab.h"
#include "qnvctrl.h"

namespace Ui {
class GpuTabNVCtrl;
}

class GpuTabNVCtrl : public GpuTab
{
public:
    explicit GpuTabNVCtrl(ControlNVCtrl*, ControlNVCtrl::NvGPU*, QSettings*, QWidget* parent = nullptr);
    ~GpuTabNVCtrl();

    void saveGpuSettings();
    void setGPUDefaults();
    void regulateFans();
    void displayFrequencies();

private slots:
    void resetMaximums();
#if USE_CHARTS
    void showChart();
#endif

private:
    ControlNVCtrl           *m_api;
    ControlNVCtrl::NvGPU    *m_gpu;

#if USE_CHARTS
    typedef struct {
        QDateTime time;
        int level;
        int temp;
    } NvLogger;
    NvLogger history[600] = {};
    NvLogger* current_entry = history;
# endif
};

#endif // GPUTAB_NVCTRL_H
