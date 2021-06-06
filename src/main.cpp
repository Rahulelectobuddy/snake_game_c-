#include "main.h"

#include <cstring>
#include <iostream>
#include <queue>

#include <ncurses.h>
#include <signal.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define HORIZONTAL_LENGTH 70
#define VERTICAL_LENTH 30

#define DEBUG

WINDOW *win;
bool Terminated;
int LastKey = 100;
int keyRead,x = 1,y = 2;
int foodPosX=0,foodPosY=0;
std::queue<std::pair<int, int> > snakeCordinates;

void CatchSIG(int sig) {
  Terminated = true;
}

int detectCollison() {
    if( x == 0 || x >= VERTICAL_LENTH || y == 0 || y >= HORIZONTAL_LENGTH)
    {
        mvaddstr(VERTICAL_LENTH/2-1,HORIZONTAL_LENGTH/2-11,"#######################");
        mvaddstr(VERTICAL_LENTH/2,HORIZONTAL_LENGTH/2-11,"###    Game Over    ###");
        mvaddstr(VERTICAL_LENTH/2+1,HORIZONTAL_LENGTH/2-11,"#######################");
        refresh();
        usleep(2000000);
        return 1;
    }
    return 0;
}

void createFood() {
    foodPosX = rand() % VERTICAL_LENTH;
    foodPosY = rand() % HORIZONTAL_LENGTH;
    mvaddch(foodPosX,foodPosY,'&');
}

void updateScreen() {
    snakeCordinates.push({x,y});
    move(snakeCordinates.back().first,snakeCordinates.back().second);
    addch('*');
    refresh();
    move(snakeCordinates.front().first,snakeCordinates.front().second);
    addch(' ');
    refresh();
    if (snakeCordinates.back().first == foodPosX && snakeCordinates.back().second == foodPosY)
        createFood();
    #ifdef DEBUG
        mvprintw(39,0,"%d,%d",snakeCordinates.front().first,snakeCordinates.front().second);
        mvprintw(40,0,"%d,%d",snakeCordinates.back().first,snakeCordinates.back().second);
    #endif
    snakeCordinates.pop();
}

void setupGame() {
    createFood();
    snakeCordinates.push({1,1});
    snakeCordinates.push({1,2});
    for (size_t temp = 0; temp < HORIZONTAL_LENGTH; temp++)
    {
        mvaddch(0,temp,'#');
        mvaddch(VERTICAL_LENTH,temp,'#');
    }
    for (size_t temp = 0; temp < VERTICAL_LENTH; temp++)
    {
        mvaddch(temp,0,'#');
        mvaddch(temp,HORIZONTAL_LENGTH,'#');
    }
}

void moveGame() {
    switch (LastKey)
    {
        case 119:
            x-=1;
            break;
        case 115:
            x+=1;
            break;
        case 100:
            y+=1;
            break;
        case 97:
            y-=1;
            break;
        default:
            break;
    }
    #ifdef DEBUG
        mvprintw(31,0,"%d,%d,%d,%d",keyRead,x,y);
    #endif
}

void checkInput() {
    keyRead = getch();
    napms(20);
    if(keyRead > 0)
    {
        switch (keyRead)
        {
        case 65:
            LastKey = 119;
            break;
        case 66:
            LastKey = 115;
            break;
        case 67:
            LastKey = 100;
            break;
        case 68:
            LastKey = 97;
            break;
        default:
            break;
        }
    }
}

void setupScreen() {
    initscr();
    keypad(stdscr, true);
    cbreak();
    noecho();
    nodelay(stdscr,TRUE);
    win = newwin(0, 0, 0, 0);
}

int main(int argc, char *argv[]) {
    int c = 0;
    signal(SIGINT, CatchSIG);
    setupScreen();
    setupGame();
    while (!Terminated) 
    {
        if(detectCollison())
            break;
        updateScreen();
        checkInput();
        moveGame();
        usleep(100000);
    }
    endwin();
}   
