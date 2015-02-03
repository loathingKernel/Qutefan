#ifndef QUTEFAN_H
#define QUTEFAN_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QIcon>
#include <QSystemTrayIcon>
#include "gputab.h"
#include "platforms/nvapi/qnvapi.h"


namespace Ui {
class QuteFan;
}

class QuteFan : public QMainWindow
{
    Q_OBJECT

public:
    explicit QuteFan(QWidget *parent = 0);
    ~QuteFan();

    void saveDefaults();
    void restoreDefaults();

private slots:
    void regulate();

private:
    Ui::QuteFan *ui;
    GpuTab::FanMode lastMode[NVAPI_MAX_PHYSICAL_GPUS] = {};
    QSystemTrayIcon *trayIcon;
    GpuTab *gpuTab[NVAPI_MAX_PHYSICAL_GPUS];
    QTimer *timer;
    QNvAPI *nvapi;
    NV_GPU_COOLER_LEVELS defaultCoolerLevels[NVAPI_MAX_PHYSICAL_GPUS];
};

#endif // QUTEFAN_H
