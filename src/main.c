//Jacob Wartofsky
//gtID 903161493
//jake@wartofsky.net

#include "myLib.h"
#include "title.h"
#include "KusInt.h"
#include "TurInt.h"
#include "TurCorv.h"
#include "TurMissCorv.h"
#include "explosion1.h"
#include "explosion2.h"
#include "explosion3.h"
#include "Bullet.h"
#include "RedBullet.h"
#include "Rocket.h"
#include "Missile.h"
#include "HealthPickup.h"
#include "ShieldPickup.h"
#include "ShotgunPickup.h"
#include "RocketPickup.h"
#include "GameOverScreen.h"
#include <stdlib.h>
#include <stdio.h>
#include "text.h"
#include "hardware.h"

//To keep track of changes to score/lives/ammo
int oldLives = -1;
int oldAmmo = -1;
int oldScore = -1;
int oldHealth = -1;
int oldLevel = -1;

int gameClock = 0;
int lives;
int ammo;
int shotgunTime;
int score;
int level;
uint16_t bgColor = RGB( 0, 0, 0 );
uint16_t barColor = RGB( 5, 5, 10 );

// struct Entity projectiles[20];
// struct Entity ships[20];
uShip player;

//to store enemy ships
int numEnemies = 0;
uShip enemies[20];

//to store explosions
int numExplosions = 0;
uEntity explosions[20];

//to store projectiles
int numProjectiles = 0;
uProjectile projectiles[40];

//to store pickups
int numPickups = 0;
uPickup pickups[5];

char buffer[41];

int main(void)
{
    hardware_video_init();
    GameState state;

    state = Start;
    char draw = false;
    char startDown = true;
    // drawRectangle(0, 0, 20, 16, RGB(0,31,31));
    // drawImage3(40, 30, KUSINT_WIDTH, KUSINT_HEIGHT, KusInt);

    player = createShip( 0, 72, KUSINT_WIDTH, KUSINT_HEIGHT, KusInt );
    player.entity.canExit = false;

    while( 1 )
    {
        switch( state )
        {

        case Start:
            if( draw == false )
            {
                draw = true;
                drawScreen( title );
                drawString( 88, 100, "Press Start", RGB( 20, 20, 24 ) );
            }
            if( KEY_UP_NOW( BUTTON_START ) )
            {
                startDown = false;
            }
            if( KEY_DOWN_NOW(BUTTON_START) && (startDown == false) )
            {
                startDown = true;
                reset();
                draw = false;
                drawRectangle( 0, 0, 240, UBound, barColor );
                drawRectangle( 0, UBound, 240, (160 - UBound), bgColor );
                level = 1;
                state = NextLevel;
            }
            break;

        case Game:
            resetUI();
            state = gameOn();
            break;

        case NextLevel:
            gameClock = 0;
            if( draw == false )
            {
                if( level == 1 )
                {
                    //bgColor = RGB(0,0,0);
                    drawRectangle( 0, 0, 240, 160, bgColor );
                    drawString( 30, 30, "Mission 1", RGB( 20, 20, 24 ) );
                    drawString( 30, 40, "The Outer Rim", RGB( 20, 20, 24 ) );
                    drawString( 70, 102, "Press Start", RGB( 20, 20, 24 ) );
                    draw = true;
                }
                else if( level == 2 )
                {
                    //bgColor = RGB(0,0,0);
                    drawRectangle( 0, 0, 240, 160, bgColor );
                    drawString( 30, 30, "Mission 2", RGB( 20, 20, 24 ) );
                    drawString( 30, 40, "The Great Wastelands", RGB( 20, 20, 24 ) );
                    drawString( 70, 102, "Press Start", RGB( 20, 20, 24 ) );
                    lives += 1;
                    draw = true;
                }
            }
            if( KEY_UP_NOW( BUTTON_START ) )
            {
                startDown = false;
            }
            if( KEY_DOWN_NOW(BUTTON_START) && (startDown == false) )
            {
                startDown = true;
                draw = false;
                drawRectangle( 0, 0, 240, UBound, barColor );
                drawRectangle( 0, UBound, 240, (160 - UBound), bgColor );
                state = Game;
            }
            break;

        case GameOver:
            if( draw == false )
            {
                draw = true;
                drawScreen( GameOverScreen );
                sprintf( buffer, "Your score: %d", score );
                drawString( 70, 102, buffer, RGB( 20, 20, 24 ) );
            }
            if( KEY_DOWN_NOW(BUTTON_START) && (startDown == false) )
            {
                startDown = true;
                draw = false;
                state = Start;
            }
            if( KEY_UP_NOW( BUTTON_START ) )
            {
                startDown = false;
            }
            break;

        case Win:
            if( draw == false )
            {
                draw = true;
                drawRectangle( 0, 0, 240, 160, RGB( 0, 0, 5 ) );
                drawString( 30, 30, "Congratulations! You've won!", RGB( 20, 20, 24 ) );
                sprintf( buffer, "Your score: %d", score );
                drawString( 70, 102, buffer, RGB( 20, 20, 24 ) );
            }
            if( KEY_DOWN_NOW(BUTTON_START) && (startDown == false) )
            {
                startDown = true;
                draw = false;
                state = Start;
            }
            if( KEY_UP_NOW( BUTTON_START ) )
            {
                startDown = false;
            }
            break;

        }
        gameClock++;
        waitForVblank();
    }
    return 0;
}

