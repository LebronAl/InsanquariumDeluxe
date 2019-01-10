#include "alien.h"
#include <cmath>
#include <QTime>
#include <QDebug>

Alien::Alien(QWidget *parent)
    :QLabel(parent),
      speed(15),
      isLeft(true),
      arriveTarget(true),
      currentSwim(0),
      currentTurn(0),
      maxDis(10000),
      scene((PlayingScene*)parent)
{}

qreal Alien::Dis(const QPointF &pt1, const QPointF &pt2)
{
     return sqrt(((pt2.x() - pt1.x())*(pt2.x() - pt1.x()) + (pt2.y() - pt1.y())*(pt2.y() - pt1.y())));
}


Alien::~Alien(){}
void Alien::attack(){}
void Alien::wander(){}


BlueAlien::BlueAlien(int health_,QWidget *parent)
    :Alien(parent)
{
    QPixmap pix(":/Resource/images/sylv.png");
    for(int i = 0;i < 2;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            if(i == 0)
            {
                swimPix.push_back(pix.copy(j*(pix.width()/10),i*(pix.height()/2),pix.width()/10,pix.height()/2));
            }
            if(i == 1)
            {
                turnPix.push_back(pix.copy(j*(pix.width()/10),i*(pix.height()/2),pix.width()/10,pix.height()/2));
            }
        }
    }

    state = 2;
    health = health_;

    currentPos.setX(0);
    currentPos.setY(300);
    this->move(currentPos.x(),currentPos.y());
    this->setPixmap(swimPix[0]);
    this->show();

    connect(this,SIGNAL(GubiFishDie(int)),parent,SLOT(GubiFishDied(int)));
    connect(this,SIGNAL(GubiMomFishDie(int)),parent,SLOT(GubiMomFishDied(int)));
    connect(this,SIGNAL(CarnivoreFishDie(int)),parent,SLOT(CarnivoreFishDied(int)));
    connect(this,SIGNAL(BigCarnivoreFishDie(int)),parent,SLOT(BigCarnivoreFishDied(int)));
    connect(this,SIGNAL(BlueAlienDie(qreal,qreal,int)),parent,SLOT(BlueAlienDied(qreal,qreal,int)));
}

BlueAlien::~BlueAlien()
{
    this->deleteLater();
}

void BlueAlien::wander()
{
    if(health  <= 0)
    {
        emit BlueAlienDie(currentPos.x(),currentPos.y(),3);
    }
    switch(state)
    {
        case 1:
        {
            if(isLeft)
            {
                this->setPixmap(turnPix[currentTurn]);
                if(currentTurn < 10)
                {
                    currentTurn++;
                    if(currentTurn == 10)
                    {
                        currentTurn = 0;
                        state = 2;
                        isLeft = false;
                    }
                }
            }
            else
            {
                this->setPixmap(turnPix[currentTurn+9]);
                if(currentTurn + 9 > 0)
                {
                    currentTurn--;
                    if(currentTurn == -9)
                    {
                        currentTurn = 0;
                        state = 2;
                        isLeft = true;
                    }
                }
            }
            break;
        }
        case 2:
        {
            if(isLeft)
            {
                this->setPixmap(swimPix[currentSwim]);
                if(currentSwim < 10)
                {
                    currentSwim++;
                    if(currentSwim == 10)
                    {
                        currentSwim = 0;
                    }
                }
            }
            else
            {
                QImage image = swimPix[currentSwim].toImage();
                QImage image_reverse = image.mirrored(true,false);
                this->setPixmap(QPixmap::fromImage(image_reverse));
                if(currentSwim < 10)
                {
                    currentSwim++;
                    if(currentSwim == 10)
                    {
                        currentSwim = 0;
                    }
                }
            }
        }
        default:
        {
            attack();
        }
    }
    this->move(currentPos.x(),currentPos.y());
}

