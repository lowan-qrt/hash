// Builds the main window and its widgets.
#include <stdlib.h>
#include <stdio.h>

#include "file.h"
#include "theme.h"
#include "ui.h"

static Ihandle *editor;
static char current_path[4096];      // current opened file
static int is_modified = 0;          // 1 if there are unsaved changes
static int confirm_discard(void);    // defined further down

// Menu callbacks: IUP calls them when the item is clicked
/** Shows the file dialog and loads the chosen Markdown file into the editor. */
static int on_open(Ihandle *item)
{
    (void)item;          // unused parameter
    if (!confirm_discard()) return IUP_DEFAULT;

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
            snprintf(current_path, sizeof(current_path), "%s", path);
            is_modified = 0;
            IupSetStrAttribute(editor, "VALUE", content);
            free(content);
        }
    }

    // path belongs to the dialog: destroy it only after use
    IupDestroy(file_dialog);

    return IUP_DEFAULT;
}

/** Asks for a path and saves the editor content into it. */
static int on_save_as(Ihandle *item)
{
    (void)item;           // unused parameter

    Ihandle *file_dialog = IupFileDlg();
    IupSetAttribute(file_dialog, "DIALOGTYPE", "SAVE");
    IupSetAttribute(file_dialog, "EXTFILTER", "Markdown|*.md|");
    IupPopup(file_dialog, IUP_CENTER, IUP_CENTER);

    if (IupGetInt(file_dialog, "STATUS") != -1)
    {
        char *path = IupGetAttribute(file_dialog, "VALUE");
        char *content = IupGetAttribute(editor, "VALUE");

        if (file_write(path, content) == -1)
        {
            IupMessage("Error", "Could not save the file");
        } else
        {
            snprintf(current_path, sizeof(current_path), "%s", path);
            is_modified = 0;
        }
    }

    IupDestroy(file_dialog);

    return IUP_DEFAULT;
}

/** Saves into the current file, or asks for a path if there is none. */
static int on_save(Ihandle *item)
{
    // No current file yet: behave like "Save as..."
    if (current_path[0] == '\0')
    {
        return on_save_as(item);
    }

    char *content = IupGetAttribute(editor, "VALUE");

    if (file_write(current_path, content) == -1)
    {
        IupMessage("Error", "Could not save the file");
    } else is_modified = 0;

    return IUP_DEFAULT;
}

/** Called each time the user edits the text: marks the content as modified. */
static int on_text_changed(Ihandle *item)
{
    (void)item;
    is_modified = 1;
    return IUP_DEFAULT;
}

/** Returns 1 if it is safe to drop the editor content, 0 to cancel. */
static int confirm_discard(void)
{
    if (!is_modified) return 1;

    const int choice = IupAlarm("Unsaved changes", "Save your changes?",
                                "Save", "Don't save", "Cancel");

    if (choice == 1)
    {
        on_save(NULL);
        return !is_modified;   // still 1 if saving failed or was cancelled
    }

    return choice == 2;
}

/** Window close button and Cmd+Q: keeps the window open if the user cancels. */
static int on_close(Ihandle *window)
{
    (void)window;

    if (confirm_discard()) return IUP_DEFAULT;
    else return IUP_IGNORE;
}

/** Closes the application, unless the user cancels. */
static int on_quit(Ihandle *item)
{
    (void)item;          // unused parameter
    if (confirm_discard()) return IUP_CLOSE;
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
    IupSetCallback(editor, "VALUECHANGED_CB", on_text_changed);

    // A dialog holds a single child
    Ihandle *window = IupDialog(editor);
    IupSetAttribute(window, "TITLE", "Hash");
    IupSetAttribute(window, "SIZE", "HALFxHALF");
    IupSetCallback(window, "CLOSE_CB", on_close);

    // File menu: built from the inside out (items, list, submenu, bar)
    Ihandle *separator = IupMenuSeparator();
    Ihandle *item_open = IupMenuItem("Open");
    Ihandle *item_save = IupMenuItem("Save");
    Ihandle *item_save_as = IupMenuItem("Save as...");
    Ihandle *item_quit = IupMenuItem("Quit");
    Ihandle *file_list = IupMenu(item_open, item_save, item_save_as, separator, item_quit, NULL);
    Ihandle *file_menu = IupSubmenu("File", file_list);
    Ihandle *menu_bar  = IupMenu(file_menu, NULL);
    IupSetAttributeHandle(window, "MENU", menu_bar);
    IupSetCallback(item_open, "ACTION", on_open);
    IupSetCallback(item_save, "ACTION", on_save);
    IupSetCallback(item_save_as, "ACTION", on_save_as);
    IupSetCallback(item_quit, "ACTION", on_quit);

    return window;
}
