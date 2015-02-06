#include "qutefantrayicon.h"

QuteFanTrayIcon::QuteFanTrayIcon(QMainWindow *parent) :
    QSystemTrayIcon(parent)
{
    main = parent;

    actionShow = new QAction(tr("&Show"), this);
    connect(actionShow, SIGNAL(triggered()), this, SLOT(showWindow()));

    actionHide = new QAction(tr("&Hide"), this);
    connect(actionHide, SIGNAL(triggered()), this, SLOT(hideWindow()));

    actionQuit = new QAction(tr("&Quit"), this);
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    menu = new QMenu();
    menu->addAction(actionShow);
    menu->addAction(actionHide);
    menu->addSeparator();
    menu->addAction(actionQuit);

    setContextMenu(menu);
    setIcon(parent->windowIcon());

    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(activated(QSystemTrayIcon::ActivationReason)));

    QTimer::singleShot(0, this, SLOT(updateMenu()));
}

void QuteFanTrayIcon::updateMenu()
{
    actionShow->setVisible(!main->isVisible());
    actionHide->setVisible(main->isVisible());
}

void QuteFanTrayIcon::showWindow()
{
    actionShow->setVisible(false);
    actionHide->setVisible(true);
    main->show();
}

void QuteFanTrayIcon::hideWindow()
{
    actionHide->setVisible(false);
    actionShow->setVisible(true);
    main->hide();
}

void QuteFanTrayIcon::activated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        if(main->isMinimized())
            main->showNormal();
        break;
    case QSystemTrayIcon::DoubleClick:
        if(main->isVisible())
            hideWindow();
        else
            showWindow();
        break;
    }

}

