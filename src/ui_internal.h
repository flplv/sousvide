#ifndef SRC_UI_INTERNAL_H_
#define SRC_UI_INTERNAL_H_

#include <ui.h>

enum ui_action
{
    ui_action_left,
    ui_action_right
};

struct ui_input_action
{
    enum ui_action type;
    uint8_t intensity;
};


struct ui_processor
{
    void (*input_temperature) (int16_t);
    void (*input_action) (struct ui_input_action);
    void (*on_enter) ();
    void (*on_exit) ();
    void (*on_one_sec_ticker) ();
};

extern const struct ui_processor ui_root_processor;
extern const struct ui_processor ui_setpoint_processor;
extern const struct ui_processor ui_timer_processor;

const struct ui_processor * ui_get_current_processor ();

#endif /* SRC_UI_INTERNAL_H_ */
