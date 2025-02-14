#include "gobu-ecs.h"
#include "gobu-util.h"
#include <glib.h>

go_internal void gobuImport(ecs_world_t *world);
go_internal go_core_scene_phases_t gbCoreScenePhases;

// Esta es la entidad que guarda la confi del proyecto abierto.
go_internal ecs_entity_t projectSettings = 0;

// Cada proyecto tiene su propio mundo
// por eso no importa globalizar esta variable.
go_internal ecs_world_t *uworld = NULL;

ECS_TAG_DECLARE(gbTagScene);
ECS_TAG_DECLARE(gbOnSceneOpen);
ECS_TAG_DECLARE(gbOnSceneClose);
ECS_TAG_DECLARE(gbOnSceneLoad);
// ECS_TAG_DECLARE(gbOnSceneSave);
ECS_TAG_DECLARE(gbOnSceneReload);
ECS_TAG_DECLARE(gbOnSceneRename);
ECS_TAG_DECLARE(gbOnSceneDelete);
ECS_TAG_DECLARE(gbOnSceneCreate);
// property inspector
ECS_COMPONENT_DECLARE(go_property_t);
// pipeline
ECS_COMPONENT_DECLARE(go_core_scene_phases_t);
ECS_COMPONENT_DECLARE(go_core_scene_t);
ECS_COMPONENT_DECLARE(go_core_scene_physics_t);
ECS_COMPONENT_DECLARE(go_core_scene_grid_t);

// game project settings
ECS_COMPONENT_DECLARE(go_core_project_settings1_t);
ECS_COMPONENT_DECLARE(go_core_project_settings2_t);
ECS_COMPONENT_DECLARE(go_core_scene_rendering);

// ECS_COMPONENT_DECLARE(gbSceneActive);
ECS_COMPONENT_DECLARE(go_origin_t);
ECS_COMPONENT_DECLARE(go_texture_flip_t);
ECS_COMPONENT_DECLARE(go_texture_filter_t);
ECS_COMPONENT_DECLARE(go_scale_mode_t);
ECS_COMPONENT_DECLARE(go_resolution_mode_t);
ECS_COMPONENT_DECLARE(go_resource_t);
ECS_COMPONENT_DECLARE(go_color_t);
ECS_COMPONENT_DECLARE(go_rect_t);
ECS_COMPONENT_DECLARE(go_vec2_t);
ECS_COMPONENT_DECLARE(go_size_t);
ECS_COMPONENT_DECLARE(go_boundingbox_t);
ECS_COMPONENT_DECLARE(go_transform_t);
ECS_COMPONENT_DECLARE(go_image_t);
ECS_COMPONENT_DECLARE(go_texture_t);
ECS_COMPONENT_DECLARE(go_font_t);
ECS_COMPONENT_DECLARE(go_frame_t);
ECS_COMPONENT_DECLARE(go_comp_text_t);
ECS_COMPONENT_DECLARE(go_comp_sprite_t);
ECS_COMPONENT_DECLARE(go_comp_circle_t);
ECS_COMPONENT_DECLARE(go_comp_rectangle_t);

// -----------------
// NOTE MARK: ENTITY PROPERTYI INSPECTOR
// -----------------
go_internal ecs_entity_t propertyEntity = 0;

#define GPROPERTY(name, ...)\
        go_ecs_create_property(name, propertyEntity, &(go_property_t)__VA_ARGS__)

#define go_ecs_struct(entity, ...)\
{\
    propertyEntity = entity;\
    ecs_struct_desc_t v = {entity, __VA_ARGS__};\
    ecs_struct_init(uworld, &v);\
}\

go_internal const char *go_ecs_create_property(const char *name, ecs_entity_t parent, const go_property_t *desc)
{
    ecs_entity_t m = ecs_entity(uworld, {.name = name, .parent = parent});
    ecs_set_id(uworld, m, ecs_id(go_property_t), sizeof(desc), desc);
    return name;
}

// -----------------
// NOTE MARK: WORLD
// -----------------
go_public void go_ecs_init(void)
{
    go_ecs_init_c("New Project", 800, 600);
}

