#ifndef LIGHTBULB_H
#define LIGHTBULB_H

#include "zb_common.h"
#include "zb_scheduler.h"
#include "zb_bufpool.h"
#include "zb_nwk.h"
#include "zb_aps.h"
#include "zb_zdo.h"
#include "zb_secur.h"
#include "zb_secur_api.h"

enum light_bulb_cmd_e {TOGGLE_CMD, ON_CMD, OFF_CMD, SET_COLLOR_CMD, SET_BRIGHT_CMD};

typedef struct non_param_cmd{
    zb_uint8_t addr;
}non_param_cmd_t;

typedef struct set_light_collor_struct{
    zb_uint8_t addr;
    zb_uint8_t red;
    zb_uint8_t green;
    zb_uint8_t blue;
}set_light_collor_struct_t;

typedef struct set_bright_struct{
    zb_uint8_t addr;
    zb_uint8_t bright;
}set_bright_struct_t;
    
void send_buf(zb_uint8_t param, zb_uint8_t size, char* data, zb_uint8_t addr);

void toggle_light(zb_uint8_t param);
void on_light(zb_uint8_t param);
void off_light(zb_uint8_t param);
void set_colour(zb_uint8_t param);
void set_brightness(zb_uint8_t param);

#endif