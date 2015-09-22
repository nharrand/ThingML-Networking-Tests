/*****************************************************
 *      THIS IS A GENERATED FILE. DO NOT EDIT.
 *        Implementation for Thing antisocial
 *  Generated from ThingML (http://www.thingml.org)
 *****************************************************/

#include "antisocial.h"

/*****************************************************************************
 * Implementation for type : antisocial
 *****************************************************************************/

// Declaration of prototypes:
void antisocial_Chart_OnExit(int state, struct antisocial_Instance *_instance);
void antisocial_send_pout_m(struct antisocial_Instance *_instance);
void antisocial_send_pout_m2(struct antisocial_Instance *_instance);
// Declaration of functions:

// On Entry Actions:
void antisocial_Chart_OnEntry(int state, struct antisocial_Instance *_instance) {
switch(state) {
case ANTISOCIAL_CHART_STATE:
_instance->antisocial_Chart_State = ANTISOCIAL_CHART_INIT_STATE;
antisocial_Chart_OnEntry(_instance->antisocial_Chart_State, _instance);
break;
case ANTISOCIAL_CHART_INIT_STATE:
fprintf(stdout, "[antisocial] Init\n");
antisocial_send_pout_m(_instance);
fprintf(stdout, "[antisocial] sent m\n");
antisocial_send_pout_m2(_instance);
fprintf(stdout, "[antisocial] sent m2\n");
break;
default: break;
}
}

// On Exit Actions:
void antisocial_Chart_OnExit(int state, struct antisocial_Instance *_instance) {
switch(state) {
case ANTISOCIAL_CHART_STATE:
antisocial_Chart_OnExit(_instance->antisocial_Chart_State, _instance);
break;
case ANTISOCIAL_CHART_INIT_STATE:
break;
default: break;
}
}

// Event Handlers for incoming messages:
void antisocial_handle_pin_m(struct antisocial_Instance *_instance) {
uint8_t antisocial_Chart_State_event_consumed = 0;
if (_instance->antisocial_Chart_State == ANTISOCIAL_CHART_INIT_STATE) {
if (antisocial_Chart_State_event_consumed == 0 && 1) {
fprintf(stdout, "[antisocial] received m\n");
antisocial_Chart_State_event_consumed = 1;
}
}
}
void antisocial_handle_pin_m3(struct antisocial_Instance *_instance) {
uint8_t antisocial_Chart_State_event_consumed = 0;
if (_instance->antisocial_Chart_State == ANTISOCIAL_CHART_INIT_STATE) {
if (antisocial_Chart_State_event_consumed == 0 && 1) {
fprintf(stdout, "[antisocial] received m3\n");
antisocial_Chart_State_event_consumed = 1;
}
}
}

// Observers for outgoing messages:
void (*external_antisocial_send_pout_m_listener)(struct antisocial_Instance *)= 0x0;
void register_external_antisocial_send_pout_m_listener(void (*_listener)(struct antisocial_Instance *)){
external_antisocial_send_pout_m_listener = _listener;
}
void (*antisocial_send_pout_m_listener)(struct antisocial_Instance *)= 0x0;
void register_antisocial_send_pout_m_listener(void (*_listener)(struct antisocial_Instance *)){
antisocial_send_pout_m_listener = _listener;
}
void antisocial_send_pout_m(struct antisocial_Instance *_instance){
if (antisocial_send_pout_m_listener != 0x0) antisocial_send_pout_m_listener(_instance);
if (external_antisocial_send_pout_m_listener != 0x0) external_antisocial_send_pout_m_listener(_instance);
;
}
void (*external_antisocial_send_pout_m2_listener)(struct antisocial_Instance *)= 0x0;
void register_external_antisocial_send_pout_m2_listener(void (*_listener)(struct antisocial_Instance *)){
external_antisocial_send_pout_m2_listener = _listener;
}
void (*antisocial_send_pout_m2_listener)(struct antisocial_Instance *)= 0x0;
void register_antisocial_send_pout_m2_listener(void (*_listener)(struct antisocial_Instance *)){
antisocial_send_pout_m2_listener = _listener;
}
void antisocial_send_pout_m2(struct antisocial_Instance *_instance){
if (antisocial_send_pout_m2_listener != 0x0) antisocial_send_pout_m2_listener(_instance);
if (external_antisocial_send_pout_m2_listener != 0x0) external_antisocial_send_pout_m2_listener(_instance);
;
}



