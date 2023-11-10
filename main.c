#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SFML/Window.h>
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <SFML/System.h>


typedef int bool;
#define true 1
#define false 0

struct Player {
    sfText* text;        // La représentation graphique du joueur
    sfVector2f position; // La position du joueur
    float rotation;        // L'angle de rotation du joueur
};

int WINDOW_X = 800;
int WINDOW_Y = 600;
int gameon = 0;
int ingame = 0;

float delta = 0;

sfClock* deltaclock;
sfClock* animclock;
int nframe = 0;
int anim_time = 75; 

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
    animclock = sfClock_create();
    gameon = 1;
}

void initPlayer(struct Player* player) {
    player->text = sfText_create();
    sfText_setFont(player->text, font1);
    sfText_setString(player->text, "A");
    sfText_setCharacterSize(player->text, 50);
    sfFloatRect bounds = sfText_getLocalBounds(player->text);
    sfText_setOrigin(player->text, (sfVector2f) { bounds.width / 2, bounds.height });
    sfText_setFillColor(player->text, sfWhite);

    player->rotation = 0.0f;

} 

void updatePlayer(struct Player* player) {
    if (sfKeyboard_isKeyPressed(sfKeyLeft)) {
        player->rotation += 3;
    }
    else if (sfKeyboard_isKeyPressed(sfKeyRight)) {
        player->rotation -= 3;
    }

    sfText_setRotation(player->text, player->rotation);
    sfText_setPosition(player->text, player->position);
}

void drawPlayer(struct Player player) {
    sfRenderWindow_drawText(window, player.text, NULL);
}

void cleanupPlayer(struct Player* player) {
    sfText_destroy(player->text);
}

void game(struct Player* player)
{
    if (ingame) {
        updatePlayer(player);

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawText(window, player->text, NULL);
        //drawPlayer(*player);
       
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
        sfText_setOrigin(title, (sfVector2f) { bounds.width / 2, bounds.height });
        sfText_setCharacterSize(title, 50);
        sfText_setPosition(title, (sfVector2f) { WINDOW_X / 2, WINDOW_Y / 4 });
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
        sfText_setOrigin(begin, (sfVector2f) { bounds1.width / 2, bounds1.height });
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
        sfText_setOrigin(leave, (sfVector2f) { bounds2.width / 2, bounds2.height });
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

int main() {
    srand(time(0));
    create();

    struct Player player;
    initPlayer(&player);
    player.position = (sfVector2f){ WINDOW_X / 2, WINDOW_Y / 2 };

    while (gameon) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        Delta();


        ////// DRAW /////
        sfTime atime = sfClock_getElapsedTime(animclock); //for animated frames
        int amseconds = sfTime_asMilliseconds(atime);
        if (amseconds >= anim_time) {
            nframe += 1;
            sfClock_restart(animclock);
        }
        if (!ingame)
        {
            menu();
        }
        else if (ingame)
        {
            game(&player);
        }
        /////////////////
    }

    sfClock_destroy(deltaclock);
    cleanupPlayer(&player);
    sfRenderWindow_destroy(window);
    return 0;
}