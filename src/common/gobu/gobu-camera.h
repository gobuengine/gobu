#ifndef GOBU_CAMERA_H
#define GOBU_CAMERA_H

#include "gobu-type.h"

go_public go_vec2_t go_camera_world_to_screen(go_vec2_t position, go_camera_t camera);
go_public go_vec2_t go_camera_screen_to_world(go_vec2_t position, go_camera_t camera);
go_public void go_camera_controller_update(go_camera_t *camera);

#endif // GOBU_CAMERA_H

