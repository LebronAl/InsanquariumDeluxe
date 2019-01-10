#include "gamescene.h"

#include <QKeyEvent>
#include <QFont>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>
#include <QLabel>

GameScene::GameScene(QWidget *parent)
    :QWidget(parent)
{}

GameScene::~GameScene()
{}


LoadingScene::LoadingScene(QWidget *parent)
    :GameScene(parent)
{
    loadingBtn = new QPushButton(this);
    loadingBtn->setGeometry(178,417,250,31);
    loadingBtn->setStyleSheet("border:0px;");
    loadingBtn->setFlat(true);

    timer = new QTimer;
    timer->start(30);

    connect(timer,SIGNAL(timeout()),this,SLOT(progressUpdate()));

    loadingBtn->installEventFilter(this);
}

LoadingScene::~LoadingScene()
{
    delete loadingBtn;
    delete timer;
}

void LoadingScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,640,480,QPixmap("://Resource/images/titlescreen.jpg"));
    painter.translate(0,0);
    if(currentProgress < 100)
    {
        painter.drawPixmap(178,417,QPixmap(":/Resource/images/loaderbar.png"),0,0,250*currentProgress/100,31);
    }
    else
    {
        painter.drawPixmap(178,417,250,31,QPixmap(":/Resource/images/loaderbarready.png"));
        isCompleted = true;
    }
}

void LoadingScene::progressUpdate()
{
    if(currentProgress < 100)
    {
        currentProgress++;
    }
    update();
}

bool LoadingScene::eventFilter(QObject *obj, QEvent *event)
{
    if(isCompleted)
    {
        if(obj == loadingBtn)
        {
            if(event->type() == QEvent::Enter)
            {
                this->setCursor(Qt::PointingHandCursor);
                QFont font("Microsoft YaHei",18);
                loadingBtn->setFont(font);
                loadingBtn->setStyleSheet("background-image:url(:/Resource/images/loaderbarreadyover.png);""border:0px;");
            }
            if(event->type() == QEvent::Leave)
            {
                this->setCursor(Qt::ArrowCursor);
                QFont font("Microsoft YaHei",16);
                loadingBtn->setFont(font);
                loadingBtn->setStyleSheet("background-image:url(:/Resource/images/loaderbarready.png);""border:0px;");
            }
            if(event->type() == QEvent::MouseButtonRelease)
            {
                emit sendCurrentIndex(0);
            }
        }
    }
}


MainScene::MainScene(QWidget *parent)
    :GameScene(parent)
{
    QFont font("Microsoft YaHei",16);
    this->setFont(font);

    this->setCursor(Qt::ArrowCursor);

    infoLabel = new QLabel("欢迎来到怪怪水族馆！",this);
    infoLabel->setGeometry(30,30,250,124);
    infoLabel->setStyleSheet("background-image:url(:/Resource/images/GSSpeechBubble.png);""color:#0d0e6d;");
    infoLabel->setContentsMargins(10,0,0,20);
    infoLabel->setAlignment(Qt::AlignCenter);

    playBtn = new QPushButton(tr("普通模式"),this);
    playBtn->setGeometry(356,47,220,67);
    playBtn->setStyleSheet("background-color:transparent;""color:#fced00");
    playBtn->setFlat(true);

    timeModeBtn = new QPushButton(tr("困难模式"),this);
    timeModeBtn->setGeometry(358,140,215,49);
    timeModeBtn->setStyleSheet("background-color:transparent;""color:#fced00");
    timeModeBtn->setFlat(true);

    challengeModeBtn = new QPushButton(tr("魔鬼模式"),this);
    challengeModeBtn->setGeometry(358,210,215,49);
    challengeModeBtn->setStyleSheet("background-color:transparent;""color:#fced00");
    challengeModeBtn->setFlat(true);

    aquariumModeBtn = new QPushButton(tr("上帝模式"),this);
    aquariumModeBtn->setGeometry(356,286,220,67);
    aquariumModeBtn->setStyleSheet("background-color:transparent;""color:#fced00");
    aquariumModeBtn->setFlat(true);

    QFont font_("Microsoft YaHei",12);

    hallOfFameBtn = new QPushButton(tr("名人堂"),this);
    hallOfFameBtn->setGeometry(400,380,130,29);
    hallOfFameBtn->setStyleSheet("background-color:transparent;""color:#fced00");
    hallOfFameBtn->setFlat(true);
    hallOfFameBtn->setFont(font_);

    optionsBtn = new QPushButton(tr("选项"),this);
    optionsBtn->setGeometry(326,413,90,29);
    optionsBtn->setStyleSheet("background-color:transparent;""color:#fced00");
    optionsBtn->setFlat(true);
    optionsBtn->setFont(font_);


    helpBtn = new QPushButton(tr("帮助"),this);
    helpBtn->setGeometry(420,413,91,29);
    helpBtn->setStyleSheet("background-color:transparent;""color:#fced00");
    helpBtn->setFlat(true);
    helpBtn->setFont(font_);

    quitBtn = new QPushButton(tr("退出"),this);
    quitBtn->setGeometry(515,413,88,29);
    quitBtn->setStyleSheet("background-color:transparent;""color:#fced00");
    quitBtn->setFlat(true);
    quitBtn->setFont(font_);

    QPixmap tailPix(":/Resource/images/tailflop.png");

    for(int i = 0;i < 8;i++)
    {
        tailPixVector.push_back(tailPix.copy(i*(tailPix.width()/8),0,tailPix.width()/8,tailPix.height()));

    }
    update();

    timer = new QTimer(this);
    timer->start(200);
    connect(timer,SIGNAL(timeout()),this,SLOT(tailPixChange()));
    connect(quitBtn,SIGNAL(clicked()),this->parent(),SLOT(close()));

    playBtn->installEventFilter(this);
    timeModeBtn->installEventFilter(this);
    challengeModeBtn->installEventFilter(this);
    aquariumModeBtn->installEventFilter(this);
    hallOfFameBtn->installEventFilter(this);
    optionsBtn->installEventFilter(this);
    helpBtn->installEventFilter(this);
    quitBtn->installEventFilter(this);
}

