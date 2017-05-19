#ifndef _ZSQTMOUSEDRAG_zsqtmousedrag_H_
#define _ZSQTMOUSEDRAG_zsqtmousedrag_H_

#include <QObject>
#include <QWidget>
#include <QPoint>
class ZSQtMouseDrag
{
public:
    explicit ZSQtMouseDrag(QWidget *pContent);
    ~ZSQtMouseDrag();
    /**
     * @brief mousePressEvent1 鼠标拖动管理
     * @param point
     */
    void mousePressEvent1(QPoint& point);

    bool mouseMoveEvent1(QPoint& point);

    void mouseReleaseEvent1();

protected:
    /**
     * @brief oldPos
     */
    QPoint oldPos;
    /**
     * @brief press
     */
    bool press;

    /**
     * @brief m_pContentWidget 继承窗体的容器对象，移动窗口指针
     */
    QWidget* m_pContentWidget;

};

#endif // _ZSWEBVIEW_zswebview_H_
