/*
 _________
/         \ tinyfiledialogs.h
|tiny file| Unique header file of "tiny file dialogs" created [November 9, 2014]
| dialogs | Copyright (c) 2014 - 2016 Guillaume Vareille http://ysengrin.com
\____  ___/ http://tinyfiledialogs.sourceforge.net
     \|           	                     mailto:tinfyfiledialogs@ysengrin.com

A big thank you to Don Heyse http://ldglite.sf.net for
                   his code contributions, bug corrections & thorough testing!
		
            git://git.code.sf.net/p/tinyfiledialogs/code

Please
	1) let me know
	- if you are including tiny file dialogs,
	  I'll be happy to add your link to the list of projects using it.
	- If you are using it on different hardware / OS / compiler.
	2) Be the first to leave a review on Sourceforge. Thanks.

tiny file dialogs (cross-platform C C++)
InputBox PasswordBox MessageBox ColorPicker
OpenFileDialog SaveFileDialog SelectFolderDialog
Native dialog library for WINDOWS MAC OSX (10.4~10.11) GTK+ QT CONSOLE & more
v2.5 [Juin 25, 2016] zlib licence

A single C file (add it to your C or C++ project) with 6 boxes:
- message / question
- input / password
- save file
- open file & multiple files
- select folder
- color picker.

Complements OpenGL GLFW GLUT GLUI VTK SFML SDL Ogre Unity ION
CEGUI MathGL CPW GLOW IMGUI GLT NGL STB & GUI less programs

NO INIT
NO MAIN LOOP

The dialogs can be forced into console mode

Windows [UTF-8 + UTF-16]
- native code & some vbs create the graphic dialogs
- enhanced console mode can use dialog.exe from
http://andrear.altervista.org/home/cdialog.php
- basic console input

Unix [UTF-8] (command line call attempts)
- applescript
- zenity / matedialog
- kdialog
- Xdialog
- python2 tkinter
- dialog (opens a console if needed)
- basic console input
The same executable can run across desktops & distributions

tested with C & C++ compilers
on VisualStudio MinGW Mac Linux Bsd Solaris Minix Raspbian C# fortran (iso_c)
using Gnome Kde Enlightenment Mate Cinnamon Unity
Lxde Lxqt Xfce WindowMaker IceWm Cde Jds OpenBox

- License -

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software.  If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef TINYFILEDIALOGS_H
#define TINYFILEDIALOGS_H

extern char tinyfd_version[8];

extern int tinyfd_forceConsole;  /* 0 (default) or 1
can be modified at run time.
for unix & windows: 0 (graphic mode) or 1 (console mode).
0: try to use a graphic solution, if it fails then it uses console mode.
1: forces all dialogs into console mode even when the X server is present.
it will use the package dialog or dialog.exe if installed.
on windows it only make sense for console applications */

extern char tinyfd_response[1024];
/* if you pass "tinyfd_query" as aTitle,
the functions will not display the dialogs
but will fill tinyfd_response with
the retain solution and return:
0 for console mode, 1 for graphic mode
possible values for tinyfd_response are (all lowercase)
for the graphic mode:
windows applescript zenity zenity3 matedialog kdialog
xdialog tkinter gdialog gxmessage xmessage
for the console mode:
dialog whiptail basicinput */

/* #define TINYFD_WIN_CONSOLE_ONLY //*/
/* On windows, define TINYFD_WIN_CONSOLE_ONLY here
if you don't want to include the code creating the graphic dialogs.
Then you won't need to link against Comdlg32.lib and Ole32.lib */


/*
if tinydialogs.c is compiled with a C++ compiler rather than with a C compiler
(ie. you change the extension from .c to .cpp), you need to comment out:
extern "C" {
and the corresponding closing bracket:
}
*/

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

int tinyfd_messageBox (
	char const * const aTitle , /* "" */
	char const * const aMessage , /* "" may contain \n \t */
	char const * const aDialogType , /* "ok" "okcancel" "yesno" */
	char const * const aIconType , /* "info" "warning" "error" "question" */
	int const aDefaultButton ) ; /* 0 for cancel/no , 1 for ok/yes */
		/* returns 0 for cancel/no , 1 for ok/yes */

char const * tinyfd_inputBox (
	char const * const aTitle , /* "" */
	char const * const aMessage , /* "" may NOT contain \n \t on windows */
	char const * const aDefaultInput ) ;  /* "" , if NULL it's a passwordBox */
	/* returns NULL on cancel */

char const * tinyfd_saveFileDialog (
	char const * const aTitle , /* "" */
	char const * const aDefaultPathAndFile , /* "" */
	int const aNumOfFilterPatterns , /* 0 */
	char const * const * const aFilterPatterns , /* NULL | {"*.jpg","*.png"} */
	char const * const aSingleFilterDescription ) ; /* NULL | "text files" */
		/* returns NULL on cancel */

char const * tinyfd_openFileDialog (
	char const * const aTitle , /* "" */
	char const * const aDefaultPathAndFile , /* "" */
	int const aNumOfFilterPatterns , /* 0 */
	char const * const * const aFilterPatterns , /* NULL {"*.jpg","*.png"} */
	char const * const aSingleFilterDescription , /* NULL | "image files" */
	int const aAllowMultipleSelects ) ; /* 0 or 1 */
		/* in case of multiple files, the separator is | */
		/* returns NULL on cancel */

char const * tinyfd_selectFolderDialog (
	char const * const aTitle , /* "" */
	char const * const aDefaultPath ) ; /* "" */
	/* returns NULL on cancel */