GameState gameOn(void)
{

    char startDown = true;
    while( 1 )
    {

        //Player controls
        if( KEY_DOWN_NOW( BUTTON_RIGHT ) )
        {
            player.entity.xVel = 2;
        }
        if( KEY_DOWN_NOW( BUTTON_LEFT ) )
        {
            player.entity.xVel = -2;
        }
        if( KEY_DOWN_NOW( BUTTON_UP ) )
        {
            player.entity.yVel = -2;
        }
        if( KEY_DOWN_NOW( BUTTON_DOWN ) )
        {
            player.entity.yVel = 2;
        }
        if( KEY_UP_NOW(BUTTON_RIGHT) && KEY_UP_NOW( BUTTON_LEFT ) )
        {
            player.entity.xVel = 0;
        }
        if( KEY_UP_NOW(BUTTON_UP) && KEY_UP_NOW( BUTTON_DOWN ) )
        {
            player.entity.yVel = 0;
        }
        if( KEY_DOWN_NOW( BUTTON_A ) )
        {
            if( player.reload <= 0 )
            {
                player.reload = 12;
                playerGun();
            }
        }
        if( KEY_DOWN_NOW( BUTTON_B ) )
        {
            if( (player.reload <= 0) && (ammo > 0) )
            {
                ammo--;
                player.reload = 24;
                playerRocket();
            }
        }
        if( KEY_DOWN_NOW(BUTTON_START) && (startDown == false) )
        {
            return Start;
        }
        if( KEY_UP_NOW( BUTTON_START ) )
        {
            startDown = false;
        }

        //clean images
        washEntity( &(player.entity) );
        for( int i = 0; i < numEnemies; i++ )
        {
            washEntity( &((enemies + i)->entity) );
        }
        for( int i = 0; i < numProjectiles; i++ )
        {
            washEntity( &((projectiles + i)->entity) );
        }
        for( int i = 0; i < numPickups; i++ )
        {
            washEntity( &((pickups + i)->entity) );
        }

        //Update things
        if( level == 1 )
        {
            if( level1() == true )
            {
                return NextLevel;
            }
        }
        else if( level == 2 )
        {
            if( level2() == true )
            {
                return Win;
            }
        }
        if( updatePlayer( &player ) == false )
        {
            return GameOver;
        }

        //update explosions
        for( int i = 0; i < numExplosions; i++ )
        {
            updateExplosion( explosions + i );
        }

        //update enemy movements and check for collisions with player
        for( int i = 0; i < numEnemies; i++ )
        {
            updateShip( enemies + i );
            char collision = detectCollision( (enemies + i)->entity, player.entity );
            if( collision )
            {
                if( (enemies + i)->ai == 4 )
                {
                    player.health -= 30;
                }
                else
                {
                    player.health -= (enemies + i)->health;
                }
                (enemies + i)->health -= 50;
                (enemies + i)->entity.removed = true;
            }
        }

        //update projectiles
        for( int i = 0; i < numProjectiles; i++ )
        {
            updateProjectile( projectiles + i );
            if( (projectiles + i)->team == 0 )
            {
                for( int j = 0; j < numEnemies; j++ )
                {
                    char collision = detectCollision( (enemies + j)->entity, (projectiles + i)->entity );
                    if( collision )
                    {
                        (enemies + j)->health -= (projectiles + i)->damage;
                        (projectiles + i)->entity.removed = true;
                    }
                }
            }
            else if( (projectiles + i)->team == 1 )
            {
                char collision = detectCollision( player.entity, (projectiles + i)->entity );
                if( collision )
                {
                    player.health -= (projectiles + i)->damage;
                    (projectiles + i)->entity.removed = true;
                }
            }
        }

        //update pickups and check for collisions
        for( int i = 0; i < numPickups; i++ )
        {
            updatePickup( pickups + i );
            char collision = detectCollision( (pickups + i)->entity, player.entity );
            if( collision )
            {
                if( (pickups + i)->type == Health )
                {
                    if( player.health < 50 )
                    {
                        player.health = 50;
                    }
                }
                else if( (pickups + i)->type == Shotgun )
                {
                    shotgunTime = 600;
                }
                else if( (pickups + i)->type == Ammo )
                {
                    if( ammo < 5 )
                    {
                        ammo += 5;
                    }
                    else
                    {
                        ammo = 9;
                    }
                }
                else if( (pickups + i)->type == Shield )
                {
                    player.health = 100;
                }
                (pickups + i)->entity.removed = true;
            }
        }

        //Remove destroyed projectiles
        for( int i = 0; i < numProjectiles; i++ )
        {
            if( (projectiles + i)->entity.removed == true )
            {
                removeProjectile( projectiles + i );
                i--;
            }
        }

        //Remove destroyed ships
        for( int i = 0; i < numEnemies; i++ )
        {
            if( (enemies + i)->entity.removed == true )
            {
                destroyEnemy( enemies + i );
                i--;
            }
        }

        //Remove explosions
        for( int i = 0; i < numExplosions; i++ )
        {
            if( (explosions + i)->canExit > 40 )
            {
                numExplosions -= 1;
                *(explosions + i) = explosions[numExplosions];
                i--;
            }
        }

        //Remove pickups
        for( int i = 0; i < numPickups; i++ )
        {
            if( (pickups + i)->entity.removed == true )
            {
                removePickup( pickups + i );
                i--;
            }
        }

        gameClock++;
        waitForVblank();

    }
    return Start;
}

