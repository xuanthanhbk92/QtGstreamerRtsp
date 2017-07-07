#ifndef IGSTREAMERVIEW_H
#define IGSTREAMERVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

enum class GView{
    NORMAL,
    RTSP,
};

class IGStreamerView
{
public:
     IGStreamerView();
    virtual ~IGStreamerView();
    virtual void start() = 0;
    virtual void start(QString url)= 0 ;
    virtual void setUrl(QString url) =0;
    virtual void stop() =0 ;
    virtual void pause() = 0 ;
    // factory method
    static IGStreamerView* makeView(GView view);
    QWidget *getView() {
        return m_view;
    }
protected:
    QGraphicsView *m_view;
};

#endif // IGSTREAMERVIEW_H
