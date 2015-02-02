#ifndef QUTEFAN_H
#define QUTEFAN_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QTextStream>
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
    QTimer *timer;
    QNvAPI *nvapi;
    NV_GPU_COOLER_LEVELS defaultCoolerLevels[NVAPI_MAX_PHYSICAL_GPUS];
    unsigned int i;
};

#endif // QUTEFAN_H
