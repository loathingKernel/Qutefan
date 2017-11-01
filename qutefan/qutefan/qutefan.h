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

#if defined(Q_OS_WIN)
    #include "qnvapi.h"
#elif defined(Q_OS_LINUX)
    #include "qnvctrl.h"
#endif

namespace Ui {
class QuteFan;
}

class QuteFan : public QMainWindow
{
    Q_OBJECT

public:
    explicit QuteFan(QWidget* parent = 0);
    ~QuteFan();

protected:
    void closeEvent(QCloseEvent* event);

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

#if defined(Q_OS_WIN)
    QNvAPI* nvapi;
#elif defined(Q_OS_LINUX)
    QNVCtrl* nvctrl;
#endif

    QList<GpuTab*> gpuTabs;

    QTimer* timer;
    int interval;

    QuteFanTrayIcon* trayIcon;
    bool firstCloseToTray = true;

    Ui::QuteFan* ui;
};

#endif // QUTEFAN_H
