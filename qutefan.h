#ifndef QUTEFAN_H
#define QUTEFAN_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include "gputab.h"
#include "qutefantrayicon.h"

#include "platforms/nvapi/qnvapi.h"
#include "platforms/nvctrl/qnvctrl.h"

namespace Ui {
class QuteFan;
}

class QuteFan : public QMainWindow
{
    Q_OBJECT

public:
    explicit QuteFan(QWidget *parent = 0);
    ~QuteFan();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void regulateFan();
    void intervalChanged(int);

private:
    void initializeNvAPI();
    void initializeNVCtrl();

    void storeGpuDefaults();
    void restoreGpuDefaults(unsigned int);

    QNvAPI *nvapi;
    NV_GPU_COOLER_LEVELS defaultCoolerLevels[NVAPI_MAX_PHYSICAL_GPUS];

    GpuTab *gpuTab[NVAPI_MAX_PHYSICAL_GPUS];
    GpuTab::FanMode lastMode[NVAPI_MAX_PHYSICAL_GPUS] = {};

    QTimer *timer;
    int interval;

    QuteFanTrayIcon *trayIcon;
    bool firstCloseToTray = true;

    Ui::QuteFan *ui;
};

#endif // QUTEFAN_H
