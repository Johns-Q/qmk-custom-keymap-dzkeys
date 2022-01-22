
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
