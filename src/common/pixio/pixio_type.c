#include "pixio_type.h"

ECS_COMPONENT_DECLARE(pixio_texture_filter_t);
ECS_COMPONENT_DECLARE(pixio_color_t);
ECS_COMPONENT_DECLARE(pixio_vector2_t);
ECS_COMPONENT_DECLARE(pixio_resource_t);
ECS_COMPONENT_DECLARE(pixio_resource_font_t);
ECS_COMPONENT_DECLARE(pixio_resource_texture_t);
ECS_COMPONENT_DECLARE(pixio_rect_t);
ECS_COMPONENT_DECLARE(pixio_font_t);
ECS_COMPONENT_DECLARE(pixio_texture_t);
ECS_COMPONENT_DECLARE(pixio_transform_t);
ECS_COMPONENT_DECLARE(pixio_text_t);
ECS_COMPONENT_DECLARE(pixio_sprite_t);

void pixio_type_moduleImport(ecs_world_t *world)
{
    ECS_MODULE(world, pixio_type_module);
    ECS_COMPONENT_DEFINE(world, pixio_texture_filter_t);
    ECS_COMPONENT_DEFINE(world, pixio_color_t);
    ECS_COMPONENT_DEFINE(world, pixio_vector2_t);
    ECS_COMPONENT_DEFINE(world, pixio_resource_t);
    ECS_COMPONENT_DEFINE(world, pixio_resource_font_t);
    ECS_COMPONENT_DEFINE(world, pixio_resource_texture_t);
    ECS_COMPONENT_DEFINE(world, pixio_rect_t);
    ECS_COMPONENT_DEFINE(world, pixio_font_t);
    ECS_COMPONENT_DEFINE(world, pixio_texture_t);
    ECS_COMPONENT_DEFINE(world, pixio_transform_t);
    ECS_COMPONENT_DEFINE(world, pixio_text_t);
    ECS_COMPONENT_DEFINE(world, pixio_sprite_t);

    ecs_enum(world, {
        .entity = ecs_id(pixio_texture_filter_t),
        .constants = {
            {.name = "Point", .value = TEXTURE_FILTER_POINT},
            {.name = "Bilinear", .value = TEXTURE_FILTER_BILINEAR},
            {.name = "Trilinear", .value = TEXTURE_FILTER_TRILINEAR},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(pixio_color_t),
        .members = {
            {.name = "r", .type = ecs_id(ecs_char_t)},
            {.name = "g", .type = ecs_id(ecs_char_t)},
            {.name = "b", .type = ecs_id(ecs_char_t)},
            {.name = "a", .type = ecs_id(ecs_char_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(pixio_vector2_t),
        .members = {
            {.name = "x", .type = ecs_id(ecs_f32_t)},
            {.name = "y", .type = ecs_id(ecs_f32_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(pixio_rect_t),
        .members = {
            {.name = "x", .type = ecs_id(ecs_f32_t)},
            {.name = "y", .type = ecs_id(ecs_f32_t)},
            {.name = "width", .type = ecs_id(ecs_f32_t)},
            {.name = "height", .type = ecs_id(ecs_f32_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(pixio_resource_t),
        .members = {
            {.name = "resource", .type = ecs_id(ecs_string_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(pixio_resource_font_t),
        .members = {
            {.name = "resource", .type = ecs_id(ecs_string_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(pixio_resource_texture_t),
        .members = {
            {.name = "resource", .type = ecs_id(ecs_string_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(pixio_transform_t),
        .members = {
            {.name = "position", .type = ecs_id(pixio_vector2_t)},
            {.name = "scale", .type = ecs_id(pixio_vector2_t)},
            {.name = "rotation", .type = ecs_id(ecs_f32_t)},
            {.name = "origin", .type = ecs_id(pixio_vector2_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(pixio_text_t),
        .members = {
            {.name = "text", .type = ecs_id(ecs_string_t)},
            {.name = "fontSize", .type = ecs_id(ecs_u32_t)},
            {.name = "spacing", .type = ecs_id(ecs_f32_t)},
            {.name = "color", .type = ecs_id(pixio_color_t)},
            {.name = "font", .type = ecs_id(pixio_resource_font_t)},
        },
    });

    ecs_struct(world, {
        .entity = ecs_id(pixio_sprite_t),
        .members = {
            {.name = "texture", .type = ecs_id(pixio_resource_texture_t)},
            {.name = "tint", .type = ecs_id(pixio_color_t)},
            {.name = "filter", .type = ecs_id(pixio_texture_filter_t)},
        },
    });
}
