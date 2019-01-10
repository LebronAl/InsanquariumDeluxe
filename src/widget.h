#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "gamescene.h"
#include <QStackedWidget>

namespace Ui {
class Widget;
}

class MainScene;
class LoadingScene;
class PlayingScene;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QStackedWidget *stackWidget;
    LoadingScene *loadingScene;
    MainScene *mainScene;
    PlayingScene *playingScene;
public slots:
    void MainScenePlay(int);
};

#endif // MAINWINDOW_H