MainScene::~MainScene()
{
    delete playBtn;
    delete timeModeBtn;
    delete challengeModeBtn;
    delete aquariumModeBtn;
    delete infoLabel;
    delete hallOfFameBtn;
    delete optionsBtn;
    delete helpBtn;
    delete quitBtn;
    delete timer;
}

bool MainScene::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == playBtn)
    {
        if(event->type() == QEvent::Enter)
        {
            this->setCursor(Qt::PointingHandCursor);
            playBtn->setStyleSheet("background-image:url(:/Resource/images/battletankbutton.png);""border:0px;");
            infoLabel->setText(tr("要开始冒险了吗？"));
            QFont font("Microsoft YaHei",18);
            playBtn->setFont(font);
        }
        if(event->type() == QEvent::Leave)
        {
            this->setCursor(Qt::ArrowCursor);
            playBtn->setStyleSheet("background-color:transparent;""color:#fced00");
            infoLabel->setText(tr("欢迎来到怪怪水族馆！"));
            QFont font("Microsoft YaHei",16);
            playBtn->setFont(font);
        }
        if(event->type() == QEvent::MouseButtonPress)
        {
            playBtn->setStyleSheet("background-image:url(:/Resource/images/battletankbuttond.png);""border:0px");
            infoLabel->setText(tr("开始冒险吧!"));
            QFont font("Microsoft YaHei",16);
            playBtn->setFont(font);
        }
        if(event->type() == QEvent::MouseButtonRelease)
        {
            playBtn->setStyleSheet("background-image:url(:/Resource/images/battletankbutton.png);""border:0px");
            QFont font("Microsoft YaHei",18);
            playBtn->setFont(font);
            emit sendCurrentIndex(1);
        }
    }
    else if(obj == aquariumModeBtn)
    {
        if(event->type() == QEvent::Enter)
        {
            this->setCursor(Qt::PointingHandCursor);
            aquariumModeBtn->setStyleSheet("background-image:url(:/Resource/images/battletankbutton.png);""border:0px;");
            infoLabel->setText((tr("你想当上帝吗？")));
            QFont font("Microsoft YaHei",18);
            aquariumModeBtn->setFont(font);
        }
        if(event->type() == QEvent::Leave)
        {
            this->setCursor(Qt::ArrowCursor);
            aquariumModeBtn->setStyleSheet("background-color:transparent;""color:#fced00");
            infoLabel->setText(tr("欢迎来到怪怪水族馆！"));
            QFont font("Microsoft YaHei",16);
            aquariumModeBtn->setFont(font);
            aquariumModeBtn->setStyleSheet("color:#fced00");
        }
        if(event->type() == QEvent::MouseButtonPress)
        {
            aquariumModeBtn->setStyleSheet("background-image:url(:/Resource/images/battletankbuttond.png);""border:0px");
            infoLabel->setText(tr("来吧上帝！"));
            QFont font("Microsoft YaHei",16);
            aquariumModeBtn->setFont(font);
        }
        if(event->type() == QEvent::MouseButtonRelease)
        {
            aquariumModeBtn->setStyleSheet("background-image:url(:/Resource/images/battletankbutton.png);""border:0px");
            infoLabel->setText((tr("上帝模式便于你认识鱼的属性哦！")));
            QFont font("Microsoft YaHei",18);
            aquariumModeBtn->setFont(font);
            emit sendCurrentIndex(4);
        }
    }
    else if(obj == timeModeBtn)
    {
        if(event->type() == QEvent::Enter)
        {
            this->setCursor(Qt::PointingHandCursor);
            timeModeBtn->setStyleSheet("background-image:url(:/Resource/images/middlebuttond.png);""border:0px;");
            infoLabel->setText((tr("困难模式有点难度的哦！")));
            QFont font("Microsoft YaHei",18);
            timeModeBtn->setFont(font);
        }
        if(event->type() == QEvent::Leave)
        {
            this->setCursor(Qt::ArrowCursor);
            timeModeBtn->setStyleSheet("background-color:transparent;""color:#fced00");
            infoLabel->setText(tr("欢迎来到怪怪水族馆！"));
            QFont font("Microsoft YaHei",16);
            timeModeBtn->setFont(font);
        }
        if(event->type() == QEvent::MouseButtonPress)
        {
            timeModeBtn->setStyleSheet("background-image:url(:/Resource/images/middlebuttond.png);""border:0px");
            infoLabel->setText(tr("期待你的成功！"));
            QFont font("Microsoft YaHei",16);
            timeModeBtn->setFont(font);
        }
        if(event->type() == QEvent::MouseButtonRelease)
        {
            timeModeBtn->setStyleSheet("background-image:url(:/Resource/images/middlebuttond.png);""border:0px");
            infoLabel->setText((tr("困难模式有点难度的哦")));
            QFont font("Microsoft YaHei",18);
            timeModeBtn->setFont(font);
            emit sendCurrentIndex(2);
        }
    }
    else if(obj == challengeModeBtn)
    {
        if(event->type() == QEvent::Enter)
        {
            this->setCursor(Qt::PointingHandCursor);
            challengeModeBtn->setStyleSheet("background-image:url(:/Resource/images/middlebuttond.png);""border:0px;");
            infoLabel->setText((tr("魔鬼模式你也敢玩？")));
            QFont font("Microsoft YaHei",18);
            challengeModeBtn->setFont(font);
        }
        if(event->type() == QEvent::Leave)
        {
            this->setCursor(Qt::ArrowCursor);
            challengeModeBtn->setStyleSheet("background-color:transparent;""color:#fced00");
            infoLabel->setText(tr("欢迎来到怪怪水族馆！"));
            QFont font("Microsoft YaHei",16);
            challengeModeBtn->setFont(font);
        }
        if(event->type() == QEvent::MouseButtonPress)
        {
            challengeModeBtn->setStyleSheet("background-image:url(:/Resource/images/middlebuttond.png);""border:0px");
            infoLabel->setText(tr("羡慕你的勇气！"));
            QFont font("Microsoft YaHei",16);
            challengeModeBtn->setFont(font);
        }
        if(event->type() == QEvent::MouseButtonRelease)
        {
            challengeModeBtn->setStyleSheet("background-image:url(:/Resource/images/middlebuttond.png);""border:0px");
            infoLabel->setText((tr("魔鬼模式可还没人闯过关哦！")));
            QFont font("Microsoft YaHei",18);
            challengeModeBtn->setFont(font);
            emit sendCurrentIndex(3);
        }
    }
    else if(obj == hallOfFameBtn)
    {
        if(event->type() == QEvent::Enter)
        {
            this->setCursor(Qt::PointingHandCursor);
            hallOfFameBtn->setStyleSheet("border:0px");
            infoLabel->setText((tr("名人堂暂未开放！")));
            QFont font("Microsoft YaHei",16);
            hallOfFameBtn->setFont(font);
        }
        if(event->type() == QEvent::Leave)
        {
            this->setCursor(Qt::ArrowCursor);
            hallOfFameBtn->setStyleSheet("color:#fced00;""border:0px");
            infoLabel->setText(tr("欢迎来到怪怪水族馆！"));
            QFont font("Microsoft YaHei",12);
            hallOfFameBtn->setFont(font);
        }
        if(event->type() == QEvent::MouseButtonPress)
        {
            hallOfFameBtn->setStyleSheet("border:0px");
            infoLabel->setText(tr("敬请期待哦！"));
            QFont font("Microsoft YaHei",12);
            hallOfFameBtn->setFont(font);
        }
        if(event->type() == QEvent::MouseButtonRelease)
        {
            hallOfFameBtn->setStyleSheet("border:0px");
            infoLabel->setText((tr("名人堂暂未开放！")));
            QFont font("Microsoft YaHei",16);
            hallOfFameBtn->setFont(font);
        }
    }
    else if(obj == optionsBtn)
    {
        if(event->type() == QEvent::Enter)
        {
            this->setCursor(Qt::PointingHandCursor);
            optionsBtn->setStyleSheet("border:0px");
            infoLabel->setText((tr("选项暂未开放！")));
            QFont font("Microsoft YaHei",16);
            optionsBtn->setFont(font);
        }
        if(event->type() == QEvent::Leave)
        {
            this->setCursor(Qt::ArrowCursor);
            optionsBtn->setStyleSheet("color:#fced00;""border:0px");
            infoLabel->setText(tr("欢迎来到怪怪水族馆！"));
            QFont font("Microsoft YaHei",12);
            optionsBtn->setFont(font);
        }
        if(event->type() == QEvent::MouseButtonPress)
        {
            optionsBtn->setStyleSheet("border:0px");
            infoLabel->setText(tr("敬请期待哦！"));
            QFont font("Microsoft YaHei",12);
            optionsBtn->setFont(font);
        }
        if(event->type() == QEvent::MouseButtonRelease)
        {
            optionsBtn->setStyleSheet("border:0px");
            infoLabel->setText((tr("选项暂未开放！")));
            QFont font("Microsoft YaHei",16);
            optionsBtn->setFont(font);
        }
    }
    else if(obj == helpBtn)
    {      
        if(event->type() == QEvent::Enter)
        {
            this->setCursor(Qt::PointingHandCursor);
            helpBtn->setStyleSheet("border:0px");
            infoLabel->setText((tr("帮助暂未开放！")));
            QFont font("Microsoft YaHei",16);
            helpBtn->setFont(font);
        }
        if(event->type() == QEvent::Leave)
        {
            this->setCursor(Qt::ArrowCursor);
            helpBtn->setStyleSheet("color:#fced00;""border:0px");
            infoLabel->setText(tr("欢迎来到怪怪水族馆！"));
            QFont font("Microsoft YaHei",12);
            helpBtn->setFont(font);
        }
        if(event->type() == QEvent::MouseButtonPress)
        {
            helpBtn->setStyleSheet("border:0px");
            infoLabel->setText(tr("敬请期待哦！"));
            QFont font("Microsoft YaHei",12);
            helpBtn->setFont(font);
        }
        if(event->type() == QEvent::MouseButtonRelease)
        {
            helpBtn->setStyleSheet("border:0px");
            infoLabel->setText((tr("帮助暂未开放！")));
            QFont font("Microsoft YaHei",16);
            helpBtn->setFont(font);
        }
    }
    else if(obj == quitBtn)
    {
        if(event->type() == QEvent::Enter)
        {
            this->setCursor(Qt::PointingHandCursor);
            quitBtn->setStyleSheet("border:0px");
            infoLabel->setText((tr("再陪我玩玩嘛！")));
            QFont font("Microsoft YaHei",16);
            quitBtn->setFont(font);
        }
        if(event->type() == QEvent::Leave)
        {
            this->setCursor(Qt::ArrowCursor);
            quitBtn->setStyleSheet("color:#fced00;""border:0px");
            infoLabel->setText(tr("欢迎来到怪怪水族馆！"));
            QFont font("Microsoft YaHei",12);
            quitBtn->setFont(font);
        }
        if(event->type() == QEvent::MouseButtonPress)
        {
            quitBtn->setStyleSheet("border:0px");
            infoLabel->setText(tr("下次再来找我玩哦！"));
            QFont font("Microsoft YaHei",12);
            quitBtn->setFont(font);
        }
        if(event->type() == QEvent::MouseButtonRelease)
        {
            quitBtn->setStyleSheet("border:0px");
            infoLabel->setText((tr("再陪我玩玩嘛！")));
            QFont font("Microsoft YaHei",16);
            quitBtn->setFont(font);
        }
    }
}

void MainScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,640,480,QPixmap("://Resource/images/selectorback.jpg"));
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.translate(0,0);
    painter.drawPixmap(63,325,181,126,tailPixVector[tailIndex]);
}

void MainScene::tailPixChange()
{
    if(tailIndex == tailPixVector.size() - 1)
    {
        tailIndex = 0;
    }
    else
    {
        tailIndex++;
    }
    update();
}

PlayingScene::PlayingScene(int id,QWidget *parent)
    :GameScene(parent),
      sceneId(id)
{
    judgeId();
    init();
    mDialog = new myDialog();
    timer = new QTimer(this);
    timer->start(120);
    connect(timer,SIGNAL(timeout()),this,SLOT(imageChange()));
    connect(mDialog->Btn,SIGNAL(clicked()),this->backBtn,SIGNAL(clicked()));
    connect(this,SIGNAL(sendInfo(bool,int,int,int,int,int,int)),this->mDialog,SLOT(getInfo(bool,int,int,int,int,int,int)));
    connect(this,SIGNAL(sendCurrentIndex(int)),parent,SLOT(MainScenePlay(int)));
}

void PlayingScene::judgeId()
{
    if(sceneId == 1)
    {
         wealth = 1000;
         targetWealth = 2000;
         GubiFishList.push_back(new GubiFish(GubiFishNum++,this));
         GubiFishList.push_back(new GubiFish(GubiFishNum++,this));
         PetList.push_back(new SnailPet(this));
         PetList.push_back(new ShellPet(QPointF(90,250),this));
         PetList.push_back(new BonePet(this));
         PetList.push_back(new SwordPet(this));
         PetList.push_back(new TortoisePet(this));
         BlueAlienHealth = 20;
         EaterAlienHealth = 40;
         hasTortoise = true;
         Alientime = 300;
    }
    else if(sceneId == 2)
    {
        wealth = 2000;
        targetWealth = 5000;
        GubiFishList.push_back(new GubiFish(GubiFishNum++,this));
        GubiFishList.push_back(new GubiFish(GubiFishNum++,this));
        CarnivoreFishList.push_back(new CarnivoreFish(CarnivoreFishNum++,this));
        CarnivoreFishList.push_back(new CarnivoreFish(CarnivoreFishNum++,this));
        PetList.push_back(new SnailPet(this));
        PetList.push_back(new ShellPet(QPointF(90,250),this));
        PetList.push_back(new BonePet(this));
        PetList.push_back(new SwordPet(this));
        BlueAlienHealth = 30;
        EaterAlienHealth = 60;
        Alientime = 200;
    }
    else if (sceneId == 3)
    {
        wealth = 3000;
        targetWealth = 10000;
        GubiFishList.push_back(new GubiFish(GubiFishNum++,this));
        GubiFishList.push_back(new GubiFish(GubiFishNum++,this));
        GubiMomFishList.push_back(new GubiMomFish(GubiMomFishNum++,this));
        GubiMomFishList.push_back(new GubiMomFish(GubiMomFishNum++,this));
        CarnivoreFishList.push_back(new CarnivoreFish(CarnivoreFishNum++,this));
        CarnivoreFishList.push_back(new CarnivoreFish(CarnivoreFishNum++,this));
        BigCarnivoreFishList.push_back(new BigCarnivoreFish(BigCarnivoreFishNum++,this));
        BigCarnivoreFishList.push_back(new BigCarnivoreFish(BigCarnivoreFishNum++,this));
        PetList.push_back(new SnailPet(this));
        PetList.push_back(new ShellPet(QPointF(90,250),this));
        PetList.push_back(new BonePet(this));
        BlueAlienHealth = 40;
        EaterAlienHealth = 80;
        Alientime = 100;
    }
    else if (sceneId == 4)
    {
        canLose = false;
        wealth = 1000000;
        targetWealth = 1;
        PetList.push_back(new SnailPet(this));
        PetList.push_back(new ShellPet(QPointF(90,250),this));
        PetList.push_back(new BonePet(this));
        PetList.push_back(new SwordPet(this));
        PetList.push_back(new TortoisePet(this));
        BlueAlienHealth = 20;
        EaterAlienHealth = 40;
        Alientime = 60;
        hasTortoise = true;
    }
}

