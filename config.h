/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "firefox"
#define EDITOR "nvim"
#define EXPLORER "lfub"

/* appearance */
static unsigned int borderpx    = 5;        /* border pixel of windows */
static unsigned int snap        = 32;       /* snap pixel */
static unsigned int gappih      = 1;        /* horiz inner gap between windows */
static unsigned int gappiv      = 1;        /* vert inner gap between windows */
static unsigned int gappoh      = 1;        /* horiz outer gap between windows and screen edge */
static unsigned int gappov      = 1;        /* vert outer gap between windows and screen edge */
static int swallowfloating      = 0;        /* 1 means swallow floating windows by default */
static int smartgaps            = 0;        /* 1 means no outer gap when there is only one window */
static int showbar              = 1;        /* 0 means no bar */
static int topbar               = 1;        /* 0 means bottom bar */
static char *fonts[] = {
    "monospace:size=11:antialias=true:autohint=true",
    "JetBrainsMono NF SemiBold:style=SemiBold:size=11:antialias=true:autohint=true",
    "NotoColorEmoji:pixelsize=11:antialias=true:autohint=true",
};
static char normbgcolor[]       = "#222222";
static char normbordercolor[]   = "#444444";
static char normfgcolor[]       = "#bbbbbb";
static char selfgcolor[]        = "#eeeeee";
static char selbordercolor[]    = "#770000";
static char selbgcolor[]        = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",  spcmd1},
	{"spcalc",  spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static char tagPaths[][238] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class        instance        title           tags mask   isfloating  isterminal  noswallow   monitor */
	{ "Gimp",       NULL,           NULL,       	1 << 8,     0,          0,          0,          -1 },
	{ TERMCLASS,    NULL,           NULL,       	0,          0,          1,          0,          -1 },
	{ NULL,         NULL,           "Event Tester", 0,          0,          0,          1,          -1 },
	{ TERMCLASS,    "floatterm",    NULL,       	0,          1,          1,          0,          -1 },
	{ TERMCLASS,    "bg",           NULL,       	1 << 7,     0,          1,          0,          -1 },
	{ TERMCLASS,    "spterm",       NULL,       	SPTAG(0),   1,          1,          0,          -1 },
	{ TERMCLASS,    "spcalc",       NULL,       	SPTAG(1),   1,          1,          0,          -1 },
};

