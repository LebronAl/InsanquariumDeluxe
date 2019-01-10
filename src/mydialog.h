#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "gamescene.h"

class PlayingScene;

namespace Ui {
class myDialog;
}

class myDialog : public QWidget
{
    Q_OBJECT

public:
    explicit myDialog(QWidget *parent = 0);
    ~myDialog();
    QLabel *GubiLabel;
    QLabel *GubiMomLabel;
    QLabel *CarnioreLabel;
    QLabel *BigCarnioreLabel;
    QLabel *foodLabel;
    QLabel *coinLabel;
    QLabel *resultLabel;
    QPushButton *Btn;
    QVBoxLayout *mainLayout;
public slots:
    void getInfo(bool,int,int,int,int,int,int);
private:
    Ui::myDialog *ui;
};

#endif // MYDIALOG_H
