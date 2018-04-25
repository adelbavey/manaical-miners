#include <pic32mx.h>
#include <stdint.h>
#include "headers/math.h"

#define DISPLAY_VDD PORTFbits.RF6
#define DISPLAY_VBATT PORTFbits.RF5
#define DISPLAY_COMMAND_DATA PORTFbits.RF4
#define DISPLAY_RESET PORTGbits.RG9


#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200

#define GAMESCREEN_WIDTH 64
#define GAMESCREEN_HEIGHT 32

char textbuffer[4][16];

static const uint8_t const font[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 94, 0, 0, 0, 0,
	0, 0, 4, 3, 4, 3, 0, 0,
	0, 36, 126, 36, 36, 126, 36, 0,
	0, 36, 74, 255, 82, 36, 0, 0,
	0, 70, 38, 16, 8, 100, 98, 0,
	0, 52, 74, 74, 52, 32, 80, 0,
	0, 0, 0, 4, 3, 0, 0, 0,
	0, 0, 0, 126, 129, 0, 0, 0,
	0, 0, 0, 129, 126, 0, 0, 0,
	0, 42, 28, 62, 28, 42, 0, 0,
	0, 8, 8, 62, 8, 8, 0, 0,
	0, 0, 0, 128, 96, 0, 0, 0,
	0, 8, 8, 8, 8, 8, 0, 0,
	0, 0, 0, 0, 96, 0, 0, 0,
	0, 64, 32, 16, 8, 4, 2, 0,
	0, 62, 65, 73, 65, 62, 0, 0,
	0, 0, 66, 127, 64, 0, 0, 0,
	0, 0, 98, 81, 73, 70, 0, 0,
	0, 0, 34, 73, 73, 54, 0, 0,
	0, 0, 14, 8, 127, 8, 0, 0,
	0, 0, 35, 69, 69, 57, 0, 0,
	0, 0, 62, 73, 73, 50, 0, 0,
	0, 0, 1, 97, 25, 7, 0, 0,
	0, 0, 54, 73, 73, 54, 0, 0,
	0, 0, 6, 9, 9, 126, 0, 0,
	0, 0, 0, 102, 0, 0, 0, 0,
	0, 0, 128, 102, 0, 0, 0, 0,
	0, 0, 8, 20, 34, 65, 0, 0,
	0, 0, 20, 20, 20, 20, 0, 0,
	0, 0, 65, 34, 20, 8, 0, 0,
	0, 2, 1, 81, 9, 6, 0, 0,
	0, 28, 34, 89, 89, 82, 12, 0,
	0, 0, 126, 9, 9, 126, 0, 0,
	0, 0, 127, 73, 73, 54, 0, 0,
	0, 0, 62, 65, 65, 34, 0, 0,
	0, 0, 127, 65, 65, 62, 0, 0,
	0, 0, 127, 73, 73, 65, 0, 0,
	0, 0, 127, 9, 9, 1, 0, 0,
	0, 0, 62, 65, 81, 50, 0, 0,
	0, 0, 127, 8, 8, 127, 0, 0,
	0, 0, 65, 127, 65, 0, 0, 0,
	0, 0, 32, 64, 64, 63, 0, 0,
	0, 0, 127, 8, 20, 99, 0, 0,
	0, 0, 127, 64, 64, 64, 0, 0,
	0, 127, 2, 4, 2, 127, 0, 0,
	0, 127, 6, 8, 48, 127, 0, 0,
	0, 0, 62, 65, 65, 62, 0, 0,
	0, 0, 127, 9, 9, 6, 0, 0,
	0, 0, 62, 65, 97, 126, 64, 0,
	0, 0, 127, 9, 9, 118, 0, 0,
	0, 0, 38, 73, 73, 50, 0, 0,
	0, 1, 1, 127, 1, 1, 0, 0,
	0, 0, 63, 64, 64, 63, 0, 0,
	0, 31, 32, 64, 32, 31, 0, 0,
	0, 63, 64, 48, 64, 63, 0, 0,
	0, 0, 119, 8, 8, 119, 0, 0,
	0, 3, 4, 120, 4, 3, 0, 0,
	0, 0, 113, 73, 73, 71, 0, 0,
	0, 0, 127, 65, 65, 0, 0, 0,
	0, 2, 4, 8, 16, 32, 64, 0,
	0, 0, 0, 65, 65, 127, 0, 0,
	0, 4, 2, 1, 2, 4, 0, 0,
	0, 64, 64, 64, 64, 64, 64, 0,
	0, 0, 1, 2, 4, 0, 0, 0,
	0, 0, 48, 72, 40, 120, 0, 0,
	0, 0, 127, 72, 72, 48, 0, 0,
	0, 0, 48, 72, 72, 0, 0, 0,
	0, 0, 48, 72, 72, 127, 0, 0,
	0, 0, 48, 88, 88, 16, 0, 0,
	0, 0, 126, 9, 1, 2, 0, 0,
	0, 0, 80, 152, 152, 112, 0, 0,
	0, 0, 127, 8, 8, 112, 0, 0,
	0, 0, 0, 122, 0, 0, 0, 0,
	0, 0, 64, 128, 128, 122, 0, 0,
	0, 0, 127, 16, 40, 72, 0, 0,
	0, 0, 0, 127, 0, 0, 0, 0,
	0, 120, 8, 16, 8, 112, 0, 0,
	0, 0, 120, 8, 8, 112, 0, 0,
	0, 0, 48, 72, 72, 48, 0, 0,
	0, 0, 248, 40, 40, 16, 0, 0,
	0, 0, 16, 40, 40, 248, 0, 0,
	0, 0, 112, 8, 8, 16, 0, 0,
	0, 0, 72, 84, 84, 36, 0, 0,
	0, 0, 8, 60, 72, 32, 0, 0,
	0, 0, 56, 64, 32, 120, 0, 0,
	0, 0, 56, 64, 56, 0, 0, 0,
	0, 56, 64, 32, 64, 56, 0, 0,
	0, 0, 72, 48, 48, 72, 0, 0,
	0, 0, 24, 160, 160, 120, 0, 0,
	0, 0, 100, 84, 84, 76, 0, 0,
	0, 0, 8, 28, 34, 65, 0, 0,
	0, 0, 0, 126, 0, 0, 0, 0,
	0, 0, 65, 34, 28, 8, 0, 0,
	0, 0, 4, 2, 4, 2, 0, 0,
	0, 120, 68, 66, 68, 120, 0, 0,
};

