void makeRasterFont(void);

void init(void);

void printString(char *s);

/* Everything above this line could be in a library 
 * that defines a font.  To make it work, you've got 
 * to call makeRasterFont() before you start making 
 * calls to printString().
 */
void display(void);

void reshape(int w, int h);

/* ARGSUSED1 */
void keyboard(unsigned char key, int x, int y);