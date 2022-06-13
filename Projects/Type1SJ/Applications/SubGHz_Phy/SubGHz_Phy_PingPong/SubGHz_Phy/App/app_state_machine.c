/**
 *
 */

#include "app_state_machine.h"
#include "stm32l0xx_hal.h"

// Used to represent dummy state
const uint8_t DUMMY_STATE_INDEX = 250;

static uint32_t state_transition_time = 0;

struct application_state *head = NULL;
struct application_state *current_state = NULL;

/**
 *
 */
void init_new_state(uint8_t index, void (*on_enter_proc)(void), void (*loop_proc)(void), void (*on_exit_proc)(void))
{
	// create a link
	struct application_state *link = (struct application_state*) malloc(sizeof(struct application_state));

	link->state_index = index;
	link->on_enter_proc = on_enter_proc;
	link->loop_proc = loop_proc;
	link->on_exit_proc = on_exit_proc;

	//point it to old first node
	link->next = head;

	//point first to new first node
	head = link;
}

static struct application_state* find(uint8_t state_index)
{

	//start from the first link
	struct application_state* current = head;

	//if list is empty
	if(head == NULL) {
		return NULL;
	}

	//navigate through list
	while(current->state_index != state_index) {
		//if it is last node
		if(current->next == NULL) {
			 return NULL;
		} else {
		 //go to next link
			current = current->next;
		}
	}

	//if data found, return the current Link
	return current;
}

/**
 * */
bool application_in_state(uint8_t state_index){
	if(current_state == NULL)
		return false;
    return current_state->state_index == state_index;
}

/**
 * */
void application_transition_state(uint8_t new_state_index)
{
	if(current_state != NULL){
		printf("Exit from state [%d]", current_state->state_index);
		if(current_state->on_exit_proc != NULL)
			(void)current_state->on_exit_proc();
	}

	// Find next state
	current_state = find(new_state_index);

	if(current_state != NULL){
		printf("Enter to state [%d]", current_state->state_index);
		if(current_state->on_enter_proc != NULL)
			(void)current_state->on_enter_proc();
	}
	state_transition_time = HAL_GetTick();
}

/**
 * */
uint32_t time_in_current_state(){
    return HAL_GetTick() - state_transition_time;
}


void app_state_machine_proc(){
	if(current_state != NULL){
		if(current_state->loop_proc != NULL)
			(void)current_state->loop_proc();
	}
}

