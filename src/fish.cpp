#include "fish.h"
#include <QTimer>
#include <QTime>
#include <cmath>


Fish::Fish(QWidget *parent)
    :QLabel(parent),
     currentSwim(0),
     currentTurn(0),
     currentDie(0),
     currentEat(0),
     hungerDegree(120),
     growth(0),
     growth_last(0),
     produceCoinSpeed(0),
     arriveTarget(true),
     canEat(false),
     isLeft(true),
     isHungry(false),
     maxDis(10000),
     scene((PlayingScene*)parent)
{}

Fish::~Fish()
{
    this->deleteLater();
}

void Fish::wander(){}
void Fish::checkAttr(){}
void Fish::findFood(){}
int Fish::coinType(){}
void Fish::hanging()
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
                currentPos.setX(targetPos.x() + foodWidth/2 - swimPix[0].width()/2);
                currentPos.setY(targetPos.y() + foodHeight/2 - swimPix[0].height()/2);
                arriveTarget = true;
            }
        }
        else
        {
            currentPos.setX(currentPos.x() - xdelta);
            currentPos.setY(currentPos.y() - ydelta);
            if(Dis(QPointF(currentPos.x() - xdelta,currentPos.y() - ydelta),targetPos) > dis)
            {
                currentPos.setX(targetPos.x() + foodWidth/2 - swimPix[0].width()/2);
                currentPos.setY(targetPos.y() + foodHeight/2 - swimPix[0].height()/2);
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
}

qreal Fish::Dis(const QPointF &pt1, const QPointF &pt2)
{
     return sqrt(((pt2.x() - pt1.x())*(pt2.x() - pt1.x()) + (pt2.y() - pt1.y())*(pt2.y() - pt1.y())));
}

GubiFish::GubiFish(const int id,QWidget *parent)
    :Fish(parent)
{
    QPixmap pix_swim(":/Resource/images/smallswim.png");
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            swimPix.push_back(pix_swim.copy(j*(pix_swim.width()/10),i*(pix_swim.height()/5),pix_swim.width()/10,pix_swim.height()/5));
        }
    }
    QPixmap pix_turn(":/Resource/images/smallturn.png");
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            turnPix.push_back(pix_turn.copy(j*(pix_turn.width()/10),i*(pix_turn.height()/5),pix_turn.width()/10,pix_turn.height()/5));
        }
    }
    QPixmap pix_eat(":/Resource/images/smalleat.png");
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            eatPix.push_back(pix_eat.copy(j*(pix_eat.width()/10),i*(pix_eat.height()/5),pix_eat.width()/10,pix_eat.height()/5));
        }
    }
    QPixmap pix_hungrySwim(":/Resource/images/hungryswim.png");
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            hungrySwimPix.push_back(pix_hungrySwim.copy(j*(pix_hungrySwim.width()/10),i*(pix_hungrySwim.height()/5),pix_hungrySwim.width()/10,pix_hungrySwim.height()/5));
        }
    }
    QPixmap pix_hungryTurn(":/Resource/images/hungryturn.png");
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            hungryTurnPix.push_back(pix_hungryTurn.copy(j*(pix_hungryTurn.width()/10),i*(pix_hungryTurn.height()/5),pix_hungryTurn.width()/10,pix_hungryTurn.height()/5));
        }
    }
    QPixmap pix_hungryEat(":/Resource/images/hungryeat.png");
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            hungryEatPix.push_back(pix_hungryEat.copy(j*(pix_hungryEat.width()/10),i*(pix_hungryEat.height()/5),pix_hungryEat.width()/10,pix_hungryEat.height()/5));
        }
    }
    QPixmap pix_die(":/Resource/images/smalldie.png");
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            diePix.push_back(pix_die.copy(j*(pix_die.width()/10),i*(pix_die.height()/5),pix_die.width()/10,pix_die.height()/5));
        }
    }

    fishId = id;
    speed = 5,
    state = 4;
    foodWidth = 40;
    foodHeight = 50;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    qreal x = qrand()%(640-swimPix[0].width());
    qreal y = 75 + qrand()%(300 - swimPix[0].height());
    currentPos.setX(x);
    currentPos.setY(y);
    this->move(currentPos.x(),currentPos.y());
    this->setPixmap(swimPix[0]);
    this->show();

    connect(this,SIGNAL(GubiFishDie(int)),parent,SLOT(GubiFishDied(int)));
    connect(this,SIGNAL(FoodDie(int)),parent,SLOT(FoodDied(int)));
    connect(this,SIGNAL(produceCoin(int,int)),parent,SLOT(GubiCoinProduced(int,int)));
}

