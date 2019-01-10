#include "coin.h"

Coin::Coin(const int id,const int type,const QPointF &p,QWidget* parent)
    :QLabel(parent),
      speed(3),
      currentPos(p),
      coinId(id),
      type(type),
      currentIndex(type*10)
{
    QPixmap pix_coin(":/Resource/images/money.png");
    for(int i = 0;i < 5;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            coinPix.push_back(pix_coin.copy(j*(pix_coin.width()/10),i*(pix_coin.height()/6),pix_coin.width()/10,pix_coin.height()/6));
        }
    }

    this->move(currentPos.x(),currentPos.y());
    this->setPixmap(coinPix[currentIndex]);
    this->show();

    connect(this,SIGNAL(CoinDie(int)),parent,SLOT(CoinDied(int)));
    connect(this,SIGNAL(CoinCollect(int)),parent,SLOT(CoinCollected(int)));
}

Coin::~Coin()
{
    this->deleteLater();
}

void Coin::floating()
{
    this->setPixmap(coinPix[currentIndex]);
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
            emit CoinDie(coinId);
        }
    }
    else
    {
        targetPos.setY(currentPos.y() + speed);
    }

    this->move(targetPos.x(),targetPos.y());
    currentPos = targetPos;
}

void Coin::mousePressEvent(QMouseEvent *e)
{
    emit CoinCollect(coinId);
}

