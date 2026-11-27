// Entry point: starts IUP, opens the main window and waits until it is closed.
//
#include <stdlib.h>
#include <iup.h>

#include "ui.h"

int main(int argc, char **argv)
{
    if (IupOpen(&argc, &argv) == IUP_ERROR) return EXIT_FAILURE;

    Ihandle *window = ui_create_main_window();
    IupShowXY(window, IUP_CENTER, IUP_CENTER);

    // Blocks here until the window is closed
    IupMainLoop();

    IupClose();

    return EXIT_SUCCESS;
}
