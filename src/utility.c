#include "utility.h"

/*
* For the moment being, this is a dump for utility functions unless I can
* find a proper place for them.
*/

void print_matrix(intMatrix *m)
{
	int rlim = m->r;
	int clim = m->c;
	printf(" ");
	for(int c=0; c<clim; c++)
		printf("__%d__",c);
	printf("\n");
	for(int r=0; r<rlim; r++)
	{
		printf("%d|",r);
		for(int c=0; c<clim; c++){
			if(m->m[r][c] != 9)
				printf(" %d |  ",m->m[r][c]);
			else
				printf(" * |  ");
		}
		printf("\n");
	}
}

int msb(unsigned int n)
{
	int msb=0;
	if(n==0) return msb;
	while((n/=2) >0) msb++;
	return msb;
}

int binary_search(int arr[], int arr_size, int key)
{
/*	int *arr_tmp = malloc(arr_size*sizeof(int));
	arr_tmp = &arr;*/
	int low = 0;
	int high =arr_size-1;
	
	int indx=(high-low)/2;
	int curr_num = arr[indx];
	// printf("%d, ",dindx);
	while( (key != curr_num) && indx>=1)
	{
		if(curr_num > key)
			high = indx;
		if(curr_num<key)
			low = indx+1;
		indx = (high+low)/2;
		curr_num = arr[indx];
		printf("%d\n",low);

	}
	printf("\n\n%d\n",curr_num);
	if(curr_num!=key) return -1;
	return indx;
}

void menu_loop(SCENE* scene,int mn)
{
	if(scene->has_menus == false)
	{
		printw("No menus ya fool");
		return;
	}

	int ch=0;
	keypad(scene_menu_win(scene,mn),TRUE);
	nodelay(scene_menu_win(scene,mn),TRUE);
	while(ch!=122)
	{
		ch=wgetch(scene_menu_win(scene,mn));
		switch(ch)
		{
			case KEY_UP:
				menu_driver(scene->ml->menu_array[mn],REQ_UP_ITEM);
			break;
			case KEY_DOWN:
				menu_driver(scene->ml->menu_array[mn],REQ_DOWN_ITEM);
			break;
		}
		refresh();
		post_menu(scene->ml->menu_array[mn]);
		doupdate();
		update_panels();
	}
	keypad(scene_menu_win(scene,mn),FALSE);
	nodelay(scene_menu_win(scene,mn),FALSE);
}