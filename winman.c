#include "winman.h"

typedef struct win windowList;

typedef struct pan panelList;




/*
* Handles errors with windows. Errnum = 1 means it breaks terminal bounds
* also displays window number (in array) to help debug.
*/
int window_error_handler(int errnum,int winnum,...){
	// get max terminal size.
	int x,y;
	getmaxyx(stdscr,y,x);
	//create window and panel to display error message
	WINDOW *errwin = newwin(y,x,0,0);
	PANEL* errpan = new_panel(errwin);
	//make it the top panel
	top_panel(errpan);
	//color pair 4, i defined as (COLOR_WHITE,COLOR_RED)
	wbkgd(panel_window(errpan),COLOR_PAIR(4));
	//variadic list for differing param numbers. Used for possible
	//future where there may be more errors to handle
	va_list valist;
	va_start(valist,winnum);
	int row=0;
	//print error number
	mvwprintw(panel_window(errpan),row++,x/4,"ERROR NUM: %d", errnum);
	if(errnum==1)
	{
		//error description
		mvwprintw(panel_window(errpan),row++,x/4,"The window you tried to create is too large");
		//window in violation
		mvwprintw(panel_window(errpan),row++,x/4,"WINDOW NUM: %d", winnum);
		//calculating broken bounds
		int ygt, xgt;
		//receive from variadic list
		int ywin = va_arg(valist,int); int xwin = va_arg(valist,int);
		int ypos = va_arg(valist,int); int xpos = va_arg(valist,int);
		ygt=((-y+ywin+ypos) > 0) ? -y+ywin+ypos : 0;
		xgt = ((-x+xwin+xpos) > 0) ? -x+xwin+xpos : 0;
		//output debug info
		mvwprintw(panel_window(errpan),row++,x/4,"WINDOW CLIPS TERM BY: %d row(s), %d col(s)",ygt,xgt);
		mvwprintw(panel_window(errpan),row++,x/4,"press any key to close window");
		update_panels();
		doupdate();
	}
	//wait for user input, then close window.
	getch();
	del_panel(errpan);
	return 0;
}

/*
* Checks and adjusts memory allocation if number of elements exceeds size 
*/
void check_size_panelList(panelList *list)
{
	if(list->n > list->size){
		list->size +=1;
		list->panel_array = realloc(list->panel_array,list->size*sizeof(list->panel_array));
		list->id = ( char**)realloc(list->id,list->size*sizeof(char*));
	}
}
/*
* Checks and adjusts memory allocation if number of elements exceeds size 
*/
void check_size_menuList(menuList *list)
{
	if(list->n > list->size){
		list->size +=1;
		list->menu_array = realloc(list->menu_array,list->size*sizeof(list->menu_array));
	}
}
/*
* Checks and adjusts memory allocation if number of elements exceeds size 
*/
void check_size_windowList( windowList *list)
{
	if(list->n > list->size)
	{
		list->size +=1;
		list->window_array = realloc(list->window_array,list->size*sizeof(list->window_array));
		list->x = realloc(list->x,list->size*sizeof(list->x));
		list->y = realloc(list->y,list->size*sizeof(list->y));
		list->row = realloc(list->row,list->size*sizeof(list->row));
		list->col = realloc(list->col,list->size*sizeof(list->col));
	}
}

/* END UTILITY FUNCTIONS */

/*
*	Creates a new, centered window, placing it offset from parameters
*/
 WINDOW* newwin_offset(int offy, int offx)
{
	int max_y,max_x;
	getmaxyx(stdscr,max_y,max_x);
	int height = max_y-offy;
	int width = max_x-offx;
	WINDOW* win = newwin(height, width, (max_y-height)/2, (max_x-width)/2);
	refresh();
	return win; 
}


/*
* initialize  and allocate memory to a newly created panelList based on size
*/
panelList* init_panelList(size_t size)
{
	panelList* ps;
	ps = malloc(sizeof(panelList*)); 
	ps->size = size;
	ps->n = 0;
	char** tmp = ps->id;
	tmp = (char**)calloc(size,sizeof(char*));
	ps->panel_array = (PANEL**)calloc(size,sizeof(PANEL*));
	ps->id = tmp;
	/*for(int i = 0;i<(int)size;i++)
		ps->id[i] = (char*)calloc(24,sizeof(char));
*/
	ps->wl_ref = init_windowList(size);
	return ps;
}
/*
*  Adds panels. Needs to change to single panels. Variadic not as good as I thought.
*/
panelList* add_panel(panelList* pl,char *id,int y, int x, int starty, int startx){
	panelList *ps = pl;	
	// ps->wl_ref = add_window(ps->wl_ref,y,x,starty,startx);
	check_size_panelList(ps);
	// char** tmp = (pl->id[0]);
	// char* tmp = &id;
	int idlen=0;
	while(id[idlen] != '\0')
		idlen++;
		// printf("%d,",idlen++);
	ps->id[ps->n] = (char*)calloc(idlen,sizeof(char));
	ps->id[ps->n] = id;
 
	// free(tmp);
	// printf("%s",ps->id[0]);
	ps->wl_ref = add_window(ps->wl_ref,y,x,starty,startx);
	ps->panel_array[ps->n] = new_panel(ps->wl_ref->window_array[ps->n]);
 	ps->n++;
	return ps;
}
 
/*panelList* add_panel_offset(panelList* pl, char*id, int offy, int offx)
{
	panelList *ps = pl;	

	check_size_panelList(ps);
	int idlen=0;
	while(id[idlen] != '\0')
		idlen++;
	ps->id[ps->n] = (char*)calloc(idlen,sizeof(char));
	ps->id[ps->n] = id;
 	
	ps->panel_array[ps->n] = new_panel(ps->wl_ref->window_array[ps->n]);
 	ps->n++;
	return ps;
}*/
/*
* move function specifically for the defined structure.
* Makes use of the already-present move_panel function,
* but makes it easier for use of panelList struct
*/
void move_panel_panelList(panelList* pl, int panel_num,int starty, int startx)
{
	move_panel(pl->panel_array[panel_num],startx,starty);
}

char* get_identifier(panelList* pl, int panel_num)
{
	if(panel_num > pl->n || panel_num < 0){
		return (char*)NULL;
	}
	return pl->id[panel_num];
}

/*
* Initialize windowList struct based on size
*/
 windowList* init_windowList(size_t size)
{
	windowList* wl = malloc(sizeof(windowList));
	wl->size = size;
	wl->n = 0;
	wl->row = calloc(size,sizeof(int));
	wl->col = calloc(size,sizeof(int));
	wl->x = calloc(size,sizeof(int));
	wl->y = calloc(size,sizeof(int));
	wl->window_array = (WINDOW**)calloc(size,sizeof(WINDOW*));
	return wl;
}

 windowList* add_window( windowList* wll, int h, int w, int y, int x)
{	
	windowList* wl = wll;
	int limy,limx;
	getmaxyx(stdscr,limy,limx);
	//if created window breaks terminal bounds, handle error
	if((h+y)>=limy || (w+x)>=limx)
	{
		window_error_handler(1,wl->n,h,w,y,x);
	}
	check_size_windowList(wl);
	WINDOW* win = newwin(h,w,y,x);
	
	wl->window_array[wl->n++]= win;
	wl->row[wl->n] = h; wl->col[wl->n] = w;
	wl->y[wl->n] = y; wl->x[wl->n] = x;
	return wl;
}

// int get_startx