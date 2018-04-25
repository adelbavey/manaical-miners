#ifndef DISPLAY_H_   /* Include guard */
#define DISPLAY_H_

void delay(int cyc);
uint8_t spi_send_recv(uint8_t data);
void display_init();
void display_string(int line, char *s);
void display_image(int x, const uint8_t *data);
void gamescreen_drawpixel(int x, int y);
void gamescreen_clearpixel(int x, int y);
void gamescreen_drawbox(int x, int y, int width, int height);
void gamescreen_clearbox(int x, int y, int width, int height);
void gamescreen_clear();
void display_update();
void display_render();

#endif