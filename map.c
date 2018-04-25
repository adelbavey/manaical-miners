#include <pic32mx.h>
#include <stdint.h>
#include "headers/display.h"
#include "headers/entity.h"

#define MAP_WIDTH 62
#define MAP_HEIGHT 30

// 0 is displayed on screen 1. 62 on screen 63
int map[MAP_WIDTH][MAP_HEIGHT];

//Render the map on the gamescreen
void map_render(){
	int x;
	int y;
	for(x = 0; x < MAP_WIDTH; x++){
		for(y = 0; y < MAP_HEIGHT; y++){
			//+1 to align 0 map coord outside border (border is on 0).
			if(map[x][y]){
				gamescreen_drawpixel(x + 1, y + 1); 
			}
		}
	}
}

void map_remove_box(int x, int y, int width, int height){
	int i;
	int j;
	for(i = x; i < x + width; i++){
		for(j = y; j < y + height; j++){
			map[i - 1][j - 1] = 0;
		}
	}
}

//Randomly generate a new map
void map_generate(){
	long seed = randomLong(getSecondsElapsed());
	int x;
	int y;
	for(x = 0; x < MAP_WIDTH; x++){
		for(y = 0; y < MAP_HEIGHT; y++){
			//Get the perlin value for each coordinate
			float value = perlin_noise((int)seed + x/6, (int)seed + y/6); //Divide by with of blocks
			if(value > 0){ //Treat high perlin as placing cells
				map[x][y] = 1;
			}else{
				map[x][y] = 0;
			}
		}
	}
}

void spawn_entity(Entity *entity){
	if((*entity).health <= 0){
		return;
	}

	int x;
	int y;
	for(x = 0; x < MAP_WIDTH; x++){
		for(y = 0; y < MAP_HEIGHT; y++){
			//If spot is empty
			if(!collides_with_map(x,y,(*entity).width, (*entity).height) &&
				!collides_with_borders(x,y,(*entity).width, (*entity).height)){
				if(entity -> type == Enemy){
					//Check distance to player
					if(abs(x - getPlayer().x) + abs(y - getPlayer().y) > 15){
						int lastX = (*entity).x;
						int lastY = (*entity).y;
						(*entity).x = x;
						(*entity).y = y;
						if(collidesWithAnyEntity(*entity)){
							(*entity).x = lastX;
							(*entity).y = lastY;
						}else{
							return;
						}
					}
				}else{
					(*entity).x = x;
					(*entity).y = y;
					return;
				}
			}
		}
	}
}

int collides_with_borders(int x, int y, int width, int height){
	if(x == 0 || x == 64 - width || y == 0 || y == 32 - height){
		return 1; //Collides with border
	}
	return 0;
}

int collides_with_map(int x, int y, int width, int height){
	x = x - 1; //Idk why this is
	int i;
	int j;
	for(i = x; i < x + width; i++){
		for(j = y; j < y + height; j++){
			if(map[i][j - 1]){
				return 1;
			}
		}
	}
	return 0;
}