go_public void go_ecs_init_c(const char *name, int width, int height)
{
    uworld = ecs_init();
    ECS_IMPORT(uworld, gobu);

    // Esto es para crear el project settings siempre
    // esto ayuda a que siempre exista un project settings
    // con valores por defecto y si se agrega un valor nuevo
    // se actualizara en el archivo de proyecto.
    go_ecs_project_settings_init(name, width, height);
}

ecs_world_t *go_ecs_world(void)
{
    return uworld;
}

go_public void go_ecs_free(void)
{
    ecs_fini(uworld);
}

go_public void go_ecs_process(float deltaTime)
{
    ecs_progress(uworld, deltaTime);
}

go_public void go_ecs_save_to_file(const char *filename)
{
    go_util_write_text_file(filename, ecs_world_to_json(uworld, NULL));
}

go_public bool go_ecs_load_from_file(const char *filename)
{
    if (go_util_exist_file(filename) == false)
        return false;

    return ecs_world_from_json_file(uworld, filename, NULL) == NULL ? false : true;
}

// -----------------
// NOTE MARK: project settings
// -----------------
go_public void go_ecs_project_settings_init(const char *name, int width, int height)
{
    if (projectSettings > 0)
        ecs_delete(uworld, projectSettings);

    projectSettings = ecs_new_low_id(uworld);
    // ecs_set_name(ecs, projectSettings, go_util_string("ProjectSettings"));

    ecs_set(uworld, projectSettings, go_core_project_settings1_t, {
        .name = go_util_string(name),
    });

    ecs_set(uworld, projectSettings, go_core_project_settings2_t, {
        .name = go_util_string("com.example.game"),
        .version = go_util_string("1.0.0"),
        .publisher = go_util_string("[Publisher]")
    });

    ecs_set(uworld, projectSettings, go_core_scene_rendering, {
        .resolution = {width, height},
        .targetFps = 60,
        .resolutionMode = GB_RESIZE_MODE_NO_CHANGE,
        .scaleMode = GB_SCALE_MODE_LINEAR
    });
}

go_public ecs_entity_t go_ecs_project_settings(void)
{
    return projectSettings;
}

// -----------------
// NOTE MARK: EntityTemplate
// -----------------
go_public ecs_entity_t go_ecs_enttiy_new_tmp_text(float x, float y)
{
    ecs_world_t *world = go_ecs_world();
    ecs_entity_t entity = go_ecs_entity_new_low(go_ecs_scene_get_open());

    g_autofree gchar *name_entity = go_util_string_format("Text%ld", entity);
    ecs_set_name(world, entity, name_entity);
    ecs_set(world, entity, go_transform_t, {.position = {x, y}, .scale = {1, 1}, .rotation = 0, .origin = GB_ORIGIN_CENTER});
    ecs_set(world, entity, go_comp_text_t, {.text = go_util_string("Hello World")});

    return entity;
}

// -----------------
// NOTE MARK: Entity
// -----------------
go_public ecs_entity_t go_ecs_entity_new_low(ecs_entity_t parent)
{
    ecs_entity_t entity = ecs_new_low_id(uworld);
    if (parent > 0)
        go_ecs_set_parent(entity, parent);

    return entity;
}

go_public ecs_entity_t go_ecs_entity_new(ecs_entity_t parent, const char *name)
{
    ecs_entity_t entity = go_ecs_entity_new_low(parent);
    g_autofree gchar *name_entity = go_util_string_format("%s%ld", name, entity);
    ecs_set_name(uworld, entity, name_entity);
    ecs_set(uworld, entity, go_transform_t, {.position = {0, 0}, .scale = {1, 1}, .rotation = 0, .origin = GB_ORIGIN_CENTER});

    return entity;
}

go_public void go_ecs_set_parent(ecs_entity_t entity, ecs_entity_t parent)
{
    ecs_add_pair(uworld, entity, EcsChildOf, parent);
}

go_public ecs_entity_t go_ecs_get_parent(ecs_entity_t entity)
{
    return ecs_get_target(uworld, entity, EcsChildOf, 0);
}

go_public bool go_ecs_has_parent(ecs_entity_t entity)
{
    return ecs_has_id(uworld, entity, ecs_pair(EcsChildOf, EcsWildcard));
}

