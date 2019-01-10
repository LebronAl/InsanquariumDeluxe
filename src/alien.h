#ifndef ALIEN_H
#define ALIEN_H

#include "gamescene.h"
#include <QVector>
#include <QMouseEvent>
#include <QPointF>

class PlayingScene;

class Alien:public QLabel   //alien base class
{
    Q_OBJECT
public:
    Alien(QWidget *parent = 0);
    virtual ~Alien();
    virtual void attack();    //function attack for alien to attck fish
    virtual void wander();    //function wander to show all alien's actions
    qreal Dis(const QPointF& pt1, const QPointF& pt2);  //Figure out the distance between two Points
    int health;               //the health of alien
    int speed;                //the speed of alien
    bool isLeft;              //does alien face left
    bool arriveTarget;        //does alien arrive the target
    QVector<QPixmap> swimPix; //swim pix
    QVector<QPixmap> turnPix; //turn pix
    int currentSwim;          //current index of swim
    int currentTurn;          //current index of turn
    int state;                //the state of alien
    int xdelta;               //x-shift
    int ydelta;               //y-shift
    int maxDis;               //a large number
    QPointF currentPos;       //current position
    QPointF targetPos;        //target position
    PlayingScene *scene;      //playingScene

};

class BlueAlien:public Alien  //BlueAlien class
{
    Q_OBJECT
public:
    BlueAlien(int health_,QWidget *parent = 0);
    ~BlueAlien();
    void attack();
    void wander();
    void mousePressEvent(QMouseEvent *ev); // let BlueAlien can be clicked
signals:
    void GubiFishDie(int);
    void CarnivoreFishDie(int);
    void BigCarnivoreFishDie(int);
    void GubiMomFishDie(int);
    void BlueAlienDie(qreal,qreal,int);
};

class EaterAlien:public Alien  //EaterAlien class
{
    Q_OBJECT
public:
    EaterAlien(int health_,QWidget *parent = 0);
    ~EaterAlien();
    void attack();
    void wander();
    void findFood();
    QVector<QPixmap> eatPix;
    int currentEat = 0;
    int foodId;
    int foodIndex;
    int foodWidth = 40;
    int foodHeight = 50;
signals:
    void GubiFishDie(int);
    void CarnivoreFishDie(int);
    void BigCarnivoreFishDie(int);
    void GubiMomFishDie(int);
    void EaterAlienDie(qreal,qreal,int);
    void FoodDie(int);
};

#endif // ALIEN_H
