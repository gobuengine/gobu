#ifndef GB_TYPE_TEXTURE_H
#define GB_TYPE_TEXTURE_H

#include <stdio.h>
#include <stdint.h>
#include "thirdparty/flecs/flecs.h"
#include "gb_gfx.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef rf_texture2d gb_texture_t;

    // global component
    extern ECS_COMPONENT_DECLARE(gb_texture_t);

    // module import
    void gb_type_texture_moduleImport(ecs_world_t* world);

#ifdef __cplusplus
}
#endif

#endif // GB_TYPE_TEXTURE_H
