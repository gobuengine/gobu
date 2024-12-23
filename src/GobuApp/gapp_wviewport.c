#include "gapp_wviewport.h"
#include "pixio/pixio.h"
#include <epoxy/gl.h>

struct _GappViewport
{
    GtkWidget parent;
    gboolean initialized;
    gboolean customRender;
};

G_DEFINE_TYPE(GappViewport, gapp_widget_viewport, GTK_TYPE_GL_AREA)

static void gapp_s_resize(GtkWidget *widget, gint width, gint height);
static gboolean gapp_s_render(GtkGLArea *area, GdkGLContext *context, GappViewport *self);
static void gapp_s_realize(GtkWidget *widget);
static void gapp_s_unrealize(GtkWidget *widget);

static void gapp_widget_viewport_class_init(GappViewportClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    g_signal_new("viewport-ready", G_TYPE_FROM_CLASS(klass),
                 G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                 0, NULL, NULL, NULL, G_TYPE_NONE, 2, G_TYPE_INT, G_TYPE_INT);

    g_signal_new("viewport-render", G_TYPE_FROM_CLASS(klass),
                 G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                 0, NULL, NULL, NULL, G_TYPE_NONE, 1, G_TYPE_DOUBLE);
}

static void gapp_widget_viewport_init(GappViewport *self)
{
    // gtk_gl_area_set_required_version(GTK_GL_AREA(self), 3, 0);
    // gtk_gl_area_set_auto_render(GTK_GL_AREA(self), TRUE);
    // gtk_gl_area_set_allowed_apis(GTK_GL_AREA(self), GDK_GL_API_GL);
    // gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(self), TRUE);
    gtk_widget_set_focus_on_click(GTK_GL_AREA(self), true);
    gtk_widget_set_focusable(GTK_GL_AREA(self), true);
    gtk_widget_set_hexpand(GTK_WIDGET(self), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(self), TRUE);

    g_signal_connect(self, "render", G_CALLBACK(gapp_s_render), self);
    g_signal_connect(self, "realize", G_CALLBACK(gapp_s_realize), self);
    g_signal_connect(self, "unrealize", G_CALLBACK(gapp_s_unrealize), self);
    g_signal_connect(self, "resize", G_CALLBACK(gapp_s_resize), self);

    // GtkEventController *zoom = gtk_event_controller_scroll_new(GTK_EVENT_CONTROLLER_SCROLL_VERTICAL);
    // gtk_widget_add_controller(self, GTK_EVENT_CONTROLLER(zoom));
    // g_signal_connect(zoom, "scroll", G_CALLBACK(signal_viewport_zoom), self);

    // GtkEventController *motion = gtk_event_controller_motion_new();
    // gtk_widget_add_controller(self, GTK_EVENT_CONTROLLER(motion));
    // g_signal_connect(motion, "motion", G_CALLBACK(signal_viewport_mouse_move), self);

    // GtkGesture *gesture = gtk_gesture_click_new();
    // gtk_widget_add_controller(self, GTK_EVENT_CONTROLLER(gesture));
    // gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture), 0);
    // g_signal_connect(gesture, "pressed", G_CALLBACK(signal_viewport_mouse_button_pressed), self);
    // g_signal_connect(gesture, "released", G_CALLBACK(signal_viewport_mouse_button_released), self);

    // gtk_widget_add_tick_callback(GTK_WIDGET(self), tick_cb_queue, self, NULL);

    // gtk_widget_queue_draw(self);
}

static void gapp_s_resize(GtkWidget *widget, gint width, gint height)
{
    if (gtk_gl_area_get_error(GTK_GL_AREA(widget)) != NULL)
        return;

    pixi_adjust_viewport_size(width, height);
}

static gboolean gapp_s_render(GtkGLArea *area, GdkGLContext *context, GappViewport *self)
{
    if (gtk_gl_area_get_error(area) != NULL)
        return FALSE;

    // g_signal_emit_by_name(self, "viewport-render", self->deltaTime, 0);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glFlush();

    return TRUE;
}

static void gapp_s_realize(GtkWidget *widget)
{
    gtk_gl_area_make_current(GTK_GL_AREA(widget));

    if (gtk_gl_area_get_error(GTK_GL_AREA(widget)) != NULL)
        return;
}

static void gapp_s_unrealize(GtkWidget *widget)
{
    gtk_gl_area_make_current(GTK_GL_AREA(widget));

    if (gtk_gl_area_get_error(GTK_GL_AREA(widget)) != NULL)
        return;

    pixi_shutdown();
}

// -- BEGIN API PUBLIC

GappViewport *gapp_widget_viewport_new(void)
{
    return g_object_new(GAPP_WIDGET_TYPE_VIEWPORT, NULL);
}

void gapp_widget_viewport_set_custom_render(GappViewport *self, gboolean customRender)
{
    g_return_if_fail(GAPP_WIDGET_IS_VIEWPORT(self));
    self->customRender = customRender;
}

// -- END API PUBLIC