/* layout(s) */
static float mfact              = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster              = 1;    /* number of clients in master area */
static int resizehints          = 0;    /* 1 means respect size hints in tiled resizals */
static int attachbelow          = 1;
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",	tile },			/* Default: Master on left, slaves on right */
	{ "TTT",	bstack },		/* Master on top, slaves on bottom */

	{ "[@]",	spiral },		/* Fibonacci spiral */
	{ "[\\]",	dwindle },		/* Decreasing in size right and leftward */

	{ "[D]",	deck },			/* Master on left, slaves in monocle-like mode on right */
	{ "[M]",	monocle },		/* All windows on top of eachother */

	{ "|M|",	centeredmaster },		/* Master in middle, slaves on sides */
	{ ">M>",	centeredfloatingmaster },	/* Same but master floats */

	{ "><>",	NULL },			/* no layout function means floating behavior */
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_j,	ACTION##stack,  {.i = INC(-1) } }, \
	{ ControlMask,  XK_space,       ACTION##stack,  {.i = 0 } }, \
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "color0",		        STRING,	&normbordercolor },
		{ "color8",		        STRING,	&selbordercolor },
		{ "color0",		        STRING,	&normbgcolor },
		{ "color11",	        STRING,	&normfgcolor },
		{ "color0",		        STRING,	&selfgcolor },
		{ "color11",	        STRING,	&selbgcolor },
		{ "borderpx",	        INTEGER, &borderpx },
		{ "snap",		        INTEGER, &snap },
		{ "showbar",	        INTEGER, &showbar },
		{ "topbar",		        INTEGER, &topbar },
		{ "nmaster",	        INTEGER, &nmaster },
		{ "resizehints",        INTEGER, &resizehints },
		{ "mfact",		        FLOAT,	&mfact },
		{ "gappih",		        INTEGER, &gappih },
		{ "gappiv",		        INTEGER, &gappiv },
		{ "gappoh",		        INTEGER, &gappoh },
		{ "gappov",		        INTEGER, &gappov },
		{ "swallowfloating",	INTEGER, &swallowfloating },
		{ "smartgaps",		    INTEGER, &smartgaps },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static const Key keys[] = {
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)

	TAGKEYS( XK_1,	0 )
	TAGKEYS( XK_2,	1 )
	TAGKEYS( XK_3,	2 )
	TAGKEYS( XK_4,	3 )
	TAGKEYS( XK_5,	4 )
	TAGKEYS( XK_6,	5 )
	TAGKEYS( XK_7,	6 )
	TAGKEYS( XK_8,	7 )
	TAGKEYS( XK_9,	8 )

	/* modifier         key             function    argument */
	{ MODKEY,		   	    XK_comma,	    focusmon,	{.i = -1 } },
	{ MODKEY|ShiftMask,	    XK_comma,	    tagmon,		{.i = -1 } },
	{ MODKEY,		   	    XK_period,	    focusmon,	{.i = +1 } },
	{ MODKEY|ShiftMask,	    XK_period,	    tagmon,		{.i = +1 } },

	{ MODKEY,			    XK_Page_Up,	    shiftview,	{ .i = -1 } },
	{ MODKEY|ShiftMask,	    XK_Page_Up,	    shifttag,	{ .i = -1 } },
	{ MODKEY,			    XK_Page_Down,	shiftview,	{ .i = +1 } },
	{ MODKEY|ShiftMask,	    XK_Page_Down,	shifttag,	{ .i = +1 } },

	{ MODKEY,			    XK_g,	        shiftview,  { .i = -1 } },
	{ MODKEY|ShiftMask,	    XK_g,	        shifttag,	{ .i = -1 } },
	{ MODKEY,			    XK_semicolon,	shiftview,	{ .i = 1 } },
	{ MODKEY|ShiftMask,	    XK_semicolon,	shifttag,	{ .i = 1 } },

	{ MODKEY,			    XK_Tab,		    view,		        {0} },
	{ MODKEY,			    XK_s,	   	    togglesticky,	    {0} },
	{ MODKEY,			    XK_f,	        togglefullscr,      {0} },
	{ MODKEY,			    XK_h,	        setmfact,           {.f = -0.05} },
	{ MODKEY,			    XK_l,		    setmfact,      	    {.f = +0.05} },
	{ MODKEY,			    XK_o,		    incnmaster,         {.i = +1 } },
	{ MODKEY|ShiftMask,	    XK_o,		    incnmaster,         {.i = -1 } },
	{ MODKEY,		        XK_d,	        settagpath,	        {0} },
	{ MODKEY|ShiftMask,     XK_d,	        settagpathtocwd,    {0} },
	{ MODKEY|ControlMask,   XK_d,	        cleartagpath,       {0} },
	{ MODKEY,			    XK_q,		    killclient,	        {0} },
	{ MODKEY,          	    XK_Return,	    spawnwithdir,	    {.v = (const char*[]){ "exec $SHELL", NULL } } },
	{ MODKEY,               XK_n,		    spawnwithdir,	    {.v = (const char*[]){ EDITOR, ".", NULL } } },
	{ MODKEY,			    XK_r,		    spawnwithdir,       {.v = (const char*[]){ EXPLORER, NULL } } },
	{ MODKEY,			    XK_apostrophe,	togglescratch,	    {.ui = 1} },
	{ MODKEY|ShiftMask,	    XK_Return,	    togglescratch,	    {.ui = 0} },
	{ MODKEY,			    XK_space,	    zoom,	        	{0} },
	{ MODKEY|ShiftMask,	    XK_f,	        togglefloating,     {0} },
	{ MODKEY,			    XK_b,		    togglebar,	        {0} },

	{ MODKEY,			    XK_t,	        setlayout,	        {.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,	    XK_t,	        setlayout,	        {.v = &layouts[1]} }, /* bstack */
	{ MODKEY|ControlMask,	XK_t,	        setlayout,	        {.v = &layouts[6]} }, /* centeredmaster */
	{ MODKEY,           	XK_m,	        selectlayout,	    {0} },

	{ MODKEY,			    XK_a,	   	    togglegaps,	        {0} },
	{ MODKEY|ShiftMask,     XK_a,	   	    defaultgaps,	    {0} },
	{ MODKEY|ShiftMask,	    XK_apostrophe,	togglesmartgaps,	{0} },
	{ MODKEY,			    XK_z,		    incrgaps,	        {.i = +3 } },
	{ MODKEY,			    XK_x,		    incrgaps,	        {.i = -3 } },

	{ MODKEY,			    XK_grave,	    spawn,      {.v = (const char*[]){ "dmenuunicode", NULL } } },
	{ MODKEY|ShiftMask,	    XK_q,		    spawn,		{.v = (const char*[]){ "sysact", NULL } } },
	{ MODKEY,			    XK_BackSpace,	spawn,		{.v = (const char*[]){ "sysact", NULL } } },
	{ MODKEY,			    XK_w,		    spawn,		{.v = (const char*[]){ BROWSER, NULL } } },
	{ MODKEY|ShiftMask,	    XK_w,		    spawn,		{.v = (const char*[]){ TERMINAL, "-e", "sudo", "nmtui", NULL } } },
	{ MODKEY,			    XK_e,		    spawn,		SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks; rmdir ~/.abook 2>/dev/null") },
	{ MODKEY|ShiftMask,	    XK_e,		    spawn,		SHCMD(TERMINAL " -e abook -C ~/.config/abook/abookrc --datafile ~/.config/abook/addressbook") },
	{ MODKEY|ShiftMask,     XK_r,		    spawn,		{.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },
	{ Mod1Mask,			    XK_space,	    spawn,      {.v = (const char*[]){ "dmenu_run", NULL } } },
	{ MODKEY,       	    XK_p,	        spawn,		{.v = (const char*[]){ "passmenu", NULL } } },
	{ MODKEY,             	XK_v,		    spawn,		{.v = (const char*[]){ "vpncontrol", "toggle" } } },
	{ MODKEY|ShiftMask, 	XK_v,		    spawn,		{.v = (const char*[]){ "vpncontrol", "switch" } } },

	{ MODKEY|ShiftMask|ControlMask|Mod1Mask,    XK_s,   spawn,  {.v = (const char*[]){ "sysact", "sleep", NULL } } },
	{ MODKEY|ShiftMask|ControlMask|Mod1Mask,    XK_h,   spawn,  {.v = (const char*[]){ "sysact", "shutdown", NULL } } },
	{ MODKEY|ShiftMask|ControlMask|Mod1Mask,    XK_r,   spawn,  {.v = (const char*[]){ "sysact", "reboot", NULL } } },

	{ 0,			   	    XK_Print,       spawn,	SHCMD("maim -q -d 0.2 | xclip -sel clip -t image/png") },
	{ Mod1Mask,		   	    XK_Print,	    spawn,	SHCMD("maim -q -d 0.2 -i \"$(xdotool getactivewindow)\" | xclip -sel clip -t image/png") },
	{ ShiftMask,	        XK_Print,	    spawn,	SHCMD("maim -u -s | xclip -sel clip -t image/png") },
	{ ControlMask,          XK_Print,	    spawn,	SHCMD("maim -q -d 0.2 screenshot-$(date '+%y%m%d-%H%M-%S').png")},
	{ ControlMask|Mod1Mask,	XK_Print,	    spawn,	SHCMD("maim -q -d 0.2 -i \"$(xdotool getactivewindow)\" screenshot-$(date '+%y%m%d-%H%M-%S').png")},
	{ ControlMask|ShiftMask,XK_Print,	    spawn,	SHCMD("maim -u -s screenshot-$(date '+%y%m%d-%H%M-%S').png")},
	{ MODKEY,	    	    XK_Print,	    spawn,	{.v = (const char*[]){ "dmenurecord", NULL } } },
	{ MODKEY,			    XK_Delete,	    spawn,	{.v = (const char*[]){ "dmenurecord", "kill", NULL } } },
	{ MODKEY,	    	    XK_Insert,	    spawn,	SHCMD("xclip -sel clip -t image/png -o | swappy -f -") },
	{ MODKEY|ShiftMask,	    XK_Insert,	    spawn,	SHCMD("xclip -sel clip -t image/png -o > screenshot-$(date '+%y%m%d-%H%M-%S').png") },
	{ MODKEY,			    XK_Scroll_Lock,	spawn,  SHCMD("killall screenkey || screenkey &") },

	{ MODKEY,	XK_F3,		spawn,		{.v = (const char*[]){ "displayselect", NULL } } },
	{ MODKEY,	XK_F4,		spawn,		SHCMD(TERMINAL " -e pulsemixer; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,	XK_F5,		xrdb,		{.v = NULL } },
	{ MODKEY,	XK_F6,		spawn,		{.v = (const char*[]){ "torwrap", NULL } } },
	{ MODKEY,	XK_F7,		spawn,		{.v = (const char*[]){ "td-toggle", NULL } } },
	{ MODKEY,	XK_F8,		spawn,		{.v = (const char*[]){ "mailsync", NULL } } },
	{ MODKEY,	XK_F9,		spawn,		{.v = (const char*[]){ "mounter", NULL } } },
	{ MODKEY,	XK_F10,		spawn,		{.v = (const char*[]){ "unmounter", NULL } } },
	{ MODKEY,	XK_F11,		spawn,		SHCMD("mpv --untimed --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
	{ MODKEY,	XK_F12,		spawn,		SHCMD("remaps") },

	{ 0, XF86XK_AudioMute,		    spawn,		SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 0%- && wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%+; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 0%+ && wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%-; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioMicMute,	    spawn,		SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
	{ 0, XF86XK_Calculator,		    spawn,		{.v = (const char*[]){ TERMINAL, "-e", "bc", "-l", NULL } } },
	{ 0, XF86XK_Sleep,		        spawn,		{.v = (const char*[]){ "sudo", "-A", "zzz", NULL } } },
	{ 0, XF86XK_WWW,		        spawn,		{.v = (const char*[]){ BROWSER, NULL } } },
	{ 0, XF86XK_DOS,		        spawn,		{.v = termcmd } },
	{ 0, XF86XK_TaskPane,		    spawn,		{.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },
	{ 0, XF86XK_Mail,		        spawn,		SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") },
	{ 0, XF86XK_MyComputer,		    spawn,		{.v = (const char*[]){ TERMINAL, "-e",  "lfub",  "/", NULL } } },
	{ 0, XF86XK_Launch1,		    spawn,		{.v = (const char*[]){ "xset", "dpms", "force", "off", NULL } } },
	{ 0, XF86XK_TouchpadToggle,	    spawn,		SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOff,	    spawn,		{.v = (const char*[]){ "synclient", "TouchpadOff=1", NULL } } },
	{ 0, XF86XK_TouchpadOn,		    spawn,		{.v = (const char*[]){ "synclient", "TouchpadOff=0", NULL } } },
	{ 0, XF86XK_MonBrightnessUp,	spawn,		{.v = (const char*[]){ "brightnessctl", "s", "+10%", NULL } } },
	{ 0, XF86XK_MonBrightnessDown,	spawn,		{.v = (const char*[]){ "brightnessctl", "s", "10%-", NULL } } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
#ifndef __OpenBSD__
	{ ClkWinTitle,      0,          Button2,    zoom,           {0} },
	{ ClkStatusText,    0,          Button1,    sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,    0,          Button2,    sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,    0,          Button3,    sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,    0,          Button4,    sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,    0,          Button5,    sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,    ShiftMask,  Button1,    sigdwmblocks,   {.i = 6} },
#endif
	{ ClkStatusText,    ShiftMask,  Button3,    spawn,          SHCMD(TERMINAL " -e nvim ~/.local/src/dwmblocks/config.h") },
	{ ClkClientWin,     MODKEY,     Button1,    movemouse,      {0} },
	{ ClkClientWin,     MODKEY,     Button2,    defaultgaps,	{0} },
	{ ClkClientWin,     MODKEY,     Button3,    resizemouse,    {0} },
	{ ClkClientWin,	    MODKEY,		Button4,    incrgaps,	    {.i = +1} },
	{ ClkClientWin,	    MODKEY,		Button5,    incrgaps,	    {.i = -1} },
	{ ClkTagBar,        0,          Button1,    view,           {0} },
	{ ClkTagBar,        0,          Button3,    toggleview,     {0} },
	{ ClkTagBar,        MODKEY,     Button1,    tag,            {0} },
	{ ClkTagBar,        MODKEY,     Button3,    toggletag,      {0} },
	{ ClkTagBar,	    0,		    Button4,    shiftview,	    {.i = -1} },
	{ ClkTagBar,	    0,		    Button5,    shiftview,	    {.i = 1} },
	{ ClkRootWin,	    0,		    Button2,    togglebar,	    {0} },
};