GubiFish::GubiFish(const int id,QPointF &p,QWidget *parent)
    :Fish(parent)
{
    QPixmap pix_swim(":/Resource/images/smallswim.png");
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            swimPix.push_back(pix_swim.copy(j*(pix_swim.width()/10),i*(pix_swim.height()/5),pix_swim.width()/10,pix_swim.height()/5));
        }
    }
    QPixmap pix_turn(":/Resource/images/smallturn.png");
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            turnPix.push_back(pix_turn.copy(j*(pix_turn.width()/10),i*(pix_turn.height()/5),pix_turn.width()/10,pix_turn.height()/5));
        }
    }
    QPixmap pix_eat(":/Resource/images/smalleat.png");
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            eatPix.push_back(pix_eat.copy(j*(pix_eat.width()/10),i*(pix_eat.height()/5),pix_eat.width()/10,pix_eat.height()/5));
        }
    }
    QPixmap pix_hungrySwim(":/Resource/images/hungryswim.png");
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            hungrySwimPix.push_back(pix_hungrySwim.copy(j*(pix_hungrySwim.width()/10),i*(pix_hungrySwim.height()/5),pix_hungrySwim.width()/10,pix_hungrySwim.height()/5));
        }
    }
    QPixmap pix_hungryTurn(":/Resource/images/hungryturn.png");
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            hungryTurnPix.push_back(pix_hungryTurn.copy(j*(pix_hungryTurn.width()/10),i*(pix_hungryTurn.height()/5),pix_hungryTurn.width()/10,pix_hungryTurn.height()/5));
        }
    }
    QPixmap pix_hungryEat(":/Resource/images/hungryeat.png");
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            hungryEatPix.push_back(pix_hungryEat.copy(j*(pix_hungryEat.width()/10),i*(pix_hungryEat.height()/5),pix_hungryEat.width()/10,pix_hungryEat.height()/5));
        }
    }
    QPixmap pix_die(":/Resource/images/smalldie.png");
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            diePix.push_back(pix_die.copy(j*(pix_die.width()/10),i*(pix_die.height()/5),pix_die.width()/10,pix_die.height()/5));
        }
    }

    fishId = id;
    speed = 5,
    state = 4;
    foodWidth = 40;
    foodHeight = 50;

    currentPos = p;
    this->move(currentPos.x(),currentPos.y());
    this->setPixmap(swimPix[0]);
    this->show();

    connect(this,SIGNAL(GubiFishDie(int)),parent,SLOT(GubiFishDied(int)));
    connect(this,SIGNAL(FoodDie(int)),parent,SLOT(FoodDied(int)));
    connect(this,SIGNAL(produceCoin(int,int)),parent,SLOT(GubiCoinProduced(int,int)));
}

GubiFish::~GubiFish()
{
    this->deleteLater();
}

void GubiFish::checkAttr()
{
    hungerDegree--;
    if(type != 0)
    {
        produceCoinSpeed++;
    }
    if(hungerDegree == 0)
    {
        state = 3;
    }
    else if(hungerDegree < 60)
    {
        speed = 8;
        isHungry = true;
    }
    else if (hungerDegree < 90)
    {
        canEat = true;
        speed = 5;
        isHungry = false;
    }
    else
    {
        canEat = false;
    }
    if(state != 1)
    {
        if(growth >= 30 && growth_last < 30)
        {
            currentDie = 10;
            currentEat = 10;
            currentSwim = 10;
            currentTurn = 10;
            type = 1;
        }
        if(growth >= 300 && growth_last < 300)
        {
            currentDie = 20;
            currentEat = 20;
            currentSwim = 20;
            currentTurn = 20;
            type = 2;
        }
        if(growth >= 1000 && growth_last < 1000)
        {
            if(qrand()%3 == 1)
            {
                currentDie = 30;
                currentEat = 30;
                currentSwim = 30;
                currentTurn = 30;
                type = 3;
            }
            else
            {
                growth = 300;
            }
        }
        growth_last = growth;
    }
    if(produceCoinSpeed == 100)
    {
        emit produceCoin(fishId,coinType());
        produceCoinSpeed = 0;
    }
}

