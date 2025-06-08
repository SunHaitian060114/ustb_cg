#ifndef MAINSCENE_H
#define MAINSCENE_H
#include "config.h"
#include "map.h"
#include "heroplane.h"
#include "bullet.h"
#include "enemyplane.h"
#include "bomb.h"
#include "gameoverr.h"
#include "button.h"
#include <ctime>
#include <cstdlib>
#include <QWidget>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QApplication>
#include <QIcon>
#include <QPainter>
#include <QMouseEvent>
#include <QSettings>
#include <QDebug>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QFile>

class MainScene : public QWidget
{
    Q_OBJECT

public:
    // MainScene(QWidget *parent = nullptr);
    explicit MainScene(QWidget *parent = nullptr);//作用是：禁止构造函数被隐式调用进行类型转换
    ~MainScene();

    int Start_Event;

    Button button_Start;
    Button button_Quit;
    Button button_Restart;
    QPixmap m_Icon;

    //初始化场景
    void initScene();

    //启动游戏
    void playGame();

    //更新所有游戏元素的坐标
    void updatePosition();

    //绘制到屏幕中 函数名称不可改
    void paintEvent(QPaintEvent* );

    //重新移动鼠标事件 函数名称不可改
    void mouseMoveEvent(QMouseEvent* );

    //地图对象
    Map m_map;

    //飞机对象
    HeroPlane m_hero;

    //敌机数组
    EnemyPlane m_enemy[ENEMY_NUM];

    //敌机出现
    void enemy_ToScene();

    //敌机出场间隔
    int m_recorder;

    //定时器
    QTimer m_Timer;

    //碰撞检测函数
    void collisionDetection();

    //爆炸数组
    Bomb m_bomb[BOMB_NUM];

    //声音设置
    QMediaPlayer player;
    QAudioOutput audioOutput;

    //游戏结束界面
    GameOverr m_Over;

    //游戏暂停界面
    GameOverr m_Pause;

    //初始界面鼠标点击
    void mousePressEvent(QMouseEvent *event);

    //分数系统
    int score;
    int max_score;

    //游戏运行速度
    double game_rate;

    //重置游戏
    void resetGame();

    //接受键盘输入
    void keyPressEvent(QKeyEvent* event);

    //用于持久记录
    QSettings m_settings;

    //测试子弹对象
    // Bullet temp_Bullet;
};



#endif // MAINSCENE_H