void PlayingScene::init()
{
    QPixmap pix_Gubi(":/Resource/images/smallswim.png");
    for(int i = 0;i < 10;i++)
    {
        Gubi.push_back(pix_Gubi.copy(i*(pix_Gubi.width()/10),0,pix_Gubi.width()/10,pix_Gubi.height()/5));
    }
    QPixmap pix_food(":/Resource/images/food.png");
    for(int i = 0;i < 5;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            food.push_back(pix_food.copy(j*(pix_food.width()/10),i*(pix_food.height()/5),pix_food.width()/10,pix_food.height()/5));
        }
    }
    QPixmap pix_Carnivore(":/Resource/images/scl_oscar.png");
    for(int i =0;i < 10;i++)
    {
        Carnivore.push_back(pix_Carnivore.copy(i*(pix_Carnivore.width()/10),0,pix_Carnivore.width()/10,pix_Carnivore.height()));
    }
    QPixmap pix_BigCarnivore(":/Resource/images/scl_ultra.png");
    for(int i =0;i < 10;i++)
    {
        BigCarnivore.push_back(pix_BigCarnivore.copy(i*(pix_BigCarnivore.width()/10),0,pix_BigCarnivore.width()/10,pix_BigCarnivore.height()));
    }
    QPixmap pix_GubiMom(":/Resource/images/scl_breeder.png");
    for(int i = 0;i < 10;i++)
    {
        GubiMom.push_back(pix_GubiMom.copy(i*(pix_GubiMom.width()/10),0,pix_GubiMom.width()/10,pix_GubiMom.height()));
    }
    QPixmap pix_eggs(":/Resource/images/EggPieces.png");
    for(int i = 0;i < 3;i++)
    {
        eggs.push_back(pix_eggs.copy(i*(pix_eggs.width()/3),0,pix_eggs.width()/3,pix_eggs.height()));
    }
    QFont ft;
    ft.setPointSize(20);
    alienWarningLabel = new QLabel(this);
    alienWarningLabel->setGeometry(0,350,640,50);
    alienWarningLabel->setText("怪兽就要出现了！！！");
    alienWarningLabel->setAlignment(Qt::AlignCenter);
    alienWarningLabel->setStyleSheet("color:#FF0000;""border:0px");
    alienWarningLabel->hide();
    alienWarningLabel->setFont(ft);

    backBtn = new QPushButton(this);
    backBtn->setGeometry(525,2,100,31);
    backBtn->setFlat(true);
    backBtn->setStyleSheet("border:0px");

    wealthLabel = new QLabel(tr("0"),this);
    wealthLabel->setGeometry(550,40,70,20);
    wealthLabel->setAlignment(Qt::AlignCenter);
    wealthLabel->setStyleSheet("color:#b3fe59;""border:0px");

    label_Gubi = new QLabel(tr("100"),this);
    label_Gubi->setGeometry(20,49,50,10);
    label_Gubi->setAlignment(Qt::AlignCenter);
    label_Gubi->setStyleSheet("color:#b3fe59;""border:0px");
    button_Gubi = new QPushButton(this);
    button_Gubi->setFlat(true);
    button_Gubi->setGeometry(17,1,57,50);
    button_Gubi->setStyleSheet("border:0px");

    label_maxType = new QLabel(tr("100"),this);
    label_maxType->setGeometry(90,49,50,10);
    label_maxType->setAlignment(Qt::AlignCenter);
    label_maxType->setStyleSheet("color:#b3fe59;""border:0px");
    button_maxType = new QPushButton(this);
    button_maxType->setFlat(true);
    button_maxType->setGeometry(86,1,57,50);
    button_maxType->setStyleSheet("border:0px");

    label_foodType = new QLabel(tr("100"),this);
    label_foodType->setGeometry(150,49,50,10);
    label_foodType->setAlignment(Qt::AlignCenter);
    label_foodType->setStyleSheet("color:#b3fe59;""border:0px");
    button_foodType = new QPushButton(this);
    button_foodType->setFlat(true);
    button_foodType->setGeometry(148,1,57,50);
    button_foodType->setStyleSheet("border:0px");


    label_Carnivore = new QLabel(tr("1000"),this);
    label_Carnivore->setGeometry(220,49,50,10);
    label_Carnivore->setAlignment(Qt::AlignCenter);
    label_Carnivore->setStyleSheet("color:#b3fe59;""border:0px");
    button_Carnivore = new QPushButton(this);
    button_Carnivore->setFlat(true);
    button_Carnivore->setGeometry(218,1,57,50);
    button_Carnivore->setStyleSheet("border:0px");

    label_BigCarniore = new QLabel(tr("10000"),this);
    label_BigCarniore->setGeometry(293,49,50,10);
    label_BigCarniore->setAlignment(Qt::AlignCenter);
    label_BigCarniore->setStyleSheet("color:#b3fe59;""border:0px");
    button_BigCarniore = new QPushButton(this);
    button_BigCarniore->setFlat(true);
    button_BigCarniore->setGeometry(291,0,57,50);
    button_BigCarniore->setStyleSheet("border:0px");

    label_GubiMom = new QLabel(tr("200"),this);
    label_GubiMom->setGeometry(368,49,50,10);
    label_GubiMom->setAlignment(Qt::AlignCenter);
    label_GubiMom->setStyleSheet("color:#b3fe59;""border:0px");
    button_GubiMom = new QPushButton(this);
    button_GubiMom->setFlat(true);
    button_GubiMom->setGeometry(358,1,57,50);
    button_GubiMom->setStyleSheet("border:0px");

    label_egg = new QLabel(QString::number(targetWealth),this);
    label_egg->setGeometry(440,49,50,10);
    label_egg->setAlignment(Qt::AlignCenter);
    label_egg->setStyleSheet("color:#b3fe59;""border:0px");
    button_egg = new QPushButton(this);
    button_egg->setFlat(true);
    button_egg->setGeometry(437,3,57,50);
    button_egg->setStyleSheet("border:0px");

    button_Gubi->installEventFilter(this);
    button_maxType->installEventFilter(this);
    button_foodType->installEventFilter(this);
    button_Carnivore->installEventFilter(this);
    button_BigCarniore->installEventFilter(this);
    button_GubiMom->installEventFilter(this);
    button_egg->installEventFilter(this);

    EaterAlientime = Alientime + rand()%100;
    BlueAlientime = Alientime + rand()%100;

    connect(backBtn,SIGNAL(clicked()),this,SLOT(backBtnClicked()));
}


