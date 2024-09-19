
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../inc/ST7735.h"
#include "../inc/PLL.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Timer0A.h"
#include "Lab3.h"
//struct headers
////////////////
struct Clock;
struct Time;


//function headers
//////////////////
void init_clock(struct Clock c);
void init_time(struct Time t);


//Constants
///////////
const size_t STRING_BUFSIZE = 10;
const int8_t MODIFY_OPTIONS = 9;



//Struct definitions
////////////////////
struct Time{
	int8_t hours;
	int8_t minutes;
	int8_t seconds;
	char string[STRING_BUFSIZE];
};

struct Clock{
	//The current time on the clock
	struct Time current_time;
	
	//The set time for the alarm
	struct Time alarm_time;
	
	//A boolean to determine whether we should display as military time
	int8_t is_military;
	
	//The index of the alarm sound we will be using
	int8_t song;
	
	//choose what element you are modifying (if any)
	int8_t modify_select;
};


//Function Definitions
//////////////////////
void init_clock(struct Clock c){
	init_time(c.current_time);
	init_time(c.alarm_time);
	c.is_military = 0;
	c.song = 0;
	c.modify_select = 0;
}

//can be used on any Time structure
void init_time(struct Time t){
	t.hours = 0;
	t.minutes = 0;
	t.seconds = 0;
	strncpy(t.string, "00:00:00", STRING_BUFSIZE);
}


// 0 - nothing
// 1 - clock hours
// 2 - clock minutes
// 3 - clock seconds
// 4 - alarm hours
// 5 - alarm minutes
// 6 - alarm seconds
// 7 - military vs non-military
// 8 - song select
void change_modify_select(struct Clock c, int8_t increment){
	c.modify_select += increment;
	if(c.modify_select >= MODIFY_OPTIONS){
		c.modify_select = 0;
	}
	if(c.modify_select < 0){
		c.modify_select = MODIFY_OPTIONS - 1;
	}
}

// num_songs based on the idea that there will be 3 options for the alarm, will change as needed.
// increment should be +/- 1
void change_song(struct Clock c, int8_t increment){
	int8_t num_songs = 3;
	c.song += increment;
	if(c.song >= num_songs){
		c.song = 0;
	}
	
	if(c.song < 0){
		c.song = num_songs - 1;
	}
}

//Simple toggle
void change_military_time(struct Clock c){
	c.is_military = !c.is_military;
}

//Edge case is when adding more than one to any number it may discard extras.
//Each call should have only 1 of the inputs non zero. The values should be +/- 1
void change_time(struct Time t, int8_t hours, int8_t minutes, int8_t seconds, int8_t is_military){
	t.hours += hours;
	t.minutes += minutes;
	t.seconds += seconds;
	
	//check for overflow
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
	
	
	//check for underflow
	if(t.seconds < 0){
		t.minutes -= 1;
		t.seconds = 59;
	}
	
	if(t.minutes < 0){
		t.hours -= 1;
		t.minutes = 59;
	}
	
	if(t.hours < 0){
		t.hours = 23;
	}
	
	//update the string with the new times
	//military time
	if(is_military){
		char h_tens = (t.hours   / 10) + '0';
		char h_ones = (t.hours   % 10) + '0';
		char m_tens = (t.minutes / 10) + '0';
		char m_ones = (t.minutes % 10) + '0';
		char s_tens = (t.seconds / 10) + '0';
		char s_ones = (t.seconds % 10) + '0';
		
		snprintf(t.string, STRING_BUFSIZE, "%c%c:%c%c:%c%c", h_tens, h_ones, m_tens, m_ones, s_tens, s_ones);
	} 
	
	//non military time
	else {
		int8_t adjusted_hours = t.hours;
		int8_t is_pm = 0;
		
		//if its after 1pm adjust the time
		if(t.hours > 12){
			is_pm = 1;
			adjusted_hours -= 12;
		}
		
		//If its between 12-1 then its in the PM but dont adjust the hours
		if(t.hours == 12){
			is_pm = 1;
		}
		
		char h_tens = (adjusted_hours   / 10) + '0';
		char h_ones = (adjusted_hours   % 10) + '0';
		char m_tens = (t.minutes        / 10) + '0';
		char m_ones = (t.minutes        % 10) + '0';
		char s_tens = (t.seconds        / 10) + '0';
		char s_ones = (t.seconds        % 10) + '0';
		
		if(is_pm){
			snprintf(t.string, STRING_BUFSIZE, "%c%c:%c%c:%c%c PM", h_tens, h_ones, m_tens, m_ones, s_tens, s_ones);
		} else {
			snprintf(t.string, STRING_BUFSIZE, "%c%c:%c%c:%c%c AM", h_tens, h_ones, m_tens, m_ones, s_tens, s_ones);
		}
		
	}
}
