#ifndef QUTEFAN_H
#define QUTEFAN_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QCloseEvent>

#include "qutefantrayicon.h"

#if defined(Q_OS_WIN)
#include "qutefan_nvapi.h"
#include "gputab_nvapi.h"
#elif defined(Q_OS_LINUX)
#include "qutefan_nvctrl.h"
#include "gputab_nvctrl.h"
#endif

namespace Ui {
class QuteFan;
}

class QuteFan : public QMainWindow
{
    Q_OBJECT

public:
    explicit QuteFan(QWidget* parent = nullptr);
    ~QuteFan();

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void onActionAboutTriggered();
    void onActionAboutQtTriggered();

    void regulateFan();
    void intervalChanged(int);

private:
    void getGpuDefaults();
    void setGpuDefaults(unsigned int);

#if defined(Q_OS_WIN)
    QuteFanNvAPI* nvapi;
#elif defined(Q_OS_LINUX)
    QuteFanNVCtrl* nvctrl;
#endif

    QList<GpuTab*> gpuTabs;

    QTimer* timer;
    int interval;

    QuteFanTrayIcon* trayIcon;
    bool firstCloseToTray = true;

    Ui::QuteFan* ui;
};

#endif // QUTEFAN_H
