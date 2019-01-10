#include "pet.h"
#include <cmath>
#include <QTime>
#include <QMouseEvent>
#include <QDebug>

Pet::Pet(QWidget *parent)
    :QLabel(parent),
      currentSwim(0),
      currentTurn(0),
      maxDis(10000),
      isLeft(true),
      arriveTarget(true)
{}

Pet::~Pet()
{
    this->deleteLater();
}

qreal Pet::Dis(const QPointF &pt1, const QPointF &pt2)
{
     return sqrt(((pt2.x() - pt1.x())*(pt2.x() - pt1.x()) + (pt2.y() - pt1.y())*(pt2.y() - pt1.y())));
}

void Pet::wander(){}
void Pet::hanging(){}

SnailPet::SnailPet(QWidget *parent)
    :Pet(parent)
{
    QPixmap pix(":/Resource/images/stinky.png");
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
                hidePix.push_back(pix.copy(j*(pix.width()/10),i*(pix.height()/3),pix.width()/10,pix.height()/3));
            }
        }
    }

    scene = (PlayingScene*)parent;
    speed = 3;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    qreal x = qrand()%(640-swimPix[0].width());
    qreal y = 390;
    currentPos.setX(x);
    currentPos.setY(y);
    this->move(currentPos.x(),currentPos.y());
    this->setPixmap(swimPix[0]);
    this->show();

    connect(this,SIGNAL(getCoin(int)),parent,SLOT(CoinCollected(int)));
}

SnailPet::~SnailPet()
{
    this->deleteLater();
}

void SnailPet::wander()
{
    if(scene->BlueAlienList.length() == 0&&scene->EaterAlienList.length() == 0)
    {
        hasAlien = false;
    }
    else
    {
        if(currentTurn % 10 == 0)
         {
            if(hasAlien == false)
            {
                state = 1;
            }
            hasAlien = true;
         }
    }
    switch(state)
    {
        case 1:
        {
            if(isLeft)
            {
                this->setPixmap(hidePix[currentHide]);
                if(currentHide < 10)
                {
                    currentHide++;
                    if(currentHide == 10)
                    {
                        currentHide = 0;
                        state = 3;
                    }
                }
            }
            else
            {
                QImage image = hidePix[currentHide].toImage();
                QImage image_reverse = image.mirrored(true,false);
                this->setPixmap(QPixmap::fromImage(image_reverse));
                if(currentHide < 10)
                {
                    currentHide++;
                    if(currentHide == 10)
                    {
                        currentHide = 0;
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
                        state = 5;
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
                        state = 5;
                        isLeft = true;
                    }
                }
            }
            break;
        }
        case 3:
        {
            if(!hasAlien)
            {
                state = 4;
            }
            break;
        }
        case 4:
        {
            if(isLeft)
            {
                this->setPixmap(hidePix[currentHide + 9]);
                if(currentHide > -9)
                {
                    currentHide--;
                    if(currentHide == -9)
                    {
                        currentHide = 0;
                        state = 5;
                    }
                }
            }
            else
            {
                QImage image = hidePix[currentHide + 9].toImage();
                QImage image_reverse = image.mirrored(true,false);
                this->setPixmap(QPixmap::fromImage(image_reverse));
                if(currentHide > -9)
                {
                    currentHide--;
                    if(currentHide == -9)
                    {
                        currentHide = 0;
                        state = 5;
                    }
                }
            }
            break;
        }
        case 5:
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
            if(!hasAlien&&scene->CoinList.length() == 0)
            {
                hanging();
            }
            else if(!hasAlien)
            {
                findCoin();
            }
        }
        this->move(currentPos.x(),currentPos.y());
    }
}

void SnailPet::hanging()
{
    speed = 3;
    if(arriveTarget)
    {
        while(true)
        {
            qreal x = qrand()%(640-swimPix[0].width());
            qreal y  = 390;
            if(abs(currentPos.x() - x) > 10)
            {
                targetPos.setX(x);
                targetPos.setY(y);
                break;
            }
        }
        xdelta = speed;
        arriveTarget = false;
    }
    if((isLeft&&currentPos.x() + swimPix[0].width()/2 < targetPos.x() + foodWidth/2)
            ||(!isLeft&&currentPos.x() + swimPix[0].width()/2 > targetPos.x() + foodWidth/2))
    {
        state = 2;
    }
    qreal dis = abs(currentPos.x() - targetPos.x());
    if(dis > speed)
    {
        if(abs(currentPos.x() + xdelta - targetPos.x()) < abs(currentPos.x() - xdelta - targetPos.x()))
        {
            currentPos.setX(currentPos.x() + xdelta);
        }
        else
        {
            currentPos.setX(currentPos.x() - xdelta);
        }
    }
    else
    {
        currentPos.setX(targetPos.x());
        arriveTarget = true;
    }
}

