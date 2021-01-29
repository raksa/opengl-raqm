#ifndef FUNS
#define FUNS

void selectFont(int newfont);
void selectMessage(int msg);
void selectColor(int color);
void tick(void);
void output(int x, int y, char *string);
void display(void);
void reshape(int w, int h);
int init_resources();
void free_resources();

#endif