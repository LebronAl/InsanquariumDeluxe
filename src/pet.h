#ifndef PET_H
#define PET_H

#include <QLabel>
#include <QPointF>
#include "gamescene.h"

class PlayingScene;

class Pet : public QLabel  //Pet base class
{
    Q_OBJECT
public:
    Pet(QWidget *parent = 0);
    virtual ~Pet();
    virtual void hanging();    //action of pet
    virtual void wander();    //all actions of pet
    qreal Dis(const QPointF& pt1, const QPointF& pt2);  //Figure out the distance between two Points
    QVector<QPixmap> swimPix;  //swim pix
    QVector<QPixmap> turnPix;  //turn pix
    int speed;                 //speed of pet
    int currentSwim;           //current index of swimPix
    int currentTurn;           //current index of turnPix
    int state;                 //the state of pet
    int maxDis;                //a large number
    bool isLeft;               //does pet face left
    qreal xdelta;              //x-shift
    qreal ydelta;              //y-shift
    bool arriveTarget;         //does pet arrive target?
    QPointF currentPos;        //current position of pet
    QPointF targetPos;         //target position  of pet
    PlayingScene *scene;       //playingScene
};

class SnailPet:public Pet  //SnailPet class
{
    Q_OBJECT
public:
    SnailPet(QWidget* parent = 0);
    ~SnailPet();
    void wander();
    void hanging();
    void findCoin();            //function findCoin for SnailPet to findCoin
    QVector<QPixmap> hidePix;   //hide Pix
    int currentHide = 0;        //current index of hidePix
    int coinId;                 //the coin id
    int coinIndex;              //the coin index
    int foodWidth = 72;         //the width of food
    int foodHeight = 72;        //the height of food
    bool hasAlien = false;      //does playingScene have alien?
signals:
    void getCoin(int);
};

class ShellPet:public Pet
{
    Q_OBJECT
public:
    ShellPet(const QPointF &p,QWidget* parent = 0);
    ~ShellPet();
    void wander();
    void mousePressEvent(QMouseEvent *ev);
    bool canCollected = false;   //can the pearl be collected?
    bool beCollected = true;     //is the pearl  collected?
    QVector<QPixmap> closePix;   //close pix
    int currentClose = 0;        //current index of closePix
    int producePearlSpeed = 0;   //the speed of produceing Pearl
signals:
    void getPearl();
};

class BonePet:public Pet
{
    Q_OBJECT
public:
    BonePet(QWidget *parent = 0);
    ~BonePet();
    void hanging();
    void wander();
    int produceCoinSpeed = 0; //the speed of BonePet's producing coin
signals:
    void produceCoin(qreal,qreal,int);
};

class SwordPet:public Pet
{
    Q_OBJECT
public:
    SwordPet(QWidget* parent = 0);
    ~SwordPet();
    void hanging();
    void wander();
    void attackAlien();
    int foodWidth = 160;
    int foodHeight = 160;
    bool hasAlien = false;   //does playingScene have alien?
};

class TortoisePet:public Pet
{
    Q_OBJECT
public:
    TortoisePet(QWidget* parent = 0);
    ~TortoisePet();
    void hanging();
    void wander();
};

#endif // PET_H
