/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
//static const char col_gray1[]       = "#222222";
static const char col_gray1[]       = "#080808"; //matching background to xdefaults/terminal
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_black[]       = "#000000";
static const char col_green[]       = "#273234"; // darkgreen
//static const char col_red[]         = "#770000";
static const char col_red[]         = "#ff0000";
static const char col_yellow[]      = "#ffff00";
static const char col_white[]       = "#ffffff";
//static const unsigned int baralpha = 0xd1;      // should be about 80% (to look like xdefaults)
static const unsigned int baralpha = 0x80;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*                 fg         bg          border   */
	[SchemeNorm]   = { col_gray3, col_gray1,  col_gray2 },
	[SchemeSel]    = { col_gray4, col_green,  col_red   },
	[SchemeWarn]   = { col_black, col_yellow, col_red   },
	[SchemeUrgent] = { col_white, col_red,    col_red   },
	[SchemeCol5]   = { col_red,   col_gray1,  col_red   },
};
static const unsigned int alphas[][3]      = {
	/*                 fg      bg        border     */
	[SchemeNorm]   = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]    = { OPAQUE, baralpha, borderalpha },
	[SchemeWarn]   = { OPAQUE, 0xff,     0xff        },
	[SchemeUrgent] = { OPAQUE, 0x80,     borderalpha },
	[SchemeCol5]   = { OPAQUE, 0xd1,     borderalpha },
};

/* tagging */
//static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
//static const char *tags[] = { "main", "2", "web", "docs", "images", "video", "7", "gimp", "steam" };
static const char *tags[] = { "main", "web", "docs", "img", "vid", "misc" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance    title       tags mask     isfloating   monitor */
	{ "Pavucontrol", NULL,       NULL,       0,            1,           -1 },
	{ "firefox",     NULL,       NULL,       1 << 1,       0,           -1 },
	{ "qutebrowser", NULL,       NULL,       1 << 1,       0,           -1 },
	{ "Zathura",     NULL,       NULL,       1 << 2,       0,           -1 },
	{ "feh",         NULL,       NULL,       1 << 3,       0,           -1 },
	{ "Sxiv",        NULL,       NULL,       1 << 3,       0,           -1 },
	{ "mpv",         NULL,       NULL,       1 << 4,       0,           -1 },
	{ "Gimp",        NULL,       NULL,       1 << 5,       0,           -1 },
	{ "Pinta",       NULL,       NULL,       1 << 5,       0,           -1 },
	{ "Minecraft",   NULL,       NULL,       1 << 5,       0,           -1 },
	/* Make all steam windows float except for the main window */
	{ "Steam",       NULL,       NULL,       1 << 5,       1,           -1 },
	{ "Steam",       NULL,       "Steam",    1 << 5,       0,           -1 },
	{ "Steam",       NULL,       "Steam ",   1 << 5,       1,           -1 },
	/* Fix pdfpc not appearing on the second monitor */
	{ "Pdfpc",       NULL,       NULL,       1 << 5,       0,           1 },
	{ "Pdfpc",       NULL,      "presenter" ,1 << 5,       0,           0 },

};

/* layout(trues) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#include "centeredmaster.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
/* Replaced view and tag with comboview/combotag */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "urxvtc", NULL };

static const char *lockscreencmd[] = { "mylock", NULL };
static const char *shutdowncmd[]   = { "prompt", "Are you sure you want to shutdown?", "shutdown now", NULL };
static const char *displayselcmd[] = { "dmenu_display_select", NULL };
static const char *mountcmd[]      = { "dmenu_mount", NULL };
static const char *umountcmd[]     = { "dmenu_umount", NULL };
static const char *displayoffcmd[] = { "xset", "dpms", "force", "off", NULL };
static const char *scrotcmd[]      = { "scrot", NULL };

static const char *launchbrowser[] = { "firefox", NULL };

