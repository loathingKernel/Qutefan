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

#include "qnvapi.h"
#include "qnvctrl.h"

#define QUTEFAN_MAX_PHYSICAL_GPUS   16

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
    void onActionAboutTriggered();
    void onActionAboutQtTriggered();

    void regulateFan();
    void intervalChanged(int);

private:
    void initializeNvAPI();
    void initializeNVCtrl();

    void getGpuDefaults();
    void setGpuDefaults(unsigned int);

    QNvAPI *nvapi;

    GpuTab *gpuTab[QUTEFAN_MAX_PHYSICAL_GPUS];

    QTimer *timer;
    int interval;

    QuteFanTrayIcon *trayIcon;
    bool firstCloseToTray = true;

    Ui::QuteFan *ui;
};

#endif // QUTEFAN_H
