#ifndef NORMALVIEW_H
#define NORMALVIEW_H
#include "igstreamerview.h"
#include <gst/gst.h>
#include <glib.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>


typedef struct _CustomData {
    GstElement *pipeline;
    GstElement *source;
    GstElement *convert;
    GstElement *video_convert;
    GstElement *sink;
    GstElement *video_sink;
} CustomData;


static void pad_added_handler (GstElement *src, GstPad *pad, CustomData *data);

class NormalView : public IGStreamerView
{
public:
    NormalView();
    ~NormalView();

private:
    CustomData data;
    WId xwinid;

    // IGStreamerView interface
public:
    void start() override;
    void start(QString url) override;
    void setUrl(QString url) override;
    void stop() override;
    void pause() override;
};

#endif // NORMALVIEW_H
