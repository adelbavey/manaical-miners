#include <pic32mx.h>
#include <stdint.h>
#include "headers/display.h"
#include "headers/entity.h"

int run = 1;

int main(void) {
	/* Set up peripheral bus clock */
	OSCCON &= ~0x180000;
	OSCCON |= 0x080000;
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	
	/* Clear SPIROV*/
	SPI2STATCLR &= ~0x40;
	/* Set CKP = 1, MSTEN = 1; */
    SPI2CON |= 0x60;
	
	/* Turn on SPI */
	SPI2CONSET = 0x8000;

	/* Initiate buttons, switches and leds */
	initIO();

	/* Initialize display*/
	display_init();

	/* Initalize timer*/
	initTimer();

	int menu = 1;
	int play = 0;
	int endscreen = 0;

	while(run){ //Main loop
		if(menu){
			menu_render();
			delay(10000000); //Wait to register button presses
		}
		while(menu){
			if(is_any_button_down()){
				menu = 0;
				endscreen = 0;
				play = 1;
			}
		}
		if(play){
			start_waves();
			setFramesDelta(0);
		}
		while(play){
			//Gameloop
			while(getFramesDelta() > 0 && play){
				if(getPlayer().health <= 0){
					play = 0;
					menu = 0;
					endscreen = 1;
				}
				if(waveTimeFinished()){
					next_wave();
				}
				
				gamescreen_clear();
				//Update game logic
				updateEntities();

				//Update render buffer
				renderAllEntities();
				map_render();
				display_update();

				//Render game
				display_render();

				setFramesDelta(getFramesDelta() - 1);
			}
		}
		if(endscreen){
			endscreen_render();
			delay(10000000); //Wait to register button presses
		}
		while(endscreen){
			if(is_any_button_down()){
				endscreen = 0;
				play = 0;
				menu = 1;
			}
		}
	}

	return 0;
}


