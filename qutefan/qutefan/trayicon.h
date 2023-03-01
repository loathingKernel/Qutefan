#ifndef TRAYICON_H
#define TRAYICON_H

#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QMainWindow>
#include <QPainter>

class QuteFanTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit QuteFanTrayIcon(QMainWindow *parent = nullptr);
    ~QuteFanTrayIcon();

public slots:
    void updateMenu();
    void updateTooltip(QString);

private slots:
    void showWindow();
    void hideWindow();

    void activated(QSystemTrayIcon::ActivationReason);

private:
    QAction* actionShow;
    QAction* actionHide;
    QAction* actionQuit;
    QMenu* menu;

    QMainWindow* main;
};

#endif // TRAYICON_H