void GubiFish::wander()
{
    checkAttr();
    switch(state)
    {
        case 1:
        {
            if(isLeft)
            {
                if(hungerDegree > 60)
                {
                    this->setPixmap(eatPix[currentEat]);
                }
                else
                {
                    this->setPixmap(hungryEatPix[currentEat]);
                }
                if(currentEat < 10)
                {
                    currentEat++;
                    if(currentEat == 3)
                    {
                         emit FoodDie(foodId);
                    }
                    if(currentEat == 10)
                    {
                        currentEat = 0;
                        state = 4;
                    }
                }
                else if(currentEat < 20)
                {
                    currentEat++;
                    if(currentEat == 13)
                    {
                         emit FoodDie(foodId);
                    }
                    if(currentEat == 20)
                    {
                        currentEat = 10;
                        state = 4;
                    }
                }
                else if(currentEat < 30)
                {
                    currentEat++;
                    if(currentEat == 23)
                    {
                        emit FoodDie(foodId);
                    }
                    if(currentEat == 30)
                    {
                         currentEat = 20;
                        state = 4;
                    }
                }
                else if(currentEat < 40)
                {
                    currentEat++;
                    if(currentEat == 33)
                    {
                        emit FoodDie(foodId);
                    }
                    if(currentEat == 40)
                    {
                        currentEat = 30;
                        state = 4;
                    }
                }
            }
            else
            {
                QImage image;
                if(hungerDegree > 60)
                {
                    image  = eatPix[currentEat].toImage();
                }
                else
                {
                    image = hungryEatPix[currentEat].toImage();
                }
                QImage image_reverse = image.mirrored(true,false);
                this->setPixmap(QPixmap::fromImage(image_reverse));
                if(currentEat < 10)
                {
                    currentEat++;
                    if(currentEat == 3)
                    {
                        emit FoodDie(foodId);
                    }
                    if(currentEat == 10)
                    {
                        currentEat = 0;
                        state = 4;
                    }
                }
                else if(currentEat < 20)
                {
                    currentEat++;
                    if(currentEat == 13)
                    {
                        emit FoodDie(foodId);
                    }
                    if(currentEat == 20)
                    {
                        currentEat = 10;
                        state = 4;
                    }
                }
                else if(currentEat < 30)
                {
                    currentEat++;
                    if(currentEat == 23)
                    {
                        emit FoodDie(foodId);
                    }
                    if(currentEat == 30)
                    {
                        currentEat = 20;
                        state = 4;
                    }
                }
                else if(currentEat < 40)
                {
                    currentEat++;
                    if(currentEat == 33)
                    {
                        emit FoodDie(foodId);
                    }
                    if(currentEat == 40)
                    {
                        currentEat = 30;
                        state = 4;
                    }
                }
            }
            break;
        }
        case 2:
        {
            if(isLeft)
            {
                if(hungerDegree > 60)
                {
                    this->setPixmap(turnPix[currentTurn]);
                }
                else
                {
                    this->setPixmap(hungryTurnPix[currentTurn]);
                }
                if(currentTurn < 10)
                {
                    currentTurn++;
                    if(currentTurn == 10)
                    {
                        currentTurn = 0;
                        state = 4;
                        isLeft = false;
                    }
                }
                else if(currentTurn < 20)
                {
                    currentTurn++;
                    if(currentTurn == 20)
                    {
                        currentTurn = 10;
                        state = 4;
                        isLeft = false;

                    }
                }
                else if(currentTurn < 30)
                {
                    currentTurn++;
                    if(currentTurn == 30)
                    {
                        currentTurn = 20;
                        state = 4;
                        isLeft = false;

                    }
                }
                else if(currentTurn < 40)
                {
                    currentTurn++;
                    if(currentTurn == 40)
                    {
                        currentTurn = 30;
                        state = 4;
                        isLeft = false;
                    }
                }
            }
            else
            {
                if(hungerDegree > 60)
                {
                    this->setPixmap(turnPix[currentTurn+9]);
                }
                else
                {
                    this->setPixmap(hungryTurnPix[currentTurn+9]);
                }
                if(currentTurn + 9 > 30)
                {
                    currentTurn--;
                    if(currentTurn == 21)
                    {
                        currentTurn = 30;
                        state = 4;
                        isLeft = true;
                    }
                }
                else if(currentTurn + 9 > 20)
                {
                    currentTurn--;
                    if(currentTurn == 11)
                    {
                        currentTurn = 20;
                        state = 4;
                        isLeft = true;
                    }
                }
                else if(currentTurn  + 9 > 10)
                {
                    currentTurn--;
                    if(currentTurn == 1)
                    {
                        currentTurn = 10;
                        state = 4;
                        isLeft = true;
                    }
                }
                else if(currentTurn + 9 > 0)
                {
                    currentTurn--;
                    if(currentTurn == -9)
                    {
                        currentTurn = 0;
                        state = 4;
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
                this->setPixmap(diePix[currentDie]);
                if(currentDie < 10)
                {
                    currentDie++;
                    if(currentDie == 10)
                    {
                        emit GubiFishDie(fishId);
                    }
                }
                else if(currentDie < 20)
                {
                    currentDie++;
                    if(currentDie == 20)
                    {
                        emit GubiFishDie(fishId);
                    }
                }
                else if(currentDie < 30)
                {
                    currentDie++;
                    if(currentDie == 30)
                    {
                        emit GubiFishDie(fishId);
                    }
                }
                else if(currentDie < 40)
                {
                    currentDie++;
                    if(currentDie == 40)
                    {
                        emit GubiFishDie(fishId);
                    }
                }
            }
            else
            {
                QImage image  = diePix[currentDie].toImage();
                QImage image_reverse = image.mirrored(true,false);
                this->setPixmap(QPixmap::fromImage(image_reverse));
                if(currentDie < 10)
                {
                    currentDie++;
                    if(currentDie == 10)
                    {
                        emit GubiFishDie(fishId);
                    }
                }
                else if(currentDie < 20)
                {
                    currentDie++;
                    if(currentDie == 20)
                    {
                        emit GubiFishDie(fishId);
                    }
                }
                else if(currentDie < 30)
                {
                    currentDie++;
                    if(currentDie == 30)
                    {
                        emit GubiFishDie(fishId);
                    }
                }
                else if(currentDie < 40)
                {
                    currentDie++;
                    if(currentDie == 40)
                    {
                        emit GubiFishDie(fishId);
                    }
                }
            }
            break;
        }
        case 4:
        {
            if(isLeft)
            {
                if(hungerDegree > 60)
                {
                    this->setPixmap(swimPix[currentSwim]);
                }
                else
                {
                    this->setPixmap(hungrySwimPix[currentSwim]);
                }
                if(currentSwim < 10)
                {
                    currentSwim++;
                    if(currentSwim == 10)
                    {
                        currentSwim = 0;
                    }
                }
                else if(currentSwim < 20)
                {
                    currentSwim++;
                    if(currentSwim == 20)
                    {
                        currentSwim = 10;
                    }
                }
                else if(currentSwim < 30)
                {
                    currentSwim++;
                    if(currentSwim == 30)
                    {
                        currentSwim = 20;
                    }
                }
                else if(currentSwim < 40)
                {
                    currentSwim++;
                    if(currentSwim == 40)
                    {
                        currentSwim = 30;
                    }
                }
            }
            else
            {
                QImage image;
                if(hungerDegree > 60)
                {
                   image = swimPix[currentSwim].toImage();
                }
                else
                {
                   image = hungrySwimPix[currentSwim].toImage();
                }
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
                else if(currentSwim < 20)
                {
                    currentSwim++;
                    if(currentSwim == 20)
                    {
                        currentSwim = 10;
                    }
                }
                else if(currentSwim < 30)
                {
                    currentSwim++;
                    if(currentSwim == 30)
                    {
                        currentSwim = 20;
                    }
                }
                else if(currentSwim < 40)
                {
                    currentSwim++;
                    if(currentSwim == 40)
                    {
                        currentSwim = 30;
                    }
                }
            }
        }
        default:
        {
            if(!canEat||scene->FoodList.length() == 0)
            {
                hanging();
            }
            else
            {
                findFood();
            }
        }
        this->move(currentPos.x(),currentPos.y());
    }
}

void GubiFish::findFood()
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
            hungerDegree = (scene->foodType)*10+140;
            growth_last = growth;
            growth = growth + (scene->foodType)*10 + 10;
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
            hungerDegree = (scene->foodType)*10+140;
            growth_last = growth;
            growth = growth + (scene->foodType)*10 + 10;
        }
    }
}

