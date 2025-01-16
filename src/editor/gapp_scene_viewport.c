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

static void gapp_scene_viewport_ready(GtkWidget *viewport, GappSceneViewport *sceneViewport);
static void gapp_scene_viewport_render(GtkWidget *viewport, int width, int height, GappSceneViewport *sceneViewport);

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

    GtkWidget *viewport = gapp_widget_viewport_new();
    g_signal_connect(viewport, "viewport-ready", G_CALLBACK(gapp_scene_viewport_ready), self);
    g_signal_connect(viewport, "viewport-render", G_CALLBACK(gapp_scene_viewport_render), self);
    gtk_box_append(GTK_BOX(self), viewport);
}

static void gapp_scene_viewport_ready(GtkWidget *viewport, GappSceneViewport *sceneViewport)
{
    // ecs_entity_t paint = go_ecs_entity_new(go_ecs_scene_get_open(), "ShapeRect");
    // ecs_set(go_ecs_world(), paint, go_comp_rectangle_t, {.width = 100, .height = 100, .color = YELLOW, .lineColor = RED, .lineWidth = 2});
    // go_transform_t *transform = ecs_get(go_ecs_world(), paint, go_transform_t);
    // transform->position = (go_vec2_t){100, 100};
}

static void gapp_scene_viewport_render(GtkWidget *viewport, int width, int height, GappSceneViewport *sceneViewport)
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

    if (go_mouse_is_down(GO_MOUSEBUTTON_LEFT))
    {
        go_vec2_t delta = go_mouse_delta();
        delta = go_math_vec2_scale(delta, -1.0f / sceneViewport->camera.zoom);
        sceneViewport->camera.target = go_math_vec2_add(sceneViewport->camera.target, delta);
    }

    float wheel = go_mouse_wheel();
    if (wheel != 0)
    {
        float scaleFactor = 1.0f + (0.25f * fabsf(wheel));
        if (wheel < 0)
            scaleFactor = 1.0f / scaleFactor;
        sceneViewport->camera.zoom = go_math_clamp(sceneViewport->camera.zoom * scaleFactor, 0.60, 64.0f);
    }

    go_gfx_viewport_begin();
    {
        go_gfx_viewport_color(scene->color.r, scene->color.g, scene->color.b);

        go_gfx_camera_begin(sceneViewport->camera);
        {
            go_draw_text("Hello, World!", rendering->resolution.width / 2 - 100, rendering->resolution.height / 2, 25, RED, GO_LAYER_VOID);
            go_draw_rect(0, 0, rendering->resolution.width, rendering->resolution.height, BLANK, go_color_adjust_contrast(gridColor, 0.2f), 2.0f, GO_LAYER_VOID);
        }
        go_gfx_camera_end();

        if (grid && grid->enabled)
            go_draw_grid(width, height, grid->size, gridColor, GO_LAYER_DEBUG);
    }
    go_gfx_viewport_end();

    go_ecs_process(0.0f);
}

GappSceneViewport *gapp_scene_viewport_new(void)
{
    return g_object_new(GAPP_TYPE_SCENE_VIEWPORT, NULL);
}

// MARK: PUBLIC
