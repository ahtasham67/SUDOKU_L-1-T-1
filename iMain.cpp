#include "iGraphics.h"
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <MMsystem.h>
#include <windows.h>
#include <conio.h>
using namespace std;
// DEFINING MACROS
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 650
#define BOARD_X 70
#define BOARD_Y 100
#define CELL_LENGTH 45
#define CELL_SPACING 10
#define TEXTBOX_X 15
#define TEXTBOX_Y 15
#define SUBGRID_CELL_LENGTH 120
#define SUBGRID_CELL_SPACING 45
#define MENU_START_X 250
#define MENU_START_Y 350
#define RETURN_X 20
#define RETURN_Y 618
#define START_GAME_X 255
#define START_GAME_Y 335
#define HOW_TO_PLAY_X 255
#define HOW_TO_PLAY_Y 290
#define QUIT_GAME_X 255
#define QUIT_GAME_Y 245
#define SCORE_BOX_X 600
#define SCORE_BOX_Y 350
#define ROWS 9
#define COLUMNS 9

int level = 0;
int currClickedI = -1;
int currClickedJ = -1;
int mode = 0;
char input;
char currNum;
char errorBox[50] = "";
char winbox[50] = "";
int game_start = 0;
int how_to_play = 0;
int win_window = 0;
int SUBGRID[3][3];
int randlevel;
int levelwindow = 0;
char time_str[20];
int score = 0;
int enterlevel = 0;
char score_temporary[10];

// FUNCTION PROTOTYPES
int isComplete();
int isValid(int num);
void maingameplay(void);
void initLevel(int lvl);
void initialtemplates();
void solutiontemplates();
void freearray(int m, int **a);
void takeInput(unsigned char key);
void checkGridClicks(int mx, int my);
void printmenubar(int x, int y, int dx, int dy, char ch[]);
int isPointInRectangle(int px, int py, int x, int y, int w, int h);

// GRID VARIABLES
int Grid[9][9];
int sGrid[9][9];
int **initialGrid;
int **solutionGrid;

// FUNCTION FOR  COPYING THE TEMPLATES FROM FILE
void initialtemplates()
{

    initialGrid = (int **)malloc(ROWS * sizeof(int *));
    assert(initialGrid != NULL);

    for (int i = 0; i < ROWS; i++)
    {
        initialGrid[i] = (int *)malloc(COLUMNS * sizeof(int));
        assert(initialGrid[i] != NULL);
    }
    FILE *file;
    file = fopen("template1.txt", "r");

    int row = 0;

    while (!feof(file))
    {
        for (int i = 0; i < COLUMNS; i++)
        {

            if (fscanf(file, "%d", &initialGrid[row][i]) == EOF)
                break;
        }

        if (row == ROWS)
            break;
        row++;
    }
    fclose(file);
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            Grid[i][j] = initialGrid[i][j];
            {
                Grid[i][j] = initialGrid[i][j];
                ; // printf("%d ", Grid[i][j]);
            }
        }
        // printf("\n");
    }
}

void solutiontemplates()
{

    solutionGrid = (int **)malloc(ROWS * sizeof(int *));
    assert(solutionGrid != NULL);

    for (int i = 0; i < ROWS; i++)
    {
        solutionGrid[i] = (int *)malloc(COLUMNS * sizeof(int));
        assert(solutionGrid[i] != NULL);
    }
    FILE *file;
    file = fopen("template1solve.txt", "r");

    int row = 0;

    while (!feof(file))
    {
        for (int i = 0; i < COLUMNS; i++)
        {

            if (fscanf(file, "%d", &solutionGrid[row][i]) == EOF)
                break;
        }

        if (row == ROWS)
            break;
        row++;
    }
    fclose(file);
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            sGrid[i][j] = solutionGrid[i][j];
            {
                sGrid[i][j] = solutionGrid[i][j];
                // printf("%d ", sGrid[i][j]);
            }
        }
        // printf("\n");
    }
}

