#ifndef HEROPLANE_H
#define HEROPLANE_H
#include <iostream>
#include <QRect>
#include "config.h"
#include "bullet.h"
#include "map.h"
using namespace std;

class HeroPlane
{
public:
    HeroPlane();

    //发射子弹
    void shoot();

    //设置飞机位置
    void setPosition(int x, int y);

    //飞机对象 资源
    QPixmap m_Plane;

    //飞机坐标
    int m_X;
    int m_Y;

    //飞机的矩形边框
    QRect m_Rect;

    bool m_Free;

    //弹匣
    Bullet m_bullets[BULLET_NUM];

    //发射间隔记录
    int m_recorder;

    // 添加移动速度
    int m_SpeedX;  // 水平移动速度
    int m_SpeedY;  // 垂直移动速度

    // 移动控制方法
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void stopMoving();
};

#endif // HEROPLANE_H
