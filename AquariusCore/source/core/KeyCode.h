#pragma once

/* The unknown key */
#define AQ_KEY_UNKNOWN            -1

/* Printable keys */
#define AQ_KEY_SPACE               32
#define AQ_KEY_APOSTROPHE      39  /* ' */
#define AQ_KEY_COMMA              44  /* , */
#define AQ_KEY_MINUS               45  /* - */
#define AQ_KEY_PERIOD              46  /* . */
#define AQ_KEY_SLASH               47  /* / */
#define AQ_KEY_0                      48
#define AQ_KEY_1                       49
#define AQ_KEY_2                      50
#define AQ_KEY_3                       51
#define AQ_KEY_4                      52
#define AQ_KEY_5                      53
#define AQ_KEY_6                      54
#define AQ_KEY_7                      55
#define AQ_KEY_8                      56
#define AQ_KEY_9                      57
#define AQ_KEY_SEMICOLON        59  /* ; */
#define AQ_KEY_EQUAL               61  /* = */
#define AQ_KEY_A                      65
#define AQ_KEY_B                      66
#define AQ_KEY_C                      67
#define AQ_KEY_D                      68
#define AQ_KEY_E                      69
#define AQ_KEY_F                      70
#define AQ_KEY_G                      71
#define AQ_KEY_H                      72
#define AQ_KEY_I                      73
#define AQ_KEY_J                      74
#define AQ_KEY_K                      75
#define AQ_KEY_L                      76
#define AQ_KEY_M                      77
#define AQ_KEY_N                      78
#define AQ_KEY_O                      79
#define AQ_KEY_P                      80
#define AQ_KEY_Q                      81
#define AQ_KEY_R                      82
#define AQ_KEY_S                      83
#define AQ_KEY_T                      84
#define AQ_KEY_U                      85
#define AQ_KEY_V                      86
#define AQ_KEY_W                      87
#define AQ_KEY_X                      88
#define AQ_KEY_Y                      89
#define AQ_KEY_Z                      90
#define AQ_KEY_LEFT_BRACKET       91  /* [ */
#define AQ_KEY_BACKSLASH          92  /* \ */
#define AQ_KEY_RIGHT_BRACKET      93  /* ] */
#define AQ_KEY_GRAVE_ACCENT       96  /* ` */
#define AQ_KEY_WORLD_1            161 /* non-US #1 */
#define AQ_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define AQ_KEY_ESCAPE             256
#define AQ_KEY_ENTER              257
#define AQ_KEY_TAB                258
#define AQ_KEY_BACKSPACE          259
#define AQ_KEY_INSERT             260
#define AQ_KEY_DELETE             261
#define AQ_KEY_RIGHT              262
#define AQ_KEY_LEFT               263
#define AQ_KEY_DOWN               264
#define AQ_KEY_UP                 265
#define AQ_KEY_PAGE_UP            266
#define AQ_KEY_PAGE_DOWN          267
#define AQ_KEY_HOME               268
#define AQ_KEY_END                269
#define AQ_KEY_CAPS_LOCK          280
#define AQ_KEY_SCROLL_LOCK        281
#define AQ_KEY_NUM_LOCK           282
#define AQ_KEY_PRINT_SCREEN       283
#define AQ_KEY_PAUSE              284
#define AQ_KEY_F1                 290
#define AQ_KEY_F2                 291
#define AQ_KEY_F3                 292
#define AQ_KEY_F4                 293
#define AQ_KEY_F5                 294
#define AQ_KEY_F6                 295
#define AQ_KEY_F7                 296
#define AQ_KEY_F8                 297
#define AQ_KEY_F9                 298
#define AQ_KEY_F10                299
#define AQ_KEY_F11                300
#define AQ_KEY_F12                301
#define AQ_KEY_F13                302
#define AQ_KEY_F14                303
#define AQ_KEY_F15                304
#define AQ_KEY_F16                305
#define AQ_KEY_F17                306
#define AQ_KEY_F18                307
#define AQ_KEY_F19                308
#define AQ_KEY_F20                309
#define AQ_KEY_F21                310
#define AQ_KEY_F22                311
#define AQ_KEY_F23                312
#define AQ_KEY_F24                313
#define AQ_KEY_F25                314
#define AQ_KEY_KP_0               320
#define AQ_KEY_KP_1               321
#define AQ_KEY_KP_2               322
#define AQ_KEY_KP_3               323
#define AQ_KEY_KP_4               324
#define AQ_KEY_KP_5               325
#define AQ_KEY_KP_6               326
#define AQ_KEY_KP_7               327
#define AQ_KEY_KP_8               328
#define AQ_KEY_KP_9               329
#define AQ_KEY_KP_DECIMAL         330
#define AQ_KEY_KP_DIVIDE          331
#define AQ_KEY_KP_MULTIPLY        332
#define AQ_KEY_KP_SUBTRACT        333
#define AQ_KEY_KP_ADD             334
#define AQ_KEY_KP_ENTER           335
#define AQ_KEY_KP_EQUAL           336
#define AQ_KEY_LEFT_SHIFT         340
#define AQ_KEY_LEFT_CONTROL       341
#define AQ_KEY_LEFT_ALT           342
#define AQ_KEY_LEFT_SUPER         343
#define AQ_KEY_RIGHT_SHIFT        344
#define AQ_KEY_RIGHT_CONTROL      345
#define AQ_KEY_RIGHT_ALT          346
#define AQ_KEY_RIGHT_SUPER        347
#define AQ_KEY_MENU               348

#define AQ_KEY_LAST               AQ_KEY_MENU

#define AQ_MOUSE_BUTTON_1         0
#define AQ_MOUSE_BUTTON_2         1
#define AQ_MOUSE_BUTTON_3         2
#define AQ_MOUSE_BUTTON_4         3
#define AQ_MOUSE_BUTTON_5         4
#define AQ_MOUSE_BUTTON_6         5
#define AQ_MOUSE_BUTTON_7         6
#define AQ_MOUSE_BUTTON_8         7
#define AQ_MOUSE_BUTTON_LAST      AQ_MOUSE_BUTTON_8
#define AQ_MOUSE_BUTTON_LEFT      AQ_MOUSE_BUTTON_1
#define AQ_MOUSE_BUTTON_RIGHT     AQ_MOUSE_BUTTON_2
#define AQ_MOUSE_BUTTON_MIDDLE    AQ_MOUSE_BUTTON_3