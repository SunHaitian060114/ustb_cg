#include "heroplane.h"
#include "mainscene.h"
#include <QTimer>
HeroPlane::HeroPlane()
{
    //初始化加载飞机图片资源
    m_Plane.load(HERO_PLANE);

    //初始化飞机坐标
    m_X = (GAME_WIDTH - m_Plane.width())*0.5;
    m_Y = (GAME_HEIGHT - m_Plane.height());

    //初始化矩形边框
    m_Rect.setWidth(m_Plane.width());
    m_Rect.setHeight(m_Plane.height());

    // 初始化子弹 - 不需要单独加载，Bullet构造函数已经处理
    for(int i=0; i<BULLET_NUM; i++)
    {
        m_bullets[i] = Bullet();

    }

    m_recorder = 0;

    m_Free = false;
}

void HeroPlane::shoot()
{
    if(m_Free)
    {
        return;
    }

    //累加时间间隔
    m_recorder++;
    // qDebug() << "Shoot recorder:" << m_recorder;  // 添加调试输出

    //未达到时间间隔直接return
    if(m_recorder < BULLET_INTERVAL)
    {
        return;
    }

    //达到发射时间
    m_recorder = 0;

    //发射子弹
    for(int i=0 ; i<BULLET_NUM ; i++)
    {
        //如果是空闲的子弹，则发射
        if(m_bullets[i].m_Free)
        {
            //将空闲状态设为假
            m_bullets[i].m_Free = false;

            //设置子弹坐标
            m_bullets[i].m_X = m_X + m_Plane.width()/2 - m_bullets[i].m_Bullet.width()/2;
            m_bullets[i].m_Y = m_Y - m_bullets[i].m_Bullet.height();
            // qDebug() << "Bullet fired at index:" << i;  // 调试输出
            break;
        }//P9
    }




}



void HeroPlane::setPosition(int x, int y)
{
    m_X = x;
    m_Y = y;
    m_Rect.moveTo(m_X,m_Y);
}