int GubiFish::coinType()
{
    if(type == 1)
        return 0;
    if(type == 2)
        return 1;
    if(type == 3)
        return 3;
}

CarnivoreFish::CarnivoreFish(const int id,QWidget *parent)
    :Fish(parent)
{
    QPixmap pix_swim(":/Resource/images/smallswim.png");
    for(int i = 0;i < 10;i++)
    {
        swimPix.push_back(pix_swim.copy(i*(pix_swim.width()/10),4*(pix_swim.height()/5),pix_swim.width()/10,pix_swim.height()/5));
    }
    QPixmap pix_turn(":/Resource/images/smallturn.png");
    for(int i = 0;i < 10;i++)
    {
        turnPix.push_back(pix_turn.copy(i*(pix_turn.width()/10),4*(pix_turn.height()/5),pix_turn.width()/10,pix_turn.height()/5));
    }
    QPixmap pix_eat(":/Resource/images/smalleat.png");
    for(int i = 0;i < 10;i++)
    {
        eatPix.push_back(pix_eat.copy(i*(pix_eat.width()/10),4*(pix_eat.height()/5),pix_eat.width()/10,pix_eat.height()/5));
    }
    QPixmap pix_hungrySwim(":/Resource/images/hungryswim.png");
    for(int i = 0;i < 10;i++)
    {
        hungrySwimPix.push_back(pix_hungrySwim.copy(i*(pix_hungrySwim.width()/10),4*(pix_hungrySwim.height()/5),pix_hungrySwim.width()/10,pix_hungrySwim.height()/5));
    }
    QPixmap pix_hungryTurn(":/Resource/images/hungryturn.png");
    for(int i = 0;i < 10;i++)
    {
        hungryTurnPix.push_back(pix_hungryTurn.copy(i*(pix_hungryTurn.width()/10),4*(pix_hungryTurn.height()/5),pix_hungryTurn.width()/10,pix_hungryTurn.height()/5));
    }
    QPixmap pix_hungryEat(":/Resource/images/hungryeat.png");
    for(int i = 0;i < 10;i++)
    {
        hungryEatPix.push_back(pix_hungryEat.copy(i*(pix_hungryEat.width()/10),4*(pix_hungryEat.height()/5),pix_hungryEat.width()/10,pix_hungryEat.height()/5));
    }
    QPixmap pix_die(":/Resource/images/smalldie.png");
    for(int i = 0;i < 10;i++)
    {
        diePix.push_back(pix_die.copy(i*(pix_die.width()/10),4*(pix_die.height()/5),pix_die.width()/10,pix_die.height()/5));
    }

    fishId = id;
    speed = 8,
    state = 4;
    hungerDegree = 150;
    foodWidth = 80;
    foodHeight = 50;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    qreal x = qrand()%(640-swimPix[0].width());
    qreal y = 75 + qrand()%(300 - swimPix[0].height());
    currentPos.setX(x);
    currentPos.setY(y);
    this->move(currentPos.x(),currentPos.y());
    this->setPixmap(swimPix[0]);
    this->show();

    connect(this,SIGNAL(GubiFishBeEaten(int)),parent,SLOT(GubiFishDied(int)));
    connect(this,SIGNAL(CarnivoreFishDie(int)),parent,SLOT(CarnivoreFishDied(int)));
    connect(this,SIGNAL(produceCoin(int,int)),parent,SLOT(CarnivoreCoinProduced(int,int)));
}

