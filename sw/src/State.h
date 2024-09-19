
#include <stdio.h>
#include <stdint.h>
#include "../inc/ST7735.h"
#include "../inc/PLL.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer0A.h"
#include "Lab3.h"

struct Time;

struct Time{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	char string[8];
};

struct Clock{
	//The current time on the clock
	struct Time current_time;
	
	//The set time for the alarm
	struct Time alarm_time;
	
	//A boolean to determine whether we should display as military time
	uint8_t is_military;
	
	//The index of the alarm sound we will be using
	uint8_t song;
};

//Edge case is when adding more than one to any number it may discard extras.
//****IMPLEMENT DECREMENTS****//
void add_time(struct Time t, uint8_t hours, uint8_t minutes, uint8_t seconds){
	t.hours += hours;
	t.minutes += minutes;
	t.seconds += seconds;
	
	if(t.seconds >= 60){
		t.minutes += 1;
		t.seconds = 0;
	}
	
	if(t.minutes >= 60){
		t.hours += 1;
		t.minutes = 0;
	}
	
	if(t.hours >= 24){
		t.hours = 0;
	}
	

}
//victor smells like old spice
void init_time(struct Time t, uint8_t is_military){
	t.hours = 0;
	t.minutes = 0;
	t.seconds = 0;
	//t.string = ['0','0',':','0','0',':','0','0'];
}

// 2 based on the idea that there will be 3 options for the alarm, will change as needed.
void cycle_song(struct Clock c){
	c.song += 1;
	if(c.song > 2){
		c.song = 0;
	}
}

void toggle_military_time(struct Clock c){
	c.is_military = !c.is_military;
}

char* time_to_string(struct Time t, uint8_t is_military){
	
}