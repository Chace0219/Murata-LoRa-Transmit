
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef APP_CONFIGURATIONS_H
#define APP_CONFIGURATIONS_H

#ifdef __cplusplus
extern "C" {
#endif


#define CONFIG_START		122
#define CONFIG_VERSION		"pu9"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

struct StrageStruct {
	char version[4];
	// The variables of your settings
	uint16_t unit_id;
};

extern struct StrageStruct setting;

void load_config(void);
void save_config(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_RADIO_HANDLER_H */

