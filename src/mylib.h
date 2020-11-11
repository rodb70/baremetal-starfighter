#ifndef MY_LIB_H
#define MY_LIB_H
#include <stdbool.h>
#include <stdint.h>

/*typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
*/
typedef volatile unsigned char vuint8_t;
typedef volatile unsigned short vuint16_t;
typedef volatile unsigned int vuint32_t;
/*
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
*/
typedef volatile signed char vint8_t;
typedef volatile signed short vint16_t;
typedef volatile signed int vint32_t;

extern unsigned short *videoBuffer;

#define OFFSET(r,c,rowlen) ((r)*(rowlen) + (c))

#define RGB(r, g, b) ((r) | ((g) << 5) | ((b) << 10))
#define WIDTH 240
#define HEIGHT 160

/* Buttons */

#define BUTTON_A      (1<<0)
#define BUTTON_B      (1<<1)
#define BUTTON_SELECT (1<<2)
#define BUTTON_START  (1<<3)
#define BUTTON_RIGHT  (1<<4)
#define BUTTON_LEFT   (1<<5)
#define BUTTON_UP     (1<<6)
#define BUTTON_DOWN   (1<<7)
#define BUTTON_R      (1<<8)
#define BUTTON_L      (1<<9)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)
#define KEY_UP_NOW(key)  ((BUTTONS) & key)

//#define false 0
//#define true 1
#define UBound 8

typedef enum
{
    Start, Game, GameOver, NextLevel, Win
} GameState;
typedef enum
{
    Health, Ammo, Shotgun, Shield
} PickupType;

typedef struct Entity
{
    int8_t canExit;
    short x;
    short y;
    short width;
    short height;
    short xVel;
    short yVel;
    bool isVisible;
    bool removed;
    const uint16_t *image;
} uEntity;

typedef struct Ship
{
    short health;
    char ai;
    char reload;
    uEntity entity;
} uShip;

typedef struct Projectile
{
    int damage;
    char team;
    uEntity entity;
} uProjectile;

typedef struct Pickup
{
    PickupType type;
    uEntity entity;
} uPickup;

void drawScreen(const unsigned short *image);
void drawImage3(int x, int y, int width, int height, const unsigned short *image);
void drawRectangle(int x, int y, int width, int height, uint16_t color);
GameState gameOn(void);
void drawEntity(uEntity entity);
void updateEntity(uEntity *entity);
uShip createShip(int x, int y, int width, int height, const unsigned short *image);
uShip* createEnemyShip(int x, int y, int width, int height, const unsigned short *image);
void updateShip(uShip *ship);
void updatePickup(uPickup *pickup);
char updatePlayer(uShip *ship);
char level1(void);
char level2(void);
char detectCollision(uEntity a, uEntity b);

void createExplosion(int x, int y);
void updateExplosion(uEntity *explosion);
void destroyEnemy(uShip *ship);
void updateDisplay(void);
void updateProjectile(uProjectile *projectile);
uProjectile* createProjectile(int x, int y, int xVel, int yVel, const unsigned short *image);
void playerGun(void);
void playerRocket(void);
void washEntity(uEntity *entity);
void removeProjectile(uProjectile *projectile);

void shootBullet(uShip ship);
void removePickup(uPickup *Pickup);
void reset(void);
void resetUI(void);

void ai1(uShip *ship);
void ai2(uShip *ship);
void ai3(uShip *ship);
void ai4(uShip *ship);
void ai5(uShip *ship);
void ai6(uShip *ship);

// typedef struct Particle {
// 	short x;
// 	short y;
// 	short xVel;
// 	short yVel;
// 	char removed;
// 	vuint16_t color;
// } uParticle;

/* Ignore this for now LOL STRUCT*/
typedef struct
{
    const volatile void *src;
    volatile void *dst;
    volatile uint32_t cnt;
} DMAREC;

#define DMA ((volatile DMAREC *)0x040000B0)

/* DMA channel 0 register definitions*/
#define REG_DMA0SAD         *(vuint32_t*)0x40000B0  /* source address*/
#define REG_DMA0DAD         *(vuint32_t*)0x40000B4  /* destination address*/
#define REG_DMA0CNT         *(vuint32_t*)0x40000B8  /* control register*/

/* DMA channel 1 register definitions*/
#define REG_DMA1SAD         *(vuint32_t*)0x40000BC  /* source address*/
#define REG_DMA1DAD         *(vuint32_t*)0x40000C0  /* destination address*/
#define REG_DMA1CNT         *(vuint32_t*)0x40000C4  /* control register*/

/* DMA channel 2 register definitions*/
#define REG_DMA2SAD         *(vuint32_t*)0x40000C8  /* source address*/
#define REG_DMA2DAD         *(vuint32_t*)0x40000CC  /* destination address*/
#define REG_DMA2CNT         *(vuint32_t*)0x40000D0  /* control register*/

/* DMA channel 3 register definitions */
#define REG_DMA3SAD         *(vuint32_t*)0x40000D4   /* source address*/
#define REG_DMA3DAD         *(vuint32_t*)0x40000D8  /* destination address*/
#define REG_DMA3CNT         *(vuint32_t*)0x40000DC  /* control register*/

/* Defines*/
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

#define START_ON_FIFO_EMPTY 0x30000000

extern uint16_t *videoBuffer;

void setPixel(int, int, unsigned short);
void waitForVblank(void);

void setPixel(int row, int col, unsigned short color);

#endif /* MY_LIB_H */

