#include "gobu-camera.h"
#include "gobu-math.h"
#include "gobu-input.h"

go_public go_vec2_t go_camera_world_to_screen(go_vec2_t position, go_camera_t camera)
{
    return (go_vec2_t){0, 0};
}

go_public go_vec2_t go_camera_screen_to_world(go_vec2_t position, go_camera_t camera)
{
    return (go_vec2_t){0, 0};
}

go_public void go_camera_controller_update(go_camera_t *camera)
{
    if (go_mouse_is_down(GO_MOUSEBUTTON_MIDDLE))
    {
        go_vec2_t delta = go_mouse_delta();
        delta = go_math_vec2_scale(delta, -1.0f / camera->zoom);
        camera->target = go_math_vec2_add(camera->target, delta);
    }

    float wheel = go_mouse_wheel() * -1;
    if (wheel != 0)
    {
        float scaleFactor = 1.0f + (0.02f * fabsf(wheel));
        if (wheel < 0)
            scaleFactor = 1.0f / scaleFactor;
        camera->zoom = go_math_clamp(camera->zoom * scaleFactor, 0.60, 64.0f);
    }
}

