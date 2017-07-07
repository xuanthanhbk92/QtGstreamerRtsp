#include "normalview.h"
#include <string>
#include <QDebug>

static void pad_added_handler (GstElement *src, GstPad *new_pad, CustomData *data) {
    GstPad *sink_pad = gst_element_get_static_pad (data->convert, "sink");
    GstPad *video_sink_pad = gst_element_get_static_pad (data->video_convert, "sink");
    GstPadLinkReturn ret;
    GstCaps *new_pad_caps = NULL;
    GstStructure *new_pad_struct = NULL;
    const gchar *new_pad_type = NULL;

    g_print ("Received new pad '%s' from '%s':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (src));

    /* If our converter is already linked, we have nothing to do here */
    if (gst_pad_is_linked (sink_pad) && gst_pad_is_linked (video_sink_pad) ) {
        g_print ("  We are already linked. Ignoring.\n");
        goto exit;
    }

    /* Check the new pad's type */
    new_pad_caps = gst_pad_query_caps (new_pad, NULL);
    new_pad_struct = gst_caps_get_structure (new_pad_caps, 0);
    new_pad_type = gst_structure_get_name (new_pad_struct);
    if ((!g_str_has_prefix (new_pad_type, "audio/x-raw")) &&
            (!g_str_has_prefix (new_pad_type, "video/x-raw")) )
    {
        g_print ("  It has type '%s' which is not raw audio, raw video. Ignoring.\n", new_pad_type);
        goto exit;
    }

    /* Attempt the link */
    if (g_str_has_prefix (new_pad_type, "audio/x-raw"))
    {
        ret = gst_pad_link (new_pad, sink_pad);
        if (GST_PAD_LINK_FAILED (ret)) {
            g_print ("  Type is '%s' but link failed.\n", new_pad_type);
        } else {
            g_print ("  Link succeeded (type '%s').\n", new_pad_type);
        }
        goto exit;
    }

    if (g_str_has_prefix (new_pad_type, "video/x-raw"))
    {
        ret = gst_pad_link (new_pad, video_sink_pad);
        if (GST_PAD_LINK_FAILED (ret)) {
            g_print ("  Type is '%s' but link failed.\n", new_pad_type);
        } else {
            g_print ("  Link succeeded (type '%s').\n", new_pad_type);
        }
        goto exit;
    }

exit:
    /* Unreference the new pad's caps, if we got them */
    if (new_pad_caps != NULL)
        gst_caps_unref (new_pad_caps);

    /* Unreference the sink pad */
    gst_object_unref (sink_pad);
    gst_object_unref (video_sink_pad);
}



NormalView::NormalView()
{
    std::string link = "rtsp://mpv.cdn3.bigCDN.com:554/bigCDN/_definst_/mp4:bigbuckbunnyiphone_400.mp4";
    data.pipeline = gst_pipeline_new ("xvoverlay");
    data.source = gst_element_factory_make ("uridecodebin", "source");
    data.convert = gst_element_factory_make ("audioconvert", "convert");
    data.sink = gst_element_factory_make ("autoaudiosink", "sink");
    data.video_convert = gst_element_factory_make ("videoconvert", "vconvert");
    data.video_sink = gst_element_factory_make ("xvimagesink", "vsink");

    g_object_set (data.source, "uri", link.c_str(), NULL);


    if (!data.pipeline || !data.source || !data.convert || !data.sink || !data.video_convert || !data.video_sink) {
        g_printerr ("Not all elements could be created.\n");
    }
    /* Build the pipeline. Note that we are NOT linking the source at this
  * point. We will do it later. */
    gst_bin_add_many (GST_BIN (data.pipeline), data.source, data.convert , data.sink,data.video_convert , data.video_sink, NULL);
    if (!gst_element_link (data.convert, data.sink)) {
        g_printerr ("Elements could not be linked.\n");
        //        gst_object_unref (pipeline);
        //        QTimer::singleShot(0, QApplication::activeWindow(), SLOT(quit()));
    }

    if (!gst_element_link (data.video_convert, data.video_sink)) {
        g_printerr ("Elements could not be linked.\n");
        //        gst_object_unref (pipeline);
        //        QTimer::singleShot(0, QApplication::activeWindow(), SLOT(quit()));
    }
    // link src
    g_signal_connect(data.source, "pad-added", G_CALLBACK (pad_added_handler), &data);
    xwinid = m_view->winId();
}

NormalView::~NormalView()
{
    qDebug () <<" Des normal view";
    gst_element_set_state(data.pipeline,GST_STATE_NULL);
    gst_object_unref(data.pipeline);
}

void NormalView::start()
{
    GstStateChangeReturn sret;

    /* we know what the video sink is in this case (xvimagesink), so we can
   * just set it directly here now (instead of waiting for a
   * prepare-window-handle element message in a sync bus handler and setting
   * it there) */
    gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (data.video_sink), xwinid);

    sret = gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
    if (sret == GST_STATE_CHANGE_FAILURE) {
        qDebug() << " cannot start pipeline " ;
    }

}

void NormalView::start(QString url)
{
    setUrl(url);
    GstStateChangeReturn sret;
    /* we know what the video sink is in this case (xvimagesink), so we can
   * just set it directly here now (instead of waiting for a
   * prepare-window-handle element message in a sync bus handler and setting
   * it there) */
    gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (data.video_sink), xwinid);

    sret = gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
    if (sret == GST_STATE_CHANGE_FAILURE) {
        qDebug() << " cannot start pipeline " ;
    }
}

void NormalView::setUrl(QString url)
{
     g_object_set(data.source,"uri",url.toStdString().c_str(),NULL);

}

void NormalView::stop()
{
    GstStateChangeReturn sret;
    /* we know what the video sink is in this case (xvimagesink), so we can
   * just set it directly here now (instead of waiting for a
   * prepare-window-handle element message in a sync bus handler and setting
   * it there) */
    sret = gst_element_set_state (data.pipeline, GST_STATE_READY);
    if (sret == GST_STATE_CHANGE_FAILURE) {
        qDebug() << " cannot stop pipeline " ;
    }
}


void NormalView::pause()
{
    GstStateChangeReturn sret;
    /* we know what the video sink is in this case (xvimagesink), so we can
   * just set it directly here now (instead of waiting for a
   * prepare-window-handle element message in a sync bus handler and setting
   * it there) */
    sret = gst_element_set_state (data.pipeline, GST_STATE_PAUSED);
    if (sret == GST_STATE_CHANGE_FAILURE) {
        qDebug() << " cannot stop pipeline " ;
    }

}