go_public ecs_entity_t go_ecs_clone(ecs_entity_t entity)
{
    ecs_entity_t clone = ecs_clone(uworld, 0, entity, TRUE);
    ecs_set_name(uworld, clone, go_util_string_format("%s%ld", ecs_get_name(uworld, entity), clone));
    go_ecs_set_parent(clone, go_ecs_get_parent(entity));

    ecs_iter_t it = ecs_children(uworld, entity);
    while (ecs_children_next(&it))
        for (int i = 0; i < it.count; i++)
            if (ecs_is_alive(uworld, it.entities[i]))
                go_ecs_set_parent(go_ecs_clone(it.entities[i]), clone);

    return clone;
}

go_public bool go_ecs_is_enabled(ecs_entity_t entity)
{
    return !ecs_has_id(uworld, entity, EcsDisabled);
}

go_public void go_ecs_add_component_name(ecs_entity_t entity, const char *component)
{
    ecs_add_id(uworld, entity, ecs_lookup(uworld, component));
}

const char *go_ecs_name(ecs_entity_t entity)
{
    return ecs_get_name(uworld, entity);
}

go_public void go_ecs_set_name(ecs_entity_t entity, const char *name)
{
    ecs_set_name(uworld, entity, name);
}

go_public void go_ecs_enable(ecs_entity_t entity, bool enable)
{
    ecs_enable(uworld, entity, enable);
}

// -----------------
// NOTE MARK: Entity Events
// -----------------
go_public ecs_entity_t go_ecs_observer(ecs_entity_t event, ecs_iter_action_t callback, void *ctx)
{
    return ecs_observer(uworld, {
        .query.terms = {{.id = EcsAny}, {EcsDisabled, .oper = EcsOptional}},
        .events = {event},
        .callback = callback,
        .ctx = ctx,
    });
}

go_public void go_ecs_emit(ecs_entity_t event, ecs_entity_t entity)
{
    ecs_emit(uworld, &(ecs_event_desc_t){.entity = entity, .event = event});
}

// -----------------
// NOTE MARK: Entity SCENE
// -----------------
go_public ecs_entity_t go_ecs_scene_new(const char *name)
{
    g_autofree gchar *new_name = go_util_string_format("WorldScene.%s", name);

    // Le agregamos un numero al final si ya existe
    int counter = 1;
    while (ecs_lookup(uworld, new_name) > 0)
        new_name = go_util_string_format("WorldScene.%s%d", name, counter++);

    ecs_entity_t scene = ecs_entity(uworld, {.name = go_util_string(new_name), .add = ecs_ids(gbTagScene)});
    ecs_set(uworld, scene, go_core_scene_t, {.color = GOBUWHITE });
    ecs_set(uworld, scene, go_core_scene_physics_t, {.enabled = TRUE, .debug = FALSE, .gravity = 980, .gravityDirection = {0, -1}});
    ecs_set(uworld, scene, go_core_scene_grid_t, {.size = 32, .enabled = TRUE});
    go_ecs_enable(scene, FALSE);
    go_ecs_emit(gbOnSceneCreate, scene);

    return scene;
}

go_public ecs_entity_t go_ecs_scene_clone(ecs_entity_t entity)
{
    const char *orig_name = go_ecs_name(entity);

    g_autofree gchar *new_name = go_util_string(orig_name);
    ecs_entity_t parent = ecs_lookup(uworld, "WorldScene");

    // Le agregamos un numero al final si ya existe
    int counter = 1;
    while (ecs_lookup_child(uworld, parent, new_name) > 0)
        new_name = go_util_string_format("%s%d", orig_name, counter++);

    ecs_entity_t scene_clone = go_ecs_clone(entity);
    ecs_set_name(uworld, scene_clone, new_name);
    go_ecs_enable(scene_clone, FALSE);

    return scene_clone;
}

go_public void go_ecs_scene_delete(ecs_entity_t entity)
{
    go_ecs_emit(gbOnSceneDelete, entity);
    ecs_delete(uworld, entity);
}

go_public void go_ecs_scene_open(ecs_entity_t entity)
{
    ecs_entity_t scene_active = go_ecs_scene_get_open();
    if (scene_active > 0)
        ecs_enable(uworld, scene_active, FALSE);

    ecs_enable(uworld, entity, TRUE);
    go_ecs_emit(gbOnSceneOpen, entity);
}

go_public ecs_entity_t go_ecs_scene_get_open(void)
{
    ecs_query_t *q = ecs_query(uworld, {.terms = {{gbTagScene}}});
    ecs_iter_t it = ecs_query_iter(uworld, q);
    ecs_entity_t scene = ecs_iter_first(&it);
    ecs_query_fini(q);
    return scene;
}

