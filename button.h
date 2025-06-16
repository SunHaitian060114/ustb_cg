#ifndef BUTTON_H
#define BUTTON_H
#include "config.h"
#include <QPixmap>

class Button
{
public:
    Button();

    int m_X;
    int m_Y;

    QPixmap m_button;

    QRect m_Rect;

    bool m_Free;

};

#endif // BUTTON_H
