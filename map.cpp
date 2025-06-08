#include "map.h"
#include "config.h"
Map::Map()
{
    //初始化加载地图对象
    m_map1.load(MAP_PATH);
    m_map2.load(MAP_PATH);

    //Y轴坐标初始化
    m_map1_posY = -GAME_HEIGHT+1;
    m_map2_posY = -1;

    //滚动速度初始化
    m_scroll_speed = MAP_SCROLL_SPEED;

}

Map::~Map()
{

}

void Map::mapPosition()
{
    m_map1_posY += m_scroll_speed;
    if(m_map1_posY >= 0)  // 让其重置位置时完全对齐
    {
        m_map1_posY = -GAME_HEIGHT;
    }

    //处理第二张地图滚动
    m_map2_posY += m_scroll_speed;
    if(m_map2_posY >= GAME_HEIGHT)  // 保证完全滚动后重置
    {
        m_map2_posY = 0;
    }

}
