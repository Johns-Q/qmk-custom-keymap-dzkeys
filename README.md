# qmk-custom-keymap-dzkeys
Custom keymap for QMK firmware which use the digitizer feature to emulate a
mouse pointer by trisecting the digitizer area.

## Install

### Install example keymaps:

- Clone the custom keymap
```
git clone https://github.com/Johns-Q/qmk-custom-keymap-dzkeys.git
cd qmk-custom-keymap-dzkeys
```
- Clone the qmk firmware
As example I use the qmk mainline repository.
You need the mainline version or a version which supports the digitizer feature.
```
git clone https://github.com/qmk/qmk_firmware.git
```
- Setup qmk
```
cd qmk_firmware
make git-submodule
```
- Alternative way using layout feature: Copy the example files
```
mkdir -p layouts/community/60_ansi/dzkeys
cp -a ../annepro2/* layouts/community/60_ansi/dzkeys
```
- Build it
```
make annepro2/c18:dzkeys
```

- Copy the example files
```
mkdir -p keyboards/annepro2/keymaps/dzkeys
cp -a ../annepro2/* keyboards/annepro2/keymaps/dzkeys/
```
- Build it
```
make annepro2/c18:dzkeys
```


### Install into existing keymap

Have a version of qmk firmware installed which supports the digitizer feature.

- Edit your rules.mk

Need mousekey and digitizer support.
```
MOUSEKEY_ENABLE = yes
DIGITIZER_ENABLE = yes
```

See [annepro2/rules.mk](annepro2/rules.mk)

- Edit your keymap.c

Add to head of the file:
```
#ifdef MOUSEKEY_ENABLE
#include "mousekey.h"
#endif
#ifdef DIGITIZER_ENABLE
#include "digitizer.h"
#endif
//
// Digitizer/Pointer emulation
//      7 8 9 U
//      4 5 6 0
//      1 2 3
//      KC_BTN1 ... KC_BTN3
//      KC_WH_U KC_WH_D KC_WH_L KC_WH_R
//
#define DZ_U LT(0, KC_U)
#define DZ_0 LT(0, KC_0)
#define DZ_7 LT(0, KC_7)
#define DZ_8 LT(0, KC_8)
#define DZ_9 LT(0, KC_9)
#define DZ_4 LT(0, KC_4)
#define DZ_5 LT(0, KC_5)
#define DZ_6 LT(0, KC_6)
#define DZ_1 LT(0, KC_1)
#define DZ_2 LT(0, KC_2)
#define DZ_3 LT(0, KC_3)

#define MAX_GRID_NR     10000000        ///< 10^7: 7 keys encoded

static int grid_nr;			///< current grid number
static uint16_t dz_timer;		///< timer for default 2s timeout

/**
**	Convert grid number into digitizer report.
*/
static void report_grid(void)
{
    int g;
    int r;
    int x;
    int y;
    digitizer_t mydigitizer;

    x = 0;
    y = 0;
    r = 1;
    g = grid_nr;

    g %= MAX_GRID_NR;
    while (g < (MAX_GRID_NR / 10)) {	// normalize 1 -> 155 55 55
        g *= 10;
        g += 5;
    }

    while (g > 0) {
	// 147=0 258=1 369=2
	x += (((g % 10) - 1) % 3) * r;
	// 123=2 456=1 789=0
	y += ((9 - (g % 10)) / 3) * r;

	g /= 10;
	r *= 3;
    }

    mydigitizer.tipswitch = 0;
    mydigitizer.inrange = 1;
    mydigitizer.x = (double)x / r;
    mydigitizer.y = (double)y / r;
    mydigitizer.status = DZ_INITIALIZED;
    digitizer_set_report(mydigitizer);
    dz_timer = timer_read();
}
```

Add to your keymap the DZ_0 ... DZ_9 keys for the digitizer emulation.
Add KC_BTN1, KC_BTN2, KC_BTN3 for the three mouse buttons emulation.
Add KC_WH_L, KC_WH_R ,KC_WH_U, KC_WH_D if you want mouse wheel emulation.

Add at the end of file or into the existing `process_record_user` function.

