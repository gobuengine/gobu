#include "gapp_scene_viewport.h"
#include "gapp_widget.h"
#include "gapp_wviewport.h"
#include "gapp.h"

#include "gobu/gobu.h"

struct _GappSceneViewport
{
    GtkBox parent_instance;
    go_camera_t camera;
};

static void gapp_scene_viewport_on_viewport_ready(GtkWidget *viewport, GappSceneViewport *sceneViewport);
static void gapp_scene_viewport_on_viewport_render(GtkWidget *viewport, int width, int height, GappSceneViewport *sceneViewport);
static GtkWidget *gapp_scene_viewport_toolbar(GappSceneViewport *scene);
static void gapp_scene_viewport_toolbar_open_popover_create_entity(GtkWidget *widget, GappSceneViewport *self);

// MARK: CLASS
G_DEFINE_TYPE(GappSceneViewport, gapp_scene_viewport, GTK_TYPE_BOX)

static void object_class_dispose(GObject *object)
{
    GappSceneViewport *self = GAPP_SCENE_VIEWPORT(object);
    G_OBJECT_CLASS(gapp_scene_viewport_parent_class)->dispose(object);
}

static void gapp_scene_viewport_class_init(GappSceneViewportClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->dispose = object_class_dispose;
}

static void gapp_scene_viewport_init(GappSceneViewport *self)
{
    self->camera = (go_camera_t){.offset = {0, 0}, .target = {0, 0}, .rotation = 0.0f, .zoom = 1.0f};

    GtkWidget *overlay = gtk_overlay_new();
    gtk_box_append(GTK_BOX(self), overlay);

    GtkWidget *toolbar = gapp_scene_viewport_toolbar(self);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), toolbar);

    GtkWidget *viewport = gapp_widget_viewport_new();
    g_signal_connect(viewport, "viewport-ready", G_CALLBACK(gapp_scene_viewport_on_viewport_ready), self);
    g_signal_connect(viewport, "viewport-render", G_CALLBACK(gapp_scene_viewport_on_viewport_render), self);
    gtk_overlay_set_child(GTK_OVERLAY(overlay), viewport);
}

// -----------------
// MARK: SIGNALS
// -----------------
static void gapp_scene_viewport_on_viewport_ready(GtkWidget *viewport, GappSceneViewport *sceneViewport)
{
    // ecs_entity_t paint = go_ecs_entity_new(go_ecs_scene_get_open(), "ShapeRect");
    // ecs_set(go_ecs_world(), paint, go_comp_rectangle_t, {.width = 100, .height = 100, .color = YELLOW, .lineColor = RED, .lineWidth = 2});
    // go_transform_t *transform = ecs_get(go_ecs_world(), paint, go_transform_t);
    // transform->position = (go_vec2_t){100, 100};
}

static void gapp_scene_viewport_on_viewport_render(GtkWidget *viewport, int width, int height, GappSceneViewport *sceneViewport)
{
    ecs_entity_t scene_id = go_ecs_scene_get_open();
    if (!scene_id)
        return;

    ecs_entity_t projectSettings = go_ecs_project_settings();
    if (!projectSettings)
        return;

    go_core_scene_t *scene = ecs_get_mut(go_ecs_world(), scene_id, go_core_scene_t);
    go_core_scene_grid_t *grid = ecs_get_mut(go_ecs_world(), scene_id, go_core_scene_grid_t);
    go_core_scene_rendering *rendering = ecs_get_mut(go_ecs_world(), projectSettings, go_core_scene_rendering);
    go_color_t gridColor = go_color_adjust_contrast(scene->color, 0.1f);

    go_camera_controller_update(&sceneViewport->camera);

    go_gfx_viewport_begin();
    {
        go_gfx_viewport_color(scene->color.r, scene->color.g, scene->color.b);

        go_gfx_camera_begin(sceneViewport->camera);
        {
            go_draw_text("Hello, World!", rendering->resolution.width / 2 - 100, rendering->resolution.height / 2, 25, RED, GO_LAYER);
            go_draw_rect(0, 0, rendering->resolution.width, rendering->resolution.height, BLANK, go_color_adjust_contrast(gridColor, 0.2f), 2.0f, GO_LAYER);
        }
        go_gfx_camera_end();

        if (grid && grid->enabled)
            go_draw_grid(width, height, grid->size, gridColor, GO_LAYER_DEBUG);
    }
    go_gfx_viewport_end();

    go_ecs_process(0.0f);
}

