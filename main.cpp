#include "mainwin.h"
#include <QApplication>
#include <QLabel>
#include <rtspview.h>
#include <QThread>
#include <QDebug>
#include <QList>

/* Install Gstreamer
  sudo apt-get install  libgstreamer-plugins-base1.0-dev
  sudo apt-get install libgstreamer-dev
add to project file
CONFIG += c++11
CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0 gstreamer-video-1.0 gstreamer-interfaces-1.0
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gst_init(&argc,&argv);
    MainWin w;
    QList<IGStreamerView *> views;
    for (int i =0 ; i<1 ;++i)
    {

        IGStreamerView *view = IGStreamerView::makeView(GView::RTSP);
        IGStreamerView *view1 = IGStreamerView::makeView(GView::NORMAL);
        w.addWidget(view->getView());
        w.addWidget(view1->getView());
        view->start("rtsp://OrbitGMT:123456@10.10.6.10:554/VideoInput/1/h264/1");
        view1->start("http://mm2.pcslab.com/mm/7h1000.mp4");
        views.push_back(view);
        views.push_back(view1);
    }
    w.show();
    QThread::sleep(10);
    qDebug() << "Pause" ;
    views.back()->pause();
    QThread::sleep(10);
    qDebug() << "Play" ;
    views.back()->start();
    QThread::sleep(10);
    qDebug() << "Stop" ;
    views.back()->stop();
    auto ret= a.exec();

    // Release resource
    foreach (auto view, views) {
        delete view;
    }
    return ret;
}
