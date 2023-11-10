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

int WINDOW_X = 800;
int WINDOW_Y = 600;
int gameon = 0;
int ingame = 0;
int rot = 0;

float delta = 0;

sfClock* deltaclock;
sfClock* animclock;
int nframe = 0;
int anim_time = 75; 

sfFont* font1;
sfRenderWindow* window;

int Delta() {
    sfTime dtime = sfClock_getElapsedTime(deltaclock);
    delta = sfTime_asMilliseconds(dtime);
    sfClock_restart(deltaclock);
}

int create() {
    sfVideoMode mode = { WINDOW_X, WINDOW_Y, 32 };

    font1 = sfFont_createFromFile("font/ubuntu.ttf");
    window = sfRenderWindow_create(mode, "asteroid",sfClose, NULL);
    sfRenderWindow_setFramerateLimit(window, 60);
    deltaclock = sfClock_create();
    animclock = sfClock_create();
    gameon = 1;
}

void game()
{
    if (ingame)
    {
        sfText* player = sfText_create();
        sfText_setFont(player, font1);
        sfText_setString(player, "A");
        sfText_setCharacterSize(player, 50);
        sfText_setPosition(player, (sfVector2f) { WINDOW_X / 2, WINDOW_Y / 2 });
        sfText_setOrigin(player, (sfVector2f) { 25, 25 });
        if (sfKeyboard_isKeyPressed(sfKeyLeft))
        {
            rot+=3;
        }
        else if (sfKeyboard_isKeyPressed(sfKeyRight))
        {
            rot-=3;
        }
        sfText_setRotation(player, rot);
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawText(window, player, NULL);
        sfRenderWindow_display(window);
    }
}


void menu()
{
    if (!ingame)
    {
        sfText* title = sfText_create();
        sfText_setFont(title, font1);
        sfText_setString(title, "ASTEROID");
        sfText_setOrigin(title, (sfVector2f) { sizeof(title) / 2, 50 / 2 });
        sfText_setCharacterSize(title, 50);
        sfText_setPosition(title, (sfVector2f) { WINDOW_X / 2  -110, WINDOW_Y / 4 });
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
        sfText_setOrigin(begin, (sfVector2f) { sizeof(begin) / 2, 25 / 2 });
        sfText_setCharacterSize(begin, 25);
        sfText_setPosition(begin, (sfVector2f) { WINDOW_X / 2 - 20, WINDOW_Y / 2 });
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
        sfText_setOrigin(leave, (sfVector2f) { sizeof(leave)/2, 25 / 2 });
        sfText_setCharacterSize(leave, 25);
        sfText_setPosition(leave, (sfVector2f) {WINDOW_X/2-20, 3*WINDOW_Y/4});
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
        menu();
        game();
        /////////////////
    }

    sfClock_destroy(deltaclock);
    sfRenderWindow_destroy(window);
    return 0;
}