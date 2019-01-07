#include "qutefan.h"
#include "ui_qutefan.h"


QuteFan::QuteFan(QWidget *parent) : QMainWindow(parent), ui(new Ui::QuteFan)
{
    ui->setupUi(this);
    setWindowTitle(qApp->applicationName());

    timer = new QTimer(this);
    trayIcon = new QuteFanTrayIcon(this);

#if defined(Q_OS_WIN)
    api = new QuteFanNvAPI();
#elif defined(Q_OS_LINUX)
    api = new QuteFanNVCtrl();
#endif
    if(api->available())
        api->initialize();
    else
        QMessageBox::critical(this, "Error", "No supported hardware was found.");

    // Dynamically add tabs for as many GPUs as were found.
    for(uint i = 0; i < api->gpuCount; i++) {
        gpuTabs.append(new GpuTabNvAPI(api, &api->gpu[i]));
        ui->tabWidgetGpu->addTab(gpuTabs[static_cast<int>(i)], QString("%1").arg(api->gpu[i].name));
    }

    // Resize window to the minimum possible and don't let it be resized.
    this->resize(minimumSizeHint());
    this->setFixedSize(this->size());

    // Signals and slots
    connect(timer, SIGNAL(timeout()), this, SLOT(regulateFan()));
    connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(onActionAboutTriggered()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), this, SLOT(onActionAboutQtTriggered()));
    connect(ui->spinBoxInterval, SIGNAL(valueChanged(int)), this, SLOT(intervalChanged(int)));

    timer->start(ui->spinBoxInterval->text().toInt() * 1000);
    trayIcon->show();
}

QuteFan::~QuteFan()
{
    foreach(GpuTab* tab, gpuTabs)
        tab->setGPUDefaults();
    delete ui;
}

void QuteFan::closeEvent(QCloseEvent* event)
{
    if (trayIcon->isVisible()) {
        if(firstCloseToTray) {
            trayIcon->showMessage(tr("Qutefan"), tr("Qutefan will now keep on running in the tray"));
            firstCloseToTray = false;
        }
        this->hide();
        trayIcon->updateMenu();
        event->ignore();
    }
}

void QuteFan::onActionAboutTriggered()
{
    QString format =
            "<h2>%1</h2>"
            "<b>Version:</b> %2<br/>"
            "<b>Author:</b> %3<br/>"
            "<b>Github:</b> <a href=\"https://www.github.com/loathingkernel/qutefan\">Qutefan</a>";

    QString txt = format.arg(qApp->applicationName(), qApp->applicationVersion(), qApp->organizationName());

    QMessageBox::about(this, ui->actionAbout->text(), txt);
}

void QuteFan::onActionAboutQtTriggered()
{
    QMessageBox::aboutQt(this);
}

void QuteFan::regulateFan()
{
    foreach(GpuTab* tab, gpuTabs) {
        tab->regulateFan();
        tab->displayStatus();
    }
}

void QuteFan::intervalChanged(int value)
{
    timer->stop();
    timer->start(value * 1000);
}
