/**
 *
 */
#include "stm32l0xx_hal.h"
#include "platform.h"
#include "app_configurations.h"

struct StrageStruct setting = {
	CONFIG_VERSION,
	// The default values
	0x1234
};

/** */
void load_config(void) {
	setting.unit_id = 0x1234;
	strcpy(setting.version, CONFIG_VERSION);
}

void save_config(void) {

}
