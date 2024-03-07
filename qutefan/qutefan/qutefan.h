#ifndef QUTEFAN_H
#define QUTEFAN_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QSettings>

#include "trayicon.h"

#if defined(Q_OS_WIN)
#include "control_nvapi.h"
#include "gputab_nvapi.h"
#elif defined(Q_OS_LINUX)
#include "qnvctrl.h"
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
    void saveSettings();
    void loadSettings();

    void getGpuDefaults();
    void setGpuDefaults(unsigned int);

    QSettings* settings;

#if defined(Q_OS_WIN)
    ControlNvAPI* m_control_nvapi;
#elif defined(Q_OS_LINUX)
    ControlNVCtrl* m_control_nvctrl;
#endif

    QList<GpuTab*> m_gpu_tab;

    QTimer* timer;
    int interval;

    QuteFanTrayIcon* tray_icon;
    bool first_close_to_tray = true;

    Ui::QuteFan* ui;
};

#endif // QUTEFAN_H