```
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // digitizer emulation 2s timeout
  if(timer_elapsed(dz_timer)>2000) {
      grid_nr = 0;
  }

  switch (keycode) {
      case LT(0, KC_U):
          if (record->tap.count && record->event.pressed) {
              grid_nr = grid_nr / 10;     // Intercept tap function
              report_grid();
          } else if (record->event.pressed) {
                                          // Intercept hold function
              // no usefull function on hold
          }
          return false;
    case LT(0, KC_0):
        if (record->tap.count && record->event.pressed) {
	    grid_nr = 0;		// Intercept tap function
	    report_grid();
        } else if (record->event.pressed) {
					// Intercept hold function
	    // no usefull function on hold
	    grid_nr = 0;
        }
	return false;
    case LT(0, KC_1):
        if (record->tap.count && record->event.pressed) {
					// Intercept tap function
	    if(grid_nr < (MAX_GRID_NR / 10)) {
		grid_nr = grid_nr * 10 + 1;
	    }
	    report_grid();
        } else if (record->event.pressed) {
					// Intercept hold function
	    mousekey_on(KC_MS_D);
	    mousekey_on(KC_MS_L);
        } else {
	    mousekey_off(KC_MS_D);
	    mousekey_off(KC_MS_L);
	}
	return false;
    case LT(0, KC_2):
        if (record->tap.count && record->event.pressed) {
					// Intercept tap function
	    if(grid_nr < (MAX_GRID_NR / 10)) {
		grid_nr = grid_nr * 10 + 2;
	    }
	    report_grid();
        } else if (record->event.pressed) {
					// Intercept hold function
	    mousekey_on(KC_MS_D);
        } else {
	    mousekey_off(KC_MS_D);
        }
	return false;
    case LT(0, KC_3):
        if (record->tap.count && record->event.pressed) {
					// Intercept tap function
	    if(grid_nr < (MAX_GRID_NR / 10)) {
		grid_nr = grid_nr * 10 + 3;
	    }
	    report_grid();
        } else if (record->event.pressed) {
					// Intercept hold function
	    mousekey_on(KC_MS_D);
	    mousekey_on(KC_MS_R);
        } else {
	    mousekey_off(KC_MS_D);
	    mousekey_off(KC_MS_R);
        }
	return false;
    case LT(0, KC_4):
        if (record->tap.count && record->event.pressed) {
					// Intercept tap function
	    if(grid_nr < (MAX_GRID_NR / 10)) {
		grid_nr = grid_nr * 10 + 4;
	    }
	    report_grid();
        } else if (record->event.pressed) {
					// Intercept hold function
	    mousekey_on(KC_MS_L);
        } else {
	    mousekey_off(KC_MS_L);
        }
	return false;
    case LT(0, KC_5):
        if (record->tap.count && record->event.pressed) {
					// Intercept tap function
	    if(grid_nr < (MAX_GRID_NR / 10)) {
		grid_nr = grid_nr * 10 + 5;
	    }
	    report_grid();
        }
	return false;
    case LT(0, KC_6):
        if (record->tap.count && record->event.pressed) {
					// Intercept tap function
	    if(grid_nr < (MAX_GRID_NR / 10)) {
		grid_nr = grid_nr * 10 + 6;
	    }
	    report_grid();
        } else if (record->event.pressed) {
					// Intercept hold function
	    mousekey_on(KC_MS_R);
        } else {
	    mousekey_off(KC_MS_R);
        }
	return false;
    case LT(0, KC_7):
        if (record->tap.count && record->event.pressed) {
					// Intercept tap function
	    if(grid_nr < (MAX_GRID_NR / 10)) {
		grid_nr = grid_nr * 10 + 7;
	    }
	    report_grid();
        } else if (record->event.pressed) {
					// Intercept hold function
	    mousekey_on(KC_MS_U);
	    mousekey_on(KC_MS_L);
        } else {
	    mousekey_off(KC_MS_U);
	    mousekey_off(KC_MS_L);
        }
	return false;
    case LT(0, KC_8):
        if (record->tap.count && record->event.pressed) {
					// Intercept tap function
	    if(grid_nr < (MAX_GRID_NR / 10)) {
		grid_nr = grid_nr * 10 + 8;
	    }
	    report_grid();
        } else if (record->event.pressed) {
					// Intercept hold function
	    mousekey_on(KC_MS_U);
        } else {
	    mousekey_off(KC_MS_U);
        }
	return false;
    case LT(0, KC_9):
        if (record->tap.count && record->event.pressed) {
					// Intercept tap function
	    if(grid_nr < (MAX_GRID_NR / 10)) {
		grid_nr = grid_nr * 10 + 9;
	    }
	    report_grid();
        } else if (record->event.pressed) {
					// Intercept hold function
	    mousekey_on(KC_MS_U);
	    mousekey_on(KC_MS_R);
        } else {
	    mousekey_off(KC_MS_U);
	    mousekey_off(KC_MS_R);
        }
	return false;

    default:
      return true;
  }
}
```

See [annepro2/keymap.c](annepro2/keymap.c) for an example.

## How it works

DZ keys is a feature that allows you to emulate a mouse using your keyboard.
This is done by trisecting the screen in 9 sectors. This sector is again
trisected into 9 smaller sectors. With only 7 keypress every pixel of a 2K
screen can be accessed.
The sector are numbered like the numberpad, 1 is bottom left, 9 is top right.