static const char *volumeup[]      = { "volume", "up", "3",  NULL };
static const char *volumedown[]    = { "volume", "down", "3",  NULL };
static const char *volumemute[]    = { "volume", "mute",  NULL };
static const char *backlightup[]   = { "backlight_wrapper", "-inc", NULL };
static const char *backlightdown[] = { "backlight_wrapper", "-dec", NULL };

#include "movestack.c"
#include "moveresize.c"
#include "focusmaster.c"
#include <X11/XF86keysym.h>
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	//{ MODKEY,                       XK_x,      spawn,          {.v = (const char*[]){"mylock", NULL} } },
	{ MODKEY,                       XK_x,      spawn,          {.v = lockscreencmd } },
	{ MODKEY|ShiftMask,             XK_x,      spawn,          {.v = shutdowncmd } },
	// use F7 instead to free F1-F6 for toggleing tags
	//{ MODKEY,                       XK_F5,     spawn,          {.v = displayselcmd } },
	{ MODKEY,                       XK_F7,     spawn,          {.v = displayselcmd } },
	{ MODKEY,                       XK_F9,     spawn,          {.v = mountcmd } },
	{ MODKEY,                       XK_F10,    spawn,          {.v = umountcmd } },
	{ 0,                            XK_Pause,  spawn,          {.v = displayoffcmd } },
	{ 0,                            XK_Print,  spawn,          {.v = scrotcmd } },
	{ 0,     XF86XK_AudioRaiseVolume,          spawn,          {.v = volumeup } },
	{ MODKEY,                       XK_Up,     spawn,          {.v = volumeup } },
	{ 0,     XF86XK_AudioLowerVolume,          spawn,          {.v = volumedown } },
	{ MODKEY,                       XK_Down,   spawn,          {.v = volumedown } },
	{ 0,     XF86XK_AudioMute,                 spawn,          {.v = volumemute } },
	{ MODKEY,                       XK_F8,     spawn,          {.v = volumemute } },
	{ 0,     XF86XK_MonBrightnessUp,           spawn,          {.v = backlightup } },
	{ MODKEY,                       XK_Right,  spawn,          {.v = backlightup } },
	{ 0,     XF86XK_MonBrightnessDown,         spawn,          {.v = backlightdown } },
	{ MODKEY,                       XK_Left,   spawn,          {.v = backlightdown } },
	{ MODKEY,                       XK_w,      spawn,          {.v = launchbrowser } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY,                       XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_y,      moveresize,     {.v = (int []){ -25, 0, 0, 0 }}},
	{ MODKEY|ShiftMask,             XK_u,      moveresize,     {.v = (int []){ 0, 25, 0, 0 }}},
	{ MODKEY|ShiftMask,             XK_i,      moveresize,     {.v = (int []){ 0, -25, 0, 0 }}},
	{ MODKEY|ShiftMask,             XK_o,      moveresize,     {.v = (int []){ 25, 0, 0, 0 }}},
	{ MODKEY|ControlMask,           XK_y,      moveresize,     {.v = (int []){ 0, 0, -25, 0 }}},
	{ MODKEY|ControlMask,           XK_u,      moveresize,     {.v = (int []){ 0, 0, 0, 25 }}},
	{ MODKEY|ControlMask,           XK_i,      moveresize,     {.v = (int []){ 0, 0, 0, -25 }}},
	{ MODKEY|ControlMask,           XK_o,      moveresize,     {.v = (int []){ 0, 0, 25, 0 }}},
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	//This is now used for cfacts reset
	//{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
	/* mod + f is now fullscreen and mod + m selects the master window */
	//{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	//{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_m,      focusmaster,    {0} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY,                       XK_F1,     toggleview,     {.ui = 1 << 0} },
	{ MODKEY,                       XK_F2,     toggleview,     {.ui = 1 << 1} },
	{ MODKEY,                       XK_F3,     toggleview,     {.ui = 1 << 2} },
	{ MODKEY,                       XK_F4,     toggleview,     {.ui = 1 << 3} },
	{ MODKEY,                       XK_F5,     toggleview,     {.ui = 1 << 4} },
	{ MODKEY,                       XK_F6,     toggleview,     {.ui = 1 << 5} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

