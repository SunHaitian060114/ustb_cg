#ifndef BOMB_H
#define BOMB_H
#include "config.h"
#include <QVector>
#include <QString>
#include <QPixmap>
class Bomb
{
public:
    Bomb();

    //更新信息（播放图片下标，播放间隔）
    void updateInfo();

    //放爆炸资源组
    QVector<QPixmap> m_pixArr;

    //爆炸位置
    int m_X;
    int m_Y;

    //爆炸状态
    bool m_free;

    //爆炸切图间隔时间
    int m_Recorder;

    //爆炸时加载的图片下标
    int m_index;

};

#endif // BOMB_H
