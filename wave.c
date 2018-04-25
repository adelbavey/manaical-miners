#include <pic32mx.h>
#include <stdint.h>
#include "headers/display.h"
#include "headers/entity.h"

int currentWave = 0;
int highscore = 0;

int getCurrentWave(){
	return currentWave;
}
int getHighscore(){
	return highscore;
}

void next_wave(){
	currentWave++;
	if(currentWave > 99){ //Max
		currentWave = 99;
	}
	int numEnemies = currentWave;
	if(numEnemies > getMaxEntities() - 1){
		numEnemies = getMaxEntities() - 1;
	}
	createEnemies(numEnemies);

	display_wave_screen(currentWave);
	if(currentWave > highscore){
		highscore = currentWave;
	}
	map_generate();
	spawnAllEntities();
	int wavetime = 10 + currentWave;
	if(wavetime > 3600){ //Max
		wavetime = 3600;
	}
	setCurrentWaveTime(wavetime);
	resetDigCd();
	resetSecondsElapsed();
}

void start_waves(){
	currentWave = 0;
	initPlayer();
	next_wave();
}