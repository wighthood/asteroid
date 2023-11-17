#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SFML/Window.h>
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <SFML/System.h>


#define pi 3.141592653589793
#define max_asteroids 50

float mspeed = 0.05f;

struct Player {
    sfText* text;  
    sfText* Life;
    sfVector2f position;
    sfVector2f force;
    float rotation;
    float velocity;
    float slowdown;
    int life;
};


struct steroid {
    sfText* text;
    sfVector2f position;
    sfVector2f force;
    float rotation;
    float velocity;
    float size;
};

struct steroid steroids[max_asteroids];

int WINDOW_X = 1080;
int WINDOW_Y = 920;
int gameon = 0;
int ingame = 0;
int dead = 0;


float delta = 0;

sfClock* deltaclock;

sfFont* font1;
sfRenderWindow* window;

void Delta() {
    sfTime dtime = sfClock_getElapsedTime(deltaclock);
    delta = sfTime_asMilliseconds(dtime);
    sfClock_restart(deltaclock);
}


void create() {
    sfVideoMode mode = { WINDOW_X, WINDOW_Y, 32 };

    font1 = sfFont_createFromFile("font/ubuntu.ttf");
    window = sfRenderWindow_create(mode, "asteroid",sfClose, NULL);
    sfRenderWindow_setFramerateLimit(window, 60);
    deltaclock = sfClock_create();
    gameon = 1;
    ingame = 0;
    dead = 0;
}


void initsteroid(int x, int y, int size)
{
    for (int i = 0; i < max_asteroids; i++)
    {
        if (steroids[i].text == NULL)
        {
            steroids[i].position.x = x;
			steroids[i].position.y = y;
            if (x == 0 && y == 0)
            {
                switch (rand() % 2)
				{
				case 0:
					steroids[i].position.x = rand() % WINDOW_X;
                    steroids[i].position.y = -100;
					break;
				case 1:
					steroids[i].position.y = rand() % WINDOW_Y;
					steroids[i].position.x = -100;
					break;
				}
            }
            steroids[i].rotation = rand();
            steroids[i].velocity = rand()%20+1;
            float directionx = cos(steroids[i].rotation) * (pi / 180.0f);
            float directiony = sin(steroids[i].rotation) * (pi / 180.0f);
            steroids[i].text = sfText_create();
            steroids[i].force.x = steroids[i].velocity *directionx;
			steroids[i].force.y = steroids[i].velocity*directiony;
            sfText_setFont(steroids[i].text, font1);
            steroids[i].size = size;
            switch (size)
            {
            case 1:
                sfText_setString(steroids[i].text, "o");
                sfText_setCharacterSize(steroids[i].text, (float)WINDOW_X * 10.0 / 256.0);
                sfText_setOrigin(steroids[i].text, (sfVector2f) { sfText_getLocalBounds(steroids[i].text).width / 2, sfText_getLocalBounds(steroids[i].text).height - (WINDOW_X * -25 / 2560) });
                break;
            case 2:
                sfText_setString(steroids[i].text, "O");
                sfText_setCharacterSize(steroids[i].text, (float)WINDOW_X * 15.0 / 256.0);
                sfText_setOrigin(steroids[i].text, (sfVector2f) { sfText_getLocalBounds(steroids[i].text).width / 2, sfText_getLocalBounds(steroids[i].text).height - (WINDOW_X * 15 / 2560) });
                break;
            case 3:
                sfText_setString(steroids[i].text, "O");
                sfText_setCharacterSize(steroids[i].text, (float)WINDOW_X * 20.0 / 256.0);
                sfText_setOrigin(steroids[i].text, (sfVector2f) { sfText_getLocalBounds(steroids[i].text).width / 2, sfText_getLocalBounds(steroids[i].text).height - (WINDOW_X * 20 / 2560) });
            }
            break;
        }
    }
}


void updatesteroid()
{
    for (int i = 0; i < max_asteroids && steroids[i].text != NULL; i++)
    {
        steroids[i].position.x += steroids[i].force.x*delta;
        steroids[i].position.y += steroids[i].force.y*delta;
        sfText_setPosition(steroids[i].text, steroids[i].position);


        sfFloatRect bounds = sfText_getLocalBounds(steroids[i].text);
        if (steroids[i].position.x > WINDOW_X + bounds.width / 2)
        {
            steroids[i].position.x = -bounds.width / 2;
        }if (steroids[i].position.x < -bounds.width / 2)
        {
            steroids[i].position.x = WINDOW_X + bounds.width / 2;
        }if (steroids[i].position.y > WINDOW_Y + bounds.height / 2)
        {
            steroids[i].position.y = -bounds.height / 2;
        }if (steroids[i].position.y < -bounds.height / 2)
        {
            steroids[i].position.y = WINDOW_Y + bounds.height / 2;
        }
    }
}

