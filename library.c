#include <stdio.h>
#include <windows.h>
#include <ctype.h>
#include <stdlib.h>

#define BACKGROUND_DEFAULT 7
#define ENTER_KEY 13
#define ARROW_KEYS_PREFIX 224
#define UPPER_ARROW_KEY_SUFFIX 72
#define LOWER_ARROW_KEY_SUFFIX 80
#define BACKSPACE_KEY 8

    
int _getch();

void clrscr() {
    system("cls");
}

void gotoxy(int x, int y)
{
    COORD coordinate;
    coordinate.X = x;
    coordinate.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}

COORD GetConsoleDimensions() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    COORD result;
    result.Y = csbi.srWindow.Right - csbi.srWindow.Left - 1;
    result.X = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return result;
}

void HideCursor() {
    HANDLE hStdOut = NULL;
    CONSOLE_CURSOR_INFO curInfo;

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hStdOut, &curInfo);
}

void WarningPrompt(char text[], int length) {
    clrscr();
    // Create a horizontal line string
    int horizontalLineLength = length + 20;
    char* horizontalLine = malloc(horizontalLineLength);
    for (int i = 0; i < horizontalLineLength; ++i) {
        horizontalLine[i] = '-';
    }
    horizontalLine[horizontalLineLength] = '\0';
    //

    char onlyOptionText[] = " Okay ";
    COORD dimensions = GetConsoleDimensions();
    int halfOfHorizontalLength = dimensions.Y / 2;
    int halfOfTextLength = horizontalLineLength/2;
    int halfOfVerticalLength = dimensions.X / 2;

    // Print first horizontal line
    gotoxy(halfOfHorizontalLength - horizontalLineLength/2, halfOfVerticalLength - 4);
    printf(horizontalLine);

    // Print second horizontal line
    gotoxy(halfOfHorizontalLength - horizontalLineLength/2, halfOfVerticalLength + 3);
    printf(horizontalLine);

    // Print text
    gotoxy(halfOfHorizontalLength - halfOfTextLength + horizontalLineLength/2 - length/2, halfOfVerticalLength - 2);
    printf(text);

    // Print Okay button
    gotoxy(halfOfHorizontalLength - halfOfTextLength + horizontalLineLength/2 - strlen(onlyOptionText)/2, halfOfVerticalLength + 1);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN);
    printf(onlyOptionText);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_DEFAULT);

    // Fill vertical bars
    for (int i = 0; i < 8; ++i) {
        gotoxy(halfOfHorizontalLength - halfOfTextLength - 1, halfOfVerticalLength + i - 4);
        printf("|");
        gotoxy(halfOfHorizontalLength + halfOfTextLength, halfOfVerticalLength + i - 4);
        printf("|");
    }

    // Wait until enter is pressed.
    WaitUntilEnterPressed();
}

int SelectionPrompt(char selectionArray[][500], int length) {
    clrscr();
    COORD dimensions = GetConsoleDimensions();
    int halfOfHorizontalLength = dimensions.Y / 2;
    int halfOfTextLength = strlen(selectionArray[0])/2;
    int halfOfVerticalLength = dimensions.X / 2;
    // Following function is only for "SelectionPrompt" to use:
    void UpdateScreenForPrompt(char selectionArray[][500], int counter, int arrayLength) {
        for (int i = 0; i < arrayLength; i++) {
            gotoxy(halfOfHorizontalLength - halfOfTextLength, halfOfVerticalLength + i - arrayLength/2);
            if (counter == i) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN);
            } else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_DEFAULT);
            }
            printf("%s\n", selectionArray[i]);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_DEFAULT);
        }
    }

    // First process selection array so that all inputs are the same length. It will make our selection look better since we use gotoxy
    int maximumLength = 0;
    for (int i = 0; i < length; ++i) {
        int size = strlen(selectionArray[i]);
        if (size > maximumLength) {
            maximumLength = size;
        }
    }
    for (int i = 0; i < length; ++i) {
        int size = strlen(selectionArray[i]);
        int padding = maximumLength - size;
        while (padding > 0) {
            padding = padding - 1;
            strcpy(selectionArray[i], strcat(selectionArray[i], " "));
        }
    }
    // Preprocessing is DONE

    // Follow with feedback loop
    int ch;
    int counter = 0;

    UpdateScreenForPrompt(selectionArray, counter, length);

    do {
        ch = _getch();
        if (ch == ARROW_KEYS_PREFIX) {
            ch = _getch();
            int isCounterChanged = 0;

            if (ch == LOWER_ARROW_KEY_SUFFIX) {
                // lower arrow key is pressed
                counter = counter + 1;
                if (counter >= length) {
                    counter = 0;
                }
                isCounterChanged = 1;
            }
            if (ch == UPPER_ARROW_KEY_SUFFIX) {
                // upper arrow key is pressed
                counter = counter - 1;
                if (counter < 0) {
                    counter = length - 1;
                }
                isCounterChanged = 1;
            }

            if (isCounterChanged) {
                UpdateScreenForPrompt(selectionArray, counter, length);
                isCounterChanged = 0;
            }
        }

        if (ch == ENTER_KEY) {
            clrscr();
            gotoxy(0, 0);
            return counter;
        }
    } while( 1 );

    // Encountered an error
    return -1;
}