go_public void go_ecs_scene_reload(void)
{
    ecs_entity_t scene_active = go_ecs_scene_get_open();
    if (scene_active > 0)
    {
        go_ecs_scene_open(scene_active);
    }
}

go_public int go_ecs_scene_count(void)
{
    return ecs_count(uworld, gbTagScene);
}

go_public ecs_entity_t go_ecs_scene_get_by_name(const char *name)
{
    ecs_entity_t parent = ecs_lookup(uworld, "WorldScene");
    return ecs_lookup_child(uworld, parent, name);
}

go_public void go_ecs_scene_rename(ecs_entity_t entity, const char *name)
{
    ecs_set_name(uworld, entity, name);
    go_ecs_emit(gbOnSceneRename, entity);
}

go_public bool go_ecs_scene_has(ecs_entity_t entity)
{
    return ecs_has(uworld, entity, gbTagScene);
}

go_public void go_ecs_scene_process(ecs_entity_t root, float delta)
{
    // // process
    // go_transform_t *transform = ecs_get(uworld, root, go_transform_t);
    // if (transform)
    // {
    //     float px = transform->position.x;
    //     float py = transform->position.y;
    //     float sx = transform->scale.x;
    //     float sy = transform->scale.y;
    //     float angle = transform->rotation;
    //     go_origin_t origin = transform->origin;

    //     go_comp_rectangle_t *shape = ecs_get(uworld, root, go_comp_rectangle_t);
    //     if (shape)
    //         go_draw_rect(px, py, shape->width, shape->height, shape->color, shape->lineColor, shape->lineWidth, 0);
    // }
    // // children root
    // ecs_iter_t it = ecs_children(uworld, root);
    // while (ecs_children_next(&it))
    // {
    //     for (int i = 0; i < it.count; i++)
    //     {
    //         ecs_entity_t entity = it.entities[i];
    //         go_ecs_scene_process(uworld, entity, delta);
    //     }
    // }
}

// -----------------
// NOTE MARK: MODULE BASE
// -----------------
go_internal void gobu_core_scene_pre_update(ecs_iter_t *it)
{
    for (int i = 0; i < it->count; i++)
    {
        ecs_entity_t e = it->entities[i];
    }
}

go_internal void gobu_core_scene_render_draw(ecs_iter_t *it)
{
    for (int i = 0; i < it->count; i++)
    {
        ecs_entity_t e = it->entities[i];
    }
}

go_internal void gobu_core_scene_render_pre_draw(ecs_iter_t *it)
{
    go_core_scene_t *scene = ecs_field(it, go_core_scene_t, 0);

    for (int i = 0; i < it->count; i++)
    {
        ecs_entity_t e = it->entities[i];

        // go_gfx_viewport_begin(scene[i].context);
        // go_gfx_viewport_color(scene[i].context, scene[i].clear_color.r, scene[i].clear_color.g, scene[i].clear_color.b);
        // go_draw_rect(0, 0, scene[i].size.width, scene[i].size.height, BLANK, scene[i].color, 2.0f, 0);
    }
}

go_internal void gobu_core_scene_render_post_draw(ecs_iter_t *it)
{
    go_core_scene_t *scene = ecs_field(it, go_core_scene_t, 0);

    for (int i = 0; i < it->count; i++)
    {
        ecs_entity_t e = it->entities[i];

        // go_gfx_viewport_end(scene[i].context);
    }
}

