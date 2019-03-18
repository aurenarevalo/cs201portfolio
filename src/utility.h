#include <stdio.h>
#include <stdlib.h>
#include <menu.h>
#include <ncurses.h>
#include "infection_ds.h"
#include "scene.h"


void print_matrix(intMatrix *m);

int msb(unsigned int n);

int binary_search(int arr[], int arr_size, int key);
void menu_loop(SCENE* scene,int mn);