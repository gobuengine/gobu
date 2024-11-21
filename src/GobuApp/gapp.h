#ifndef GAPP_H
#define GAPP_H
#include "config.h"
#include "pixio/pixio.h"

#define GAPP_ECS_WORLD gapp_get_world_instance()

typedef enum GappResourceIcon
{
    GAPP_RESOURCE_ICON_NONE = 0,
    GAPP_RESOURCE_ICON_SCENE,
    GAPP_RESOURCE_ICON_PREFAB,
    GAPP_RESOURCE_ICON_TILEMAP,
    GAPP_RESOURCE_ICON_SCRIPT,
    GAPP_RESOURCE_ICON_ANIM2D,
    GAPP_RESOURCE_ICON_COUNT
} GappResourceIcon;

G_BEGIN_DECLS

#define GAPP_TYPE_MAIN (gapp_main_get_type())
G_DECLARE_FINAL_TYPE(GappMain, gapp_main, GAPP, MAIN, GtkApplication)

GObject *gapp_get_editor_instance(void);

GObject *gapp_get_config_instance(void);

ecs_world_t *gapp_get_world_instance(void);

GdkPaintable *gapp_get_resource_icon(GappResourceIcon icon);

void gapp_open_project(GappMain *self, const gchar *path);

void gapp_append_right_panel(GappResourceIcon icon, const gchar *title, GtkWidget *module, gboolean is_button_close);

void gapp_append_left_panel(const gchar *icon, GtkWidget *module);

const gchar *gapp_get_project_path(void);
void gapp_set_project_path(const gchar *path);

G_END_DECLS

#endif // GAPP_H
