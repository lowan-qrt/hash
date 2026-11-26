#ifndef UI_H
#define UI_H

#include <iup.h>

/**
 * Builds the main window and its widgets (a multiline text area).
 *
 * IupOpen() must have been called first. The window is not shown:
 * the caller displays it, and IupClose() releases it.
 *
 * @return the main window (dialog)
 */
Ihandle *ui_create_main_window(void);

#endif
