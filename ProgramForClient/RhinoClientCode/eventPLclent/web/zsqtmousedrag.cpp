#include "zsqtmousedrag.h"

ZSQtMouseDrag::ZSQtMouseDrag(QWidget *pContent):m_pContentWidget(pContent),press(false)
{

}

ZSQtMouseDrag::~ZSQtMouseDrag()
{

}

void ZSQtMouseDrag::mousePressEvent1(QPoint& point)
{
    oldPos = point;

    press = true;
}

bool ZSQtMouseDrag::mouseMoveEvent1(QPoint& point)
{
    if (press)
    {
        QPoint pos = m_pContentWidget->pos() + point - oldPos;
        m_pContentWidget->move(pos);
        oldPos = point;

    }
    return press;
}

void ZSQtMouseDrag::mouseReleaseEvent1()
{
    press = false;
}
