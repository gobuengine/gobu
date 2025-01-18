// gapp_project_manager.h
#ifndef GAPP_PROJECT_MANAGER_H
#define GAPP_PROJECT_MANAGER_H

#include "config.h"

G_BEGIN_DECLS

#define CUSTOM_SIZE_KEY 8
#define CUSTOM_SIZE_SELECTED 2
#define DEFAULT_RESOLUTION 4

static const char *resolutions[] = {
    // Retro y clásicas
    "Low resolution 320x240",
    "Standard resolution 640x360",
    "High resolution 800x600",

    // Móviles y tabletas
    "Mobile portrait 720x1280",
    "Desktop & Mobile landscape 1280x720",
    "Tablet portrait 768x1024",
    "Tablet landscape 1024x768",

    // Modernas
    "Full HD 1920x1080",

    // Personalizada
    "Custom size",
    NULL,
};

static const int resolutions_values[] = {
    // Retro y clásicas
    320, 240, // "Low resolution 320x240"
    640, 360, // "Standard resolution 640x360"
    800, 600, // "High resolution 800x600"

    // Móviles y tabletas
    720, 1280, // "Mobile portrait 720x1280"
    1280, 720, // "Desktop & Mobile landscape 1280x720"
    768, 1024, // "Tablet portrait 768x1024"
    1024, 768, // "Tablet landscape 1024x768"

    // Modernas
    1920, 1080, // "Full HD 1920x1080"
};

#define GAPP_TYPE_PROJECT_MANAGER (gapp_project_manager_get_type())
G_DECLARE_FINAL_TYPE(GappProjectManager, gapp_project_manager, GAPP, PROJECT_MANAGER, GtkBox)

GappProjectManager *gapp_project_manager_new(void);

G_END_DECLS

#endif // GAPP_PROJECT_MANAGER_H