void drawsteroid()
{
    for (int i = 0; i < max_asteroids &&steroids[i].text != NULL; i++)
    {
        sfRenderWindow_drawText(window,steroids[i].text, NULL);
    }
}

void cleanupSteroid()
{

	for (int i = 0; i < max_asteroids && steroids[i].text != NULL; i++)
	{
			steroids[i].text = NULL;
            sfText_destroy(steroids[i].text);
			break;
	}
}

void initPlayer(struct Player* player) {
    player->text = sfText_create();
    sfText_setFont(player->text, font1);
    sfText_setString(player->text, "A");
    sfText_setCharacterSize(player->text, 50);
    sfFloatRect bounds = sfText_getLocalBounds(player->text);
    sfText_setOrigin(player->text, (sfVector2f) { bounds.width / 2, bounds.height / 2 });
    sfText_setFillColor(player->text, sfWhite);
    player->rotation = -90.0f;
    player->velocity = 0.008f;
    player->force.x = 0.0f;
    player->force.y = 0.0f;
    player->life = 3;
}

void updatePlayer(struct Player* player) {
    //player rotation
    if (sfKeyboard_isKeyPressed(sfKeyLeft)) {
        player->rotation -= 3;
    }
    if (sfKeyboard_isKeyPressed(sfKeyRight)) {
        player->rotation += 3;
    }

    float directionx = cos(player->rotation * (pi / 180.0f));
    float directiony = sin(player->rotation * (pi / 180.0f));

    if (sfKeyboard_isKeyPressed(sfKeyUp))
    {
        //acceleration
        if (fabs(player->force.x + player->velocity * directionx*delta) < fabs(mspeed * directionx*delta))
        {
            player->force.x += player->velocity * directionx*delta;
        }
        if (fabs(player->force.y + player->velocity * directiony*delta) < fabs(mspeed * directiony*delta))
        {
            player->force.y += player->velocity * directiony*delta;
        }
    }
	float norm = sqrt(player->force.x * player->force.x + player->force.y * player->force.y);   
    if (norm != 0)
    {
        //inertia
		float normalized_x = player->force.x / norm;
		float normalized_y = player->force.y / norm;
        if (fabs(player->force.x) - 0.001*normalized_x *delta > 0)
        {
			player->force.x -= 0.001*normalized_x*delta;
        }
        else
        {
            player->force.x = 0.0f;
        }
        if (fabs(player->force.y) - 0.001*normalized_y *delta > 0)
        {
			player->force.y -= 0.001*normalized_y*delta; 
        }
        else
        {
            player->force.y = 0;
        }
		
    }
    
    player->position.x += player->force.x*delta;
    player->position.y += player->force.y*delta;

    //wrap around
    sfFloatRect bounds = sfText_getLocalBounds(player->text);
    if (player->position.x > WINDOW_X + bounds.width / 2)
    {
        player->position.x = -bounds.width/2;
    }if (player->position.x < -bounds.width / 2)
    {
        player->position.x = WINDOW_X+ bounds.width/2;
    }if (player->position.y > WINDOW_Y + bounds.height / 2)
    {
        player->position.y = -bounds.height/2;
    }if (player->position.y < - bounds.height / 2)
    {
        player->position.y = WINDOW_Y+bounds.height/2;
    }

    for (int i = 0; i < max_asteroids && steroids[i].text != NULL; i++)
    {
        float distx = fabs(steroids[i].position.x - player->position.x);
        float disty = fabs(steroids[i].position.y - player->position.y);
        float dist = sqrtf(distx * distx + disty * disty);
        sfFloatRect steroidbound = sfText_getGlobalBounds(steroids[i].text);
        if (dist < steroidbound.width/2 + bounds.width/2)
        {
            player->position.x = WINDOW_X/2;
            player->position.y = WINDOW_Y/2;
            player->force.x = 0;
            player->force.y = 0;
            player->rotation = -90.0f;
            player->life -= 1;
        }
    }


    sfText_setRotation(player->text, player->rotation+90);
    sfText_setPosition(player->text, player->position);
}

void drawPlayer(struct Player player) {
    sfRenderWindow_drawText(window, player.text, NULL);
}

