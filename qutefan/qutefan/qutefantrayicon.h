#ifndef QUTEFANTRAYICON_H
#define QUTEFANTRAYICON_H

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
    explicit QuteFanTrayIcon(QMainWindow *parent = 0);
    ~QuteFanTrayIcon();

signals:

public slots:
    void updateMenu();
    void updateTooltip(QString);

    void showWindow();
    void hideWindow();

    void activated(QSystemTrayIcon::ActivationReason);

private:
    QAction* actionShow;
    QAction* actionHide;
    QAction* actionQuit;
    QMenu* menu;

    QMainWindow *main;
};

#endif // QUTEFANTRAYICON_H
