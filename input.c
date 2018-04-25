#include <stdint.h>
#include <pic32mx.h>
int getswitches(){
	return (PORTD >> 8) & 0xf;
}

//Buttons 2-4
int getbuttons(){
	return ((PORTD >> 4) & 0xe);
}

//If button n is down
int is_button_down(int n){

	if(n == 1){
		return ((PORTF >> 1) & 0x1);
	}

	int btns = getbuttons();
	return (btns >> (n-1)) & 0x1;
}

//If switch is up
int is_switch_up(int swt){
	int swts = getswitches();
	return (swts >> (swt-1)) & 0x1;
}

int is_any_button_down(){
	if(((PORTF >> 1) & 0x1)){
		return 1;
	}

	int btns = getbuttons();
	if(getbuttons() != 0){
		return 1;
	}
	return 0;
}

//Convert number to binary led indexes
void set_leds(int num_leds){

	int leds = 0;
	int i;
	for (i = 0; i < num_leds; ++i)
	{
		leds |= 1 << i;
	}
	PORTE = leds;
}

//Initialize input/output ports
void initIO(){

	//Init buttons
	TRISDSET = 0x7 << 5; // btns 2-4
	TRISFSET = 2; // btn 1

	//Init switches
	TRISDSET = 0xf << 8; // swt 1-4

	//Init leds
	TRISECLR = 0xff; // leds 1-8
}