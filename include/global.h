#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <time.h>
#include <windows.h>
#include <math.h>
#include <conio.h>

#pragma  comment (lib,"Winmm.lib")

#define bool int
#define TRUE 1
#define FALSE 0
#define max_length_short 64
#define max_length_medium 256
#define max_length_long 1024
#define STACKED_OBJECT_OFFSET 5
#define TARGET_FPS 30
#define REFRESHRATE 1000.0f/TARGET_FPS
#define LINEWIDTH 5
#define APPROACHCIRCLERADIUS 75
#define CIRCLERADIUS 32
#define CIRCLERATIO 0.8f
#define SHADOWRATIO 1.05f
#define AFTER_CLICK_RATIO 1.25f
#define INNER_SHADOWRATIO 0.85f
#define BEZIERPOINTCNT 30
#define BEZIERSTEP 1.0f/BEZIERPOINTCNT
#define FONTHEIGHT 24
#define FONTWIDTH 16
#define BORDERCOLOR WHITE
#define SHADOWCOLOR BLACK
#define KB_KEY_ENTER 13
#define KB_KEY_DIRECTIONAL -32
#define KB_KEY_UP 72
#define KB_KEY_DOWN 80
#define KB_KEY_ESC 27
#define KB_KEY_LEFT 75
#define KB_KEY_RIGHT 77
#define TIME_INTERVAL 200
#define KB_KEY_Q_1 'q'
#define KB_KEY_Q_2 'Q'
#define KB_KEY_A_1 'a'
#define KB_KEY_A_2 'A'
#define KB_KEY_S_1 's'
#define KB_KEY_S_2 'S'
#define KB_KEY_D_1 'd'
#define KB_KEY_D_2 'D'
#define KB_KEY_F_1 'f'
#define KB_KEY_F_2 'F'
#define KB_KEY_G_1 'g'
#define KB_KEY_G_2 'G'
#define KB_KEY_H_1 'h'
#define KB_KEY_H_2 'H'
#define KB_KEY_L_1 'l'
#define KB_KEY_L_2 'L'
#define LANGUAGE_ENGLISH 0
#define LANGUAGE_CHINESE 1
#define PI 3.14159265359f
#define EPS 0.0000000001f