void cleanupPlayer(struct Player* player) {
    sfText_destroy(player->text);
}

void drawlive(struct Player* player)
{
    char dlife[20];
    snprintf(dlife, sizeof(dlife), "lives = %d", player->life);
    sfText* Life = sfText_create();
    sfText_setFont(Life, font1);
    sfText_setString(Life, dlife);
    sfText_setCharacterSize(Life, 40);
    sfText_setPosition(Life, (sfVector2f) {0, 9*WINDOW_Y/10 });
    sfText_setFillColor(Life, sfWhite);
    sfRenderWindow_drawText(window, Life, NULL);
}

void game(struct Player* player)
{
    if (ingame) {
        updatePlayer(player);
		updatesteroid();
        sfRenderWindow_clear(window, sfBlack);
        drawPlayer(*player);
		drawsteroid(player);
		drawlive(player);
        
        if (player->life < 1)
        {
            dead = 1;
			ingame = 0;
		}

        sfRenderWindow_display(window);
       
    }
}

void menu()
{
    {
        sfText* title = sfText_create();
        sfText_setFont(title, font1);
        sfText_setString(title, "ASTEROID");
        sfFloatRect bounds = sfText_getLocalBounds(title);
        sfText_setOrigin(title, (sfVector2f) { bounds.width/ 2, bounds.height/2 });
        sfText_setCharacterSize(title, 50);
        sfText_setPosition(title, (sfVector2f) { 4*WINDOW_X / 9, WINDOW_Y / 4 });
        sfText_setFillColor(title, sfWhite);

        //bouton play

        sfRectangleShape* start = sfRectangleShape_create();
        sfRectangleShape_setSize(start, (sfVector2f){ 150,50 });
        sfRectangleShape_setFillColor(start, sfWhite);
        sfRectangleShape_setOrigin(start, (sfVector2f) { 75, 25 });
        sfRectangleShape_setPosition(start, (sfVector2f) { WINDOW_X / 2, WINDOW_Y / 2 });

        sfText* begin = sfText_create();
        sfText_setFont(begin, font1);
        sfText_setString(begin, "play");
        sfFloatRect bounds1 = sfText_getLocalBounds(begin);
        sfText_setOrigin(begin, (sfVector2f) { bounds1.width / 2, bounds1.height/2 });
        sfText_setCharacterSize(begin, 25);
        sfText_setPosition(begin, (sfVector2f) { WINDOW_X / 2 , WINDOW_Y / 2 });
        sfText_setFillColor(begin, sfBlack);

        //bouton fermer
        sfRectangleShape* exit = sfRectangleShape_create();
        sfRectangleShape_setSize(exit, (sfVector2f) { 150, 50 });
        sfRectangleShape_setFillColor(exit, sfWhite);
        sfRectangleShape_setOrigin(exit, (sfVector2f){ 75,25 });
        sfRectangleShape_setPosition(exit, (sfVector2f) { WINDOW_X/2, 3*WINDOW_Y/4 });


        sfText* leave = sfText_create();
        sfText_setFont(leave, font1);
        sfText_setString(leave, "exit");
        sfFloatRect bounds2 = sfText_getLocalBounds(leave);
        sfText_setOrigin(leave, (sfVector2f) { bounds2.width / 2, bounds2.height/2 });
        sfText_setCharacterSize(leave, 25);
        sfText_setPosition(leave, (sfVector2f) {WINDOW_X/2, 3*WINDOW_Y/4});
        sfText_setFillColor(leave, sfBlack);

    
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawRectangleShape(window, exit, NULL);
        sfRenderWindow_drawText(window, leave, NULL);
        sfRenderWindow_drawRectangleShape(window, start, NULL);
        sfRenderWindow_drawText(window, begin, NULL);
        sfRenderWindow_drawText(window, title, NULL);
        sfRenderWindow_display(window);
    
   
        if (sfMouse_isButtonPressed(sfMouseLeft)) {
            sfVector2i mousePosition = sfMouse_getPositionRenderWindow(window);

            if (sfRectangleShape_getGlobalBounds(exit).left <= mousePosition.x &&
                mousePosition.x <= sfRectangleShape_getGlobalBounds(exit).left + sfRectangleShape_getGlobalBounds(exit).width &&
                sfRectangleShape_getGlobalBounds(exit).top <= mousePosition.y &&
                mousePosition.y <= sfRectangleShape_getGlobalBounds(exit).top + sfRectangleShape_getGlobalBounds(exit).height) {
                gameon = 0;
                sfRectangleShape_destroy(exit);
                sfRectangleShape_destroy(start);
                sfText_destroy(leave);
                sfText_destroy(begin);
                sfText_destroy(title);
            }
            else if (sfRectangleShape_getGlobalBounds(start).left <= mousePosition.x &&
                mousePosition.x <= sfRectangleShape_getGlobalBounds(start).left + sfRectangleShape_getGlobalBounds(start).width &&
                sfRectangleShape_getGlobalBounds(start).top <= mousePosition.y &&
                mousePosition.y <= sfRectangleShape_getGlobalBounds(start).top + sfRectangleShape_getGlobalBounds(start).height)
            {
                sfRectangleShape_destroy(exit);
                sfRectangleShape_destroy(start);
                sfText_destroy(leave);
                sfText_destroy(begin);
                sfText_destroy(title);
                ingame = 1;
            }
        }  
    }

}

