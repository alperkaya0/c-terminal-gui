#include "library.h"

int main()
{
    HideCursor();

    char helloString[] = "You are warned for a reason we don't know for now!";

    WarningPrompt(helloString, sizeof(helloString));

    // You should use string length of 500. Visually it doesn't change anything.
    char exampleStrings[3][500] = {"great string 1","unbelievable string 2","evangelion string 3"};

    int counter = SelectionPrompt(exampleStrings, sizeof(exampleStrings)/sizeof(exampleStrings[0]));

    printf("You selected this:%s!\nPress Enter to Continue...\n", exampleStrings[counter]);

    WaitUntilEnterPressed();

    printf("You entered this input: %s", InputPrompt());

    return 0;
}
