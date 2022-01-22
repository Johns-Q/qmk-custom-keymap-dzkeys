 /* Copyright 2021 OpenAnnePro community
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */

#include QMK_KEYBOARD_H

#ifdef MOUSEKEY_ENABLE
#include "mousekey.h"
#endif
#ifdef DIGITIZER_ENABLE
#include "digitizer.h"
#endif


// Digitizer/Pointer emulation
//	7 8 9
//	4 5 6 0
//	1 2 3
//	KC_BTN1 ... KC_BTN3
//	KC_WH_U KC_WH_D KC_WH_L KC_WH_R
//
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

#ifdef KEYBOARD_annepro2

/* map annepro2 matrix to standard 60% ansi */
#define LAYOUT_60_ansi LAYOUT

enum anne_pro_layers {
    _BASE_LAYER,
    _FN1_LAYER,
    _FN2_LAYER,
};

// clang-format off
// Key symbols are based on QMK. Use them to remap your keyboard
/*
* Layer _BASE_LAYER
* ,-----------------------------------------------------------------------------------------.
* | Esc |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |    Bksp   |
* |-----------------------------------------------------------------------------------------+
* | Tab    |  q  |  w  |  e  |  r  |  t  |  y  |  u  |  i  |  o  |  p  |  [  |  ]  |   \    |
* |-----------------------------------------------------------------------------------------+
* | FN2     |  a  |  s  |  d  |  f  |  g  |  h  |  j  |  k  |  l  |  ;  |  '  |    Enter    |
* |-----------------------------------------------------------------------------------------+
* | Shift      |  z  |  x  |  c  |  v  |  b  |  n  |  m  |  ,  |  .  |  /  |    Shift       |
* |-----------------------------------------------------------------------------------------+
* | Ctrl  |  L1   |  Alt  |               space             |  Alt  |  FN1  |  FN2  | Ctrl  |
* \-----------------------------------------------------------------------------------------/
* Layer TAP in _BASE_LAYER
* ,-----------------------------------------------------------------------------------------.
* |     |     |     |     |     |     |     |     |     |     |     |     |     |           |
* |-----------------------------------------------------------------------------------------+
* |        |     |     |     |     |     |     |     |     |     |     |     |     |        |
* |-----------------------------------------------------------------------------------------+
* | Nubs    |     |     |     |     |     |     |     |     |     |     |     |             |
* |-----------------------------------------------------------------------------------------+
* |            |     |     |     |     |     |     |     |     |     |     |       UP       |
* |-----------------------------------------------------------------------------------------+
* |       |       |       |                                 |       |  LEFT | DOWN  | RIGHT |
* \-----------------------------------------------------------------------------------------/
*/
 const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 [_BASE_LAYER] = LAYOUT_60_ansi(
    KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,
    KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
    LT(_FN2_LAYER,KC_NUBS), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,
    KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, RSFT_T(KC_UP),
    KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_RALT, LT(_FN1_LAYER,KC_LEFT), LT(_FN2_LAYER,KC_DOWN), RCTL_T(KC_RGHT)
),
  /*
  * Layer _FN1_LAYER
  * ,-----------------------------------------------------------------------------------------.
  * |  `  |  F1 |  F2 |  F3 |  F4 |  F5 |  F6 |  F7 |  F8 |  F9 | F10 | F11 | F12 |  DELETE   |
  * |-----------------------------------------------------------------------------------------+
  * | Tab    |  q  | UP  |  e  |  r  |  t  |  y  |  u  |  i  |  o  | PS | HOME | END |   \    |
  * |-----------------------------------------------------------------------------------------+
  * | FN2     |LEFT |DOWN |RIGHT|  f  |  g  |  h  |  j  |  k  |  l  | PGUP|PGDN |    Enter    |
  * |-----------------------------------------------------------------------------------------+
  * | Shift      |V-UP |V-DWN|MUTE |  v  |  b  |  n  |  m  |  ,  |INSRT| DEL |    Shift       |
  * |-----------------------------------------------------------------------------------------+
  * | Ctrl  |  L1   |  Alt  |               space             |  Alt  |  FN1  |  FN2  | Ctrl  |
  * \-----------------------------------------------------------------------------------------/
  *
  */
 [_FN1_LAYER] = LAYOUT_60_ansi(
    KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL,
    KC_TRNS, KC_TRNS, KC_UP, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PSCR, KC_HOME, KC_END, KC_TRNS,
    KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PGUP, KC_PGDN, KC_TRNS,
    KC_TRNS, KC_VOLU, KC_VOLD, KC_MUTE, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_INS, KC_DEL, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, MO(_FN2_LAYER), KC_TRNS
),
  /*
  * Layer _FN2_LAYER
  * ,-----------------------------------------------------------------------------------------.
  * | ESC | BT1 | BT2 | BT3 | BT4 |  F5 |  F6 |     |     |     |     |     |     |    Bksp   |
  * |-----------------------------------------------------------------------------------------+
  * | Caps   |  q  | UP  |  e  |  r  |  t  |  y  | DZ7 | DZ8 | DZ9 | WHU |  [  |  ]  |   \    |
  * |-----------------------------------------------------------------------------------------+
  * | FN2     |LEFT |DOWN |RIGHT|  f  |  g  | DZ0 | DZ4 | DZ5 | DZ6 | WHD |  '  |    Enter    |
  * |-----------------------------------------------------------------------------------------+
  * | Shift      |  z  |  x  |  c  |  v  |  b  | WHL | DZ1 | DZ2 | DZ3 | WHR |    Shift       |
  * |-----------------------------------------------------------------------------------------+
  * | Ctrl  |  L1   |  Alt  |               BTN1              |  Alt  |  FN1  |  FN2  | Ctrl  |
  * \-----------------------------------------------------------------------------------------/
  *
  */
 [_FN2_LAYER] = LAYOUT_60_ansi(
    KC_TRNS, KC_AP2_BT1, KC_AP2_BT2, KC_AP2_BT3, KC_AP2_BT4, KC_TRNS, KC_TRNS, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, RGB_MOD, RGB_TOG, KC_TRNS,
    KC_CAPS, KC_TRNS, KC_UP, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, DZ_7, DZ_8, DZ_9, KC_WH_U, KC_TRNS, KC_TRNS, KC_TRNS,
    MO(_FN2_LAYER), KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_TRNS, DZ_0, DZ_4, DZ_5, DZ_6, KC_WH_D, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN2, KC_BTN3, KC_WH_L, DZ_1, DZ_2, DZ_3, KC_WH_R, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN1, KC_TRNS, MO(_FN1_LAYER), MO(_FN2_LAYER), KC_TRNS
 ),
};
// clang-format on

#endif

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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  // digitizer emulation 2s timeout
  if(timer_elapsed(dz_timer)>2000) {
      grid_nr = 0;
  }

  switch (keycode) {
    case LT(0, KC_0):
        if (record->tap.count && record->event.pressed) {
	    grid_nr = 0;		// Intercept tap function
	    report_grid();
        } else if (record->event.pressed) {
					// Intercept hold function
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