void BlueAlien::attack()
{
    if(arriveTarget)
    {
        while(true)
        {
            qreal x = qrand()%(640-swimPix[0].width());
            qreal y = 75 + qrand()%(480-75-45- swimPix[0].height());
            if(abs(currentPos.x() - x) > 10)
            {
                targetPos.setX(x);
                targetPos.setY(y);
                if((isLeft&&currentPos.x() < targetPos.x())||(!isLeft&&currentPos.x() > targetPos.x()))
                {
                    state = 1;
                }
                break;
            }
        }
        double k = (double)(targetPos.y() - currentPos.y())/(double)(targetPos.x() - currentPos.x());
        xdelta = speed/sqrt(1+k*k);
        ydelta = speed*k/sqrt(1+k*k);
        arriveTarget = false;
    }
    qreal dis = Dis(currentPos,targetPos);
    if(dis > speed)
    {
        if(Dis(QPoint(currentPos.x() + xdelta,currentPos.y() + ydelta),targetPos) <
                Dis(QPoint(currentPos.x() - xdelta,currentPos.y() - ydelta),targetPos))
        {
            currentPos.setX(currentPos.x() + xdelta);
            currentPos.setY(currentPos.y() + ydelta);
            if(Dis(QPointF(currentPos.x() + xdelta,currentPos.y() + ydelta),targetPos) > dis)
            {
                currentPos.setX(targetPos.x());
                currentPos.setY(targetPos.y());
                arriveTarget = true;
            }
        }
        else
        {
            currentPos.setX(currentPos.x() - xdelta);
            currentPos.setY(currentPos.y() - ydelta);
            if(Dis(QPointF(currentPos.x() - xdelta,currentPos.y() - ydelta),targetPos) > dis)
            {
                currentPos.setX(targetPos.x());
                currentPos.setY(targetPos.y());
                arriveTarget = true;
            }
        }
    }
    else
    {
        currentPos.setX(targetPos.x());
        currentPos.setY(targetPos.y());
        arriveTarget = true;
    }
    for(int i = 0;i < scene->GubiFishList.length();i++)
    {
        if(Dis(QPointF(currentPos.x() + swimPix[0].width()/2,currentPos.y() + swimPix[0].height()/2),
               scene->GubiFishList[i]->currentPos) < 6*speed)
        {
            emit GubiFishDie(scene->GubiFishList[i]->fishId);
        }
    }
    for(int i = 0;i < scene->GubiMomFishList.length();i++)
    {
        if(Dis(QPointF(currentPos.x() + swimPix[0].width()/2,currentPos.y() + swimPix[0].height()/2),
               scene->GubiMomFishList[i]->currentPos) < 6*speed)
        {
            emit GubiMomFishDie(scene->GubiMomFishList[i]->fishId);
        }
    }
    for(int i = 0;i < scene->CarnivoreFishList.length();i++)
    {
        if(Dis(QPointF(currentPos.x() + swimPix[0].width()/2,currentPos.y() + swimPix[0].height()/2),
               scene->CarnivoreFishList[i]->currentPos) < 6*speed)
        {
            emit CarnivoreFishDie(scene->CarnivoreFishList[i]->fishId);
        }
    }
    for(int i = 0;i < scene->BigCarnivoreFishList.length();i++)
    {
        if(Dis(QPointF(currentPos.x() + swimPix[0].width()/2,currentPos.y() + swimPix[0].height()/2),
               scene->BigCarnivoreFishList[i]->currentPos) < 6*speed)
        {
            emit BigCarnivoreFishDie(scene->BigCarnivoreFishList[i]->fishId);
        }
    }
}

