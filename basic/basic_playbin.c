#include <gst/gst.h>

int main(int argc, char * argv[]) {
    GstElement *pipeline, *playbin;
    GstBus *bus;
    GstMessage *msg;

    // Initialize GStreamer
    gst_init(&argc, &argv);
    // Check if GStreamer was initialized successfully
    if (!gst_is_initialized()) {
        g_printerr("GStreamer initialization failed.\n");
        return -1;
    }
    
    // Create the playbin element
    playbin = gst_element_factory_make("playbin", "playbin");
    if (!playbin) {
        g_printerr("Failed to create playbin element.\n");
        return -1;
    }
    // Set the URI to play
    g_object_set(playbin, "uri", "https://gstreamer.freedesktop.org/data/media/sintel_trailer-480p.webm", NULL);
    // Create the pipeline and add the playbin element
    pipeline = gst_pipeline_new("playbin-pipeline");
    if (!pipeline) {
        g_printerr("Failed to create pipeline.\n");
        gst_object_unref(playbin);
        return -1;
    }

    // Add the playbin element to the pipeline
    gst_bin_add(GST_BIN(pipeline), playbin);
    // Link the playbin element to the pipeline
    // Note: playbin handles its own linking internally, so we don't need to link it manually.
    // Set the pipeline to the playbin element
    g_object_set(pipeline, "playbin", playbin, NULL);
    // Alternatively, you can set the playbin directly to the pipeline
    // g_object_set(pipeline, "uri", "https://gstreamer.freedesktop.org/data/media/sintel_trailer-480p.webm", NULL);
    // Set the pipeline to NULL state before starting
    gst_element_set_state(pipeline, GST_STATE_NULL);
    // Check if the pipeline is in NULL state
    GstState state;
    gst_element_get_state(pipeline, &state, NULL, GST_CLOCK_TIME_NONE);
    if (state != GST_STATE_NULL) {
        g_printerr("Pipeline is not in NULL state.\n");
        gst_object_unref(pipeline);
        return -1;
    }
    // Set the pipeline to READY state
    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_READY);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Failed to set pipeline to READY state.\n");
        gst_object_unref(pipeline);
        return -1;
    }
    // Start playing
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Failed to set pipeline to playing state.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    // Wait until an error or EOS occurs
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
        GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
    if (msg != NULL) { 
        switch (GST_MESSAGE_TYPE(msg)) {
            case GST_MESSAGE_ERROR: {
                GError *err;
                gchar *debug_info;
                gst_message_parse_error(msg, &err, &debug_info);
                g_printerr("Error received from element %s: %s\n", GST_OBJECT_NAME(msg->src), err->message);
                g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
                g_clear_error(&err);
                g_free(debug_info);
                break;
            }
            case GST_MESSAGE_EOS:
                g_print("End-Of-Stream reached.\n");
                break;

            case GST_MESSAGE_STATE_CHANGED: {
                GstState old_state, new_state, pending_state;
                gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);
                g_print("Pipeline state changed from %s to %s.\n", gst_element_state_get_name(old_state), gst_element_state_get_name(new_state));
                break;
            }
            default:
                g_printerr("Unexpected message received. %s\n", GST_MESSAGE_TYPE_NAME(msg));
                break;
        }
        gst_message_unref(msg);
    }

    // Free resources
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    gst_deinit();
    return 0;
}