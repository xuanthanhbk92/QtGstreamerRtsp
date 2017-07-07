#include "rtspview.h"
#include <QDebug>
RtspView::RtspView()
{
    /* Build the pipeline */
    //location=rtsp://OrbitGMT:123456@10.10.6.10:554/VideoInput/1/h264/1
    pipeline = gst_parse_launch ("rtspsrc latency=0 name=src ! decodebin ! xvimagesink name=sink", NULL);
    //get xwinid
    sink= gst_bin_get_by_name(GST_BIN(pipeline) , "sink");
    src =  gst_bin_get_by_name(GST_BIN(pipeline) , "src");
    if (sink == NULL)
    {
        g_printerr ("Can not get sink.\n");
    }
    if (src == NULL)
    {
        g_printerr ("Can not get src.\n");
    }
    xwinid = m_view->winId();
}

RtspView::~RtspView()
{
    qDebug() << "Rtsp view des";
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);
}

void RtspView::start()
{
    GstStateChangeReturn sret;
    /* we know what the video sink is in this case (xvimagesink), so we can
   * just set it directly here now (instead of waiting for a
   * prepare-window-handle element message in a sync bus handler and setting
   * it there) */
    sret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (sret == GST_STATE_CHANGE_FAILURE) {
        //        gst_element_set_state (pipeline, GST_STATE_NULL);
        //        gst_object_unref (pipeline);
        /* Exit application */
        qDebug() <<"ERROR: Cannot start rtsp stream";
        //        QTimer::singleShot(0, QApplication::activeWindow(), SLOT(quit()));
    }

    gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY(sink), xwinid);
}

void RtspView::start(QString url)
{

    /* we know what the video sink is in this case (xvimagesink), so we can
   * just set it directly here now (instead of waiting for a
   * prepare-window-handle element message in a sync bus handler and setting
   * it there) */
    setUrl(url);
    GstStateChangeReturn sret;
    sret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (sret == GST_STATE_CHANGE_FAILURE) {
        //        gst_element_set_state (pipeline, GST_STATE_NULL);
        //        gst_object_unref (pipeline);
        /* Exit application */
        qDebug() <<"ERROR: Cannot start rtsp stream";
        //        QTimer::singleShot(0, QApplication::activeWindow(), SLOT(quit()));
    }
    gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY(sink), xwinid);

}

void RtspView::setUrl(QString url)
{
    g_object_set(src,"location",url.toStdString().c_str(),NULL);
}

void RtspView::stop()
{
    GstStateChangeReturn sret;
    sret = gst_element_set_state (pipeline, GST_STATE_READY);
    if (sret == GST_STATE_CHANGE_FAILURE) {
        //        gst_element_set_state (pipeline, GST_STATE_NULL);
        //        gst_object_unref (pipeline);
        /* Exit application */
        qDebug() <<"ERROR: Cannot stop rtsp stream";
        //        QTimer::singleShot(0, QApplication::activeWindow(), SLOT(quit()));
    }
}

void RtspView::pause()
{
    GstStateChangeReturn sret;
    sret = gst_element_set_state (pipeline, GST_STATE_PAUSED);
    if (sret == GST_STATE_CHANGE_FAILURE) {
        //        gst_element_set_state (pipeline, GST_STATE_NULL);
        //        gst_object_unref (pipeline);
        /* Exit application */
        qDebug() <<"ERROR: Cannot pause rtsp stream";
        //        QTimer::singleShot(0, QApplication::activeWindow(), SLOT(quit()));
    }
}