void BlueAlien::mousePressEvent(QMouseEvent *e)
{
    health--;
    if(e->x() < 80)
    {
        targetPos.setX(currentPos.x()  + 10 > 320?320:currentPos.x() + 10);
        currentPos.setX(currentPos.x() + 10 > 320?320:currentPos.x() + 10);
    }
    else
    {
        if(!isLeft)
        {
            state = 2;
        }
        targetPos.setX(currentPos.x()  - 10 < 0?0:currentPos.x() - 10);
        currentPos.setX(currentPos.x() - 10 < 0?0:currentPos.x() - 10);
    }
    this->move(currentPos.x(),currentPos.y());
}

EaterAlien::EaterAlien(int health_,QWidget *parent)
    :Alien(parent)
{
    QPixmap pix(":/Resource/images/gus.png");
    for(int i = 0;i < 3;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            if(i == 0)
            {
                swimPix.push_back(pix.copy(j*(pix.width()/10),i*(pix.height()/3),pix.width()/10,pix.height()/3));
            }
            if(i == 1)
            {
                turnPix.push_back(pix.copy(j*(pix.width()/10),i*(pix.height()/3),pix.width()/10,pix.height()/3));
            }
            if(i == 2)
            {
                eatPix.push_back(pix.copy(j*(pix.width()/10),i*(pix.height()/3),pix.width()/10,pix.height()/3));
            }
        }
    }

    state = 2;
    health = health_;

    currentPos.setX(0);
    currentPos.setY(300);
    this->move(currentPos.x(),currentPos.y());
    this->setPixmap(swimPix[0]);
    this->show();

    connect(this,SIGNAL(FoodDie(int)),parent,SLOT(FoodDied(int)));
    connect(this,SIGNAL(GubiFishDie(int)),parent,SLOT(GubiFishDied(int)));
    connect(this,SIGNAL(GubiMomFishDie(int)),parent,SLOT(GubiMomFishDied(int)));
    connect(this,SIGNAL(CarnivoreFishDie(int)),parent,SLOT(CarnivoreFishDied(int)));
    connect(this,SIGNAL(BigCarnivoreFishDie(int)),parent,SLOT(BigCarnivoreFishDied(int)));
    connect(this,SIGNAL(EaterAlienDie(qreal,qreal,int)),parent,SLOT(EaterAlienDied(qreal,qreal,int)));

}

EaterAlien::~EaterAlien()
{
    this->deleteLater();
}

void EaterAlien::findFood()
{
    maxDis = 10000;
    for(int i = 0;i < scene->FoodList.length();i++)
    {
        if(Dis(currentPos,QPointF(scene->FoodList[i]->x(),scene->FoodList[i]->y())) < maxDis)
        {
            maxDis = Dis(currentPos,QPointF(scene->FoodList[i]->x(),scene->FoodList[i]->y()));
            foodIndex = i;
            foodId = scene->FoodList[i]->foodId;
        }
    }
    targetPos.setX(scene->FoodList[foodIndex]->x());
    targetPos.setY(scene->FoodList[foodIndex]->y());
    if((isLeft&&currentPos.x() + swimPix[0].width()/2 < targetPos.x() + foodWidth/2)
            ||(!isLeft&&currentPos.x() + swimPix[0].width()/2 > targetPos.x() + foodWidth/2))
    {
        state = 2;
    }
    if(abs(targetPos.x() + foodWidth/2 - currentPos.x() - swimPix[0].width()/2) > 1)
    {
        double k = (double)(targetPos.y() + foodHeight/2 - currentPos.y() - swimPix[0].height()/2)/(double)(targetPos.x() + foodWidth/2 - currentPos.x() - swimPix[0].width()/2);
        xdelta = speed/sqrt(1+k*k);
        ydelta = speed*k/sqrt(1+k*k);
    }
    else
    {
        xdelta = 0;
        if((targetPos.y() +  foodHeight/2 > currentPos.y() + swimPix[0].height()/2))
        {
            ydelta = -speed;
        }
        else
        {
            ydelta = speed;
        }
    }
    if(isLeft)
    {
        if(Dis(QPointF(currentPos.x() + swimPix[0].width()/2,currentPos.y()+ swimPix[0].height()/2),
               QPointF(targetPos.x() + foodWidth/2,targetPos.y() + foodHeight/2)) > speed)

        {
            currentPos.setX(currentPos.x() - xdelta);
            currentPos.setY(currentPos.y() - ydelta);
        }
        else
        {
            currentPos.setX(targetPos.x() + foodWidth/2 - swimPix[0].width()/2);
            currentPos.setY(targetPos.y() + foodHeight/2 - swimPix[0].height()/2);
            targetPos = currentPos;
            arriveTarget = true;
            state = 1;
            health = health - (scene->foodType + 1);
        }
    }
    else
    {
        if(Dis(QPointF(currentPos.x() + swimPix[0].width()/2,currentPos.y()+ swimPix[0].height()/2),
               QPointF(targetPos.x() + foodWidth/2,targetPos.y() + foodHeight/2)) > speed)
        {
            if((targetPos.y() +  foodHeight/2 > currentPos.y() + swimPix[0].height()/2)&&xdelta == 0)
            {
                ydelta = -ydelta;
            }
            currentPos.setX(currentPos.x() + xdelta);
            currentPos.setY(currentPos.y() + ydelta);
        }
        else
        {
            currentPos.setX(targetPos.x() + foodWidth/2 - swimPix[0].width()/2);
            currentPos.setY(targetPos.y() + foodHeight/2 - swimPix[0].height()/2);
            targetPos = currentPos;
            arriveTarget = true;
            state = 1;
            health = health - (scene->foodType + 1);
        }
    }
}

