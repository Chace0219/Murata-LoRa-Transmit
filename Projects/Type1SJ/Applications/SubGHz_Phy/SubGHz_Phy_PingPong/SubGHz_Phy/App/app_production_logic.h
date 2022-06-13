
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_PRODUCTION_LOGIC_H
#define APP_PRODUCTION_LOGIC_H

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    SYSTEM_BOOTING_STATE,
    SYSTEM_NORMAL_STATE,
    SYSTEM_IDLE_STATE,
    SYSTEM_SLEEP_STATE,
};

void app_production_logic_init(void);
void app_production_logic_process();

void water_detection_interrupt_callback();
void button_clicked_interrupt_callback();

#ifdef __cplusplus
}
#endif

#endif /* APP_PRODUCTION_LOGIC_H */

