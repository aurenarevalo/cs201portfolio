#include "scene.h"

typedef struct choice choiceList;

typedef struct win windowList;

typedef struct pan panelList;

typedef struct m menuList;

typedef struct scene SCENE;



/*
* Handles errors with windows. Errnum = 1 means it breaks terminal bounds
* also displays window number (in array) to help debug.
*/
int window_error_handler(int errnum,...){
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
	va_start(valist,errnum);
	int row=0;
	//print error number
	// mvwprintw(panel_window(errpan),row++,x/4,"ERROR NUM: %d", errnum);
	if(errnum==1)
	{
		//error description
		mvwprintw(panel_window(errpan),row++,x/4,"The window you tried to create is too large");
		//window in violation
		mvwprintw(panel_window(errpan),row++,x/4,"WINDOW NUM: %d", va_arg(valist,int));
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

	if(errnum==2)
	{
		printf("ERROR 2: Out of bounds exception: trying to access menu or panel not yet defined\n");
		printf("Attempted to access menu -%d- and panel -%d-\n",va_arg(valist,int),va_arg(valist,int));
		printf("Menus defined: %d \n Panels defined: %d\n",va_arg(valist,int),va_arg(valist,int));
		printf("No windows assigned to menu\n");
	}
	if(errnum==3)
	{
		printf("ERROR 3: Attempt to set menu where none exist in specified scene.\n");
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

/*****
** PANEL LIST FUNCTIONS
****/


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
*  Adds panels to panelList
*/
void add_panel(SCENE* scene,char *id,int y, int x, int starty, int startx){	
	// scene->pl = init_panelList(2);
	check_size_panelList(scene->pl);
	int idlen=0;
	while(id[idlen] != '\0')
		idlen++;
	scene->pl->id[scene->pl->n] = (char*)calloc(idlen,sizeof(char));
	scene->pl->id[scene->pl->n] = id;
	scene->pl->wl_ref = add_window(scene->pl->wl_ref,y,x,starty,startx);
	scene->pl->panel_array[scene->pl->n] = new_panel(scene->pl->wl_ref->window_array[scene->pl->n]);
 	scene->pl->n++;
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

WINDOW* panelList_window(panelList* pl, int indx)
{
	if(indx > pl->n)
	{
		printf("PANEL DOESN'T EXIST: %d\n",indx);
		return NULL;
	}
	return panel_window(pl->panel_array[indx]);
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

// WINDOW* get_win_windowList(windowList* wl, int indx)


/***********
****CHOICE LIST FNS
*********/

choiceList* init_choiceList(size_t n)
{
	choiceList * list = malloc(sizeof(choiceList));
	list->choice_array = calloc(n,sizeof(char**));
	list->size = n;
	list->n = 0;
	return list;
}

choiceList* add_choices(const char *str,...)
{
	choiceList* list = init_choiceList(2);
	va_list valist;
	va_start(valist,str);
	int i=0;
	while(str){
		list->n++;
		check_size(list);
		list->choice_array[i++] = str;
		str = va_arg(valist,const char*);
	}
	va_end(valist);
	return list;
}

void check_size(choiceList *list)
{
	if(list->n > list->size){
		list->size +=1;
		list->choice_array = realloc(list->choice_array,list->size*sizeof(list->choice_array));
	}
}

ITEM** set_items(choiceList *list)
{
	ITEM** items;
	printf("\n%d\n",(int)list->size+1);
	items = (ITEM**)calloc(list->n+1,sizeof(ITEM*));
	for(int i=0;i<(int)list->size;i++){
		items[i]=new_item(list->choice_array[i],"");
	}
	items[(int)list->size]=(ITEM*)NULL;
	return items;
}

MENU* create_menu(choiceList *list)
{
	ITEM** items= (ITEM**)calloc(list->n+1,sizeof(ITEM*));
	MENU* menu = malloc(sizeof(MENU*));
	items =set_items(list);
	menu = new_menu((ITEM**)items);
	return menu;
}


MENU* _new_menu(va_list args)
{
	choiceList* list = init_choiceList(2);
	// va_list valist;
	// va_start(valist,str);
	const char *str;
	str= va_arg(args,const char*);
	int i=0;
	while(str){
		printw("%s\n",str );
		list->n++;
		check_size(list);
		list->choice_array[i++] = str;
		str = va_arg(args,const char*);
	}
	// va_end();

	MENU* menu =create_menu(list);
	return menu;
}

menuList* init_menuList(size_t size)
{
	menuList* ml = malloc(sizeof(menuList));
	ml->menu_array = (MENU**)malloc(size*sizeof(MENU*));
	// for(int i=0;i<size; i++){
	// 	ml->menu_array[i] = new_menu(NULL);
	// }
	ml->n = 0;
	ml->size = size;
	return ml;
}

void add_menu(SCENE* scene,...)
{
	// SCENE* ptr = *scene;
	// scene->ml = init_menuList(2);
	scene->has_menus = true;
	// ptr->ml->n++;
	check_size_menuList(scene->ml);
	// choiceList* list = init_choiceList(2);
	va_list valist;
	va_start(valist,scene);

	MENU* menu =_new_menu(valist);

	va_end(valist);

	scene->ml->menu_array[scene->ml->n++] = menu;

}

void assign_menu(PANEL* p, MENU** menu)
{
	MENU* tmp_menu = *menu;
	// WINDOW* ptr = panel_window(pl->panel_array[pn]);
	 set_menu_win(tmp_menu,panel_window(p));
}

void assign_menu_sub(MENU** menu,int xpad,int ypad, int offy, int offx)
{
	MENU* ptr = *menu;
	WINDOW* mainwin = menu_win(ptr);
	int xlim,ylim;
	getmaxyx(mainwin,ylim,xlim);
	WINDOW* sub = derwin(mainwin,ylim-ypad,xlim-xpad,offy,offx);
	set_menu_sub(ptr,sub);
}

/*
* sets scene's menu window and subwindow.
* finds menu based on menu num, finds window to assign based on panel num
* creates subwin based on xpad thru offx
*/

void set_smenu_winsub(SCENE* scene, int mn,int pn,int xpad, int ypad, int offy, int offx)
{
	// SCENE* ptr = *scene;
	if(mn > scene->ml->n || pn > scene->pl->n){
		window_error_handler(2,mn,pn,scene->ml->n,scene->pl->n);
		return;
	}
	else if(scene->has_menus == false){
		window_error_handler(3);
		return;
	}
	assign_menu(scene->pl->panel_array[pn],&scene->ml->menu_array[mn]);
	assign_menu_sub(&scene->ml->menu_array[mn],xpad,ypad,offy,offx);
}

void set_smenu_win(SCENE* scene, int mn, int pn)
{
	if(mn > scene->ml->n || pn > scene->pl->n){
		window_error_handler(2,mn,pn,scene->ml->n,scene->pl->n);
		return;
	}
	else if(scene->has_menus == false){
		window_error_handler(3,scene->has_menus);
		return;
	}
	assign_menu(scene->pl->panel_array[pn],&scene->ml->menu_array[mn]);
}

WINDOW* scene_window(SCENE* scene, int pn)
{
	/* add BETTER error handle*/
	if(pn > scene->pl->n || pn <0)
		mvprintw(2,2,"Hey dumbass, panel %d doesn't exist ",pn);
	return panel_window(scene->pl->panel_array[pn]);
}

WINDOW* scene_menu_win(SCENE* scene, int mn)
{
	/* add BETTER error handle*/
	if(mn > scene->ml->n || scene->has_menus == false || mn <0)
		mvprintw(2,2,"Hey dumbass, menu %d doesn't exist ",mn);
	return menu_win(scene->ml->menu_array[mn]);
}



/*void create_margin(WIN* win,int left, int right, int top, int bot)
{

}t

void create_border(WIN* win, int width, int height,char bchar)
{

}*/

SCENE* init_scene(size_t panels, size_t menus)
{
	SCENE* scene = malloc(sizeof(SCENE));
	scene->pl = init_panelList(panels);
	if(menus >0){
		printf("initializing menus");
		scene->has_menus=true;
		scene->ml = init_menuList(menus);
	}
	else
	{
		scene->has_menus = false;
		scene->ml = NULL;
	}
	return scene;
}










void resize_handler(int sig)
{
	clear();
	// unpost_menu(mm);
	// format_menu();
	refresh();
}
void mainMenuLoop()
{
	wchar_t ch; 	
	while((ch=getch())!=122){
		/*switch(ch)
		{
			case (int)KEY_DOWN:
			case  (int)KEY_s:
				menu_driver(mm,REQ_DOWN_ITEM);
				break;
			case (int)KEY_w :
			case (int)KEY_UP :
				menu_driver(mm,REQ_UP_ITEM);
				break;
			case (int)ENTER_KEY:
				mvprintw(0,0, "Item selected is : %s", 
						item_name(current_item(mm)));
				pos_menu_cursor(mm);
				break;
			case (int)KEY_RESIZE:
				resize_handler(1);
				break;
			default:
				break;
			}*/
			/*post_menu(mm);
			refresh();
			wrefresh(mm_win);*/
		mvprintw(0,0,"Hello");
		refresh();

		}
}