CarnivoreFish::~CarnivoreFish()
{
    this->deleteLater();
}

void CarnivoreFish::checkAttr()
{
    hungerDegree--;
    produceCoinSpeed++;
    if(hungerDegree == 0)
    {
       state = 3;
    }
    else if(hungerDegree < 60)
    {
        speed = 10;
        isHungry = true;
    }
    else if (hungerDegree < 100)
    {
        canEat = true;
        speed = 8;
        isHungry = false;
    }
    else
    {
        canEat = false;
    }
    if(produceCoinSpeed == 500)
    {
        emit produceCoin(fishId,coinType());
        produceCoinSpeed = 0;
    }
}

void CarnivoreFish::wander()
{
    checkAttr();
    switch(state)
    {
        case 1:
        {
            if(isLeft)
            {
                if(hungerDegree <= 60)
                {
                    this->setPixmap(hungryEatPix[currentEat]);
                }
                else
                {
                    this->setPixmap(eatPix[currentEat]);
                }
                if(currentEat < 10)
                {
                    currentEat++;
                    if(currentEat == 1)
                    {
                        emit GubiFishBeEaten(foodId);
                    }
                    if(currentEat == 10)
                    {
                        currentEat = 0;
                        state = 4;
                    }
                }
            }
            else
            {
                QImage image;
                if(hungerDegree <= 60)
                {
                    image = hungryEatPix[currentEat].toImage();
                }
                else
                {
                    image  = eatPix[currentEat].toImage();
                }
                QImage image_reverse = image.mirrored(true,false);
                this->setPixmap(QPixmap::fromImage(image_reverse));
                if(currentEat < 10)
                {
                    currentEat++;
                    if(currentEat == 1)
                    {
                        emit GubiFishBeEaten(foodId);
                    }
                    if(currentEat == 10)
                    {
                        currentEat = 0;
                        state = 4;
                    }
                }
            }
            break;
        }
        case 2:
        {
            if(isLeft)
            {
                if(hungerDegree <= 60)
                {
                    this->setPixmap(hungryTurnPix[currentTurn]);
                }
                else
                {
                    this->setPixmap(turnPix[currentTurn]);
                }
                if(currentTurn < 10)
                {
                    currentTurn++;
                    if(currentTurn == 10)
                    {
                        currentTurn = 0;
                        state = 4;
                        isLeft = false;
                    }
                }
            }
            else
            {
                if(hungerDegree <= 60)
                {
                    this->setPixmap(hungryTurnPix[currentTurn+9]);
                }
                else
                {
                    this->setPixmap(turnPix[currentTurn+9]);
                }
                if(currentTurn + 9 > 0)
                {
                    currentTurn--;
                    if(currentTurn == -9)
                    {
                        currentTurn = 0;
                        state = 4;
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
                this->setPixmap(diePix[currentDie]);
                if(currentDie < 10)
                {
                    currentDie++;
                    if(currentDie == 10)
                    {
                        emit CarnivoreFishDie(fishId);
                    }
                }
            }
            else
            {
                QImage image  = diePix[currentDie].toImage();
                QImage image_reverse = image.mirrored(true,false);
                this->setPixmap(QPixmap::fromImage(image_reverse));
                if(currentDie < 10)
                {
                    currentDie++;
                    if(currentDie == 10)
                    {
                        emit CarnivoreFishDie(fishId);
                    }
                }
            }
            break;
        }
        case 4:
        {
            if(isLeft)
            {
                if(hungerDegree <= 60)
                {
                    this->setPixmap(hungrySwimPix[currentSwim]);
                }
                else
                {
                    this->setPixmap(swimPix[currentSwim]);
                }
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
                QImage image;
                if(hungerDegree <= 60)
                {
                   image = hungrySwimPix[currentSwim].toImage();
                }
                else
                {
                   image = swimPix[currentSwim].toImage();
                }
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
            bool hasFood = false;
            for(int i = 0;i < scene->GubiFishList.length();i++)
            {
                if(scene->GubiFishList[i]->type == 0)
                {
                    hasFood = true;
                    break;
                }
            }
            if(!canEat||!hasFood)
            {
                hanging();
            }
            else
            {
                findFood();
            }
        }
        this->move(currentPos.x(),currentPos.y());
    }
}

void CarnivoreFish::findFood()
{
    maxDis = 10000;
    for(int i = 0;i < scene->GubiFishList.length();i++)
    {
        if(scene->GubiFishList[i]->type == 0)
        {
            if(Dis(currentPos,QPointF(scene->GubiFishList[i]->x(),scene->GubiFishList[i]->y())) < maxDis)
            {
                maxDis = Dis(currentPos,QPointF(scene->GubiFishList[i]->x(),scene->GubiFishList[i]->y()));
                foodIndex = i;
                foodId = scene->GubiFishList[i]->fishId;
            }
        }
    }
    targetPos.setX(scene->GubiFishList[foodIndex]->x());
    targetPos.setY(scene->GubiFishList[foodIndex]->y());
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
            hungerDegree += 120;
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
            hungerDegree += 120;
        }
    }
}

int CarnivoreFish::coinType()
{
    return 3;
}

BigCarnivoreFish::BigCarnivoreFish(const int id,QWidget *parent)
    :Fish(parent)
{
    QPixmap pix(":/Resource/images/ultravore.png");
    for(int i = 0;i < 4;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            if(i == 0)
            {
                swimPix.push_back(pix.copy(j*(pix.width()/10),i*(pix.height()/4),pix.width()/10,pix.height()/4));
            }
            if(i == 1)
            {
                eatPix.push_back(pix.copy(j*(pix.width()/10),i*(pix.height()/4),pix.width()/10,pix.height()/4));
            }
            if(i == 2)
            {
                turnPix.push_back(pix.copy(j*(pix.width()/10),i*(pix.height()/4),pix.width()/10,pix.height()/4));
            }
            if(i == 3)
            {
                diePix.push_back(pix.copy(j*(pix.width()/10),i*(pix.height()/4),pix.width()/10,pix.height()/4));
            }
        }
    }

    fishId = id;
    speed = 12,
    state = 4;
    hungerDegree = 150;
    foodWidth = 80;
    foodHeight = 50;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    qreal x = qrand()%(640-swimPix[0].width());
    qreal y = 75 + qrand()%(300 - swimPix[0].height());
    currentPos.setX(x);
    currentPos.setY(y);
    this->move(currentPos.x(),currentPos.y());
    this->setPixmap(swimPix[0]);
    this->show();

    connect(this,SIGNAL(CarnivoreFishBeEaten(int)),parent,SLOT(CarnivoreFishDied(int)));
    connect(this,SIGNAL(BigCarnivoreFishDie(int)),parent,SLOT(BigCarnivoreFishDied(int)));
    connect(this,SIGNAL(produceCoin(int,int)),parent,SLOT(BigCarnivoreCoinProduced(int,int)));

}

