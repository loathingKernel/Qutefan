#ifndef GPUTAB_NVCTRL_H
#define GPUTAB_NVCTRL_H

#include <QLabel>
#include <QDateTime>

#include "dual_label.h"
#include "gputab.h"
#include "qutefan_nvctrl.h"

namespace Ui {
class GpuTabNVCtrl;
}

class GpuTabNVCtrl : public GpuTab
{
public:
    explicit GpuTabNVCtrl(QuteFanNVCtrl*, QuteFanNVCtrl::NvGPU*, QSettings*, QWidget* parent = nullptr);
    ~GpuTabNVCtrl();

    void saveGpuSettings();
    void setGPUDefaults();
    void regulateFan();
    void displayStatus();

private slots:
    void resetMax();
#if USE_CHARTS
    void showChart();
#endif

private:
    QuteFanNVCtrl* api;
    QuteFanNVCtrl::NvGPU* gpu;

    QLabel*    temp_label;
    DualLabel* temp_info;

    QVector<QLabel*>    fan_label;
    QVector<DualLabel*> fan_info;

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
