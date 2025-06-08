#include "mainscene.h"
#include "map.h"
#include "bullet.h"
#include "heroplane.h"
#include "enemyplane.h"

MainScene::MainScene(QWidget *parent)
    : QWidget(parent),m_settings()
{


    score = 0;
    max_score = m_settings.value("highscore",0).toInt();

    Start_Event = 0;//0是初始界面，1是游戏界面，2是结束界面，3是暂停界面

    game_rate = GAME_RATE;

    setFocusPolicy(Qt::StrongFocus);

    //调用初始化场景接口
    initScene();

    // //启动游戏
    // if(Start_Event)
    //     playGame();
}

MainScene::~MainScene()
{

}

void MainScene::initScene()
{
    //设置窗口固定尺寸
    setFixedSize(GAME_WIDTH,GAME_HEIGHT);

    //设置标题
    setWindowTitle(GAME_TITLE);

    //加载图标
    setWindowIcon(QIcon(GAME_ICON));

    //定时器设置

    m_Timer.setInterval(game_rate);

    //敌机出场间隔初始化
    m_recorder = 0;

    //随机数种子
    srand((unsigned int)time(NULL));

    button_Start.m_Rect = QRect(175, 400, 160, 91);
    button_Quit.m_Rect = QRect(156, 530, 200, 69);
    button_Restart.m_Rect = QRect(156,400,200,78);
    m_Icon.load(GAME_ICON);
    button_Start.m_button.load(BUTTON_STRAT);
    button_Quit.m_button.load(BUTTON_QUIT);
    button_Restart.m_button.load(GAMEOVER_RESTART);
    m_Pause.m_over.load(PAUSE_IMAGE);

    connect(&m_Timer ,&QTimer::timeout, [=](){
        // qDebug() << "Timer timeout triggered";  // 添加调试输出
        //敌机出场
        enemy_ToScene();
        //更新游戏元素的坐\标
        updatePosition();
        //绘制到屏幕中
        update();
        //碰撞检测
        collisionDetection();
    });

}

void MainScene::playGame()
{
    //启动背景音乐
    player.setAudioOutput(&audioOutput);
    player.setSource(QUrl(SOUND_BACKGROUND));  // 设置MP3文件
    audioOutput.setVolume(1.0);
    m_Timer.stop();
    qDebug() << "音频路径:" << player.source().toString();
    bool hasAudioOutput = !QMediaDevices::audioOutputs().isEmpty();

    qDebug() << "SOUND_BACKGROUND path:" << SOUND_BACKGROUND;
    qDebug() << "SOUND_BOMB path:" << SOUND_BOMB;

    if (QFile::exists(SOUND_BACKGROUND)) {
        qDebug() << "BGM file found!";
    } else {
        qDebug() << "Error: BGM file not found!";
    }


    m_Timer.setInterval(game_rate);
    player.play();

    //启动计时器
    m_Timer.start();

    connect(&player, &QMediaPlayer::playbackStateChanged, this, [](QMediaPlayer::PlaybackState state){
        if (state == QMediaPlayer::PlayingState) {
            qDebug() << "正在播放";
        } else if (state == QMediaPlayer::PausedState) {
            qDebug() << "已暂停";
        } else if (state == QMediaPlayer::StoppedState) {
            qDebug() << "已停止";
        }
    });


}

void MainScene::updatePosition()
{
    //更新地图坐标
    m_map.mapPosition();

    //发射子弹
    m_hero.shoot();


    //计算所有非空闲子弹的当前坐标
    for(int i=0 ; i<BULLET_NUM ; i++)
    {
        //如果非空闲，计算发射位置
        if(m_hero.m_bullets[i].m_Free == false)
        {
            m_hero.m_bullets[i].updatePosition();
        }
    }

    //更新敌机坐标
    for(int i=0 ; i<ENEMY_NUM ; i++)
    {
        if(m_enemy[i].m_Free == false)
        {
            m_enemy[i].updatePosition();
        }
    }

    //爆炸
    for(int i=0 ; i<BOMB_NUM ; i++)
    {
        if(m_bomb[i].m_free == false)
        {
            m_bomb[i].updateInfo();
        }
    }

    //更新子弹坐标
    // temp_Bullet.m_Free = false;
    // temp_Bullet.updatePosition();

}