char const * tinyfd_colorChooser(
	char const * const aTitle , /* "" */
	char const * const aDefaultHexRGB , /* NULL or "#FF0000" */
	unsigned char const aDefaultRGB[3] , /* { 0 , 255 , 255 } */
	unsigned char aoResultRGB[3] ) ; /* { 0 , 0 , 0 } */
	/* returns the hexcolor as a string "#FF0000" */
	/* aoResultRGB also contains the result */
	/* aDefaultRGB is used only if aDefaultHexRGB is NULL */
	/* aDefaultRGB and aoResultRGB can be the same array */
	/* returns NULL on cancel */


/************ NOT CROSS PLATFORM SECTION STARTS HERE ************************/
#ifdef _WIN32
#ifndef TINYFD_WIN_CONSOLE_ONLY

/* windows only */
wchar_t const * tinyfd_utf8to16(char const * const aUtf8string);

/* windows only */
char const * tinyfd_utf16to8(wchar_t const * const aUtf16string);

/* windows only - utf-16 version */
int tinyfd_messageBoxW(
	wchar_t const * const aTitle ,
	wchar_t const * const aMessage, /* "" may contain \n \t */
	wchar_t const * const aDialogType, /* "ok" "okcancel" "yesno" */
	wchar_t const * const aIconType, /* "info" "warning" "error" "question" */
	int const aDefaultButton ); /* 0 for cancel/no , 1 for ok/yes */
		/* returns 0 for cancel/no , 1 for ok/yes */

/* windows only - utf-16 version */
wchar_t const * tinyfd_saveFileDialogW(
	wchar_t const * const aTitle, /* NULL or "" */
	wchar_t const * const aDefaultPathAndFile, /* NULL or "" */
	int const aNumOfFilterPatterns, /* 0 */
	wchar_t const * const * const aFilterPatterns, /* NULL or {"*.jpg","*.png"} */
	wchar_t const * const aSingleFilterDescription); /* NULL or "image files" */

/* windows only - utf-16 version */
wchar_t const * tinyfd_openFileDialogW(
	wchar_t const * const aTitle, /* "" */
	wchar_t const * const aDefaultPathAndFile, /* "" */
	int const aNumOfFilterPatterns , /* 0 */
	wchar_t const * const * const aFilterPatterns, /* NULL {"*.jpg","*.png"} */
	wchar_t const * const aSingleFilterDescription, /* NULL | "image files" */
	int const aAllowMultipleSelects ) ; /* 0 or 1 */
		/* in case of multiple files, the separator is | */
		/* returns NULL on cancel */

/* windows only - utf-16 version */
	wchar_t const * tinyfd_selectFolderDialogW(
	wchar_t const * const aTitle, /* "" */
	wchar_t const * const aDefaultPath); /* "" */
		/* returns NULL on cancel */

/* windows only - utf-16 version */
wchar_t const * tinyfd_colorChooserW(
	wchar_t const * const aTitle, /* "" */
	wchar_t const * const aDefaultHexRGB, /* NULL or "#FF0000" */
	unsigned char const aDefaultRGB[3] , /* { 0 , 255 , 255 } */
	unsigned char aoResultRGB[3] ) ; /* { 0 , 0 , 0 } */
		/* returns the hexcolor as a string "#FF0000" */
		/* aoResultRGB also contains the result */
		/* aDefaultRGB is used only if aDefaultHexRGB is NULL */
		/* aDefaultRGB and aoResultRGB can be the same array */
		/* returns NULL on cancel */


#endif /*TINYFD_WIN_CONSOLE_ONLY*/
#else /*_WIN32*/

/* unix zenity only */
char const * tinyfd_arrayDialog(
	char const * const aTitle , /* "" */
	int const aNumOfColumns , /* 2 */
	char const * const * const aColumns, /* {"Column 1","Column 2"} */
	int const aNumOfRows, /* 2*/
	char const * const * const aCells);
		/* {"Row1 Col1","Row1 Col2","Row2 Col1","Row2 Col2"} */

#endif /*_WIN32*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* TINYFILEDIALOGS_H */

/*
- This is not for android nor ios.
- The code is pure C, perfectly compatible with C++.
- The API is Fortran ISO_C_BINDING compliant
- C# via dll, see example file
- AVOID USING " AND ' IN TITLES AND MESSAGES.
- There's one file filter only, it may contain several patterns.
- If no filter description is provided,
  the list of patterns will become the description.
- char const * filterPatterns[3] = { "*.obj" , "*.stl" , "*.dxf" } ;
- On windows, inputbox and passwordbox are not as smooth as they should be:
  they open a console window for a few seconds.
- On windows link against Comdlg32.lib and Ole32.lib
  This linking is not compulsary for console mode (see above).
- On unix: it tries command line calls, so no such need.
- On unix you need applescript, zenity, matedialog, kdialog, Xdialog,
  python2/tkinter or dialog (will open a terminal if running without console).
- One of those is already included on most (if not all) desktops.
- In the absence of those it will use gdialog, gxmessage or whiptail
  with a textinputbox.
- If nothing is found, it switches to basic console input,
  it opens a console if needed.
- Use windows separators on windows and unix separators on unix.
- String memory is preallocated statically for all the returned values.
- File and path names are tested before return, they are valid.
- If you pass only a path instead of path + filename,
  make sure it ends with a separator.
- tinyfd_forceConsole=1; at run time, forces dialogs into console mode.
- On windows, console mode only make sense for console applications.
- Mutiple selects are not allowed in console mode.
- The package dialog must be installed to run in enhanced console mode.
  It is already installed on most unix systems.
- On osx, the package dialog can be installed via http://macports.org
- On windows, for enhanced console mode,
  dialog.exe should be copied somewhere on your executable path.
  It can be found at the bottom of the following page:
  http://andrear.altervista.org/home/cdialog.php
- If dialog is missing, it will switch to basic console input.
- You can query the type of dialog that will be use.
- The Hello World (and a bit more) is on the sourceforge site:
*/