void freearray(int m, int **a)
{
    for (int i = 0; i < m; i++)
    {
        if (a[i] != NULL)
            free(a[i]);
    }
    if (a != NULL)
        free(a);
}
void iDraw()
{
    // place your drawing codes here
    iClear();
    // THE  MAIN WINDOW SHOWING SUDOKU
    if (game_start == 1)
    {
        iShowBMP2(0, 0, "woodenbg.bmp", 0x00ffffff);
        maingameplay();
        iSetColor(224, 255, 255);
        iFilledRectangle(RETURN_X + 20, RETURN_Y - 5, 127, 28);
        printmenubar(RETURN_X - 5, RETURN_Y - 10, 120, 32, "<=RETURN");

        // SHOWING SCOREBOX
        printmenubar(SCORE_BOX_X, SCORE_BOX_Y, 120, 32, "SCORE");
        iSetColor(255, 255, 255);
        iFilledRectangle(SCORE_BOX_X, SCORE_BOX_Y - 30, 127, 28);

        itoa(score, score_temporary, 10);

        iSetColor(0, 0, 0);
        iText(SCORE_BOX_X + 55, SCORE_BOX_Y - 22.5, score_temporary, GLUT_BITMAP_HELVETICA_18);
    }
    // SHOWING THE WINDOW FOR MENUBAR
    if (game_start == 0)
    {
        // iSetColor(0,0,255);
        // iFilledRectangle(0,0,1000,1000);
        iShowBMP2(0, 0, "nature.bmp", 0x00ffffff);
        iSetColor(0, 0, 0);
        printmenubar(START_GAME_X, START_GAME_Y, 176, 28, "START GAME");
        printmenubar(HOW_TO_PLAY_X, HOW_TO_PLAY_Y, 190, 28, "HOW TO PLAY");
        printmenubar(QUIT_GAME_X, QUIT_GAME_Y, 155, 28, "QUIT GAME");

        if (how_to_play == 1)
        {
            iShowBMP(0, 0, "HOWTOPLAY.bmp");
            iSetColor(220, 220, 220);
            iFilledRectangle(RETURN_X + 10, RETURN_Y + 3, 150, 32);
            printmenubar(RETURN_X, RETURN_Y, 120, 32, "<=RETURN");
        }
        return;

        // working on level
        /* if (enterlevel == 1)
         {
             iShowBMP(0, 0, "HOWTOPLAY.bmp");
             printmenubar(START_GAME_X,START_GAME_Y,176,28,"EASY");
          printmenubar(HOW_TO_PLAY_X,HOW_TO_PLAY_Y,190,28,"MEDIUM");
          printmenubar(QUIT_GAME_X,QUIT_GAME_Y,155,28,"HARD");
          printmenubar(RETURN_X-5,RETURN_Y-10,120,32,"<=RETURN");
         }
         return;
         */
    }
    // SHOWING WIN WINDOW//
    if (win_window == 1)
    {
        iShowBMP(0, 0, "win_edited.bmp");
        iSetColor(220, 220, 220);
        iFilledRectangle(330, 615, 165, 32);
        printmenubar(320, 610, 127, 28, "QUIT GAME");
        iFilledRectangle(HOW_TO_PLAY_X, HOW_TO_PLAY_Y, 150, 30);
        iText(380, 208, score_temporary, GLUT_BITMAP_TIMES_ROMAN_24);
    }

    // iText(20 + 25, 20 + 13, time_str, GLUT_BITMAP_TIMES_ROMAN_24);

    // iText(100+BOARD_X, 100+BOARD_Y - 50, winbox, GLUT_BITMAP_TIMES_ROMAN_24);
}
void maingameplay(void)
{
    // iSetColor(230,230,230);
    // iFilledRectangle(0,0,1000,1000);

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            iSetColor(255, 255, 0);
            if (currClickedI == i && currClickedJ == j)
            {
                if (strcmp(errorBox, "") == 0)
                {
                    // SETTING COLOUR BOX SELECTION--->BLUE
                    iSetColor(0, 0, 255);
                }
                else
                {
                    iSetColor(255, 0, 0);
                }
                iFilledRectangle(BOARD_X + (i * CELL_LENGTH + CELL_SPACING * i), BOARD_Y + (j * CELL_LENGTH + CELL_SPACING * j), CELL_LENGTH, CELL_LENGTH);
                iFilledRectangle(BOARD_X + (i * CELL_LENGTH + CELL_SPACING * i) + 1, BOARD_Y + (j * CELL_LENGTH + CELL_SPACING * j) + 1, CELL_LENGTH - 2, CELL_LENGTH - 2);
            }
            else
            {
                iSetColor(85, 85, 85);
                iFilledRectangle(BOARD_X + (i * CELL_LENGTH + CELL_SPACING * i), BOARD_Y + (j * CELL_LENGTH + CELL_SPACING * j), CELL_LENGTH, CELL_LENGTH);
            }
            iSetColor(0, 255, 0);
            currNum = (char)Grid[i][j] + '0';
            iSetColor(255, 255, 255);
            if (Grid[i][j] != 0)
            {
                iText(BOARD_X + TEXTBOX_X + (i * CELL_LENGTH + CELL_SPACING * i), BOARD_Y + TEXTBOX_Y + (j * CELL_LENGTH + CELL_SPACING * j), &currNum, GLUT_BITMAP_HELVETICA_18);
            }
        }
    }

    // DRAWING THE SUBGRID
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            iSetColor(0, 0, 0);
            iRectangle(BOARD_X - 7.5 + (i * SUBGRID_CELL_LENGTH + SUBGRID_CELL_SPACING * i), BOARD_Y - 7.5 + (j * SUBGRID_CELL_LENGTH + SUBGRID_CELL_SPACING * j), CELL_LENGTH + 125, CELL_LENGTH + 125);
        }
    }

    // SHOWING WINBOX FOR GAME END
    iSetColor(0, 255, 0);
    iText(700 + BOARD_X, 300 + BOARD_Y - 50, winbox, GLUT_BITMAP_TIMES_ROMAN_24);

    // SHOWING ERRORBOX FOR WRONG ANSWER
    iSetColor(200, 0, 0);
    iText(BOARD_X, BOARD_Y - 50, errorBox, GLUT_BITMAP_TIMES_ROMAN_24);
    // iShowBMP2(50,50,"download.bmp",0x00ffffff)
}