void EaterAlien::attack()
{
    if(arriveTarget)
    {
        while(true)
        {
            qreal x = qrand()%(640-swimPix[0].width());
            qreal y = 75 + qrand()%(480-75-45- swimPix[0].height());
            if(abs(currentPos.x() - x) > 10)
            {
                targetPos.setX(x);
                targetPos.setY(y);
                if((isLeft&&currentPos.x() < targetPos.x())||(!isLeft&&currentPos.x() > targetPos.x()))
                {
                    state = 2;
                }
                break;
            }
        }
        double k = (double)(targetPos.y() - currentPos.y())/(double)(targetPos.x() - currentPos.x());
        xdelta = speed/sqrt(1+k*k);
        ydelta = speed*k/sqrt(1+k*k);
        arriveTarget = false;
    }
    qreal dis = Dis(currentPos,targetPos);
    if(dis > speed)
    {
        if(Dis(QPoint(currentPos.x() + xdelta,currentPos.y() + ydelta),targetPos) <
                Dis(QPoint(currentPos.x() - xdelta,currentPos.y() - ydelta),targetPos))
        {
            currentPos.setX(currentPos.x() + xdelta);
            currentPos.setY(currentPos.y() + ydelta);
            if(Dis(QPointF(currentPos.x() + xdelta,currentPos.y() + ydelta),targetPos) > dis)
            {
                currentPos.setX(targetPos.x());
                currentPos.setY(targetPos.y());
                arriveTarget = true;
            }
        }
        else
        {
            currentPos.setX(currentPos.x() - xdelta);
            currentPos.setY(currentPos.y() - ydelta);
            if(Dis(QPointF(currentPos.x() - xdelta,currentPos.y() - ydelta),targetPos) > dis)
            {
                currentPos.setX(targetPos.x());
                currentPos.setY(targetPos.y());
                arriveTarget = true;
            }
        }
    }
    else
    {
        currentPos.setX(targetPos.x());
        currentPos.setY(targetPos.y());
        arriveTarget = true;
    }
    for(int i = 0;i < scene->GubiFishList.length();i++)
    {
        if(Dis(QPointF(currentPos.x() + swimPix[0].width()/2,currentPos.y() + swimPix[0].height()/2),
               scene->GubiFishList[i]->currentPos) < 6*speed)
        {
            health--;
            emit GubiFishDie(scene->GubiFishList[i]->fishId);
        }
    }
    for(int i = 0;i < scene->GubiMomFishList.length();i++)
    {
        if(Dis(QPointF(currentPos.x() + swimPix[0].width()/2,currentPos.y() + swimPix[0].height()/2),
               scene->GubiMomFishList[i]->currentPos) < 6*speed)
        {
            health--;
            emit GubiMomFishDie(scene->GubiMomFishList[i]->fishId);
        }
    }
    for(int i = 0;i < scene->CarnivoreFishList.length();i++)
    {
        if(Dis(QPointF(currentPos.x() + swimPix[0].width()/2,currentPos.y() + swimPix[0].height()/2),
               scene->CarnivoreFishList[i]->currentPos) < 6*speed)
        {
            health--;
            emit CarnivoreFishDie(scene->CarnivoreFishList[i]->fishId);
        }
    }
    for(int i = 0;i < scene->BigCarnivoreFishList.length();i++)
    {
        if(Dis(QPointF(currentPos.x() + swimPix[0].width()/2,currentPos.y() + swimPix[0].height()/2),
               scene->BigCarnivoreFishList[i]->currentPos) < 6*speed)
        {
            health = health - 2;
            emit BigCarnivoreFishDie(scene->BigCarnivoreFishList[i]->fishId);
        }
    }
}

