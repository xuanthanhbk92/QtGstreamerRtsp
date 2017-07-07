#ifndef RTSPVIEW_H
#define RTSPVIEW_H

#include "igstreamerview.h"
#include <gst/gst.h>
#include <glib.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>

class RtspView : public IGStreamerView
{
public:
    RtspView();
    ~RtspView();

    // IGStreamerView interface
public:
    void start() override;
    void start(QString url) override;
    void setUrl(QString url) override;
    void stop() override;
    void pause() override;

private:
    GstElement *pipeline;
    GstElement *src;
    GstElement *sink;
    WId xwinid;

};

#endif // RTSPVIEW_H