bool PlayingScene::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == button_Gubi)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            if(wealth >= 100)
            {
                wealth  = wealth - 100;
                GubiFishList.push_back(new GubiFish(GubiFishNum++,this));
            }
        }
    }
    else if (obj == button_maxType)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            if(maxFood < 10)
            {
                if(wealth >= maxFood*100)
                {
                    wealth  = wealth - maxFood*100;
                    maxFood++;
                    if(maxFood < 10)
                    {
                        label_maxType->setText(QString::number(maxFood*100));
                    }
                    else
                    {
                        label_maxType->setText("MAX");
                    }
                }
            }
            else
            {
                label_maxType->setText("MAX");
            }
        }
    }
    else if(obj == button_foodType)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            if(foodType < 4)
            {
                if(wealth >= 100 + 200*foodType)
                {
                    wealth  = wealth - 100 + 200*foodType;
                    foodType++;
                    if(foodType < 4)
                    {
                        label_foodType->setText(QString::number(100 + 200*foodType));
                    }
                    else
                    {
                        label_foodType->setText("MAX");
                    }
                }
            }
            else
            {
                label_foodType->setText("MAX");
            }
        }
    }
    else if (obj == button_Carnivore)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            if(wealth >= 1000)
            {
                wealth  = wealth - 1000;
                CarnivoreFishList.push_back(new CarnivoreFish(CarnivoreFishNum++,this));
            }
        }
    }
    else if (obj == button_BigCarniore)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            if(wealth >= 10000)
            {
                wealth  = wealth - 10000;
                BigCarnivoreFishList.push_back(new BigCarnivoreFish(BigCarnivoreFishNum++,this));
            }
        }
    }
    else if(obj == button_GubiMom)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            if(wealth >= 200)
            {
                wealth  = wealth - 200;
                GubiMomFishList.push_back(new GubiMomFish(GubiMomFishNum++,this));
            }
        }
    }
    else if (obj == button_egg)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            if(wealth >= targetWealth)
            {
                wealth  = wealth - targetWealth;
                if(currentProcess < 2)
                {
                    currentProcess++;
                }
                else
                {
                    emit sendInfo(1,GubiFishNum ,GubiMomFishNum,CarnivoreFishNum,BigCarnivoreFishNum ,foodNum ,coinNum);
                    this->timer->stop();
                    mDialog->show();
                }
            }
        }
    }
    wealthLabel->setText(QString::number(wealth));
}

