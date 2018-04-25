#include <stdint.h>
#include <pic32mx.h>
#include "headers/display.h"
#include "headers/entity.h"

//Kopierad från labb 3

#define FPS 60
#define DIG_DURATION 1
#define DIG_COOLDOWN 4

int framesDelta = 0;
long frames = 0;
int digTimerFrames = 0;
int digCdFrames = 0;

int currentWaveTime = 0; //In seconds

int setCurrentWaveTime(int seconds){
  currentWaveTime = seconds;
}

long getFrames(){
  return frames;
}

int getFramesDelta(){
  return framesDelta;
}

void setFramesDelta(int frames){
  framesDelta = frames;
}

void resetDigCd(){
  digCdFrames = DIG_COOLDOWN * FPS;
}

void updateDigging(){
  if(isDigging()){
    digTimerFrames++;
  }
  if(digTimerFrames/FPS > DIG_DURATION){
    setDigging(0);
    digTimerFrames = 0;
  }
  if(!isDigging()){
    digCdFrames++;
  }
}

void tryDig(){
  if(digCdFrames/FPS >= DIG_COOLDOWN){
    setDigging(1);
    digCdFrames = 0;
  }
}

/* Interrupt Service Routine */
void user_isr( void )
{
	//Timer flag
	if (1)
	{
    framesDelta++;
    frames++;
    updateDigging();
	}
  
  //Reset
  IFS(0) = 0;
}

long getSecondsElapsed(){
  return frames/FPS;
}
void resetSecondsElapsed(){
  frames = 0;
}

int getMinutesToDisplay(){
  return floorInt((currentWaveTime - getSecondsElapsed())/60.0);
}

int getSecondsToDisplay(){
  int secsLeft = currentWaveTime - getSecondsElapsed();
  return (secsLeft % 60);
}

int waveTimeFinished(){
  if(currentWaveTime < getSecondsElapsed()){
    return 1;
  }
  return 0;
}

volatile int* iec0 = (volatile int*) 0xbf881060;
volatile int* ipc2 = (volatile int*) 0xbf8810b0;

void initTimer(){

  /* Initiate gameloop interrups */
  T2CON = 0x70; //Prescale 1:256
  PR2 = (80000000/256)/FPS; // Set period to 17ms (60 times per second)
  T2CONSET = 0x8000; // Set timer2 ON

  enable_interrupt();
  *ipc2 = 4;
  *iec0 |= 0x100;
}