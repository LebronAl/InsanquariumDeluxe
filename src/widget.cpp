#include "widget.h"
#include "ui_widget.h"
#include <QVBoxLayout>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setMinimumSize(640,480);
    setMaximumSize(640,480);
    setWindowTitle(tr("Insanquarium Deluxe"));
    setWindowIcon(QIcon(":/Resource/images/appIcon.gif"));

    loadingScene = new LoadingScene(this);
    mainScene = new MainScene(this);

    stackWidget = new QStackedWidget(this);
    stackWidget->addWidget(loadingScene);
    stackWidget->addWidget(mainScene);



    connect(loadingScene,SIGNAL(sendCurrentIndex(int)),this,SLOT(MainScenePlay(int)));
    connect(mainScene,SIGNAL(sendCurrentIndex(int)),this,SLOT(MainScenePlay(int)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(stackWidget);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::MainScenePlay(int id)
{
    if(id == 0)
    {
        stackWidget->setCurrentIndex(1);
    }
    else if(id  < 5)
    {
        playingScene = new PlayingScene(id,this);
        stackWidget->addWidget(playingScene);
        stackWidget->setCurrentIndex(2);
    }
    else
    {
        delete playingScene;
        playingScene = NULL;
        stackWidget->setCurrentIndex(1);
    }
}