void PlayingScene::mousePressEvent(QMouseEvent *e)
{
    if((e->x()>0&&e->x()<640)&&(e->y()>75&&e->y()<420))
    {
        if(currentFood < maxFood)
        {
            Food *temp = new Food(foodNum,foodType,QPointF(e->x(),e->y()),this);
            if(hasTortoise)
            {
                temp->speed = 1;
            }
            FoodList.push_back(temp);
            currentFood++;
            foodNum++;
        }
    }
}

void PlayingScene::figureAlien()
{
    if(!EaterAlienList.length() &&!BlueAlienList.length())
    {
        switch (sceneId)
        {
            case 1:
            {
                EaterAlientime--;
                if(EaterAlientime == 50)
                {
                    alienWarningLabel->show();
                }
                if(EaterAlientime == 10)
                {
                    alienWarningLabel->hide();
                }
                if(EaterAlientime == 0)
                {
                    EaterAlienList.push_back(new EaterAlien(EaterAlienHealth,this));
                    EaterAlientime = Alientime;
                }
                break;
            }
            case 2:
            case 3:
            {
                BlueAlientime--;
                if(BlueAlientime == 50)
                {
                    alienWarningLabel->show();
                }
                if(BlueAlientime == 10)
                {
                    alienWarningLabel->hide();
                }
                if(BlueAlientime == 0)
                {
                    BlueAlienList.push_back(new BlueAlien(BlueAlienHealth,this));
                    BlueAlientime = Alientime;
                }
                break;
            }
            default:
            {
                if(alienNum  < 4)
                {
                    BlueAlientime--;
                    if(BlueAlientime <= 50)
                    {
                        alienWarningLabel->show();
                    }
                    if(BlueAlientime == 10)
                    {
                        alienWarningLabel->hide();
                    }
                    if(BlueAlientime == 0)
                    {
                        BlueAlienList.push_back(new BlueAlien(BlueAlienHealth,this));
                        alienNum++;
                    }
                    EaterAlientime--;
                    if(EaterAlientime <= 50)
                    {
                        alienWarningLabel->show();
                    }
                    if(EaterAlientime == 10)
                    {
                        alienWarningLabel->hide();
                    }
                    if(EaterAlientime == 0)
                    {
                        EaterAlienList.push_back(new EaterAlien(EaterAlienHealth,this));
                        alienNum++;
                    }
                }

            }
        }
    }
    else
    {
        alienWarningLabel->hide();
    }


}

