// used in src/Languages.cpp, encoding=UTF8
#include "../locale/bitmaps/germany.png.h"
#include "../locale/bitmaps/united_kingdom.png.h"
#include "../locale/bitmaps/portugal.png.h"
#include "../locale/bitmaps/russia.png.h"
#include "../locale/bitmaps/spain.png.h"

struct Lang {
	const char* name;
	wxLanguage id;
	const unsigned char* bitmap;
	unsigned int bmpLength;
};

Lang s_langs[] = {
	{"Deutsch", wxLANGUAGE_GERMAN, (const unsigned char *) germany_png, sizeof(germany_png)},
	{"English", wxLANGUAGE_ENGLISH, (const unsigned char *) united_kingdom_png, sizeof(united_kingdom_png)},
	{"Español", wxLANGUAGE_SPANISH, (const unsigned char *) spain_png, sizeof(spain_png)},
	{"Português", wxLANGUAGE_PORTUGUESE, (const unsigned char *) portugal_png, sizeof(portugal_png)},
	{"Pyccкий", wxLANGUAGE_RUSSIAN, (const unsigned char *) russia_png, sizeof(russia_png)},
};