```
Ascii art 
.--.--.--.--.--.--.--.--.--.
|77|78|79|        |        |
+--+--+--+        |        |
|74|75|76|   8    |   9    |
+--+--+--+        |        |
|71|72|73|        |        |
.--.--.--.--.--.--.--.--.--.
|        |        |        |
+        +        |        |
|   4    |   5    |   6    |
+        +        |        |
|        |        |        |
.--.--.--.--.--.--.--.--.--.
|        |        |        |
+        +        |        |
|   1    |   2    |   3    |
+        +        |        |
|        |        |        |
'--'--'--'--'--'--'--'--'--'
```

- 1111111 is the bottom left pixel
- 3333333 is the bottom right pixel
- ...
- 5 or 5555555 is the screen middle
- ...

The 9 sectors are handled by the DZ_1 .. DZ_9 macros.  When you tap the key the
number is added to grid number.  'DZ_1' 'DZ_5' 'DZ_9' is the sector '159'.  
After a timeout of 2s the grid number is reset to 0 or 'DZ_0' resets the grid
number to 0.  
DZ_U undos the last input, 0123U gives 012.

You can still use the mousekey feature of qmk.
A long press of the DZ_1 .. DZ_9 keys moves the mouse in this direction.
DZ_4 moves left. DZ_9 moves right...

## Example to get the grid number

This is a C example for linux/x11 which gets the current mouse coordinate and
prints as grid number. Most the time you only need to enter the first three or
six numbers.

Here a link how to rember numbers:
  [Mnemonic major system](https://en.wikipedia.org/wiki/Mnemonic_major_system)


```
#include <stdio.h>
#include <X11/Xlib.h>

static int ScreenWidth;
static int ScreenHeight;

/**
**	Convert coordinates to grid.
*/
static int XY2Grid(int mx, int my)
{
    int g;
    int x;
    int y;
    int t;
    int r;

    // Mouse coordinates -> Grid number
    // 637,   688 -> 05....
    // 7 8 9
    // 4 5 6
    // 1 2 3

    g = 0;
    //r = 19683;                                // 3^9
    r = 2187;				// 3^7
    x = (mx * r * 3) / ScreenWidth;
    y = (my * r * 3) / ScreenHeight;

    // 0/3-1/3  1/3-2/3 2/3-3/3
    // 1 2 3
    // 0/3-1/3  1/3-2/3 2/3-3/3
    // 1 4 7
    for (;;) {
	t = (x / r) % 3;		// 0 1 2
	x -= t * r;
	g += t + 1;			// 1 2 3
	t = (y / r) % 3;		// 0 1 2
	y -= t * r;
	g += (2 - t) * 3;		// 6 3 0

	if (r <= 3) {
	    break;
	}
	r /= 3;
	g *= 10;
    }
    //fprintf(stderr, "%d\n", g);

    //return g / 100;                   // drop precision (NOT WORKING)
    return g;
}

int main()
{
    Display *d;
    Window inwin;
    Window inchildwin;
    int rootx, rooty;
    int childx, childy;
    Atom atom_type_prop;
    int actual_format;
    unsigned int mask;
    unsigned long n_items, bytes_after_ret;
    Window *props;
    int grid_nr;

    d = XOpenDisplay(NULL);
    ScreenWidth = DisplayWidth(d, DefaultScreen(d));
    ScreenHeight = DisplayHeight(d, DefaultScreen(d));
    (void)XGetWindowProperty(d, DefaultRootWindow(d), XInternAtom(d,
	    "_NET_ACTIVE_WINDOW", True), 0, 1, False, AnyPropertyType,
	&atom_type_prop, &actual_format, &n_items, &bytes_after_ret,
	(unsigned char **)&props);

    XQueryPointer(d, props[0], &inwin, &inchildwin, &rootx, &rooty, &childx,
	&childy, &mask);
    grid_nr = XY2Grid(rootx, rooty);

    printf("%5d, %5d: %01d %03d %03d %01d\n", rootx, rooty,
	(grid_nr / 10000000) % 1000, (grid_nr / 10000) % 1000,
	(grid_nr / 10) % 1000, (grid_nr / 1) % 10);

    XFree(props);
    (void)XCloseDisplay(d);
    return 0;
}
```

See also [x11-get-mouse-gridnr.c](x11-get-mouse-gridnr.c)

```
gcc -o x11-get-mouse-gridnr x11-get-mouse-gridnr.c -lX11
./x11-get-mouse-gridnr
```
or
```
watch -n 1 ./x11-get-mouse-gridnr
```