void drawEntity(uEntity entity)
{
    drawImage3( entity.x, entity.y, entity.width, entity.height, entity.image );
}

uShip createShip(int x, int y, int width, int height, const unsigned short *image)
{
    uShip ship;
    ship.entity.x = x;
    ship.entity.y = y;
    ship.entity.xVel = 0;
    ship.entity.yVel = 0;
    ship.entity.width = width;
    ship.entity.height = height;
    ship.entity.image = image;
    ship.entity.canExit = true;
    ship.entity.isVisible = true;
    ship.entity.removed = false;
    ship.health = 50;
    ship.ai = 0;
    ship.reload = 0;
    return ship;
}

uShip* createEnemyShip(int x, int y, int width, int height, const unsigned short *image)
{
    uShip ship = createShip( x, y, width, height, image );
    enemies[numEnemies] = ship;
    if( numEnemies < 20 )
    {
        enemies[numEnemies] = ship;
        numEnemies += 1;
        return (enemies + numEnemies - 1);
    }
    return enemies;
}

uProjectile* createProjectile(int x, int y, int width, int height, const unsigned short *image)
{

    uProjectile projectile;
    projectile.entity.x = x;
    projectile.entity.y = y;
    projectile.entity.xVel = 4;
    projectile.entity.yVel = 0;
    projectile.entity.width = width;
    projectile.entity.height = height;
    projectile.entity.image = image;
    projectile.entity.canExit = true;
    projectile.entity.isVisible = true;
    projectile.entity.removed = false;
    projectile.damage = 20;
    projectile.team = 0;
    if( numProjectiles < 40 )
    {
        projectiles[numProjectiles] = projectile;
        numProjectiles += 1;
        return (projectiles + numProjectiles - 1);
    }
    return projectiles;
}

void createExplosion(int x, int y)
{
    if( numExplosions < 20 )
    {
        uEntity explosion;
        explosion.x = x;
        explosion.y = y;
        explosion.width = EXPLOSION1_WIDTH;
        explosion.height = EXPLOSION1_HEIGHT;
        explosion.image = explosion1;
        explosion.canExit = 0;
        explosions[numExplosions] = explosion;
        drawEntity( explosion );
        numExplosions += 1;
    }
}

uPickup* createPickup(int x, int y, int width, int height, const unsigned short *image)
{

    uPickup Pickup;
    Pickup.entity.x = x;
    Pickup.entity.y = y;
    Pickup.entity.xVel = 0;
    Pickup.entity.yVel = 0;
    Pickup.entity.width = width;
    Pickup.entity.height = height;
    Pickup.entity.image = image;
    Pickup.entity.canExit = true;
    Pickup.entity.isVisible = true;
    Pickup.entity.removed = false;
    Pickup.type = Health;
    if( numPickups < 5 )
    {
        pickups[numPickups] = Pickup;
        numPickups += 1;
        return (pickups + numPickups - 1);
    }
    return pickups;
}

void destroyEnemy(uShip *ship)
{
    drawRectangle( ship->entity.x, ship->entity.y, ship->entity.width, ship->entity.height, bgColor );
    if( ship->entity.isVisible && ship->entity.x >= 0 )
    {
        createExplosion( ship->entity.x, ship->entity.y );
    }
    numEnemies--;
    *ship = enemies[numEnemies];
}

void removeProjectile(uProjectile *projectile)
{
    drawRectangle( projectile->entity.x, projectile->entity.y, projectile->entity.width,
            projectile->entity.height, bgColor );
    numProjectiles -= 1;
    *projectile = projectiles[numProjectiles];
}