// -----------------
// MARK: UI
// -----------------
static GtkWidget *gapp_scene_viewport_toolbar(GappSceneViewport *scene)
{
    GtkWidget *toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_add_css_class(toolbar, "toolbar_scene");
    // gtk_widget_set_can_target(toolbar, FALSE);
    gtk_widget_set_halign(toolbar, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(toolbar, GTK_ALIGN_START);
    gtk_widget_set_margin_top(toolbar, 10);
    {
        GtkWidget *btn_item;

        btn_item = gapp_widget_button_new_icon_with_label("list-add-symbolic", NULL);
        gtk_box_append(toolbar, btn_item);
        g_signal_connect(btn_item, "clicked", G_CALLBACK(gapp_scene_viewport_toolbar_open_popover_create_entity), scene);

        gtk_box_append(toolbar, gapp_widget_separator_h());

        btn_item = gapp_widget_button_new_icon_with_label("square-outline-thick-symbolic", NULL);
        gtk_box_append(toolbar, btn_item);
        // g_signal_connect(btn_item, "clicked", G_CALLBACK(gapp_signal_project_preview), scene);

        btn_item = gapp_widget_button_new_icon_with_label("circle-outline-thick-symbolic", NULL);
        gtk_box_append(toolbar, btn_item);
        // g_signal_connect(btn_item, "clicked", G_CALLBACK(gapp_signal_project_preview), scene);

        btn_item = gapp_widget_button_new_icon_with_label("draw-text-symbolic", NULL);
        gtk_box_append(toolbar, btn_item);
        // g_signal_connect(btn_item, "clicked", G_CALLBACK(gapp_signal_project_preview), scene);

        gtk_box_append(toolbar, gapp_widget_separator_h());

        btn_item = gapp_widget_button_new_icon_with_label("media-playback-start-symbolic", NULL);
        // g_signal_connect(btn_item, "clicked", G_CALLBACK(gapp_signal_project_preview), scene);
        gtk_box_append(toolbar, btn_item);
    }

    return toolbar;
}

static void gapp_scene_viewport_toolbar_open_popover_create_entity(GtkWidget *widget, GappSceneViewport *self)
{
    GtkWidget *popover = gtk_popover_new();
    gtk_widget_set_parent(popover, widget);
    gtk_popover_set_cascade_popdown(GTK_POPOVER(popover), FALSE);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_popover_set_child(GTK_POPOVER(popover), vbox);
    {
        GtkWidget *btn_item;

        btn_item = gapp_widget_button_new_icon_with_label("background-app-ghost-symbolic", "Sprite");
        gtk_box_append(GTK_BOX(vbox), btn_item);

        btn_item = gapp_widget_button_new_icon_with_label("image-symbolic", "Tiled Sprite");
        gtk_box_append(GTK_BOX(vbox), btn_item);

        gtk_box_append(vbox, gapp_widget_separator_h());

        btn_item = gapp_widget_button_new_icon_with_label("square-outline-thick-symbolic", "Shape Square");
        gtk_box_append(vbox, btn_item);

        btn_item = gapp_widget_button_new_icon_with_label("circle-outline-thick-symbolic", "Shape Circle");
        gtk_box_append(vbox, btn_item);

        btn_item = gapp_widget_button_new_icon_with_label("draw-text-symbolic", "Text");
        gtk_box_append(vbox, btn_item);

        gtk_box_append(vbox, gapp_widget_separator_h());

        btn_item = gapp_widget_button_new_icon_with_label("circle-anchor-center-symbolic", "Empty");
        gtk_box_append(vbox, btn_item);
    }

    gtk_popover_popup(GTK_POPOVER(popover));
}


// -----------------
// MARK: PUBLIC API
// -----------------
GappSceneViewport *gapp_scene_viewport_new(void)
{
    return g_object_new(GAPP_TYPE_SCENE_VIEWPORT, NULL);
}

// MARK: PUBLIC