go_internal void gobuImport(ecs_world_t *world)
{
    ECS_MODULE(world, gobu);

    ECS_TAG_DEFINE(world, gbTagScene);
    ECS_TAG_DEFINE(world, gbOnSceneOpen);
    ECS_TAG_DEFINE(world, gbOnSceneClose);
    ECS_TAG_DEFINE(world, gbOnSceneLoad);
    // ECS_TAG_DEFINE(world, gbOnSceneSave);
    ECS_TAG_DEFINE(world, gbOnSceneReload);
    ECS_TAG_DEFINE(world, gbOnSceneRename);
    ECS_TAG_DEFINE(world, gbOnSceneDelete);
    ECS_TAG_DEFINE(world, gbOnSceneCreate);
    // property inspector
    ECS_COMPONENT_DEFINE(world, go_property_t);
    // pipeline
    ECS_COMPONENT_DEFINE(world, go_core_scene_phases_t);
    ECS_COMPONENT_DEFINE(world, go_core_scene_t);
    ECS_COMPONENT_DEFINE(world, go_core_scene_physics_t);
    ECS_COMPONENT_DEFINE(world, go_core_scene_grid_t);
    // game project settings
    ECS_COMPONENT_DEFINE(world, go_core_project_settings1_t);
    ECS_COMPONENT_DEFINE(world, go_core_project_settings2_t);
    ECS_COMPONENT_DEFINE(world, go_core_scene_rendering);

    // ECS_COMPONENT_DEFINE(world, gbSceneActive);
    ECS_COMPONENT_DEFINE(world, go_origin_t);
    ECS_COMPONENT_DEFINE(world, go_texture_flip_t);
    ECS_COMPONENT_DEFINE(world, go_texture_filter_t);
    ECS_COMPONENT_DEFINE(world, go_scale_mode_t);
    ECS_COMPONENT_DEFINE(world, go_resolution_mode_t);
    ECS_COMPONENT_DEFINE(world, go_resource_t);
    ECS_COMPONENT_DEFINE(world, go_color_t);
    ECS_COMPONENT_DEFINE(world, go_rect_t);
    ECS_COMPONENT_DEFINE(world, go_vec2_t);
    ECS_COMPONENT_DEFINE(world, go_size_t);
    ECS_COMPONENT_DEFINE(world, go_boundingbox_t);
    ECS_COMPONENT_DEFINE(world, go_transform_t);
    ECS_COMPONENT_DEFINE(world, go_image_t);
    ECS_COMPONENT_DEFINE(world, go_texture_t);
    ECS_COMPONENT_DEFINE(world, go_font_t);
    ECS_COMPONENT_DEFINE(world, go_comp_text_t);
    ECS_COMPONENT_DEFINE(world, go_comp_sprite_t);
    ECS_COMPONENT_DEFINE(world, go_frame_t);
    ECS_COMPONENT_DEFINE(world, go_comp_circle_t);
    ECS_COMPONENT_DEFINE(world, go_comp_rectangle_t);

// MARK: ENUM
    ecs_enum(world, {
        .entity = ecs_id(go_origin_t),
        .constants = {
            {.name = "TopLeft", .value = GB_ORIGIN_TOP_LEFT},
            {.name = "TopCenter", .value = GB_ORIGIN_TOP_CENTER},
            {.name = "TopRight", .value = GB_ORIGIN_TOP_RIGHT},
            {.name = "CenterLeft", .value = GB_ORIGIN_CENTER_LEFT},
            {.name = "Center", .value = GB_ORIGIN_CENTER},
            {.name = "CenterRight", .value = GB_ORIGIN_CENTER_RIGHT},
            {.name = "BottomLeft", .value = GB_ORIGIN_BOTTOM_LEFT},
            {.name = "BottomCenter", .value = GB_ORIGIN_BOTTOM_CENTER},
            {.name = "BottomRight", .value = GB_ORIGIN_BOTTOM_RIGHT},
        },
    });

    ecs_enum(world, {
        .entity = ecs_id(go_texture_flip_t),
        .constants = {
            {.name = "None", .value = GB_NO_FLIP},
            {.name = "Vertical", .value = GB_FLIP_VERTICAL},
            {.name = "Horizontal", .value = GB_FLIP_HORIZONTAL},
            {.name = "Both", .value = GB_FLIP_BOTH},
        },
    });

    ecs_enum(world, {
        .entity = ecs_id(go_texture_filter_t),
        .constants = {
            {.name = "Point", .value = GB_FILTER_POINT},
            {.name = "Bilinear", .value = GB_FILTER_BILINEAR},
            {.name = "Trilinear", .value = GB_FILTER_TRILINEAR},
        },
    });

    ecs_enum(world, {
        .entity = ecs_id(go_scale_mode_t),
        .constants = {
            {.name = "scale_nearest", .value = GB_SCALE_MODE_NEAREST},
            {.name = "scale_linear", .value = GB_SCALE_MODE_LINEAR},
        },
    });

    ecs_enum(world, {
        .entity = ecs_id(go_resolution_mode_t),
        .constants = {
            {.name = "resize_no_change", .value = GB_RESIZE_MODE_NO_CHANGE},
            {.name = "resize_fill_screen", .value = GB_RESIZE_MODE_FILL_SCREEN},
            {.name = "resize_adjust_width", .value = GB_RESIZE_MODE_ADJUST_WIDTH},
            {.name = "resize_adjust_height", .value = GB_RESIZE_MODE_ADJUST_HEIGHT},
        },
    });

// MARK: DATA-COMPONENT
    ecs_struct(world, {
        .entity = ecs_id(go_resource_t),
        .members = {
            {.name = "resource", .type = ecs_id(ecs_string_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(go_color_t),
        .members = {
            {.name = "r", .type = ecs_id(ecs_byte_t)},
            {.name = "g", .type = ecs_id(ecs_byte_t)},
            {.name = "b", .type = ecs_id(ecs_byte_t)},
            {.name = "a", .type = ecs_id(ecs_byte_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(go_size_t),
        .members = {
            {.name = "width", .type = ecs_id(ecs_f32_t)},
            {.name = "height", .type = ecs_id(ecs_f32_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(go_vec2_t),
        .members = {
            {.name = "x", .type = ecs_id(ecs_f32_t)},
            {.name = "y", .type = ecs_id(ecs_f32_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(go_rect_t),
        .members = {
            {.name = "x", .type = ecs_id(ecs_f32_t)},
            {.name = "y", .type = ecs_id(ecs_f32_t)},
            {.name = "width", .type = ecs_id(ecs_f32_t)},
            {.name = "height", .type = ecs_id(ecs_f32_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(go_transform_t),
        .members = {
            {.name = "position", .type = ecs_id(go_vec2_t)},
            {.name = "scale", .type = ecs_id(go_vec2_t)},
            {.name = "rotation", .type = ecs_id(ecs_f32_t)},
            {.name = "origin", .type = ecs_id(go_origin_t)},
        },
    });

// MARK: COMPONENT DRAW
    go_ecs_struct(ecs_id(go_comp_text_t),{
        {.name = "text", .type = ecs_id(ecs_string_t)},
        {.name = "fontSize", .type = ecs_id(ecs_u32_t)},
        {.name = "spacing", .type = ecs_id(ecs_f32_t)},
        {.name = "color", .type = ecs_id(go_color_t)},
        {.name = GPROPERTY("font", {.type = GB_PROPERTY_TYPE_FONT}), .type = ecs_id(go_resource_t)},
    });

    go_ecs_struct(ecs_id(go_comp_sprite_t),{
        {.name = GPROPERTY("texture", {.type = GB_PROPERTY_TYPE_TEXTURE}), .type = ecs_id(go_resource_t)},
        {.name = "filter", .type = ecs_id(go_texture_filter_t)},
        {.name = "flip", .type = ecs_id(go_texture_flip_t)},
        {.name = "tint", .type = ecs_id(go_color_t)},
    });

    ecs_struct(world, {
        .entity = ecs_id(go_frame_t),
        .members = {
            {.name = "hframes", .type = ecs_id(ecs_u32_t)},
            {.name = "vframes", .type = ecs_id(ecs_u32_t)},
            {.name = "frame", .type = ecs_id(ecs_u32_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(go_comp_circle_t),
        .members = {
            {.name = "radius", .type = ecs_id(ecs_f32_t)},
            {.name = "thickness", .type = ecs_id(ecs_f32_t), .range = {0, 20}},
            {.name = "color", .type = ecs_id(go_color_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(go_comp_rectangle_t),
        .members = {
            {.name = "width", .type = ecs_id(ecs_f32_t)},
            {.name = "height", .type = ecs_id(ecs_f32_t)},
            {.name = "roundness", .type = ecs_id(ecs_f32_t), .range = {0.0, 1.0}},
            // {.name = "segments", .type = ecs_id(ecs_f32_t), .range = {0, 60}},
            {.name = "color", .type = ecs_id(go_color_t)},
            {.name = "lineWidth", .type = ecs_id(ecs_f32_t), .range = {0, 20}},
            {.name = "lineColor", .type = ecs_id(go_color_t)},
        },
    });

// MARK: CORE SCENE 
    ecs_struct(world, {
        .entity = ecs_id(go_core_scene_grid_t),
        .members = {
            {.name = "enabled", .type = ecs_id(ecs_bool_t)},
            {.name = "size", .type = ecs_id(ecs_u32_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(go_core_scene_t),
        .members = {
            {.name = "color", .type = ecs_id(go_color_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(go_core_scene_physics_t),
        .members = {
            {.name = "gravity", .type = ecs_id(ecs_u32_t)},
            {.name = "gravityDirection", .type = ecs_id(go_vec2_t)},
        },
    });

// MARK: PROJECT SETTINGS
    go_ecs_struct(ecs_id(go_core_project_settings1_t),{
        {.name = "name", .type = ecs_id(ecs_string_t)},
        {.name = GPROPERTY("description", {.type = GB_PROPERTY_TYPE_TEXT}), .type = ecs_id(ecs_string_t)},
        {.name = "author", .type = ecs_id(ecs_string_t)},
    });

    ecs_struct(world, {
        .entity = ecs_id(go_core_project_settings2_t),
        .members = {
            {.name = "name", .type = ecs_id(ecs_string_t)},
            {.name = "version", .type = ecs_id(ecs_string_t)},
            {.name = "publisher", .type = ecs_id(ecs_string_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(go_core_scene_rendering),
        .members = {
            {.name = "resolution", .type = ecs_id(go_size_t)},
            {.name = "targetFps", .type = ecs_id(ecs_u32_t)},
            {.name = "resolutionMode", .type = ecs_id(go_resolution_mode_t)},
            {.name = "scaleMode", .type = ecs_id(go_scale_mode_t)},
        },
    });

// MARK: System and pipelines
    gbCoreScenePhases.PreDraw = ecs_new_w_id(world, EcsPhase);
    gbCoreScenePhases.Background = ecs_new_w_id(world, EcsPhase);
    gbCoreScenePhases.Draw = ecs_new_w_id(world, EcsPhase);
    gbCoreScenePhases.PostDraw = ecs_new_w_id(world, EcsPhase);

    ecs_add_pair(world, gbCoreScenePhases.PreDraw, EcsDependsOn, EcsOnStore);
    ecs_add_pair(world, gbCoreScenePhases.Background, EcsDependsOn, gbCoreScenePhases.PreDraw);
    ecs_add_pair(world, gbCoreScenePhases.Draw, EcsDependsOn, gbCoreScenePhases.Background);
    ecs_add_pair(world, gbCoreScenePhases.PostDraw, EcsDependsOn, gbCoreScenePhases.Draw);

    ecs_system(world, {
        .entity = ecs_entity(world, { .add = ecs_ids(ecs_dependson(EcsPreUpdate)) }),
        .query.terms = {
            {ecs_id(go_transform_t)}, 
            {ecs_id(go_comp_text_t), .oper = EcsOptional},
            {ecs_id(go_comp_circle_t), .oper = EcsOptional},
            {ecs_id(go_comp_rectangle_t), .oper = EcsOptional},
            {ecs_id(go_comp_sprite_t), .oper = EcsOptional},
        },
        .callback = gobu_core_scene_pre_update,
    });

    ecs_system(world, {
        .entity = ecs_entity(world, { .add = ecs_ids(ecs_dependson(gbCoreScenePhases.Draw)) }),
        .query.terms = {
            {ecs_id(go_transform_t)}, 
            {ecs_id(go_comp_text_t), .oper = EcsOptional},
            {ecs_id(go_comp_circle_t), .oper = EcsOptional},
            {ecs_id(go_comp_rectangle_t), .oper = EcsOptional},
            {ecs_id(go_comp_sprite_t), .oper = EcsOptional},
        },
        .callback = gobu_core_scene_render_draw,
    });

    ecs_system(world, {
        .entity = ecs_entity(world, { .add = ecs_ids(ecs_dependson(gbCoreScenePhases.PreDraw)) }),
        .query.terms = {
            {ecs_id(go_core_scene_t)}, 
        },
        .callback = gobu_core_scene_render_pre_draw,
    });

    ecs_system(world, {
        .entity = ecs_entity(world, { .add = ecs_ids(ecs_dependson(gbCoreScenePhases.PostDraw)) }),
        .query.terms = {
            {ecs_id(go_core_scene_t)}, 
        },
        .callback = gobu_core_scene_render_post_draw,
    });
}
