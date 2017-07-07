#include "igstreamerview.h"
#include "rtspview.h"
#include "normalview.h"
IGStreamerView::IGStreamerView()
{
    QGraphicsScene * scene = new QGraphicsScene(-100,-100,200,200);
    QGraphicsView *view = new QGraphicsView(scene);
    m_view = view;
}

IGStreamerView::~IGStreamerView()
{

}

IGStreamerView *IGStreamerView::makeView(GView view)
{
    switch (view) {
    case GView::NORMAL:
        return new NormalView();
        break;
    case GView::RTSP:
        return new RtspView();
        break;
    default:
        break;
    }
}