BigCarnivoreFish::~BigCarnivoreFish()
{
    this->deleteLater();
}

void BigCarnivoreFish::checkAttr()
{
    hungerDegree--;
    produceCoinSpeed++;
    if(hungerDegree == 0)
    {
        state = 3;
    }
    else if(hungerDegree < 60)
    {
        speed = 15;
        isHungry = true;
    }
    else if (hungerDegree < 100)
    {
        canEat = true;
        speed = 12;
        isHungry = false;
    }
    else
    {
        canEat = false;
    }
    if(produceCoinSpeed == 300)
    {
        emit produceCoin(fishId,coinType());
        produceCoinSpeed = 0;
    }
}

void BigCarnivoreFish::wander()
{
    checkAttr();
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
                        emit CarnivoreFishBeEaten(foodId);
                    }
                    if(currentEat == 10)
                    {
                        currentEat = 0;
                        state = 4;
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
                        emit CarnivoreFishBeEaten(foodId);
                    }
                    if(currentEat == 10)
                    {
                        currentEat = 0;
                        state = 4;
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
                        state = 4;
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
                        state = 4;
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
                this->setPixmap(diePix[currentDie]);
                if(currentDie < 10)
                {
                    currentDie++;
                    if(currentDie == 10)
                    {
                        emit BigCarnivoreFishDie(fishId);
                    }
                }
            }
            else
            {
                QImage image  = diePix[currentDie].toImage();
                QImage image_reverse = image.mirrored(true,false);
                this->setPixmap(QPixmap::fromImage(image_reverse));
                if(currentDie < 10)
                {
                    currentDie++;
                    if(currentDie == 10)
                    {
                        emit BigCarnivoreFishDie(fishId);
                    }
                }
            }
            break;
        }
        case 4:
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
            if(!canEat||scene->CarnivoreFishList.length() == 0)
            {
                hanging();
            }
            else
            {
                findFood();
            }
        }
        this->move(currentPos.x(),currentPos.y());
    }
}