// PRINTING MENUBAR FOR SHOWING MANUALS AND INSTRUCTIONS
void printmenubar(int x, int y, int dx, int dy, char ch[])
{
    iSetColor(0, 200, 0);
    // iFilledRectangle(x,y,dx,dy);
    iSetColor(0, 0, 0);
    iText(x + 25, y + 13, ch, GLUT_BITMAP_TIMES_ROMAN_24);
}

/*void initLevel(int lvl)
{
    level = lvl;

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            Grid[i][j] = initialGrid[lvl][i][j];
        }
    }
}*/

int isValid(int num)
{
    if (Grid[currClickedI][currClickedJ] == 0)
    {
        return 0;
    }
    else if (Grid[currClickedI][currClickedJ] != sGrid[currClickedI][currClickedJ])
    {
        return 0;
    }

    return 1;
}

int isComplete()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (Grid[i][j] == 0)
            {
                return 0;
            }
        }
    }
    return 1;
}

void takeInput(unsigned char key)
{
    int i;
    if (mode == 1)
    {
        if (key == '\r')
        {
            if (isValid(Grid[currClickedI][currClickedJ]))
            {
                mode = 0;
                currClickedI = -1;
                currClickedJ = -1;

                strcpy(errorBox, "");

                if (isComplete())
                {
                    FILE * score = fopen("score.txt", "a");
                    fprintf(score, "%s \n", score_temporary);
                    fclose(score);
                    // strcpy(winbox, "WON!!!");
                    win_window = 1;
                }

                score += 20;
            }
            else
            {
                strcpy(errorBox, "NOT VALID INPUT");
                score -= 10;
            }
        }
        else if (key == '\b')
        {
            Grid[currClickedI][currClickedJ] = 0;
        }
        else if (key >= '1' && key <= '9')
        {
            Grid[currClickedI][currClickedJ] = key - '0';
        }
    }
}

int isPointInRectangle(int px, int py, int x, int y, int w, int h)
{
    if (px >= x && px <= (x + w) && py >= y && py <= (y + h))
    {
        return 1;
    }
    return 0;
}