void PlayingScene::imageChange()
{
    figureAlien();
    if(canLose)
    {
        if(GubiFishList.length() == 0&&GubiMomFishList.length() == 0&&CarnivoreFishList.length() == 0&&BigCarnivoreFishList.length() == 0)
        {
            emit sendInfo(0,GubiFishNum ,GubiMomFishNum,CarnivoreFishNum,BigCarnivoreFishNum ,foodNum ,coinNum );
            this->timer->stop();
            mDialog->show();
        }
    }
    for(int i = 0;i < FoodList.length();i++)
    {
        if(FoodList[i] != NULL)
        {
            FoodList[i]->floating();
        }
    }
    for(int i = 0;i < CoinList.length();i++)
    {
        if(CoinList[i] != NULL)
        {
            CoinList[i]->floating();
            if(hasTortoise)
            {
                CoinList[i]->speed = 1;
            }
        }
    }
    for(int i = 0;i < GubiFishList.length();i++)
    {
        if(GubiFishList[i] != NULL)
        {
            GubiFishList[i]->wander();
        }
    }
    for(int i = 0;i < CarnivoreFishList.length();i++)
    {
        if(CarnivoreFishList[i] != NULL)
        {
            CarnivoreFishList[i]->wander();
        }
    }
    for(int i = 0;i < BigCarnivoreFishList.length();i++)
    {
        if(BigCarnivoreFishList[i] != NULL)
        {
            BigCarnivoreFishList[i]->wander();
        }
    }
    for(int i = 0;i < GubiMomFishList.length();i++)
    {
        if(GubiMomFishList[i] != NULL)
        {
            GubiMomFishList[i]->wander();
        }
    }
    for(int i = 0;i < PetList.length();i++)
    {
        if(PetList[i] != NULL)
        {
            PetList[i]->wander();
        }
    }
    for(int i = 0;i < BlueAlienList.length();i++)
    {
        if(BlueAlienList[i] != NULL)
        {
            BlueAlienList[i]->wander();
        }
    }
    for(int i = 0;i < EaterAlienList.length();i++)
    {
        if(EaterAlienList[i] != NULL)
        {
            EaterAlienList[i]->wander();
        }
    }
    if(currentIndex < 10)
    {
        currentIndex++;
        if(currentIndex == 10)
        {
            currentIndex = 0;
        }
    }
    update();
}

int PlayingScene::CoinValue(int type)
{
    if(type == 0)
        return 15;
    if(type == 1)
        return 35;
    if(type == 3)
        return 200;
    if(type == 4)
        return 2000;
}

void PlayingScene::GubiFishDied(int id)
{
    for(int i = 0;i < GubiFishList.length();i++)
    {
        if(GubiFishList[i]!=NULL&&GubiFishList[i]->fishId == id)
        {
            GubiFishList[i]->deleteLater();
            GubiFishList[i] = NULL;
            GubiFishList.removeAt(i);
        }
    }
}

void PlayingScene::GubiMomFishDied(int id)
{
    for(int i = 0;i < GubiMomFishList.length();i++)
    {
        if(GubiMomFishList[i]!=NULL&&GubiMomFishList[i]->fishId == id)
        {
            GubiMomFishList[i]->deleteLater();
            GubiMomFishList[i] = NULL;
            GubiMomFishList.removeAt(i);
        }
    }
}

void PlayingScene::CarnivoreFishDied(int id)
{
    for(int i = 0;i < CarnivoreFishList.length();i++)
    {
        if(CarnivoreFishList[i]!=NULL&&CarnivoreFishList[i]->fishId == id)
        {
            CarnivoreFishList[i]->deleteLater();
            CarnivoreFishList[i] = NULL;
            CarnivoreFishList.removeAt(i);
        }
    }
}

void PlayingScene::BigCarnivoreFishDied(int id)
{
    for(int i = 0;i < BigCarnivoreFishList.length();i++)
    {
        if(BigCarnivoreFishList[i] != NULL&&BigCarnivoreFishList[i]->fishId == id)
        {
            BigCarnivoreFishList[i]->deleteLater();
            BigCarnivoreFishList[i] = NULL;
            BigCarnivoreFishList.removeAt(i);
        }
    }
}

void PlayingScene::FoodDied(int id)
{
    for(int i = 0;i < FoodList.length();i++)
    {
        if(FoodList[i]!=NULL&&FoodList[i]->foodId == id)
        {
            FoodList[i]->deleteLater();
            FoodList[i] = NULL;
            FoodList.removeAt(i);
            currentFood--;
        }
    }
}

void PlayingScene::CoinDied(int id)
{
    for(int i = 0;i < CoinList.length();i++)
    {
        if(CoinList[i]!=NULL&&CoinList[i]->coinId == id)
        {
            CoinList[i]->deleteLater();
            CoinList[i] = NULL;
            CoinList.removeAt(i);
        }
    }
}

void PlayingScene::CoinCollected(int id)
{
    for(int i = 0;i < CoinList.length();i++)
    {
        if(CoinList[i]!=NULL&&CoinList[i]->coinId == id)
        {
            int coinType = CoinList[i]->type;
            wealth += CoinValue(coinType);
            wealthLabel->setText(QString::number(wealth));
            QLabel *coinLabel = new QLabel(this);
            coinLabel->setGeometry(CoinList[i]->currentPos.x(),CoinList[i]->currentPos.y(),CoinList[i]->width(),CoinList[i]->height());
            coinLabel->setPixmap(CoinList[i]->coinPix[(CoinList[i]->type)*10]);
            coinLabel->show();
            QPropertyAnimation *animation = new QPropertyAnimation(coinLabel,"geometry");
            animation->setDuration(1000);
            animation->setStartValue(QRect(CoinList[i]->currentPos.x(),CoinList[i]->currentPos.y(),CoinList[i]->width(),CoinList[i]->height()));
            animation->setEndValue(QRect(570,60,CoinList[i]->width(),CoinList[i]->height()));
            animation->setEasingCurve(QEasingCurve::OutBounce);
            animation->start();
            CoinList[i]->deleteLater();
            CoinList[i] = NULL;
            CoinList.removeAt(i);
        }
    }
}