void MainScene::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    //绘制地图
    painter.drawPixmap(0,m_map.m_map1_posY,m_map.m_map1);
    painter.drawPixmap(0,m_map.m_map2_posY,m_map.m_map2);


    if(Start_Event == 1 || Start_Event == 3)
    {


            // 在右上角显示分数（白色字体，30号大小）
            painter.setPen(Qt::white); // 设置画笔颜色为白色
            painter.setFont(QFont("Arial", 30)); // 设置字体和大小
            painter.drawText(GAME_WIDTH - 200, 50, QString("Score: %1").arg(score)); // 在右上角显示分数

            //绘制英雄飞机
            if(m_hero.m_Free==false)
                painter.drawPixmap(m_hero.m_X,m_hero.m_Y,m_hero.m_Plane);

            //绘制所有非空闲子弹

            for(int i=0 ; i<BULLET_NUM ; i++)
            {
                //如果非空闲，绘制
                if(m_hero.m_bullets[i].m_Free == false)
                {
                    painter.drawPixmap(m_hero.m_bullets[i].m_X, m_hero.m_bullets[i].m_Y, m_hero.m_bullets[i].m_Bullet);

                }
            }

            //绘制所有非空闲飞机
            for(int i=0 ; i<ENEMY_NUM ; i++)
            {
                if(m_enemy[i].m_Free == false)
                {
                    painter.drawPixmap(m_enemy[i].m_X,m_enemy[i].m_Y,m_enemy[i].m_enemy);
                }

            }

            // 绘制爆炸
            for(int i=0 ; i<BOMB_NUM ; i++)
            {
                if(m_bomb[i].m_free == false)
                {
                    painter.drawPixmap(m_bomb[i].m_X,m_bomb[i].m_Y,m_bomb[i].m_pixArr[m_bomb[i].m_index]);
                }
            }
            if(Start_Event == 3)
            {
                painter.drawPixmap(100, 105, m_Pause.m_over);
                painter.setPen(Qt::white); // 设置画笔颜色为白色
                painter.setFont(QFont("Arial", 30)); // 设置字体和大小
                painter.drawText(GAME_WIDTH - 200, 50, QString("Score: %1").arg(score)); // 在右上角显示分数
                painter.drawPixmap(156,400,button_Restart.m_button);
                painter.drawPixmap(156,520,button_Quit.m_button);
            }

            painter.drawText(30, 50,
                             QString("Best: %1").arg(max_score));



        //测试子弹
        // painter.drawPixmap(temp_Bullet.m_X,temp_Bullet.m_Y,temp_Bullet.m_Bullet);
    }
    else if(Start_Event == 0)
    {

        painter.drawPixmap((GAME_WIDTH - m_Icon.width())*0.5,60,m_Icon);
        painter.drawPixmap(175,400,button_Start.m_button);
        painter.drawPixmap(156,530,button_Quit.m_button);

    }
    else if(Start_Event == 2)
    {
        // 绘制爆炸
        for(int i=0 ; i<BOMB_NUM ; i++)
        {
            if(m_bomb[i].m_free == false)
            {
                painter.drawPixmap(m_bomb[i].m_X,m_bomb[i].m_Y,m_bomb[i].m_pixArr[m_bomb[i].m_index]);
            }
        }

            painter.drawPixmap(100, 105, m_Over.m_over);
            painter.setPen(Qt::white); // 设置画笔颜色为白色
            painter.setFont(QFont("Arial", 30)); // 设置字体和大小
            painter.drawText(GAME_WIDTH - 200, 50, QString("Score: %1").arg(score)); // 在右上角显示分数
            painter.drawPixmap(156,400,button_Restart.m_button);
            painter.drawPixmap(156,520,button_Quit.m_button);

            if(score > max_score)
            {
                max_score = score;
                m_settings.setValue("highscore",max_score);
                m_settings.sync();
                painter.drawText(30, 100, QString("New Record!"));
            }

            painter.setPen(Qt::white);
            painter.setFont(QFont("Arial", 30));
            painter.drawText(30, 0, QString("Your Score: %1").arg(score));


            // 显示历史最高分
            painter.drawText(30, 0 + 50,
                             QString("Best: %1").arg(max_score));
    }

}

void MainScene::mouseMoveEvent(QMouseEvent * event)
{


        if(m_hero.m_Free==false)
        {
        int x = event->x() - m_hero.m_Rect.width()*0.5;
        int y = event->y() - m_hero.m_Rect.height()*0.5;

        //边界检测
        if(x<=0){x=0;}
        if(x>=GAME_WIDTH - m_hero.m_Rect.width()){x=GAME_WIDTH - m_hero.m_Rect.width();}
        if(y<=0){y=0;}
        if(y>=GAME_HEIGHT - m_hero.m_Rect.height()){y=GAME_HEIGHT - m_hero.m_Rect.height();}

        m_hero.setPosition(x,y);
        }
        else
        {
            for(int i=0 ; i<BULLET_NUM ; i++)
            {
                m_hero.m_bullets[i].m_Free = true;
            }
        }

}

