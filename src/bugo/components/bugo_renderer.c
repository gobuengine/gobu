#include "bugo_renderer.h"

static ecs_entity_t ecs_renderer_t;

void
bugo_ecs_init_renderer(ecs_world_t* world)
{
  ecs_renderer_t = ecs_component_init(
    world,
    &(ecs_component_desc_t){
      .entity = ecs_entity(world, { .name = "ComponentRenderer" }),
      .type.size = ECS_SIZEOF(ComponentRenderer),
      .type.alignment = ECS_ALIGNOF(ComponentRenderer),
    });

  ecs_struct(world, {
        .entity = ecs_renderer_t,
        .members = {
            { .name = "name", .type = ecs_id(ecs_string_t) },
            { .name = "visible", .type = ecs_id(ecs_bool_t) },
            { .name = "active", .type = ecs_id(ecs_bool_t) },
            { .name = "position", .type = bugo_ecs_get_vector2_id()},
            { .name = "scale", .type = bugo_ecs_get_vector2_id()},
            { .name = "rotation", .type = ecs_id(ecs_f32_t) },
            { .name = "origin", .type = bugo_ecs_get_vector2_id() },
            { .name = "zindex", .type = ecs_id(ecs_i32_t) },
        },
    });
}

void
bugo_ecs_set_renderer(ecs_entity_t entity, ComponentRenderer* props)
{
  ecs_set_id(
    bugo_ecs_world(), entity, ecs_renderer_t, sizeof(ComponentRenderer), props);
}

ecs_entity_t
bugo_ecs_get_renderer_id(void)
{
  return ecs_renderer_t;
}