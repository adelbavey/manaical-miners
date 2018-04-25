

#ifndef ENTITY_H_   /* Include guard */
#define ENTITY_H_

#define MAX_ENTITIES 20
enum entityType
{
	Player,
	Enemy
};

//Only created for potential use
typedef struct 
{
	int vx;
	int vy;
} Velocity;

typedef struct entity{
	int index;
	int x;
	int y;
	int width;
	int height;
	int health;
	int attack;
	enum entityType type;
} Entity;

struct entity entityCollection[MAX_ENTITIES];

int getMaxEntities();
void initPlayer();
Entity getPlayer();
void createPlayer(int x, int y);
void createEnemy(int x, int y);
void createEntityCollection(int numOfEnemies);
void renderEntity(Entity entity);
void renderAllEntities();
void update(Entity *entity);
int collision(Entity entity1, Entity entity2);
void updateEntities();
int collidesWithAnyEntity(Entity entity);

#endif