void BigCarnivoreFish::findFood()
{
    maxDis = 10000;
    for(int i = 0;i < scene->CarnivoreFishList.length();i++)
    {
        if(Dis(currentPos,QPointF(scene->CarnivoreFishList[i]->x(),scene->CarnivoreFishList[i]->y())) < maxDis)
        {
            maxDis = Dis(currentPos,QPointF(scene->CarnivoreFishList[i]->x(),scene->CarnivoreFishList[i]->y()));
            foodIndex = i;
            foodId = scene->CarnivoreFishList[i]->fishId;
        }
    }
    targetPos.setX(scene->CarnivoreFishList[foodIndex]->x());
    targetPos.setY(scene->CarnivoreFishList[foodIndex]->y());
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
            hungerDegree += 140;
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
            hungerDegree += 140;
        }
    }
}

int BigCarnivoreFish::coinType()
{
    return 4;
}

GubiMomFish::GubiMomFish(const int id,QWidget *parent)
    :Fish(parent)
{
    QPixmap pix_normal(":/Resource/images/breeder.png");
    for(int i = 0;i < 9;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            if(i == 3||i == 6)
            {
                swimPix.push_back(pix_normal.copy(j*(pix_normal.width()/10),i*(pix_normal.height()/9),pix_normal.width()/10,pix_normal.height()/9));
            }
            if(i == 4||i == 7)
            {
                turnPix.push_back(pix_normal.copy(j*(pix_normal.width()/10),i*(pix_normal.height()/9),pix_normal.width()/10,pix_normal.height()/9));
            }
            if(i == 5||i == 8)
            {
                 eatPix.push_back(pix_normal.copy(j*(pix_normal.width()/10),i*(pix_normal.height()/9),pix_normal.width()/10,pix_normal.height()/9));
            }
        }
    }
    QPixmap pix_hungry(":/Resource/images/hungrybreeder.png");
    for(int i = 0;i < 12;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            if(i == 3 || i == 6)
            {
                hungrySwimPix.push_back(pix_hungry.copy(j*(pix_hungry.width()/10),i*(pix_hungry.height()/12),pix_hungry.width()/10,pix_hungry.height()/12));
            }
            if(i == 4 || i == 7)
            {
                hungryTurnPix.push_back(pix_hungry.copy(j*(pix_hungry.width()/10),i*(pix_hungry.height()/12),pix_hungry.width()/10,pix_hungry.height()/12));
            }
            if(i == 5 || i == 8 )
            {
                diePix.push_back(pix_hungry.copy(j*(pix_hungry.width()/10),i*(pix_hungry.height()/12),pix_hungry.width()/10,pix_hungry.height()/12));
            }
            if(i == 10 || i == 11)
            {
                hungryEatPix.push_back(pix_hungry.copy(j*(pix_hungry.width()/10),i*(pix_hungry.height()/12),pix_hungry.width()/10,pix_hungry.height()/12));
            }
        }
    }

    fishId = id;
    speed = 5,
    state = 4;
    foodWidth = 40;
    foodHeight = 50;

    scene = (PlayingScene*)parent;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    qreal x = qrand()%(640-swimPix[0].width());
    qreal y = 75 + qrand()%(300 - swimPix[0].height());
    currentPos.setX(x);
    currentPos.setY(y);
    this->move(currentPos.x(),currentPos.y());
    this->setPixmap(swimPix[0]);
    this->show();

    connect(this,SIGNAL(GubiMomFishDie(int)),parent,SLOT(GubiMomFishDied(int)));
    connect(this,SIGNAL(FoodDie(int)),parent,SLOT(FoodDied(int)));
    connect(this,SIGNAL(produceGubiFish(int)),parent,SLOT(GubiFishProduced(int)));
}


GubiMomFish::~GubiMomFish()
{
    this->deleteLater();
}

void GubiMomFish::checkAttr()
{
    hungerDegree--;
    if(type == 0)
    {
        produceCoinSpeed++;
    }
    else
    {
        produceCoinSpeed += 2;
    }
    if(hungerDegree == 0)
    {
        state = 3;
    }
    else if(hungerDegree < 60)
    {
        speed = 8;
        isHungry = true;
    }
    else if (hungerDegree < 90)
    {
        canEat = true;
        speed = 5;
        isHungry = false;
    }
    else
    {
        canEat = false;
    }
    if(state != 1)
    {
        if(growth >= 300 && growth_last < 300)
        {
            currentDie = 10;
            currentEat = 10;
            currentSwim = 10;
            currentTurn = 10;
            type = 1;
        }
        growth_last = growth;
    }
    if(produceCoinSpeed == 300)
    {
        emit produceGubiFish(fishId);
        produceCoinSpeed = 0;
    }

}

