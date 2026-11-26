// Builds the main window and its widgets.
#include "theme.h"
#include "ui.h"

Ihandle *ui_create_main_window(void)
{
    Ihandle *text = IupText();
    // MULTILINE must be set before the window is shown
    IupSetAttribute(text, "MULTILINE", "YES");
    IupSetAttribute(text, "EXPAND", "YES");
    IupSetAttribute(text, "BGCOLOR", THEME_BG);
    IupSetAttribute(text, "FGCOLOR", THEME_FG);

    // A dialog holds a single child
    Ihandle *window = IupDialog(text);
    IupSetAttribute(window, "TITLE", "Hash");
    IupSetAttribute(window, "SIZE", "HALFxHALF");

    return window;
}
