// Builds the main window and its widgets.
#include <stdlib.h>

#include "file.h"
#include "theme.h"
#include "ui.h"

static Ihandle *editor;

// Menu callbacks: IUP calls them when the item is clicked
static int on_quit(Ihandle *item)
{
    (void)item;          // unused parameter
    return IUP_CLOSE;
}

/** Shows the file dialog and loads the chosen Markdown file into the editor. */
static int on_open(Ihandle *item)
{
    (void)item;          // unused parameter

    Ihandle *file_dialog = IupFileDlg();
    IupSetAttribute(file_dialog, "DIALOGTYPE", "OPEN");
    IupSetAttribute(file_dialog, "EXTFILTER", "Markdown|*.md|");
    IupPopup(file_dialog, IUP_CENTER, IUP_CENTER);

    // STATUS is -1 when the user cancels
    if (IupGetInt(file_dialog, "STATUS") != -1)
    {
        char *path = IupGetAttribute(file_dialog, "VALUE");
        char *content = file_read(path);

        if (content == NULL) IupMessage("Error", "Could not read the file");
        else
        {
            IupSetStrAttribute(editor, "VALUE", content);
            free(content);
        }
    }

    // path belongs to the dialog: destroy it only after use
    IupDestroy(file_dialog);

    return IUP_DEFAULT;
}

Ihandle *ui_create_main_window(void)
{
    editor = IupText();
    // MULTILINE must be set before the window is shown
    IupSetAttribute(editor, "MULTILINE", "YES");
    IupSetAttribute(editor, "EXPAND", "YES");
    IupSetAttribute(editor, "BGCOLOR", THEME_BG);
    IupSetAttribute(editor, "FGCOLOR", THEME_FG);

    // A dialog holds a single child
    Ihandle *window = IupDialog(editor);
    IupSetAttribute(window, "TITLE", "Hash");
    IupSetAttribute(window, "SIZE", "HALFxHALF");

    // File menu: built from the inside out (items, list, submenu, bar)
    Ihandle *item_open = IupMenuItem("Open");
    Ihandle *separator = IupMenuSeparator();
    Ihandle *item_quit = IupMenuItem("Quit");
    Ihandle *file_list = IupMenu(item_open, separator, item_quit, NULL);
    Ihandle *file_menu = IupSubmenu("File", file_list);
    Ihandle *menu_bar  = IupMenu(file_menu, NULL);
    IupSetAttributeHandle(window, "MENU", menu_bar);
    IupSetCallback(item_open, "ACTION", on_open);
    IupSetCallback(item_quit, "ACTION", on_quit);

    return window;
}