void MainScene::enemy_ToScene()
{
    m_recorder++;

    if(m_recorder < ENEMY_INTERVAL)
    {
        return;
    }

    m_recorder = 0;

    for(int i=0 ; i<ENEMY_NUM ; i++)
    {
        //如果空闲，出场
        if(m_enemy[i].m_Free)
        {
            m_enemy[i].m_Free = false;

            m_enemy[i].m_X = rand()%(GAME_WIDTH - m_enemy[i].m_Rect.width());
            m_enemy[i].m_Y = -m_enemy[i].m_Rect.height();
            break;
        }
    }

}

void MainScene::collisionDetection()
{
    //遍历所有非空闲飞机
    for(int i=0 ; i<ENEMY_NUM ; i++)
    {
        if(m_enemy[i].m_Free)
        {
            continue;
        }

        //遍历所有非空闲子弹
            for(int j=0 ; j<BULLET_NUM ; j++)
        {
            if(m_hero.m_bullets[j].m_Free)
            {
                continue;
            }
            //子弹打敌机
            if(m_enemy[i].m_Rect.intersects(m_hero.m_bullets[j].m_Rect))
            {
                m_enemy[i].m_Free = true;
                m_hero.m_bullets[j].m_Free = true;

                //打中一个加分
                score++;
                if(game_rate>=2)
                    game_rate-=0.05;
                m_Timer.setInterval(game_rate);

                //播放爆炸效果
                for(int k=0 ; k<BOMB_NUM ; k++)
                {
                    if(m_bomb[k].m_free)
                    {
                        m_bomb[k].m_free = false;
                        m_bomb[k].m_X = m_enemy[i].m_X;
                        m_bomb[k].m_Y = m_enemy[i].m_Y;

                        break;
                    }
                }
            }
            //飞机撞敌机
            if(m_enemy[i].m_Rect.intersects(m_hero.m_Rect))
            {
                m_enemy[i].m_Free = true;
                m_hero.m_Free = true;
                m_Timer.stop();
                //播放爆炸效果
                for(int k=0 ; k<BOMB_NUM ; k++)
                {
                    if(m_bomb[k].m_free)
                    {
                        m_bomb[k].m_free = false;
                        m_bomb[k].m_X = m_enemy[i].m_X;
                        m_bomb[k].m_Y = m_enemy[i].m_Y;

                        break;
                    }
                }
                for(int k=0 ; k<BOMB_NUM ; k++)
                {
                    if(m_bomb[k].m_free)
                    {
                        m_bomb[k].m_free = false;

                        m_bomb[k].m_X = m_hero.m_X+m_hero.m_Rect.width()*0.3;
                        m_bomb[k].m_Y = m_hero.m_Y+m_hero.m_Rect.height()*0.15;

                        break;
                    }
                }

                Start_Event = 2;
            }
        }
    }
}

void MainScene::mousePressEvent(QMouseEvent *event)
{
    if(Start_Event==0 && button_Start.m_Rect.contains(event->pos())) {
        resetGame();
        Start_Event = 1;
        playGame();  // 启动游戏
        update();    // 重绘界面
    }
    else if(Start_Event==0 && button_Quit.m_Rect.contains(event->pos())) {
        resetGame();
        QApplication::quit();  // 退出游戏
    }
    else if((Start_Event==2||Start_Event==3) && button_Restart.m_Rect.contains(event->pos()))
    {
        resetGame();
        Start_Event = 1;
        playGame();  // 启动游戏
        update();    // 重绘界面
    }
    else if((Start_Event==2||Start_Event==3) && button_Quit.m_Rect.contains(event->pos())) {
        resetGame();

        QApplication::quit();  // 退出游戏
    }
}

void MainScene::resetGame()
{
    score = 0;
    game_rate = GAME_RATE;
    m_Timer.setInterval(GAME_RATE);
    m_Timer.stop();
    m_hero.m_Free = false;
    m_recorder = 0;
    m_hero.setPosition(GAME_WIDTH/2 - m_hero.m_Rect.width()/2, GAME_HEIGHT - m_hero.m_Rect.height());
    // 重置所有敌机
    for(int i=0; i<ENEMY_NUM; i++) {
        m_enemy[i].m_Free = true;
    }

    // 重置所有子弹
    for(int i=0; i<BULLET_NUM; i++) {
        m_hero.m_bullets[i].m_Free = true;
    }

    // 重置所有爆炸效果
    for(int i=0; i<BOMB_NUM; i++) {
        m_bomb[i].m_free = true;
    }
}

void MainScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        if(Start_Event == 1)
        {
            Start_Event = 3;
            m_Timer.stop();
            update();
        }
        else if(Start_Event == 3)
        {
            Start_Event = 1;
            m_Timer.start();
            update();
        }
    }
    else
    {
        QWidget::keyPressEvent(event);
    }
}






