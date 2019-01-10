#ifndef COIN_H
#define COIN_H

#include <QLabel>
#include <QVector>
#include <QPointF>

class Coin : public QLabel
{
    Q_OBJECT
public:
    Coin(const int id,const int type,const QPointF &p,QWidget* parent = 0);
    ~Coin();
    void floating();  //function floating to show coin's behaviour
    int coinId;       //id of coin
    int speed;        //speed of coin
    QPointF currentPos;  //current position
    QPointF targetPos;   //target position
    QVector<QPixmap> coinPix; //coin pix
    int currentIndex;         //current index of coinPix
    int type;                 //coin type
    int count = 0;            //count the time when coin is on the earth
    void mousePressEvent(QMouseEvent *ev); // let coin can be clicked
signals:
    void CoinDie(int);
    void CoinCollect(int);
};

#endif // COIN_H
