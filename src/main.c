// Entry point: starts IUP, opens the main window and waits until it is closed.
//
#include <stdlib.h>
#include <stdio.h>
#include <iup.h>

int main(int argc, char **argv)
{
    if (IupOpen(&argc, &argv) == IUP_ERROR) return EXIT_FAILURE;

    Ihandle *text = IupText();
    // MULTILINE must be set before the window is shown
    IupSetAttribute(text, "MULTILINE", "YES");
    IupSetAttribute(text, "EXPAND", "YES");

    // A dialog holds a single child
    Ihandle *window = IupDialog(text);
    IupSetAttribute(window, "TITLE", "Hash");
    IupSetAttribute(window, "SIZE", "HALFxHALF");

    IupShowXY(window, IUP_CENTER, IUP_CENTER);

    // Blocks here until the window is closed
    IupMainLoop();

    IupClose();

    return EXIT_SUCCESS;
}
