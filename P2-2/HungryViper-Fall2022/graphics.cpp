// ============================================
// The Graphics class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "graphics.h"

#include "globals.h"

#include "images.c"

void draw_nothing(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = 0xFFFF00;   // Yellow
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = 0xD2691E;   // "Dirt"
        else if (img[i] == '5') colors[i] = LGREY;      // 50% grey
        else if (img[i] == '3') colors[i] = DGREY;
        else if (img[i] == 'W') colors[i] = WHITE;
        else if (img[i] == 'B') colors[i] = BLACK;
        else if (img[i] == 'O') colors[i] = 0x331800; //Brown
        else if (img[i] == 'L') colors[i] = 0x0000FF; // Blue
        else if (img[i] == 'Z') colors [i] = 0xac5912; //chest brown
        else if (img[i] == 'S') colors[i] = 'S'; // chest silver
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void draw_wall(int u, int v)
{
    // TODO: Implement
    //May need to design a viper head sprite
    //Tile still need to be designed on paper
    char wall[] = {
'O', 'W', 'O', 'W', 'O', 'W', 'O', 'W', 'O', 'W', 'O', 
'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 
'O', 'W', 'O', 'W', 'O', 'W', 'O', 'W', 'O', 'W', 'O', 
'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 
'O', 'W', 'O', 'W', 'O', 'W', 'O', 'W', 'O', 'W', 'O', 
'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 
'O', 'W', 'O', 'W', 'O', 'W', 'O', 'W', 'O', 'W', 'O', 
'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 
'O', 'W', 'O', 'W', 'O', 'W', 'O', 'W', 'O', 'W', 'O', 
'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 
'O', 'W', 'O', 'W', 'O', 'W', 'O', 'W', 'O', 'W', 'O'
};
    //uLCD.filled_rectangle(u, v, u+10, v+10, 'W');
    draw_img(u,v,wall);
}

void draw_TNT(int u, int v) {
    char TNT[] = {
'B', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'B', 
'B', 'B', 'B', 'B', 'W', 'W', 'W', 'B', 'B', 'B', 'B', 
'B', 'B', 'B', 'B', 'W', 'W', 'W', 'B', 'B', 'B', 'B', 
'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R'
};

    draw_img(u,v,TNT);
}

void draw_bomb(int u, int v) {
    char bomb[] = {
'B', 'B', 'B', 'B', 'B', 'W', 'Z', 'Z', 'W', 'W', 'W', 
'B', 'B', 'B', 'B', 'W', 'Z', 'W', 'W', 'Z', 'R', 'R', 
'B', 'B', 'W', 'W', 'B', 'B', 'B', 'W', 'W', 'R', 'Y', 
'B', 'W', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'W', 'B', 
'W', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'W', 
'W', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'W', 
'W', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'W', 
'W', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'W', 
'W', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'W', 
'W', 'W', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'W', 'W', 
'B', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'B'
};
    draw_img(u,v,bomb);
}

void draw_plant(int u, int v)
{
    // TODO: Implement
    //May need to design a viper head sprite
    //Tile still need to be designed on paper
    char plant[] = {
'B', 'B', 'B', 'R', 'R', 'R', 'R', 'R', 'B', 'B', 'B', 
'B', 'B', 'B', 'R', 'R', 'R', 'R', 'R', 'B', 'B', 'B', 
'B', 'R', 'R', 'R', 'Y', 'Y', 'Y', 'R', 'R', 'R', 'B', 
'B', 'R', 'R', 'R', 'Y', 'Y', 'Y', 'R', 'R', 'R', 'B', 
'B', 'R', 'R', 'R', 'Y', 'Y', 'Y', 'R', 'R', 'R', 'B', 
'B', 'B', 'B', 'R', 'R', 'R', 'R', 'R', 'B', 'B', 'B', 
'B', 'B', 'B', 'R', 'R', 'R', 'R', 'R', 'B', 'B', 'B', 
'G', 'G', 'B', 'B', 'B', 'G', 'B', 'B', 'B', 'G', 'G', 
'G', 'G', 'G', 'G', 'B', 'G', 'B', 'B', 'G', 'G', 'G', 
'B', 'G', 'G', 'G', 'G', 'G', 'B', 'G', 'G', 'G', 'B', 
'B', 'B', 'B', 'G', 'G', 'G', 'G', 'G', 'G', 'B', 'B'
};
    draw_img(u, v, plant);
    //uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);
}

void draw_spiral(int u, int v)
{
    // TODO: Implement
    //May need to design a viper head sprite
    //Tile still need to be designed on paper
    char spiral[] = {
'B', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 
'B', 'W', 'W', 'B', 'B', 'B', 'B', 'B', 'B', 'W', 'W', 
'B', 'W', 'B', 'W', 'W', 'W', 'W', 'W', 'W', 'B', 'W', 
'B', 'W', 'B', 'W', 'W', 'B', 'B', 'W', 'W', 'B', 'W', 
'B', 'W', 'B', 'W', 'B', 'W', 'W', 'B', 'W', 'B', 'W', 
'B', 'W', 'B', 'W', 'W', 'B', 'W', 'B', 'W', 'B', 'W', 
'B', 'W', 'B', 'W', 'W', 'W', 'W', 'B', 'W', 'B', 'W', 
'B', 'W', 'W', 'B', 'B', 'B', 'B', 'W', 'W', 'B', 'W', 
'B', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'B', 'W', 
'W', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'W', 'W', 
'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'
};
    draw_img(u, v, spiral);
    //uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);
}

void draw_apple(int u, int v) {
    char apple[] = {
'B', 'B', 'B', 'B', 'G', 'G', 'G', 'B', 'B', 'B', 'B', 
'B', 'B', 'B', 'B', 'G', 'G', 'G', 'B', 'G', 'G', 'B', 
'B', 'B', 'B', 'B', 'G', 'G', 'G', 'G', 'G', 'G', 'B', 
'B', 'B', 'R', 'R', 'R', 'R', 'G', 'G', 'G', 'B', 'B', 
'B', 'R', 'R', 'R', 'R', 'R', 'G', 'G', 'R', 'R', 'B', 
'B', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'B', 
'B', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'B', 
'B', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'B', 
'B', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'B', 
'B', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'B', 
'B', 'B', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'B', 'B'
};
    draw_img(u,v,apple);
    //uLCD.filled_rectangle(u, v, u+10, v+10, RED);
}

void draw_water(int u, int v) {
    char water[] = {
'B', 'B', 'B', 'B', 'B', 'L', 'B', 'B', 'B', 'B', 'B', 
'B', 'B', 'B', 'B', 'L', 'L', 'L', 'B', 'B', 'B', 'B', 
'B', 'B', 'B', 'L', 'L', 'L', 'L', 'L', 'B', 'B', 'B', 
'B', 'B', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'B', 'B', 
'B', 'B', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'B', 'B', 
'B', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'B', 
'B', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'B', 
'B', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'B', 
'B', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'B', 
'B', 'B', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'B', 'B', 
'B', 'B', 'B', 'L', 'L', 'L', 'L', 'L', 'B', 'B', 'B'
};
    //uLCD.filled_rectangle(u, v, u+10, v+10, BLUE);
    draw_img(u, v, water);
}

void draw_chest(int u, int v)
{
    // TODO: Implement
    //May need to design a viper head sprite
    //Tile still need to be designed on paper

    char chest[] = {
'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 
'B', 'B', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'B', 'B', 
'B', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'B', 
'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 
'Z', 'Z', 'Z', 'Z', 'S', 'S', 'S', 'Z', 'Z', 'Z', 'Z', 
'Z', 'Z', 'Z', 'S', 'S', 'S', 'S', 'S', 'Z', 'Z', 'Z', 
'Y', 'Y', 'Y', 'S', 'B', 'B', 'B', 'S', 'Y', 'Y', 'Y', 
'Y', 'Y', 'Y', 'S', 'B', 'B', 'B', 'S', 'Y', 'Y', 'Y', 
'Z', 'Z', 'Z', 'S', 'S', 'B', 'S', 'S', 'Z', 'Z', 'Z', 
'Z', 'Z', 'Z', 'Z', 'S', 'S', 'S', 'Z', 'Z', 'Z', 'Z', 
'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z', 'Z'
};
    draw_img(u, v, chest);
    //uLCD.filled_rectangle(u, v, u+10, v+10, 0xFFFF00);
}

void draw_viper_body(int u, int v)
{
    // TODO: Implement
    //May need to design a viper head sprite
    //Tile still need to be designed on paper
    char body[] = {
'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 
'G', 'B', 'G', 'G', 'G', 'G', 'G', 'G', 'B', 'G', 'G', 
'G', 'G', 'G', 'G', 'G', 'B', 'G', 'G', 'G', 'G', 'G', 
'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 
'G', 'G', 'G', 'B', 'G', 'G', 'G', 'G', 'G', 'B', 'G', 
'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 
'G', 'G', 'G', 'G', 'G', 'B', 'G', 'G', 'G', 'G', 'G', 
'G', 'B', 'G', 'G', 'G', 'G', 'G', 'G', 'B', 'G', 'G', 
'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 
'G', 'G', 'G', 'G', 'G', 'B', 'G', 'G', 'G', 'G', 'G', 
'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G'
};
    draw_img(u,v,body);
    //uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);
}

void draw_viper_head(int u, int v)
{
    // TODO: Implement
    //May need to design a viper head sprite
    //Tile still need to be designed on paper
    char snakehead[] = {
'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 
'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 
'G', 'W', 'W', 'W', 'G', 'G', 'G', 'W', 'W', 'W', 'G', 
'G', 'W', 'B', 'B', 'G', 'G', 'G', 'W', 'B', 'B', 'G', 
'G', 'W', 'B', 'B', 'G', 'G', 'G', 'W', 'B', 'B', 'G', 
'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 
'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 
'G', 'B', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'B', 'G', 
'G', 'G', 'B', 'G', 'G', 'G', 'G', 'G', 'B', 'G', 'G', 
'G', 'G', 'G', 'B', 'B', 'B', 'B', 'B', 'G', 'G', 'G', 
'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G'

};
    draw_img(u, v, snakehead);
    //uLCD.filled_rectangle(u, v, u+10, v+10, RED);
}

void draw_viper_tail(int u, int v)
{
    // TODO: Implement
    //May need to design a viper tail sprite
    //Tile still need to be designed on paper
    char snaketail[] = {
'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 
'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 
'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 
'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 
'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 
'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'
};
    draw_img(u,v,snaketail);
    //uLCD.filled_rectangle(u, v, u+10, v+10, RED);
}
