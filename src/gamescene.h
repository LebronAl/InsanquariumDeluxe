#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QVector>
#include <QTimer>
#include "fish.h"
#include "alien.h"
#include "pet.h"
#include "food.h"
#include "coin.h"
#include "mydialog.h"
#include "widget.h"

class GubiFish;
class Food;
class CarnivoreFish;
class GubiMomFish;
class BigCarnivoreFish;
class Pet;
class BlueAlien;
class EaterAlien;
class myDialog;
class Widget;

class GameScene : public QWidget      //Base Scene
{
public:
    explicit GameScene(QWidget *parent = 0);
    virtual ~GameScene();
};

class LoadingScene:public GameScene  //Loading Scene
{
    Q_OBJECT
public:
    LoadingScene(QWidget *parent = 0);
    ~LoadingScene();
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
    bool isCompleted = false;
    int currentProgress = 0;
private:
    QPushButton *loadingBtn;
    QTimer *timer;
signals:
    void sendCurrentIndex(int);
public slots:
    void progressUpdate();
};

class MainScene:public GameScene  //Main Scene
{
    Q_OBJECT
public:
    MainScene(QWidget *parent = 0);
    ~MainScene();
    virtual void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
private:
    QPushButton *playBtn;
    QPushButton *timeModeBtn;
    QPushButton *challengeModeBtn;
    QPushButton *aquariumModeBtn;
    QLabel *infoLabel;
    QPushButton *hallOfFameBtn;
    QPushButton *optionsBtn;
    QPushButton *helpBtn;
    QPushButton *quitBtn;
    QVector<QPixmap> tailPixVector;
    QTimer *timer;
    int tailIndex = 0;
signals:
    void sendCurrentIndex(int);
public slots:
    void tailPixChange();
};

class PlayingScene:public GameScene  //Playing Scene
{
    Q_OBJECT
public:
    PlayingScene(int id,QWidget *parent = 0);
    ~PlayingScene();
    virtual void paintEvent(QPaintEvent *event);
    void init();
    void judgeId();
    void mousePressEvent(QMouseEvent *event);
    int CoinValue(int type);
    void figureAlien();
    bool eventFilter(QObject *obj, QEvent *event);
    QTimer *timer;
    QPushButton *backBtn;
    QLabel *wealthLabel;
    QLabel *label_Gubi;
    QLabel *label_foodType;
    QLabel *label_maxType;
    QLabel *label_Carnivore;
    QLabel *label_BigCarniore;
    QLabel *label_GubiMom;
    QLabel *label_egg;
    QLabel *alienWarningLabel;
    QPushButton *button_Gubi;
    QPushButton *button_foodType;
    QPushButton *button_maxType;
    QPushButton *button_Carnivore;
    QPushButton *button_BigCarniore;
    QPushButton *button_GubiMom;
    QPushButton *button_egg;
    QVector<Food *> FoodList;
    QVector<Coin *> CoinList;
    QVector<GubiFish *> GubiFishList;
    QVector<CarnivoreFish *> CarnivoreFishList;
    QVector<BigCarnivoreFish *> BigCarnivoreFishList;
    QVector<GubiMomFish *> GubiMomFishList;
    QVector<BlueAlien *> BlueAlienList;
    QVector<EaterAlien *> EaterAlienList;
    QVector<Pet *> PetList;
    QVector<QPixmap> Gubi;
    QVector<QPixmap> food;
    QVector<QPixmap> Carnivore;
    QVector<QPixmap> BigCarnivore;
    QVector<QPixmap> GubiMom;
    QVector<QPixmap> eggs;
    int currentIndex = 0;
    int currentProcess = 0;
    int wealth = 200000;
    int targetWealth = 50000;
    int maxFood = 1;
    int currentFood = 0;
    int foodType = 0;
    int foodNum = 0;
    int coinNum = 0;
    int GubiFishNum = 0;
    int CarnivoreFishNum = 0;
    int BigCarnivoreFishNum = 0;
    int GubiMomFishNum = 0;
    int BlueAlienHealth = 40;
    int EaterAlienHealth = 100;
    int alienNum = 0;
    bool hasTortoise = false;
    myDialog *mDialog;
    bool canLose = true;
    int sceneId;
    int Alientime;
    int BlueAlientime;
    int EaterAlientime;
signals:
    void sendCurrentIndex(int);
    void sendInfo(bool,int,int,int,int,int,int);
public slots:
    void backBtnClicked();
    void imageChange();
    void FoodDied(int);
    void CoinDied(int);
    void CoinCollected(int);
    void GubiFishDied(int);
    void GubiCoinProduced(int,int);
    void CarnivoreFishDied(int);
    void CarnivoreCoinProduced(int,int);
    void BigCarnivoreFishDied(int);
    void BigCarnivoreCoinProduced(int,int);
    void GubiMomFishDied(int);
    void GubiFishProduced(int);
    void PearlCollected();
    void BonePetProduceCoin(qreal,qreal,int);
    void BlueAlienDied(qreal,qreal,int);
    void EaterAlienDied(qreal,qreal,int);
};

#endif // GAMESCENE_H
