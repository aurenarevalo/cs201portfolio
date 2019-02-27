#include "menumanager.h"


typedef struct choice choiceList;

typedef struct m menuList;


WINDOW** win;
MENU* mm;


choiceList* init_choiceList(size_t n)
{
	choiceList * list = (choiceList*)NULL;
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

void resize_handler(int sig)
{
	clear();
	unpost_menu(mm);
	// format_menu();
	refresh();
}

/*void create_margin(WIN* win,int left, int right, int top, int bot)
{

}

void create_border(WIN* win, int width, int height,char bchar)
{

}*/


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
