#include "light_bulb.h"

void send_buf(zb_uint8_t param, zb_uint8_t size, char* data, zb_uint8_t addr)
{    
    zb_buf_t *buf = ZB_BUF_FROM_REF(param);
    TRACE_MSG(TRACE_APS2, "Start alloc", (FMT__0)); 
    zb_uint8_t *ptr = NULL;
    ZB_BUF_INITIAL_ALLOC(buf, size, ptr);
    
    for (int i = 0; i < size; i++)
    {
        ptr[i] = data[i];
        TRACE_MSG(TRACE_APS2, "WRITE IN BUF %d", (FMT__D, ptr[i]));
    }


    zb_apsde_data_req_t *req = ZB_GET_BUF_TAIL(buf, sizeof(zb_apsde_data_req_t));
    req->dst_addr.addr_short = addr; // send to ZC
    req->addr_mode = ZB_APS_ADDR_MODE_16_ENDP_PRESENT;
    req->tx_options = ZB_APSDE_TX_OPT_ACK_TX;
    req->radius = 1;
    req->profileid = 2;
    req->src_endpoint = 10;
    req->dst_endpoint = 10;
    TRACE_MSG(TRACE_APS2, "Alloc worked", (FMT__0));

    ZB_SCHEDULE_CALLBACK(zb_apsde_data_request, ZB_REF_FROM_BUF(buf));
}


void toggle_light(zb_uint8_t param){
    zb_buf_t *buf= ZB_BUF_FROM_REF(param);
    non_param_cmd * params = ZB_GET_BUF_PARAM(buf, non_param_cmd);
    char data[1];
    data[0] = TOGGLE_CMD;    
    send_buf(param, 1, data, params->addr);  
}
void on_light(zb_uint8_t param){
    zb_buf_t *buf= ZB_BUF_FROM_REF(param);
    non_param_cmd *params = ZB_GET_BUF_PARAM(buf, non_param_cmd);
    char data[1];
    data[0] = ON_CMD;    
    send_buf(param, 1, data, params->addr);  
}
void off_light(zb_uint8_t param){
    zb_buf_t *buf= ZB_BUF_FROM_REF(param);
    non_param_cmd *params = ZB_GET_BUF_PARAM(buf, non_param_cmd);
    char data[1];
    data[0] = OFF_CMD;    
    send_buf(param, 1, data, params->addr);  
}

void set_colour(zb_uint8_t param){
    zb_buf_t *buf= ZB_BUF_FROM_REF(param);
    set_light_collor_struct_t *params = ZB_GET_BUF_PARAM(buf, set_light_collor_struct_t);
    char data[4];
    data[0] = SET_COLLOR_CMD;
    data[1] = params.red;
    data[2] = params.green;
    data[3] = params.blue;

    send_buf(param, 4, data, params->addr);  
}

void set_brightness(zb_uint8_t param){
    zb_buf_t *buf= ZB_BUF_FROM_REF(param);
    set_bright_struct_t *params = ZB_GET_BUF_PARAM(buf, set_bright_struct_t);
    char data[1];
    data[0] = SET_BRIGHT_CMD;  
    data[1] = params->bright  
    send_buf(param, 2, data, params->addr);  
}
