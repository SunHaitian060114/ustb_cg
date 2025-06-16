#include "enemyplane.h"
#include "config.h"
EnemyPlane::EnemyPlane()
{
    //资源加载
    m_enemy.load(ENEMY_PATH);

    //敌机位置
    m_X = 0;
    m_Y = 0;

    //敌机速度
    m_Speed = ENEMY_SPEED;

    //敌机状态
    m_Free = true;

    //矩形
    m_Rect.setWidth(m_enemy.width());
    m_Rect.setHeight(m_enemy.height());
    m_Rect.moveTo(m_X,m_Y);

}

void EnemyPlane::updatePosition()
{
    //空闲的敌机不需要计算坐标
    if(m_Free)
    {
        return;
    }

    //敌机向下移动
    m_Y += m_Speed;
    m_Rect.moveTo(m_X,m_Y);

    //敌机超出窗口
    if(m_Y>=GAME_HEIGHT + m_Rect.height())
    {
        m_Free = true;
    }
}
