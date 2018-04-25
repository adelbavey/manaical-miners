#include <stdint.h>
#include <pic32mx.h>
#include "headers/display.h"

int dig = 0;

int isDigging(){
	return dig;
}

void setDigging(int digging){
	dig = digging;
}

enum entityType
{
	Player,
	Enemy
};

//Only created for potential use
typedef struct 
{
	int x;
	int y;
} Velocity;

#define MAX_ENTITIES 50
typedef struct entity{
	int index; //Index in entity collection
	int x;
	int y;
	int width;
	int height;
	int health;
	int attack;
	enum entityType type;
} Entity;

struct entity entityCollection[MAX_ENTITIES];

void createPlayer(int x, int y){
	Entity player = {0, x, y, 3, 3, 8, 2, Player};
	entityCollection[0] = player;
}

Entity getPlayer(){
	return entityCollection[0];
}

void createEnemy(int x, int y, int index){
	Entity enemy = {index, x, y, 2, 2, 4, 1, Enemy};
	entityCollection[index] = enemy;
}

int currentEntities = 0;

int getMaxEntities(){
	return MAX_ENTITIES;
}

void initPlayer(){
	createPlayer(5, 5);
	set_leds(getPlayer().health);
}

//numOfEnemies < MAX_ENTITIES - 1
void createEnemies(int numOfEnemies){
	currentEntities = numOfEnemies + 1;
	
	//Create enemies
	int i;
	for (i = 1; i < numOfEnemies + 1; ++i){
		createEnemy(-10, -10, i); //Spawn outside screen
	}
}

void renderEntity(Entity entity){
	if(entity.health > 0){
		gamescreen_drawbox(entity.x, entity.y, entity.width, entity.height);
	}
}

void renderAllEntities(){
	int i;
	for (i = 0; i < currentEntities; ++i){
		renderEntity(entityCollection[i]);
	}
}

void spawnAllEntities(){
	int i;
	for (i = 0; i < currentEntities; ++i){
		spawn_entity(&entityCollection[i]);
	}
}

//Get velocity towards player from enemy
Velocity enemyVelocity(Entity entity){

	int playerX = getPlayer().x;
	int playerY = getPlayer().y;

	int enemyX = entity.x;
	int enemyY = entity.y;

	Velocity v = {0,0};

	if (playerX > enemyX)
	{
		v.x = 1;
	}
	else if(playerX < enemyX){
		v.x = -1;
	}

	if (playerY > enemyY)
	{
		v.y = 1;
	}
	else if(playerY < enemyY){
		v.y = -1;
	}

	return v;

}

//Collsion that checks when the player is colliding
//with something(assumes rectangular shapes)
int collision(Entity entity1, Entity entity2){
	if(entity2.health <= 0 || entity1.health <= 0){
		return 0;
	}

	//Simplify field access for e1
	int x1 = entity1.x;
	int y1 = entity1.y;
	int w1 = entity1.width;
	int h1 = entity1.height;

	//Simplify field access for e2
	int x2 = entity2.x;
	int y2 = entity2.y;
	int w2 = entity2.width;
	int h2 = entity2.height;

	return !(y1 + h1 <= y2 ||
		y1 >= y2 + h2 ||
		x1 >= x2 + w2 ||
		x1 + w1 <= x2);
}
int collidesWithAnyEntity(Entity entity){
	if(entity.health <= 0){
		return 0;
	}
	int i;
	for(i = 0; i < currentEntities; i++){
		if(collision(entity, entityCollection[i]) && i != entity.index){
			return 1;
		}
	}
	return 0;
}

//Moves entity according to type
void update(Entity *entity){
	if((*entity).health <= 0){
		return;
	}

	//If player, move according to buttons
	if (entity -> type == Player)
	{
		int button_states = getbuttons();

		//Needs to store in case buttons changes
		int up = 0;
		int down = 0;
		int left = 0;
		int right = 0;

		if(is_switch_up(4)){
			tryDig();
		}else{
			setDigging(0);
		}

		//Update position
		if(is_button_down(1)){
			(*entity).y -= 1;
			up = 1;
		}
		if(is_button_down(2)){
			(*entity).y += 1;
			down = 1;
		}
		if(is_button_down(3)){
			(*entity).x += 1;
			left = 1;
		}
		if(is_button_down(4)){
			(*entity).x -= 1;
			right = 1;
		}

		//If player hit a wall, reverse changes or dig through
		if(collides_with_borders((*entity).x, (*entity).y, (*entity).width, (*entity).height)){
			if(up){
				(*entity).y += 1;
			}
			if(down){
				(*entity).y -= 1;
			}
			if(left){
				(*entity).x -= 1;
			}
			if(right){
				(*entity).x += 1;
			}
		}else if (collides_with_map((*entity).x, (*entity).y, (*entity).width, (*entity).height)){
			if(isDigging()){
				map_remove_box((*entity).x, (*entity).y, (*entity).width, (*entity).height);
			}else{
				if(up){
					(*entity).y += 1;
				}
				if(down){
					(*entity).y -= 1;
				}
				if(left){
					(*entity).x -= 1;
				}
				if(right){
					(*entity).x += 1;
				}
			}
		}
	}

	//If enemy, move according to AI(not built yet)
	else if (entity -> type == Enemy){
		map_remove_box((*entity).x, (*entity).y, (*entity).width, (*entity).height);
		if((randomLong(getFrames()) % 8) == 0){
			Velocity v = enemyVelocity(*entity);
			(*entity).x += v.x;
			(*entity).y += v.y;

			//Check if enemy has hit the player
			if((randomLong(getFrames()) % 16) == 0){
				if (collision(*entity, getPlayer())){
					entityCollection[0].health -= (*entity).attack;
					(*entity).health -= getPlayer().attack; //Counter attack!

					//Change leds dispay
					set_leds(getPlayer().health);
				}
			}

			//If enemy hit a wall, reverse changes
			if (collides_with_borders((*entity).x, (*entity).y, (*entity).width, (*entity).height)
				|| collidesWithAnyEntity(*entity)){
				(*entity).x -= v.x;
				(*entity).y -= v.y;
			}
		}
	}
}

//Move all Entities
void updateEntities(){
	int i;
	for (i = 0; i < currentEntities; ++i){
		update(&entityCollection[i]);
	}
}

