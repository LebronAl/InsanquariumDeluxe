#ifndef FISH_H
#define FISH_H

#include <QLabel>
#include <QPointF>
#include "gamescene.h"

class PlayingScene;

class Fish : public QLabel   //Fish Base class
{
    Q_OBJECT
public:
    Fish(QWidget *parent = 0);
    virtual ~Fish();
    virtual void wander();    //function wander to show all Fish's behaviour
    void hanging();   // function hanging to show Fish's behaviour when canEat is false
    virtual void checkAttr(); //function checkAttr to check Fish's Attributes
    virtual void findFood();  //function findFood for Fish to find food
    virtual int coinType();   //function coinType for Fish to produce different kinds of coin
    qreal Dis(const QPointF& pt1, const QPointF& pt2);  //Figure out the distance between two Points
    int currentSwim;  //current index of swimPix
    int currentTurn;  //current index of turnPix
    int currentDie;   //current index of diePix
    int currentEat;   //current index of eatPix
    int hungerDegree; //the degree of hunger
    int growth;       //the growth of fish
    int growth_last;  //for check
    int speed;        //the speed of fish
    int fishId;       //the id of fish
    int foodId;       //the id of food
    int foodIndex;    //the index of food which was eaten in foodList
    int state;        //the state of fish
    int produceCoinSpeed;//the speed for fish to produce coin
    int foodWidth;    //the width of it's food pix
    int foodHeight;   //the width of it's food pix
    bool arriveTarget;//does fish arrive the target?
    bool canEat;      //can fish eat?
    bool isLeft;      //does fish face left?
    bool isHungry;    //is fish hungry?
    qreal maxDis;     //a big number
    qreal xdelta;     //x -shift
    qreal ydelta;     //y -shift
    QPointF currentPos; //current position
    QPointF targetPos;  //target position
    QVector<QPixmap> swimPix;  //swim pix
    QVector<QPixmap> turnPix;  //turn pix
    QVector<QPixmap> eatPix;   //eat pix
    QVector<QPixmap> hungrySwimPix;  //hungrySwim pix
    QVector<QPixmap> hungryEatPix;   //hungryEat pix
    QVector<QPixmap> hungryTurnPix;  //hungryTurn pix
    QVector<QPixmap> diePix;         //die Pix
    PlayingScene *scene;        //playingScene
};

class GubiFish:public Fish   //GubiFish class
{
    Q_OBJECT
public:
    GubiFish(const int id,QWidget *parent = 0);
    GubiFish(const int id,QPointF &p,QWidget *parent = 0);
    ~GubiFish();
    void wander();    //function wander to show all Fish's behaviour
    void checkAttr(); //function checkAttr to check Fish's Attributes
    void findFood();  //function findFood for Fish to find food
    int coinType();   //function coinType for Fish to produce different kinds of coin
    int type = 0;         //0-small Gubi 1-middle Gubi 2-big Gubi，3-old Gubi king
signals:
    void GubiFishDie(int);
    void FoodDie(int);
    void produceCoin(int,int);
};

class CarnivoreFish:public Fish // CarnivoreFish class
{
    Q_OBJECT
public:
    CarnivoreFish(const int id,QWidget *parent = 0);
    ~CarnivoreFish();
    void wander();    //function wander to show all Fish's behaviour
    void checkAttr(); //function checkAttr to check Fish's Attributes
    void findFood();  //function findFood for Fish to find food
    int coinType();   //function coinType for Fish to produce different kinds of coin
signals:
    void GubiFishBeEaten(int);
    void CarnivoreFishDie(int);
    void produceCoin(int,int);
};

class BigCarnivoreFish:public Fish //BigCarnivoreFish class
{
    Q_OBJECT
public:
    BigCarnivoreFish(const int id,QWidget *parent = 0);
    ~BigCarnivoreFish();
    void wander();    //function wander to show all Fish's behaviour
    void checkAttr(); //function checkAttr to check Fish's Attributes
    void findFood();  //function findFood for Fish to find food
    int coinType();   //function coinType for Fish to produce different kinds of coin
    int type = 0;     //0-CarnivoreFish 1-big CarnivoreFish
    int foodWidth = 80;   //food width
    int foodHeight = 50;  //food height
signals:
    void CarnivoreFishBeEaten(int);
    void BigCarnivoreFishDie(int);
    void produceCoin(int,int);
};


class GubiMomFish:public Fish   //GubiMomFish class
{
    Q_OBJECT
public:
    GubiMomFish(const int id,QWidget *parent = 0);
    ~GubiMomFish();
    void wander();    //function wander to show all Fish's behaviour
    void checkAttr(); //function checkAttr to check Fish's Attributes
    void findFood();  //function findFood for Fish to find food
    int produceKidSpeed = 300; //the speed of GubiMomFish's produceing kid
    int type = 0;     //0-middle GubiMomFish，1-big GubiMomFish
signals:
    void FoodDie(int);
    void GubiMomFishDie(int);
    void produceGubiFish(int);
};

#endif // FISH_H