void EaterAlien::wander()
{
    if(health <= 0)
    {
        emit EaterAlienDie(currentPos.x(),currentPos.y(),3);
    }
    switch(state)
    {
        case 1:
        {
            if(isLeft)
            {
                this->setPixmap(eatPix[currentEat]);
                if(currentEat < 10)
                {
                    currentEat++;
                    if(currentEat == 1)
                    {
                        emit FoodDie(foodId);
                    }
                    if(currentEat == 10)
                    {
                        currentEat = 0;
                        state = 3;
                    }
                }
            }
            else
            {
                QImage image = eatPix[currentEat].toImage();
                QImage image_reverse = image.mirrored(true,false);
                this->setPixmap(QPixmap::fromImage(image_reverse));
                if(currentEat < 10)
                {
                    currentEat++;
                    if(currentEat == 1)
                    {
                        emit FoodDie(foodId);
                    }
                    if(currentEat == 10)
                    {
                        currentEat = 0;
                        state = 3;
                    }
                }
            }
            break;
        }
        case 2:
        {
            if(isLeft)
            {
                this->setPixmap(turnPix[currentTurn]);
                if(currentTurn < 10)
                {
                    currentTurn++;
                    if(currentTurn == 10)
                    {
                        currentTurn = 0;
                        state = 3;
                        isLeft = false;
                    }
                }
            }
            else
            {
                this->setPixmap(turnPix[currentTurn+9]);
                if(currentTurn + 9 > 0)
                {
                    currentTurn--;
                    if(currentTurn == -9)
                    {
                        currentTurn = 0;
                        state = 3;
                        isLeft = true;
                    }
                }
            }
            break;
        }
        case 3:
        {
            if(isLeft)
            {
                this->setPixmap(swimPix[currentSwim]);
                if(currentSwim < 10)
                {
                    currentSwim++;
                    if(currentSwim == 10)
                    {
                        currentSwim = 0;
                    }
                }
            }
            else
            {
                QImage image = swimPix[currentSwim].toImage();
                QImage image_reverse = image.mirrored(true,false);
                this->setPixmap(QPixmap::fromImage(image_reverse));
                if(currentSwim < 10)
                {
                    currentSwim++;
                    if(currentSwim == 10)
                    {
                        currentSwim = 0;
                    }
                }
            }
        }
        default:
        {
            if(scene->FoodList.length() != 0)
            {
                findFood();
            }
            else
            {
                attack();
            }
        }
        this->move(currentPos.x(),currentPos.y());
    }
}