uint8_t gamescreen[] = { //(8x32) = img width = 64, height = 32
	255, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 255,

	255, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 255,

	255, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 255,

	255, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 255 //Lower right square (8x8)
	//Columns ->

	//Every int corresponds to 8 pixels (2^8 = 256)
	//0 means no pixels are lit
};

void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 0x01));
	return SPI2BUF;
}

void display_init() {
	DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
	delay(10);
	DISPLAY_VDD_PORT &= ~DISPLAY_VDD_MASK;
	delay(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_RESET_PORT &= ~DISPLAY_RESET_MASK;
	delay(10);
	DISPLAY_RESET_PORT |= DISPLAY_RESET_MASK;
	delay(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_VBATT_PORT &= ~DISPLAY_VBATT_MASK;
	delay(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}

void display_string(int line, char *s) {
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;
	
	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}

void display_image(int x, const uint8_t *data) {
	int i, j;
	
	for(i = 0; i < 4; i++) { //i = row (4 rows in total)
		//Clear commands
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22); //Set page number command
		spi_send_recv(i); //Page number
		
		spi_send_recv(x & 0xF); //Set low nybble of column (Determines X start on screen)
		spi_send_recv(0x10 | ((x >> 4) & 0xF)); //Set high nybble of column
		
		//Display command
		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
		
		for(j = 0; j < GAMESCREEN_WIDTH; j++) // Pixels drawn on row
			spi_send_recv(data[i * GAMESCREEN_WIDTH + j]);
	}
}

//Make sure you can't affect borders
//y = 1-31
//x = 1-63
void gamescreen_setpixel(int x, int y, int lit){
	if(x >= GAMESCREEN_WIDTH - 1 || x < 1 || y >= GAMESCREEN_HEIGHT - 1 || y < 1){
		return;
	}
	int rowIndex = floor(y/8);
	int numberSetIndex = rowIndex * GAMESCREEN_WIDTH + x;
	int numberSet = gamescreen[numberSetIndex]; //The column of pixels to modify
	int drawPixelIndex = 8 - ((rowIndex + 1) * 8 - y);
	int hexPixel = pow(2, drawPixelIndex); //Convert index to bit mask
	if(lit){
		numberSet |= hexPixel;
	}else{
		numberSet &= ~hexPixel;
	}
	gamescreen[numberSetIndex] = numberSet;
}

void gamescreen_drawpixel(int x, int y){
	gamescreen_setpixel(x, y, 1);
}
void gamescreen_clearpixel(int x, int y){
	gamescreen_setpixel(x, y, 0);
}

//x and y is the origin of the box (top left corner)
void gamescreen_drawbox(int x, int y, int width, int height){
	int i;
	int j;
	for(i = x; i < x + width; i++){
		for(j = y; j < y + height; j++){
			gamescreen_drawpixel(i, j);
		}
	}
}

void gamescreen_clearbox(int x, int y, int width, int height){
	int i;
	int j;
	for(i = x; i < x + width; i++){
		for(j = y; j < y + height; j++){
			gamescreen_clearpixel(i, j);
		}
	}
}

void gamescreen_clear(){
	gamescreen_clearbox(1, 1, 62, 30);
}

void display_update(){
	char wavebuf[32], *w;
	char timebuf[32], *t;

	//Place 9 spaces before string
	int i;
	for(i = 0; i < 9; i++){
		wavebuf[i] = ' ';
		timebuf[i] = ' ';
	}

	//Place 1 # before wave number
	wavebuf[9] = '#';
	wavebuf[10] = '0';
	wavebuf[11] = '0';
	

	w = wavebuf + 10 + 2; //Jump to where digits end
	int waves = getCurrentWave();
	while (waves > 0) { //Break into digits
 		int digit = waves % 10;
 		*--w = '0' + digit; //Convert digit to ascii
 		waves /= 10;
	}
	wavebuf[12] = 0; //Null terminator

	timebuf[9] = '0';
	timebuf[10] = '0';
	timebuf[11] = ':';
	timebuf[12] = '0';
	timebuf[13] = '0';

	t = timebuf + 9 + 2; //Jump to where minutes end
	int minutes = getMinutesToDisplay();
	while (minutes > 0) { //Break into digits
 		int digit = minutes % 10;
 		*--t = '0' + digit; //Convert digit to ascii
 		minutes /= 10;
	}

	t = timebuf + 9 + 2 + 1 + 2; //Jump to where seconds end
	int seconds = getSecondsToDisplay();
	while (seconds > 0) { //Break into digits
 		int digit = seconds % 10;
 		*--t = '0' + digit; //Convert digit to ascii
 		seconds /= 10;
	}
	timebuf[14] = 0; //Null terminator

	display_string(0, "         Wave:");
	display_string(1, wavebuf);
	display_string(2, "         Time:");
	display_string(3, timebuf);
}

void render_text(){
	int i, j, k;
	int c;
	//Render strings
	for(i = 0; i < 4; i++) {
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

void display_wave_screen(int waveNr){
	char wavebuf[32], *w;
	w = wavebuf + 10 + 2; //End of before string + digits
	wavebuf[0] = ' ';
	wavebuf[1] = ' ';
	wavebuf[2] = ' ';
	wavebuf[3] = ' ';
	wavebuf[4] = 'W';
	wavebuf[5] = 'a';
	wavebuf[6] = 'v';
	wavebuf[7] = 'e';
	wavebuf[8] = ' ';
	wavebuf[9] = '#';
	wavebuf[10] = '0';
	wavebuf[11] = '0';
	int wave = waveNr;
	while (wave > 0) { //Break into digits
 		int digit = wave % 10;
 		*--w = '0' + digit; //Convert digit to ascii
 		wave /= 10;
	}
	wavebuf[12] = 0; //Null terminator
	display_string(0, " ");
	display_string(1, wavebuf);
	display_string(2, " ");
	display_string(3, " ");
	render_text();
}

void menu_render(){
	display_string(0, "Maniacal Miners!");
	display_string(1, "By: Felix & Adel");
	display_string(2, " ");
	display_string(3, "Press any button");
	render_text();
}

void endscreen_render() {
	char scorebuf[32], *s;
	char highbuf[32], *h;
	s = scorebuf + 13 + 2; //End of before string + digits
	scorebuf[0] = 'S';
	scorebuf[1] = 'c';
	scorebuf[2] = 'o';
	scorebuf[3] = 'r';
	scorebuf[4] = 'e';
	scorebuf[5] = ':';
	scorebuf[6] = ' ';
	scorebuf[7] = 'W';
	scorebuf[8] = 'a';
	scorebuf[9] = 'v';
	scorebuf[10] = 'e';
	scorebuf[11] = ' ';
	scorebuf[12] = '#';
	scorebuf[13] = '0';
	scorebuf[14] = '0';
	int score = getCurrentWave();
	while (score > 0) { //Break into digits
 		int digit = score % 10;
 		*--s = '0' + digit; //Convert digit to ascii
 		score /= 10;
	}
	scorebuf[15] = 0; //Null terminator

	h = highbuf + 12 + 2; //End of before string + digits
	highbuf[0] = 'B';
	highbuf[1] = 'e';
	highbuf[2] = 's';
	highbuf[3] = 't';
	highbuf[4] = ':';
	highbuf[5] = ' ';
	highbuf[6] = 'W';
	highbuf[7] = 'a';
	highbuf[8] = 'v';
	highbuf[9] = 'e';
	highbuf[10] = ' ';
	highbuf[11] = '#';
	highbuf[12] = '0';
	highbuf[13] = '0';
	int best = getHighscore();
	while (best > 0) { //Break into digits
 		int digit = best % 10;
 		*--h = '0' + digit; //Convert digit to ascii
 		best /= 10;
	}
	highbuf[14] = 0; //Null terminator

	display_string(0, "Oh, you died...");
	display_string(1, scorebuf);
	display_string(2, highbuf);
	display_string(3, "Press any button");
	render_text();
}

void display_render() {
	render_text();
	//Render gamescreen
	display_image(0, gamescreen);
}