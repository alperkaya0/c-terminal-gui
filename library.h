#include <windows.h>

extern int _getch();
extern void clrscr();
extern void gotoxy(int x, int y);
extern COORD GetConsoleDimensions();
extern void WarningPrompt(char text[], int length);
extern int SelectionPrompt(char selectionArray[][500], int length);
extern char* InputPrompt();
extern void WaitUntilEnterPressed();
