#include "mydialog.h"
#include "ui_mydialog.h"
#include <QPainter>


myDialog::myDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    QFont font("Microsoft YaHei",12);
    this->setFont(font);

    resultLabel = new QLabel(this);
    resultLabel->setAlignment(Qt::AlignCenter);
    GubiLabel = new QLabel(this);
    GubiLabel->setAlignment(Qt::AlignCenter);
    GubiMomLabel = new QLabel();
    GubiMomLabel->setAlignment(Qt::AlignCenter);
    CarnioreLabel = new QLabel();
    CarnioreLabel->setAlignment(Qt::AlignCenter);
    BigCarnioreLabel = new QLabel();
    BigCarnioreLabel->setAlignment(Qt::AlignCenter);
    foodLabel = new QLabel();
    foodLabel->setAlignment(Qt::AlignCenter);
    coinLabel = new QLabel();
    coinLabel->setAlignment(Qt::AlignCenter);
    Btn = new QPushButton("确认");

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(resultLabel);
    mainLayout->addWidget(GubiLabel);
    mainLayout->addWidget(GubiMomLabel);
    mainLayout->addWidget(CarnioreLabel);
    mainLayout->addWidget(BigCarnioreLabel);
    mainLayout->addWidget(foodLabel);
    mainLayout->addWidget(coinLabel);
    mainLayout->addWidget(Btn);

    connect(this->Btn,SIGNAL(clicked()),this,SLOT(close()));
}

myDialog::~myDialog()
{
    delete ui;
    delete resultLabel;
    delete GubiLabel;
    delete GubiMomLabel;
    delete CarnioreLabel;
    delete BigCarnioreLabel;
    delete foodLabel;
    delete coinLabel;
    delete Btn;
}


void myDialog::getInfo(bool a, int gubi, int gubimom, int carnivore, int bigcarnivore, int food, int coin)
{
    if(a)
    {
        resultLabel->setText("恭喜你！闯关成功！");
    }
    else
    {
        resultLabel->setText(("真遗憾！闯关失败！"));
    }
    GubiLabel->setText("您共养了" + QString::number(gubi) + "条古比鱼！");
    GubiMomLabel->setText("您共养了" + QString::number(gubimom) + "条古比妈咪！");
    CarnioreLabel->setText("您共养了" + QString::number(carnivore) + "条食肉鱼！");
    BigCarnioreLabel->setText("您共养了" + QString::number(bigcarnivore) + "条大型食肉鱼！");
    foodLabel->setText("您共投放了" + QString::number(food) + "颗鱼食！");
    coinLabel->setText("此局游戏共产生了" + QString::number(coin) + "个宝物！");
}
