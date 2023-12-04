#include "gobu_gizmos.h"
#include "gobu_rendering.h"
#include "gobu_bounding.h"
#include "gobu_input.h"
#include "gobu_camera.h"
#include "gobu_transform.h"
#include "raygo/raymath.h"

ECS_COMPONENT_DECLARE(ecs_gizmos_t);

static void GobuGizmos_Update(ecs_iter_t* it);
static void GobuGizmos_Draw(ecs_iter_t* it);

static void unselected_all(ecs_iter_t* it, ecs_gizmos_t* gizmos)
{
    for (int i = it->count - 1; i >= 0; i--)
    {
        gizmos[i].selected = false;
    }
}

static float point_to_angle(Vector2 mouse, Rectangle bonding)
{
    float mx = mouse.x - (bonding.x + bonding.width / 2);
    float my = mouse.y - (bonding.y + bonding.height / 2);
    float angle = atan2(my, mx);
    return angle;
}

void GobuGizmosImport(ecs_world_t* world)
{
    ECS_MODULE(world, GobuGizmos);
    ECS_IMPORT(world, GobuRendering);
    ECS_IMPORT(world, GobuBoundingBox);
    ECS_IMPORT(world, GobuInputSystem);
    ECS_IMPORT(world, GobuCamera);
    ECS_IMPORT(world, GobuTransform);

    ECS_COMPONENT_DEFINE(world, ecs_gizmos_t);

    RenderPhases* phases = ecs_singleton_get(world, RenderPhases);

    ecs_struct(world, {
        .entity = ecs_id(ecs_gizmos_t),
        .members = {
            {.name = "selected", .type = ecs_id(ecs_bool_t) },
        }
    });

    ECS_SYSTEM(world, GobuGizmos_Draw, phases->Draw, ecs_gizmos_t, GBoundingBox);
    ECS_SYSTEM(world, GobuGizmos_Update, EcsPostUpdate, ecs_gizmos_t, GBoundingBox, GPosition, GScale, GRotation);
}

static void GobuGizmos_Update(ecs_iter_t* it)
{
    ecs_gizmos_t* gizmos = ecs_field(it, ecs_gizmos_t, 1);
    GBoundingBox* box = ecs_field(it, GBoundingBox, 2);
    GPosition* post = ecs_field(it, GPosition, 3);
    GScale* sca = ecs_field(it, GScale, 4);
    GRotation* rot = ecs_field(it, GRotation, 5);

    ecs_entity_t Engine = ecs_lookup(it->world, "Engine");
    GCamera* camera = ecs_get(it->world, Engine, GCamera);
    GInputSystem* input = ecs_get(it->world, Engine, GInputSystem);

    Vector2 mouse = input->mouse.get_screen_to_world(*camera);

    bool shift = input->keyboard.down(KEY_LEFT_SHIFT);
    bool ctrl = input->keyboard.down(KEY_LEFT_CONTROL);
    bool mouse_btn_pres_left = input->mouse.button_pressed(MOUSE_BUTTON_LEFT);
    bool mouse_btn_down_left = input->mouse.button_down(MOUSE_BUTTON_LEFT);

    for (int i = it->count - 1; i > 0; i--)
    {
        Rectangle bonding = (Rectangle){ box[i].min.x, box[i].min.y, box[i].max.x, box[i].max.y };
        ecs_entity_t entity = it->entities[i];

        // seleccionamos una sola entidad por click
        if (mouse_btn_pres_left)
        {
            if (CheckCollisionPointRec(mouse, bonding))
            {
                gizmos[i].selected = true;
                break;
            }

            // no deseleccionamos cuando tenemos shift presionado
            if (!shift)
            {
                unselected_all(it, gizmos);
            }
        }

        // movemos la entidad seleccionadas
        if (mouse_btn_down_left)
        {
            if (gizmos[i].selected)
            {
                // !TODO: Calcular el mouse con la camara para que se mueva correctamente, cuando tenemos un zoom diferente a 1.0f
                Vector2 delta = input->mouse.get_delta();
                // Rotamos la entidad seleccionada si tenemos Ctrl presionado
                // pero si no lo tenemos presionado, movemos la entidad
                if (ctrl)
                    rot[i].x = point_to_angle(Vector2Subtract(mouse, delta), bonding) * RAD2DEG;
                else {
                    post[i].x += delta.x;
                    post[i].y += delta.y;
                }
            }
        }

        // Duplicamos la entidad seleccionadas
    }
}

static void GobuGizmos_Draw(ecs_iter_t* it)
{
    ecs_gizmos_t* gizmos = ecs_field(it, ecs_gizmos_t, 1);
    GBoundingBox* box = ecs_field(it, GBoundingBox, 2);

    for (int i = 0; i < it->count; i++)
    {
        if (!gizmos[i].selected) continue;

        Rectangle bonding = (Rectangle){ box[i].min.x, box[i].min.y, box[i].max.x, box[i].max.y };

        DrawRectangleLinesEx(bonding, 2, SKYBLUE);

        // creamos 4 rectangulos en cada esquina del bounding box para cambiar el tamaño de la entidad
        Rectangle rect1 = (Rectangle){ bonding.x - 4, bonding.y - 4, 8, 8 };
        Rectangle rect2 = (Rectangle){ bonding.x - 4, bonding.y + bonding.height - 4, 8, 8 };
        Rectangle rect3 = (Rectangle){ bonding.x + bonding.width - 4, bonding.y - 4, 8, 8 };
        Rectangle rect4 = (Rectangle){ bonding.x + bonding.width - 4, bonding.y + bonding.height - 4, 8, 8 };

        DrawRectangleRec(rect1, SKYBLUE);
        DrawRectangleRec(rect2, SKYBLUE);
        DrawRectangleRec(rect3, SKYBLUE);
        DrawRectangleRec(rect4, SKYBLUE);

        // En cada rectangulo dibujamos un cuadrado de color blanco para poder verlo
        DrawRectangle(rect1.x + 2.5, rect1.y + 2.5, 4, 4, WHITE);
        DrawRectangle(rect2.x + 2.5, rect2.y + 2.5, 4, 4, WHITE);
        DrawRectangle(rect3.x + 2.5, rect3.y + 2.5, 4, 4, WHITE);
        DrawRectangle(rect4.x + 2.5, rect4.y + 2.5, 4, 4, WHITE);

        // Creamos un círculo de línea en el centro del bounding box para rotar la entidad seleccionada
        Vector2 center = (Vector2){ bonding.x + bonding.width / 2, (bonding.y + bonding.height / 2) + (bonding.height / 2 + 20) };
        DrawCircle(center.x, center.y, 5, WHITE);
        DrawCircleLines(center.x, center.y, 5, BLACK);
    }
}