void gameover(struct Player* player)
{
    if (dead)
    {
        sfRenderWindow_clear(window, sfBlack);

        sfText* GAMEOVER = sfText_create();
        sfText_setFont(GAMEOVER, font1);
        sfText_setString(GAMEOVER, "GAME OVER");
        sfText_setCharacterSize(GAMEOVER, 40);
        sfFloatRect bounds = sfText_getLocalBounds(GAMEOVER);
        sfText_setOrigin(GAMEOVER, (sfVector2f) { bounds.width / 2, bounds.height / 2 });
        sfText_setPosition(GAMEOVER, (sfVector2f) { WINDOW_X / 2, WINDOW_Y / 4 });
        sfText_setFillColor(GAMEOVER, sfWhite);

        sfRectangleShape* exit = sfRectangleShape_create();
        sfRectangleShape_setSize(exit, (sfVector2f) { 150, 50 });
        sfRectangleShape_setFillColor(exit, sfWhite);
        sfRectangleShape_setOrigin(exit, (sfVector2f) { 75, 25 });
        sfRectangleShape_setPosition(exit, (sfVector2f) { WINDOW_X / 2, 3 * WINDOW_Y / 4 });

        sfText* leave = sfText_create();
        sfText_setFont(leave, font1);
        sfText_setString(leave, "exit");
        sfFloatRect bounds2 = sfText_getLocalBounds(leave);
        sfText_setOrigin(leave, (sfVector2f) { bounds2.width / 2, bounds2.height / 2 });
        sfText_setCharacterSize(leave, 25);
        sfText_setPosition(leave, (sfVector2f) { WINDOW_X / 2, 3 * WINDOW_Y / 4 });
        sfText_setFillColor(leave, sfBlack);


        sfRenderWindow_drawText(window, GAMEOVER, NULL);
        sfRenderWindow_drawRectangleShape(window, exit, NULL);
        sfRenderWindow_drawText(window, leave, NULL);
        sfRenderWindow_display(window);


        if (sfMouse_isButtonPressed(sfMouseLeft)) {
            sfVector2i mousePosition = sfMouse_getPositionRenderWindow(window);

            if (sfRectangleShape_getGlobalBounds(exit).left <= mousePosition.x &&
                mousePosition.x <= sfRectangleShape_getGlobalBounds(exit).left + sfRectangleShape_getGlobalBounds(exit).width &&
                sfRectangleShape_getGlobalBounds(exit).top <= mousePosition.y &&
                mousePosition.y <= sfRectangleShape_getGlobalBounds(exit).top + sfRectangleShape_getGlobalBounds(exit).height) {
                gameon = 0;
                sfRectangleShape_destroy(exit);
                sfText_destroy(leave);
                sfText_destroy(GAMEOVER);
            }
        }
    }
}

int main() {
    srand(time(0));
    create();

    struct Player player;
	struct steroid Steroid;
    initPlayer(&player);
    for (int i = 0;i < 9; i++)
    {
        initsteroid(0, 0, 3);
    }
    player.position = (sfVector2f){ WINDOW_X / 2, WINDOW_Y / 2 };

    while (gameon) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(window);
            }
        }

        Delta();

        ////// DRAW /////
        if (!ingame&&!dead)
        {
            menu();
        }
        else if (ingame)
        {
            game(&player);
        }
        else if (dead)
        {
			gameover(&player);
        }
        /////////////////
    }

    sfClock_destroy(deltaclock);
    cleanupPlayer(&player);
    cleanupSteroid();
    sfRenderWindow_destroy(window);
    return 0;
}