void removePickup(uPickup *pickup)
{
    drawRectangle( pickup->entity.x, pickup->entity.y, pickup->entity.width, pickup->entity.height, bgColor );
    numPickups -= 1;
    *pickup = pickups[numPickups];
}

//erases image of entity
void washEntity(uEntity *entity)
{
    int j = ((entity->y >= UBound) ? (0) : (UBound - entity->y));
    drawRectangle( entity->x, entity->y + j, entity->width, (entity->height) - j, bgColor );
}

char level1()
{
    if( gameClock == 300 )
    {
        for( int i = 0; i < 3; i++ )
        {
            uShip *ship = createEnemyShip( 260, 72, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
            ship->health = 40;
            ship->entity.x += i * 10;
            ship->entity.y += i * 20;
            ship->entity.xVel = -1;
            ship->ai = 1;
        }
    }
    if( gameClock == 400 )
    {
        uShip *ship = createEnemyShip( 200, -20, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
        ship->ai = 3;
    }
    if( gameClock == 500 )
    {
        uShip *ship = createEnemyShip( 200, -20, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
        ship->ai = 6;
    }
    if( gameClock == 600 )
    {
        uPickup *pickup = createPickup( 280, 120, HEALTHPICKUP_WIDTH, HEALTHPICKUP_HEIGHT, HealthPickup );
        pickup->type = Health;
        pickup->entity.xVel = -1;
    }
    if( gameClock == 800 )
    {
        uShip *ship = createEnemyShip( 100, -20, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
        ship->ai = 3;
    }
    if( gameClock == 900 )
    {
        uShip *ship = createEnemyShip( 270, 70, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
        ship->entity.xVel = -1;
        ship->ai = 3;
    }
    if( gameClock == 1000 )
    {
        uShip *ship = createEnemyShip( 280, 74, TURMISSCORV_WIDTH, TURMISSCORV_HEIGHT, TurMissCorv );
        ship->health = 400;
        ship->ai = 5;
    }
    if( gameClock == 1100 )
    {
        for( int i = 0; i < 3; i++ )
        {
            uShip *ship = createEnemyShip( 260, 100, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
            ship->health = 40;
            ship->entity.x += i * 10;
            ship->entity.y += i * 20;
            ship->entity.xVel = -1;
            ship->ai = 1;
        }
    }
    if( gameClock == 1200 )
    {
        uPickup *pickup = createPickup( 280, 72, SHOTGUNPICKUP_WIDTH, SHOTGUNPICKUP_HEIGHT, ShotgunPickup );
        pickup->type = Shotgun;
        pickup->entity.xVel = -1;
    }
    if( gameClock == 1400 )
    {
        for( int i = 0; i < 5; i++ )
        {
            uShip *ship = createEnemyShip( 260, 30, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
            ship->health = 40;
            ship->entity.y += i * 20;
            ship->entity.xVel = -1;
            ship->ai = 1;
        }
    }
    if( gameClock == 1600 )
    {
        uShip *ship = createEnemyShip( 150, 190, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
        ship->ai = 2;
        uShip *ship1 = createEnemyShip( 140, -20, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
        ship1->ai = 3;
    }
    if( gameClock == 1700 )
    {
        for( int i = 0; i < 3; i++ )
        {
            uShip *ship = createEnemyShip( 260, 72, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
            ship->health = 40;
            ship->entity.x += i * 10;
            ship->entity.y += i * 20;
            ship->entity.xVel = -1;
            ship->ai = 1;
        }
    }
    if( gameClock == 1900 )
    {
        uPickup *pickup = createPickup( 280, 10, HEALTHPICKUP_WIDTH, HEALTHPICKUP_HEIGHT, RocketPickup );
        pickup->type = Ammo;
        pickup->entity.xVel = -1;
    }
    if( gameClock == 2000 )
    {
        uPickup *pickup = createPickup( 280, 40, HEALTHPICKUP_WIDTH, HEALTHPICKUP_HEIGHT, HealthPickup );
        pickup->type = Health;
        pickup->entity.xVel = -1;
    }
    if( gameClock == 2050 )
    {
        uShip *ship = createEnemyShip( 200, -20, TURCORV_WIDTH, TURCORV_HEIGHT, TurCorv );
        ship->health = 300;
        ship->ai = 6;
    }
    if( gameClock == 2100 )
    {
        for( int i = 0; i < 4; i++ )
        {
            uShip *ship = createEnemyShip( 260, 72, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
            ship->health = 40;
            ship->entity.x += i * 10;
            ship->entity.y += i * 20;
            ship->entity.xVel = -1;
            ship->ai = 1;
        }
    }
    if( gameClock == 2300 )
    {
        for( int i = 0; i < 2; i++ )
        {
            uShip *ship = createEnemyShip( 260, 0, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
            ship->health = 40;
            ship->entity.x += i * 10;
            ship->entity.y += i * 20;
            ship->entity.xVel = -1;
            ship->ai = 3;
        }
    }
    if( gameClock == 2400 )
    {
        uShip *ship = createEnemyShip( 250, -30, TURMISSCORV_WIDTH, TURMISSCORV_HEIGHT, TurMissCorv );
        ship->health = 400;
        ship->entity.xVel = -1;
        ship->ai = 5;
    }
    if( gameClock == 2450 )
    {
        uShip *ship = createEnemyShip( 200, 72, TURCORV_WIDTH, TURCORV_HEIGHT, TurCorv );
        ship->health = 300;
        ship->ai = 6;
    }
    if( gameClock == 2500 )
    {
        uPickup *pickup = createPickup( 280, 50, HEALTHPICKUP_WIDTH, HEALTHPICKUP_HEIGHT, ShieldPickup );
        pickup->type = Shield;
        pickup->entity.xVel = -1;
    }
    if( gameClock == 2700 )
    {
        for( int i = 0; i < 2; i++ )
        {
            uShip *ship = createEnemyShip( 250, 20, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
            ship->health = 40;
            ship->entity.x += i * 10;
            ship->entity.y += i * 50;
            ship->entity.xVel = -1;
            ship->ai = 3;
        }
    }
    if( gameClock == 3000 )
    {
        uShip *ship = createEnemyShip( 250, 100, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
        ship->health = 40;
        ship->ai = 4;
    }
    if( gameClock == 3100 )
    {
        uShip *ship = createEnemyShip( 200, 72, TURCORV_WIDTH, TURCORV_HEIGHT, TurCorv );
        ship->health = 300;
        ship->ai = 6;
    }
    if( gameClock == 3500 )
    {
        level = 2;
        return true;
    }
    return false;
}

char level2()
{
    if( gameClock == 100 )
    {
        uPickup *pickup = createPickup( 280, 50, HEALTHPICKUP_WIDTH, HEALTHPICKUP_HEIGHT, HealthPickup );
        pickup->type = Health;
        pickup->entity.xVel = -1;
    }
    if( gameClock == 200 )
    {
        uShip *ship = createEnemyShip( 280, 74, TURMISSCORV_WIDTH, TURMISSCORV_HEIGHT, TurMissCorv );
        ship->health = 400;
        ship->ai = 5;
    }
    if( gameClock == 300 )
    {
        for( int i = 0; i < 5; i++ )
        {
            uShip *ship = createEnemyShip( 240, 72, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
            ship->health = 40;
            ship->entity.x += i * 10;
            ship->entity.y += i * 10;
            ship->entity.xVel = -1;
            ship->ai = 1;
        }
    }
    if( gameClock == 500 )
    {
        for( int i = 0; i < 5; i++ )
        {
            uShip *ship = createEnemyShip( 240, 72, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
            ship->health = 40;
            ship->entity.x += i * 10;
            ship->entity.y += i * 10;
            ship->entity.xVel = -1;
            ship->ai = 1;
        }
    }
    if( gameClock == 700 )
    {
        uShip *ship = createEnemyShip( 280, 74, TURMISSCORV_WIDTH, TURMISSCORV_HEIGHT, TurMissCorv );
        ship->health = 400;
        ship->ai = 5;
    }
    if( gameClock == 900 )
    {
        uPickup *pickup = createPickup( 280, 72, SHOTGUNPICKUP_WIDTH, SHOTGUNPICKUP_HEIGHT, ShotgunPickup );
        pickup->type = Shotgun;
        pickup->entity.xVel = -1;
    }
    if( gameClock == 1000 )
    {
        for( int i = 0; i < 3; i++ )
        {
            uShip *ship = createEnemyShip( 250, 20, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
            ship->health = 40;
            ship->entity.x += i * 10;
            ship->entity.y += i * 50;
            ship->entity.xVel = -1;
            ship->ai = 6;
        }
    }
    if( gameClock == 1100 )
    {
        uPickup *pickup = createPickup( 280, 120, HEALTHPICKUP_WIDTH, HEALTHPICKUP_HEIGHT, RocketPickup );
        pickup->type = Ammo;
        pickup->entity.xVel = -1;
    }
    if( gameClock == 1300 )
    {
        for( int i = 0; i < 7; i++ )
        {
            uShip *ship = createEnemyShip( 220, 30, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
            ship->health = 40;
            ship->entity.x += i * 10;
            ship->entity.y += i * 10;
            ship->entity.xVel = -1;
            ship->ai = 1;
        }
    }
    if( gameClock == 1350 )
    {
        uPickup *pickup = createPickup( 280, 130, HEALTHPICKUP_WIDTH, HEALTHPICKUP_HEIGHT, HealthPickup );
        pickup->type = Health;
        pickup->entity.xVel = -1;
    }
    if( gameClock == 1400 )
    {
        uShip *ship = createEnemyShip( 160, -20, TURMISSCORV_WIDTH, TURMISSCORV_HEIGHT, TurMissCorv );
        ship->health = 400;
        ship->ai = 5;
    }
    if( gameClock == 1500 )
    {
        uPickup *pickup = createPickup( 280, 10, HEALTHPICKUP_WIDTH, HEALTHPICKUP_HEIGHT, RocketPickup );
        pickup->type = Ammo;
        pickup->entity.xVel = -1;
    }
    if( gameClock == 1600 )
    {
        uShip *ship = createEnemyShip( 280, 0, TURMISSCORV_WIDTH, TURMISSCORV_HEIGHT, TurMissCorv );
        ship->health = 400;
        ship->ai = 5;
    }
    if( gameClock == 1800 )
    {
        for( int i = 0; i < 7; i++ )
        {
            uShip *ship = createEnemyShip( 220, 30, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
            ship->health = 40;
            ship->entity.x += i * 10;
            ship->entity.y += i * 10;
            ship->entity.xVel = -1;
            ship->ai = 1;
        }
    }
    if( gameClock == 2100 )
    {
        for( int i = 0; i < 3; i++ )
        {
            uShip *ship = createEnemyShip( 250, 20, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
            ship->health = 40;
            ship->entity.x += i * 10;
            ship->entity.y += i * 50;
            ship->entity.xVel = -1;
            ship->ai = 3;
        }
    }
    if( gameClock == 2300 )
    {
        uShip *ship = createEnemyShip( 280, 72, TURCORV_WIDTH, TURCORV_HEIGHT, TurCorv );
        ship->health = 300;
        ship->ai = 6;
    }
    if( gameClock == 2500 )
    {
        for( int i = 0; i < 7; i++ )
        {
            uShip *ship = createEnemyShip( 220, 30, TURINT_WIDTH, TURINT_HEIGHT, TurInt );
            ship->health = 40;
            ship->entity.x += i * 10;
            ship->entity.y += i * 10;
            ship->entity.xVel = -1;
            ship->ai = 1;
        }
    }
    if( gameClock == 2700 )
    {
        uShip *ship = createEnemyShip( 280, 72, TURCORV_WIDTH, TURCORV_HEIGHT, TurCorv );
        ship->health = 300;
        ship->ai = 6;
    }
    if( gameClock == 2800 )
    {
        uShip *ship = createEnemyShip( 280, 120, TURCORV_WIDTH, TURCORV_HEIGHT, TurCorv );
        ship->health = 300;
        ship->ai = 6;
    }
    if( gameClock == 3100 )
    {
        return true;
    }
    return false;
}

void updateEntity(uEntity *entity)
{
    entity->x += entity->xVel;
    entity->y += entity->yVel;
    if( !entity->canExit )
    {
        if( entity->x > (240 - entity->width) )
        {
            entity->x = 240 - entity->width;
        }
        if( entity->x < 0 )
        {
            entity->x = 0;
        }
        if( entity->y > (160 - entity->height) )
        {
            entity->y = 160 - entity->height;
        }
        if( entity->y < UBound )
        {
            entity->y = UBound;
        }
    }
    else
    {
        if( entity->x >= 300 || entity->x < -60 || entity->y > 200 || entity->y < -40 )
        {
            entity->removed = true;
        }
    }
    if( entity->isVisible )
    {
        drawEntity( *entity );
    }
}

char updatePlayer(uShip *ship)
{

    if( ship->health <= 0 )
    {
        drawRectangle( ship->entity.x, ship->entity.y, ship->entity.width, ship->entity.height, bgColor );
        createExplosion( ship->entity.x, ship->entity.y );
        ship->entity.x = 5;
        ship->entity.y = 72;
        ship->health = 50;
        lives--;
        if( lives <= 0 )
        {
            return false;
        }
    }
    if( ship->reload > 0 )
    {
        ship->reload--;
    }
    if( shotgunTime > 0 )
    {
        shotgunTime--;
    }
    updateDisplay();
    updateEntity( &(ship->entity) );
    return true;
}

void updateShip(uShip *ship)
{
    updateEntity( &(ship->entity) );
    if( ship->reload > 0 )
    {
        ship->reload--;
    }
    if( ship->health <= 0 )
    {
        ship->entity.removed = true;
        score += 5;
    }
    switch( ship->ai )
    {
    case 1:
        ai1( ship );
        break;
    case 2:
        ai2( ship );
        break;
    case 3:
        ai3( ship );
        break;
    case 4:
        ai4( ship );
        break;
    case 5:
        ai5( ship );
        break;
    case 6:
        ai6( ship );
        break;
    default:
        break;
    }

}

void updateProjectile(uProjectile *projectile)
{
    updateEntity( &(projectile->entity) );
}

void updateExplosion(uEntity *explosion)
{
    if( explosion->image == explosion1 && explosion->canExit == 6 )
    {
        explosion->image = explosion2;
        drawEntity( *explosion );
    }
    else if( explosion->image == explosion2 && explosion->canExit == 12 )
    {
        explosion->image = explosion3;
        drawEntity( *explosion );
    }
    else if( explosion->canExit == 18 )
    {
        drawRectangle( explosion->x, explosion->y, explosion->width, explosion->height, bgColor );
    }
    explosion->canExit += 1;
}

void updatePickup(uPickup *Pickup)
{
    updateEntity( &(Pickup->entity) );
}

void updateDisplay()
{
    if( oldScore != score )
    {
        oldScore = score;
        sprintf( buffer, "Score %d", score );
        drawRectangle( 175, 0, 65, UBound, barColor ); // The 14 is allowing for as many as 14 characters
        drawString( 175, 0, buffer, RGB( 31, 31, 31 ) );
    }
    if( oldLevel != level )
    {
        oldLevel = level;
        sprintf( buffer, "Lvl %d", level );
        drawRectangle( 140, 0, 35, UBound, barColor ); // The 14 is allowing for as many as 14 characters
        drawString( 140, 0, buffer, RGB( 31, 31, 31 ) );
    }
    if( oldHealth != player.health )
    {
        oldHealth = player.health;
        drawRectangle( 85, 0, 50, UBound, barColor );
        drawRectangle( 85, 1, 50, 6, RGB( 31, 0, 0 ) );
        if( player.health <= 50 )
        {
            drawRectangle( 85, 1, player.health, 6, RGB( 0, 31, 0 ) );
        }
        else
        {
            drawRectangle( 85, 1, 50, 6, RGB( 0, 31, 0 ) );
            drawRectangle( 85, 1, (player.health - 50), 6, RGB( 31, 0, 22 ) );
        }
    }
    if( oldAmmo != ammo )
    {
        oldAmmo = ammo;
        sprintf( buffer, "Rkt %d", ammo );
        drawRectangle( 45, 0, 40, UBound, barColor ); // The 14 is allowing for as many as 14 characters
        drawString( 45, 0, buffer, RGB( 31, 31, 31 ) );
    }
    if( oldLives != lives )
    {
        oldLives = lives;
        sprintf( buffer, "Life %d", lives );
        drawRectangle( 0, 0, 45, UBound, barColor ); // The 14 is allowing for as many as 14 characters
        drawString( 0, 0, buffer, RGB( 31, 31, 31 ) );
    }

}

char detectCollision(uEntity a, uEntity b)
{
    char xOverlap = false;
    char yOverlap = false;
    if( a.x <= b.x )
    {
        if( (b.x - a.x) < a.width )
        {
            xOverlap = true;
        }
    }
    else
    {
        if( (a.x - b.x) < b.width )
        {
            xOverlap = true;
        }
    }

    if( a.y <= b.y )
    {
        if( (b.y - a.y) < a.height )
        {
            yOverlap = true;
        }
    }
    else
    {
        if( (a.y - b.y) < b.height )
        {
            yOverlap = true;
        }
    }

    if( xOverlap && yOverlap )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void playerGun()
{
    if( shotgunTime <= 0 )
    {
        uProjectile *projectile = createProjectile( player.entity.x + player.entity.width,
                player.entity.y + (player.entity.height / 2), BULLET_WIDTH, BULLET_HEIGHT, Bullet );
        projectile->damage = 50;
    }
    else
    {
        uProjectile *projectile = createProjectile( player.entity.x + player.entity.width,
                player.entity.y + (player.entity.height / 2), BULLET_WIDTH, BULLET_HEIGHT, Bullet );
        projectile->damage = 50;
        uProjectile *projectileUp = createProjectile( player.entity.x + player.entity.width,
                player.entity.y + (player.entity.height / 2), BULLET_WIDTH, BULLET_HEIGHT, Bullet );
        projectileUp->damage = 50;
        projectileUp->entity.yVel = -1;
        uProjectile *projectileDown = createProjectile( player.entity.x + player.entity.width,
                player.entity.y + (player.entity.height / 2), BULLET_WIDTH, BULLET_HEIGHT, Bullet );
        projectileDown->damage = 50;
        projectileDown->entity.yVel = 1;
    }
}

void playerRocket()
{
    uProjectile *projectile = createProjectile( player.entity.x + player.entity.width,
            player.entity.y + (player.entity.height / 2), ROCKET_WIDTH, ROCKET_HEIGHT, Rocket );
    projectile->entity.xVel = 3;
    projectile->damage = 300;
}

void shootBullet(uShip ship)
{
    uProjectile *projectile = createProjectile( ship.entity.x - 8, ship.entity.y + (ship.entity.height / 2),
            REDBULLET_WIDTH, REDBULLET_HEIGHT, RedBullet );
    projectile->team = 1;
    projectile->entity.xVel = -4;
    projectile->damage = 16;

}

//Basic ai
void ai1(uShip *ship)
{
    if( ship->reload <= 0 )
    {
        shootBullet( *ship );
        ship->reload = 60;
    }
}

//Oscillate around players y coord
void ai2(uShip *ship)
{
    if( player.entity.y < (ship->entity.y - 16) )
    {
        ship->entity.yVel = -1;
    }
    else if( player.entity.y > (ship->entity.y + 16) )
    {
        ship->entity.yVel = 1;
    }
    if( ship->reload <= 0 )
    {
        shootBullet( *ship );
        ship->reload = 60;
    }
}

//Line up with player's y coord and shoot
void ai3(uShip *ship)
{
    if( ship->entity.x > player.entity.x )
    {
        if( player.entity.y < (ship->entity.y - 9) )
        {
            ship->entity.yVel = -1;
        }
        else if( player.entity.y > (ship->entity.y + 9) )
        {
            ship->entity.yVel = 1;
        }
        else if( abs( player.entity.y - ship->entity.y ) < 16 )
        {
            ship->entity.yVel = 0;
        }
        if( ship->reload <= 0 )
        {
            shootBullet( *ship );
            ship->reload = 45;
        }
    }
}

//Missile ai
void ai4(uShip *ship)
{
    if( player.entity.y < (ship->entity.y - 6) )
    {
        ship->entity.yVel = -1;
    }
    else if( player.entity.y > (ship->entity.y + 6) )
    {
        ship->entity.yVel = 1;
    }
    else if( abs( player.entity.y - ship->entity.y ) < 10 )
    {
        ship->entity.yVel = 0;
    }
    if( ship->entity.xVel == 0 )
    {
        ship->entity.xVel = -1;
    }
    else if( ship->entity.xVel == -1 )
    {
        ship->entity.xVel = -2;
    }
}

//Line up with player's y coord and shoot missiles
void ai5(uShip *ship)
{

    if( player.entity.y < (ship->entity.y - 38) )
    {
        ship->entity.yVel = -1;
    }
    else if( player.entity.y > (ship->entity.y + 38) )
    {
        ship->entity.yVel = 1;
    }
    if( ship->reload <= 0 )
    {
        uShip *missile = createEnemyShip( ship->entity.x - (ship->entity.width + 10),
                ship->entity.y + (ship->entity.height / 2), MISSILE_WIDTH, MISSILE_HEIGHT, Missile );
        missile->health = 10;
        missile->ai = 4;
        ship->reload = 90;
    }

    if( ship->entity.x > 240 )
    {
        ship->entity.xVel = -1;
    }
    if( ship->entity.x < 180 )
    {
        ship->entity.xVel = 0;
    }
}

//Aggressively attack the player
void ai6(uShip *ship)
{

    if( player.entity.y < (ship->entity.y - 9) )
    {
        ship->entity.yVel = -1;
    }
    else if( player.entity.y > (ship->entity.y + 9) )
    {
        ship->entity.yVel = 1;
    }

    if( player.entity.x < (ship->entity.x - 65) )
    {
        ship->entity.xVel = -1;
    }
    else if( player.entity.x > (ship->entity.x - 50) )
    {
        ship->entity.xVel = 1;
    }
    if( ship->reload <= 0 )
    {
        shootBullet( *ship );
        ship->reload = 50;
    }

}

//Resets player stats
void reset()
{
    player.health = 50;
    player.entity.x = 5;
    player.entity.y = 72;
    ammo = 5;
    lives = 5;
    score = 0;
    level = 0;
    shotgunTime = 0;
    numEnemies = 0;
    numProjectiles = 0;
    numExplosions = 0;
    numPickups = 0;
    gameClock = 0;

    oldHealth = -1;
    oldAmmo = -1;
    oldLives = -1;
    oldScore = -1;
    oldLevel = -1;

}

//Sets old values to impossible values so that the UI will update
void resetUI()
{
    oldHealth = -1;
    oldAmmo = -1;
    oldLives = -1;
    oldLevel = -1;
}

/*
void convertImages(vuint16_t toChange, vuint16_t toChangeTo) {
	convertImage(KusInt, toChange, toChangeTo);
}

void convertImage(const unsigned short *image, vuint16_t toChange, vuint16_t toChangeTo) {
	int size = sizeof(image)/sizeof(image[0]);
	for (int i = 0; i < size; i++) {
		if (image[i] == toChange) {
			image[i] = toChangeTo;
		}
	}
}
*/