void checkGridClicks(int mx, int my)
{
    printf("%d %d\n", mx, my);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (isPointInRectangle(mx, my, BOARD_X + (i * CELL_LENGTH + CELL_SPACING * i), BOARD_Y + (j * CELL_LENGTH + CELL_SPACING * j), CELL_LENGTH, CELL_LENGTH))
            {
                if (mode == 1)
                {
                    if (Grid[currClickedI][currClickedJ] == 0)
                        Grid[currClickedI][currClickedJ] = 0;
                    mode = 0;
                    strcpy(errorBox, "");
                }

                // if(Grid[i][j] != 0)
                //{
                if (Grid[i][j] == 0)
                {
                    currClickedI = i;
                    currClickedJ = j;

                    mode = 1;

                    break;
                }
                //}
            }
        }
    }
}
/*
    function iMouseMove() is called when the user presses and drags the mouse.
    (mx, my) is the position where the mouse pointer is.
    */
void iMouseMove(int mx, int my)
{
}
/*
    function iMouse() is called when the user presses/releases the mouse.
    (mx, my) is the position where the mouse pointer is.
    */
// GRID SELECTION CHECKS
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        checkGridClicks(mx, my);
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
    }
    // GAME START BUTTON EXECUTION
    if (mx >= START_GAME_X && mx <= START_GAME_X + 180 && my >= START_GAME_Y && my <= START_GAME_Y + 32)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            if (game_start == 0)
            {
                game_start = 1;
                how_to_play = 0;
            }
        }
    }

    // HOW TO PLAY WINDOW OPEN EXECUTION

    if (mx >= HOW_TO_PLAY_X && mx <= HOW_TO_PLAY_X + 190 && my >= HOW_TO_PLAY_Y && my <= HOW_TO_PLAY_Y + 28)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            if (game_start == 0)
            {
                how_to_play = (how_to_play == 0) ? 1 : 0;
            }
        }
    }
    // enterlevel
    /*if (mx >= 500 && mx <= 500 + 190 && my >= 500 && my <= 500 + 150)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            if (game_start == 0)
            {
                enterlevel = (enterlevel == 0) ? 1 : 0;
            }
        }
    }*/

    // RETURN FROM HOW TO PLAY WINDOW

    if (mx >= RETURN_X && mx <= RETURN_X + 120 && my >= RETURN_Y && my <= RETURN_Y + 32)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            how_to_play = (how_to_play == 1) ? 0 : 1;
            game_start == 0;
        }
    }
    // RETURN FROM MAIN GAME WINDOW

    if (mx >= 40 && mx <= 168 && my >= 611 && my <= 641)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            game_start = (game_start == 1) ? 0 : 1;
            game_start = 0;
            how_to_play = 0;
        }
    }

    if (mx >= QUIT_GAME_X && mx <= QUIT_GAME_X + 155 && my >= QUIT_GAME_Y && my <= QUIT_GAME_Y + 32)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            exit(0);
        }
    }
    // EXECUTING QUIT BUTTON
    if (mx >= 330 && mx <= 330 + 165 && my >= 615 && my <= 615 + 32)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            if (win_window == 1)
                exit(0);
        }
    }
}

/*
    function iKeyboard() is called whenever the user hits a key in keyboard.
    key- holds the ASCII value of the key pressed.
    */
void iKeyboard(unsigned char key)
{
    takeInput(key);

    // place your codes for other keys here
}

/*
function iSpecialKeyboard() is called whenver user hits special keys like-
function keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{

    if (key == GLUT_KEY_END)
    {
        exit(0);
    }
    // place your codes for other keys here
}
int main()
{

    PlaySound(TEXT("bgmusic.wav"), NULL, SND_ASYNC | SND_LOOP);


    srand(time(0));
    randlevel = rand() % 3;
    // initLevel(0);

    initialtemplates();
    solutiontemplates();
    iInitialize(WINDOW_WIDTH, WINDOW_HEIGHT, "Sudoku");
    freearray(ROWS, initialGrid);
    freearray(ROWS, solutionGrid);
    // printf("%d",score);
    return 0;
}
