#include "food.h"
#include <QPixmap>
#include <QDebug>
Food::Food(const int id,const int type,const QPointF& p,QWidget *parent)
    :QLabel(parent),
      speed(3),
      currentPos(p),
      foodId(id),
      currentIndex(type*10)
{
    QPixmap pix_food(":/Resource/images/food.png");
    for(int i = 0;i < 5;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            foodPix.push_back(pix_food.copy(j*(pix_food.width()/10),i*(pix_food.height()/5),pix_food.width()/10,pix_food.height()/5));
        }
    }

    this->move(currentPos.x(),currentPos.y());
    this->setPixmap(foodPix[currentIndex]);
    this->show();

    connect(this,SIGNAL(FoodDie(int)),parent,SLOT(FoodDied(int)));
}

Food::~Food()
{
    this->deleteLater();
}

void Food::floating()
{
    this->setPixmap(foodPix[currentIndex]);
    if(currentIndex < 10)
    {
        currentIndex++;
        if(currentIndex == 10)
        {
            currentIndex = 0;
        }
    }
    else if(currentIndex < 20)
    {
        currentIndex++;
        if(currentIndex == 20)
        {
            currentIndex = 10;
        }
    }
    else if(currentIndex < 30)
    {
        currentIndex++;
        if(currentIndex == 30)
        {
            currentIndex = 20;
        }
    }
    else if(currentIndex < 40)
    {
        currentIndex++;
        if(currentIndex == 40)
        {
            currentIndex = 30;
        }
    }
    else if(currentIndex < 50)
    {
        currentIndex++;
        if(currentIndex == 50)
        {
            currentIndex = 40;
        }
    }

    targetPos.setX(currentPos.x());
    if(currentPos.y() + speed > 400)
    {
        targetPos.setY(400);
        if(count++ == 10)
        {
            emit FoodDie(foodId);
        }
    }
    else
    {
        targetPos.setY(currentPos.y() + speed);
    }
    this->move(targetPos.x(),targetPos.y());
    currentPos = targetPos;
}

