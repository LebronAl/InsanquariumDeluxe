#ifndef FOOD_H
#define FOOD_H

#include <QLabel>
#include <QVector>
#include <QPointF>

class Food : public QLabel //Food class
{
    Q_OBJECT
public:
    Food(const int id,const int type,const QPointF &p,QWidget* parent = 0);
    ~Food();
    void floating();  //function floating to show food's behaviour
    int foodId;       //id of food
    int speed;        //speed of food
    QPointF currentPos;  //current position
    QPointF targetPos;   //target position
    QVector<QPixmap> foodPix; //food pix
    int currentIndex;         //current index of foodPix
    int count = 0;            //count the time when food is on the earth
signals:
    void FoodDie(int);
};

#endif // FOOD_H