void SnailPet::findCoin()
{
    speed = 6;
    maxDis = 10000;
    for(int i = 0;i < scene->CoinList.length();i++)
    {
        if(Dis(currentPos,QPointF(scene->CoinList[i]->x(),scene->CoinList[i]->y())) < maxDis)
        {
            maxDis = Dis(currentPos,QPointF(scene->CoinList[i]->x(),scene->CoinList[i]->y()));
            coinIndex = i;
            coinId = scene->CoinList[i]->coinId;
        }
        targetPos.setX(scene->CoinList[coinIndex]->x());
        targetPos.setY(scene->CoinList[coinIndex]->y());
    }
    if((isLeft&&currentPos.x() + swimPix[0].width()/2 < targetPos.x() + foodWidth/2)
            ||(!isLeft&&currentPos.x() + swimPix[0].width()/2 > targetPos.x() + foodWidth/2))
    {
        state = 2;
    }
    qreal dis = abs(currentPos.x() - targetPos.x());
    if(dis > speed)
    {
        if(abs(currentPos.x() + xdelta - targetPos.x()) < abs(currentPos.x() - xdelta - targetPos.x()))
        {
            currentPos.setX(currentPos.x() + xdelta);
        }
        else
        {
            currentPos.setX(currentPos.x() - xdelta);
        }
    }
    else
    {
        currentPos.setX(targetPos.x());
        arriveTarget = true;
    }
    if((Dis(QPointF(targetPos.x() + foodWidth/2,targetPos.y() + foodHeight/2),
           QPointF(currentPos.x() + swimPix[0].width()/2,currentPos.y() + swimPix[0].height()/2))< 6*speed)
            &&(targetPos.y() > 370))
    {
        emit getCoin(coinId);
    }
}

ShellPet::ShellPet(const QPointF &p,QWidget *parent)
    :Pet(parent)
{
    QPixmap pix(":/Resource/images/niko.png");
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
                closePix.push_back(pix.copy(j*(pix.width()/10),i*(pix.height()/3),pix.width()/10,pix.height()/3));
            }
        }
    }

    state = 3;
    currentPos = p;
    this->move(currentPos.x(),currentPos.y());
    this->setPixmap(swimPix[0]);
    this->show();

    connect(this,SIGNAL(getPearl()),parent,SLOT(PearlCollected()));
}

ShellPet::~ShellPet()
{
    this->deleteLater();
}

void ShellPet::wander()
{
    producePearlSpeed++;
    if(producePearlSpeed == 300)
    {
        state = 1;
    }
    switch(state)
    {
        case 1:
        {
            this->setPixmap(turnPix[currentTurn]);
            if(currentTurn < 10)
            {
                currentTurn++;
                if(currentTurn == 10)
                {
                    currentTurn = 9;
                    canCollected = true;
                }
            }
            break;
        }
        case 2:
        {
            this->setPixmap(closePix[currentClose+9]);
            if(currentClose + 9 > 0)
            {
                currentClose--;
                if(currentClose == -9)
                {
                    currentClose = 0;
                    state = 3;
                    producePearlSpeed = 0;
                }
            }
            break;
        }
        case 3:
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
            break;
        }
    }
}

void ShellPet::mousePressEvent(QMouseEvent *e)
{
    if(canCollected)
    {
        if((e->x() < 60&&e->x() > 20)&&(e->y() < 60&&e->y() > 20))
        {
            emit getPearl();
            currentTurn = 0;
            canCollected = false;
            state = 2;
        }
    }
}

BonePet::BonePet(QWidget *parent)
    :Pet(parent)
{
    QPixmap pix(":/Resource/images/vert.png");
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

    speed = 5,
    state = 2;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    qreal x = qrand()%(640-swimPix[0].width());
    qreal y = 75 + qrand()%(300 - swimPix[0].height());
    currentPos.setX(x);
    currentPos.setY(y);
    this->move(currentPos.x(),currentPos.y());
    this->setPixmap(swimPix[0]);
    this->show();

    connect(this,SIGNAL(produceCoin(qreal,qreal,int)),parent,SLOT(BonePetProduceCoin(qreal,qreal,int)));
}