void PlayingScene::PearlCollected()
{
    wealth += 250;
    wealthLabel->setText(QString::number(wealth));
    QLabel *coinLabel = new QLabel(this);
    coinLabel->setGeometry(90,250,72,72);
    coinLabel->setStyleSheet("background-image:url(:/Resource/images/pearl.png);");
    coinLabel->show();
    QPropertyAnimation *animation = new QPropertyAnimation(coinLabel,"geometry");
    animation->setDuration(1000);
    animation->setStartValue(QRect(90,250,72,72));
    animation->setEndValue(QRect(570,60,72,72));
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}

void PlayingScene::GubiCoinProduced(int id, int type)
{
    for(int i = 0;i < GubiFishList.length();i++)
    {
        if(GubiFishList[i]!=NULL&&GubiFishList[i]->fishId == id)
        {
            CoinList.push_back(new Coin(coinNum++,type,GubiFishList[i]->currentPos,this));
        }
    }
}

void PlayingScene::GubiFishProduced(int id)
{
    for(int i = 0;i < GubiMomFishList.length();i++)
    {

        if(GubiMomFishList[i]!=NULL&&GubiMomFishList[i]->fishId == id)
        {
            GubiFishList.push_back(new GubiFish(GubiFishNum++,GubiMomFishList[i]->currentPos,this));
        }
    }
}

void PlayingScene::CarnivoreCoinProduced(int id, int type)
{
    for(int i = 0;i < CarnivoreFishList.length();i++)
    {
        if(CarnivoreFishList[i]!=NULL&&CarnivoreFishList[i]->fishId == id)
        {
            CoinList.push_back(new Coin(coinNum++,type,CarnivoreFishList[i]->currentPos,this));
        }
    }
}

void PlayingScene::BigCarnivoreCoinProduced(int id, int type)
{
    for(int i = 0;i < BigCarnivoreFishList.length();i++)
    {
        if(BigCarnivoreFishList[i]!=NULL&&BigCarnivoreFishList[i]->fishId == id)
        {
            CoinList.push_back(new Coin(coinNum++,type,BigCarnivoreFishList[i]->currentPos,this));
        }
    }
}

void PlayingScene::BonePetProduceCoin(qreal x,qreal y,int type)
{
    CoinList.push_back((new Coin(coinNum++,type,QPointF(x,y),this)));
}

void PlayingScene::BlueAlienDied(qreal x, qreal y , int type)
{
    for(int i = 0;i < BlueAlienList.length();i++)
    {
        BlueAlienList[i]->deleteLater();
        BlueAlienList[i] = NULL;
        BlueAlienList.removeAt(i);
    }
    CoinList.push_back((new Coin(coinNum++,type,QPointF(x,y),this)));
    BlueAlientime = Alientime + 100 +rand()%30;
    EaterAlientime = Alientime;
}

void PlayingScene::EaterAlienDied(qreal x, qreal y , int type)
{
    for(int i = 0;i < EaterAlienList.length();i++)
    {
        EaterAlienList[i]->deleteLater();
        EaterAlienList[i] = NULL;
        EaterAlienList.removeAt(i);
    }
    CoinList.push_back((new Coin(coinNum++,type,QPointF(x,y),this)));
    EaterAlientime = Alientime + 100 +rand()%30;
    BlueAlientime = Alientime;
}

PlayingScene::~PlayingScene()
{
    for(int i = 0;i < GubiFishList.length();i++)
    {
        GubiFishList[i]->deleteLater();
        GubiFishList[i] = NULL;
    }
    for(int i = 0;i < GubiMomFishList.length();i++)
    {
        GubiMomFishList[i]->deleteLater();
        GubiMomFishList[i] = NULL;
    }
    for(int i = 0;i < FoodList.length();i++)
    {
        FoodList[i]->deleteLater();
        FoodList[i] = NULL;
    }
    for(int i = 0;i < CarnivoreFishList.length();i++)
    {
        CarnivoreFishList[i]->deleteLater();
        CarnivoreFishList[i] = NULL;
    }
    for(int i =0;i < BigCarnivoreFishList.length();i++)
    {
        BigCarnivoreFishList[i]->deleteLater();
        BigCarnivoreFishList[i] = NULL;
    }
    for(int i = 0;i < PetList.length();i++)
    {
        PetList[i]->deleteLater();
        PetList[i] = NULL;
    }
    for(int i = 0;i < BlueAlienList.length();i++)
    {
        BlueAlienList[i]->deleteLater();
        BlueAlienList[i] = NULL;
    }
    for(int i = 0;i < EaterAlienList.length();i++)
    {
        EaterAlienList[i]->deleteLater();
        EaterAlienList[i] = NULL;
    }
    delete mDialog;
}

void PlayingScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,640,480,QPixmap("://Resource/images/aquarium1.jpg"));
    painter.translate(0,0);
    painter.drawPixmap(0,0,640,75,QPixmap(":/Resource/images/menubar.png"));
    painter.translate(0,0);
    painter.drawPixmap(6,-15,80,80,Gubi[currentIndex]);
    painter.translate(0,0);
    painter.setPen(QColor("#ffffff"));
    painter.drawText(QRectF(95,5,40,40),Qt::AlignCenter,QString::number(maxFood));
    painter.translate(0,0);
    painter.drawPixmap(155,5,40,40,food[currentIndex + foodType*10]);
    painter.translate(0,0);
    painter.drawPixmap(225,5,40,40,Carnivore[currentIndex]);
    painter.translate(0,0);
    painter.drawPixmap(299,5,40,40,BigCarnivore[currentIndex]);
    painter.translate(0,0);
    painter.drawPixmap(375,5,40,40,GubiMom[currentIndex]);
    painter.translate(0,0);
    painter.drawPixmap(442,5,46,39,eggs[currentProcess]);
}

void PlayingScene::backBtnClicked()
{
    emit sendCurrentIndex(sceneId + 4);
}