void GubiMomFish::wander()
{
    checkAttr();
    switch(state)
    {
        case 1:
        {
            if(isLeft)
            {
                if(hungerDegree > 60)
                {
                    this->setPixmap(eatPix[currentEat]);
                }
                else
                {
                    this->setPixmap(hungryEatPix[currentEat]);
                }
                if(currentEat < 10)
                {
                    currentEat++;
                    if(currentEat == 3)
                    {
                         emit FoodDie(foodId);
                    }
                    if(currentEat == 10)
                    {
                        currentEat = 0;
                        state = 4;
                    }
                }
                else if(currentEat < 20)
                {
                    currentEat++;
                    if(currentEat == 13)
                    {
                         emit FoodDie(foodId);
                    }
                    if(currentEat == 20)
                    {
                        currentEat = 10;
                        state = 4;
                    }
                }
            }
            else
            {
                QImage image;
                if(hungerDegree > 60)
                {
                    image  = eatPix[currentEat].toImage();
                }
                else
                {
                    image = hungryEatPix[currentEat].toImage();
                }
                QImage image_reverse = image.mirrored(true,false);
                this->setPixmap(QPixmap::fromImage(image_reverse));
                if(currentEat < 10)
                {
                    currentEat++;
                    if(currentEat == 3)
                    {
                        emit FoodDie(foodId);
                    }
                    if(currentEat == 10)
                    {
                        currentEat = 0;
                        state = 4;
                    }
                }
                else if(currentEat < 20)
                {
                    currentEat++;
                    if(currentEat == 13)
                    {
                        emit FoodDie(foodId);
                    }
                    if(currentEat == 20)
                    {
                        currentEat = 10;
                        state = 4;
                    }
                }
            }
            break;
        }
        case 2:
        {
            if(isLeft)
            {
                if(hungerDegree > 60)
                {
                    this->setPixmap(turnPix[currentTurn]);
                }
                else
                {
                    this->setPixmap(hungryTurnPix[currentTurn]);
                }
                if(currentTurn < 10)
                {
                    currentTurn++;
                    if(currentTurn == 10)
                    {
                        currentTurn = 0;
                        state = 4;
                        isLeft = false;
                    }
                }
                else if(currentTurn < 20)
                {
                    currentTurn++;
                    if(currentTurn == 20)
                    {
                        currentTurn = 10;
                        state = 4;
                        isLeft = false;

                    }
                }
            }
            else
            {
                if(hungerDegree > 60)
                {
                    this->setPixmap(turnPix[currentTurn+9]);
                }
                else
                {
                    this->setPixmap(hungryTurnPix[currentTurn+9]);
                }
                if(currentTurn  + 9 > 10)
                {
                    currentTurn--;
                    if(currentTurn == 1)
                    {
                        currentTurn = 10;
                        state = 4;
                        isLeft = true;
                    }
                }
                else if(currentTurn + 9 > 0)
                {
                    currentTurn--;
                    if(currentTurn == -9)
                    {
                        currentTurn = 0;
                        state = 4;
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
                this->setPixmap(diePix[currentDie]);
                if(currentDie < 10)
                {
                    currentDie++;
                    if(currentDie == 10)
                    {
                        emit GubiMomFishDie(fishId);
                    }
                }
                else if(currentDie < 20)
                {
                    currentDie++;
                    if(currentDie == 20)
                    {
                        emit GubiMomFishDie(fishId);
                    }
                }
            }
            else
            {
                QImage image  = diePix[currentDie].toImage();
                QImage image_reverse = image.mirrored(true,false);
                this->setPixmap(QPixmap::fromImage(image_reverse));
                if(currentDie < 10)
                {
                    currentDie++;
                    if(currentDie == 10)
                    {
                        emit GubiMomFishDie(fishId);
                    }
                }
                else if(currentDie < 20)
                {
                    currentDie++;
                    if(currentDie == 20)
                    {
                        emit GubiMomFishDie(fishId);
                    }
                }
            }
            break;
        }
        case 4:
        {
            if(isLeft)
            {
                if(hungerDegree > 60)
                {
                    this->setPixmap(swimPix[currentSwim]);
                }
                else
                {
                    this->setPixmap(hungrySwimPix[currentSwim]);
                }
                if(currentSwim < 10)
                {
                    currentSwim++;
                    if(currentSwim == 10)
                    {
                        currentSwim = 0;
                    }
                }
                else if(currentSwim < 20)
                {
                    currentSwim++;
                    if(currentSwim == 20)
                    {
                        currentSwim = 10;
                    }
                }
            }
            else
            {
                QImage image;
                if(hungerDegree > 60)
                {
                   image = swimPix[currentSwim].toImage();
                }
                else
                {
                   image = hungrySwimPix[currentSwim].toImage();
                }
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
                else if(currentSwim < 20)
                {
                    currentSwim++;
                    if(currentSwim == 20)
                    {
                        currentSwim = 10;
                    }
                }
            }
        }
        default:
        {
            if(!canEat||scene->FoodList.length() == 0)
            {
                hanging();
            }
            else
            {
                findFood();
            }
        }
        this->move(currentPos.x(),currentPos.y());
    }

}

void GubiMomFish::findFood()
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
            hungerDegree = (scene->foodType)*10+140;
            growth_last = growth;
            growth = growth + (scene->foodType)*10 + 10;
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
            hungerDegree = (scene->foodType)*10+140;
            growth_last = growth;
            growth = growth + (scene->foodType)*10 + 10;
        }
    }
}