BonePet::~BonePet()
{
    this->deleteLater();
}

void BonePet::wander()
{
    produceCoinSpeed++;
    if(produceCoinSpeed == 60)
    {
        emit produceCoin(currentPos.x(),currentPos.y() ,1);
        produceCoinSpeed = 0;
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
            hanging();
        }
        this->move(currentPos.x(),currentPos.y());
    }
}

void BonePet::hanging()
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
}

SwordPet::SwordPet(QWidget *parent)
    :Pet(parent)
{
    QPixmap pix(":/Resource/images/itchy.png");
    for(int i = 0;i < 2;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            if(i == 0)
            {
                swimPix.push_back(pix.copy(j*(pix.width()/10),i*(pix.height()/4),pix.width()/10,pix.height()/4));
            }
            if(i == 1)
            {
                turnPix.push_back(pix.copy(j*(pix.width()/10),i*(pix.height()/4),pix.width()/10,pix.height()/4));
            }
        }
    }

    speed = 8,
    state = 2;
    scene = (PlayingScene*)parent;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    qreal x = qrand()%(640-swimPix[0].width());
    qreal y = 75 + qrand()%(300 - swimPix[0].height());
    currentPos.setX(x);
    currentPos.setY(y);
    this->move(currentPos.x(),currentPos.y());
    this->setPixmap(swimPix[0]);
    this->show();
}

void SwordPet::wander()
{
    if(scene->BlueAlienList.length() == 0&&scene->EaterAlienList.length() == 0)
    {
        hasAlien = false;
    }
    else
    {
        if(currentTurn % 10 == 0)
         {
            if(hasAlien == false)
            {
                state = 2;
            }
            hasAlien = true;
         }
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
            if(!hasAlien)
            {
                hanging();
            }
            else
            {
                attackAlien();
            }
        }
        this->move(currentPos.x(),currentPos.y());
    }
}

SwordPet::~SwordPet()
{
    this->deleteLater();
}

void SwordPet::attackAlien()
{
    bool target = 0;
    if(scene->BlueAlienList.length() == 0)
    {
        targetPos.setX(scene->EaterAlienList[0]->x());
        targetPos.setY(scene->EaterAlienList[0]->y());
        target = 0;
    }
    else if (scene->EaterAlienList.length() == 0)
    {
        targetPos.setX(scene->BlueAlienList[0]->x());
        targetPos.setY(scene->BlueAlienList[0]->y());
        target = 1;
    }
    else if(Dis(currentPos,QPointF(scene->EaterAlienList[0]->x(),scene->EaterAlienList[0]->y())) < Dis(currentPos,QPointF(scene->BlueAlienList[0]->x(),scene->BlueAlienList[0]->y())))
    {
        targetPos.setX(scene->EaterAlienList[0]->x());
        targetPos.setY(scene->EaterAlienList[0]->y());
        target = 0;
    }
    else
    {
        targetPos.setX(scene->BlueAlienList[0]->x());
        targetPos.setY(scene->BlueAlienList[0]->y());
        target = 1;
    }
    if((isLeft&&currentPos.x() + swimPix[0].width()/2 < targetPos.x() + foodWidth/2)
            ||(!isLeft&&currentPos.x() + swimPix[0].width()/2 > targetPos.x() + foodWidth/2))
    {
        state = 1;
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
            if(target == 0)
            {
                scene->EaterAlienList[0]->health--;
            }
            else
            {
                scene->BlueAlienList[0]->health--;
            }
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
            if(target == 0)
            {
                scene->EaterAlienList[0]->health--;
            }
            else
            {
                scene->BlueAlienList[0]->health--;
            }
        }
    }
}

void SwordPet::hanging()
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
}

TortoisePet::TortoisePet(QWidget *parent)
    :Pet(parent)
{
    QPixmap pix(":/Resource/images/seymour.png");
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

    speed = 5,
    state = 2;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    qreal x = qrand()%(640-swimPix[0].width());
    qreal y = 75 + qrand()%(300 - swimPix[0].height());
    currentPos.setX(x);
    currentPos.setY(y);
    this->move(currentPos.x(),currentPos.y());
    this->setPixmap(swimPix[0]);
    this->show();

}

TortoisePet::~TortoisePet()
{
    this->deleteLater();
}

void TortoisePet::wander()
{
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
            hanging();
        }
        this->move(currentPos.x(),currentPos.y());
    }

}

void TortoisePet::hanging()
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

}
