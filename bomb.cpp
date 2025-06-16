#include "bomb.h"

Bomb::Bomb()
{
    //将所有的爆炸的pixmap放在数组中准备好
    for(int i=0 ; i<BOMB_MAX ; i++)
    {
        QString str = QString(BOMB_PATH).arg(i);
        m_pixArr.push_back(QPixmap(str));
    }

    //坐标
    m_X = 0;
    m_Y = 0;

    //空闲状态
    m_free = true;

    //当前播放图片下标
    m_index = 0;

    //播放爆炸间隔记录
    m_Recorder = 0;

}

void Bomb::updateInfo()
{
    //空闲状态下的爆炸效果
    if(m_free)
    {

        return;
    }

    m_Recorder++;
    //未到达爆炸间隔，不需要换图片
    if(m_Recorder < BOMB_INTERVAL)
    {
        return;
    }

    //重置记录
    m_Recorder = 0;

    //切换爆炸播放图片的下标
    m_index++;

    if(m_index >= m_pixArr.size()/*数组下标从0到max-1*/)
    {
        m_index = 0;//数组下标从零开始计算
        m_free = true;
    }
}