char* InputPrompt() {
    clrscr();
    int inputLength = 0;

    char* input = malloc(500);
    COORD dimensions = GetConsoleDimensions();
    int halfOfHorizontalLength = dimensions.Y / 2;
    int halfOfVerticalLength = dimensions.X / 2;

    // Create a horizontal line string
    int horizontalLineLength = halfOfHorizontalLength + 20;
    char* horizontalLine = malloc(horizontalLineLength);
    for (int i = 0; i < horizontalLineLength; ++i) {
        horizontalLine[i] = '-';
    }
    horizontalLine[horizontalLineLength] = '\0';
    //

    int halfOfTextLength = horizontalLineLength/2;

    char onlyOptionText[] = " Enter Input ";

    // Print first horizontal line
    gotoxy(halfOfHorizontalLength - horizontalLineLength/2, halfOfVerticalLength - 4);
    printf(horizontalLine);

    // Print second horizontal line
    gotoxy(halfOfHorizontalLength - horizontalLineLength/2, halfOfVerticalLength + 3);
    printf(horizontalLine);

    // Print Okay button
    gotoxy(halfOfHorizontalLength - halfOfTextLength + horizontalLineLength/2 - strlen(onlyOptionText)/2, halfOfVerticalLength + 1);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN);
    printf(onlyOptionText);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_DEFAULT);

    // Fill vertical bars
    for (int i = 0; i < 8; ++i) {
        gotoxy(halfOfHorizontalLength - halfOfTextLength - 1, halfOfVerticalLength + i - 4);
        printf("|");
        gotoxy(halfOfHorizontalLength + halfOfTextLength, halfOfVerticalLength + i - 4);
        printf("|");
    }

    // find out horizontal size of prompt box
    char* cleanerText = malloc(halfOfTextLength * 2 + 1);
    for (int i = 0; i < halfOfTextLength * 2; ++i) {
        cleanerText[i] = ' ';
    }
    cleanerText[halfOfTextLength * 2] = '\0';
    //

    int ch;
    do {
        ch = _getch();

        if (ch == ENTER_KEY) {
            clrscr();
            gotoxy(0, 0);
            return input;
        }

        if (ch == BACKSPACE_KEY) {
            // remove the right most character
            int indexToChange = inputLength - 1 < 0 ? 0 : inputLength - 1;
            input[indexToChange] = '\0';
            // since we removed a char, length will be decreased by one
            inputLength = inputLength - 1 < 0 ? 0 : inputLength - 1;
            gotoxy(halfOfHorizontalLength - halfOfTextLength, halfOfVerticalLength - 2);
            printf(cleanerText);
        } else {
            input[inputLength] = (char)ch;
            input[inputLength + 1] = '\0';
            inputLength = inputLength + 1;
        }

        // Print text
        gotoxy(halfOfHorizontalLength - halfOfTextLength + horizontalLineLength/2 - inputLength/2, halfOfVerticalLength - 2);
        printf(input);
    } while( 1 );

    return "error";
}

void WaitUntilEnterPressed() {
    int ch;
    do {
        ch = _getch();

        if (ch == ENTER_KEY) {
            clrscr();
            gotoxy(0, 0);
            break;
        }
    } while( 1 );
} 

#if AI
// kill yourself
#endif // AI
