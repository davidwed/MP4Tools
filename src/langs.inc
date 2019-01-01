// used in src/Languages.cpp, encoding=UTF8
#include "../locale/bitmaps/china.png.h"
#include "../locale/bitmaps/french.png.h"
#include "../locale/bitmaps/germany.png.h"
#include "../locale/bitmaps/hungary.png.h"
#include "../locale/bitmaps/italy.png.h"
#include "../locale/bitmaps/poland.png.h"
#include "../locale/bitmaps/portugal.png.h"
#include "../locale/bitmaps/russia.png.h"
#include "../locale/bitmaps/spain.png.h"
#include "../locale/bitmaps/sri_lanka.png.h"
#include "../locale/bitmaps/taiwan.png.h"
#include "../locale/bitmaps/turkey.png.h"
#include "../locale/bitmaps/united_kingdom.png.h"

struct Lang {
	const char* name;
	wxLanguage id;
	const unsigned char* bitmap;
	unsigned int bmpLength;
};

Lang s_langs[] = {
	{ "Deutsch", wxLANGUAGE_GERMAN, (const unsigned char *) germany_png, sizeof(germany_png)},
	{ "English", wxLANGUAGE_ENGLISH, (const unsigned char *) united_kingdom_png, sizeof(united_kingdom_png)},
	{ "Español", wxLANGUAGE_SPANISH, (const unsigned char *) spain_png, sizeof(spain_png)},
	{ "Français", wxLANGUAGE_FRENCH, (const unsigned char *) french_png, sizeof(french_png)},
	{ "Italiano", wxLANGUAGE_ITALIAN, (const unsigned char *) italy_png, sizeof(italy_png)},
	{ "Magyar", wxLANGUAGE_HUNGARIAN, (const unsigned char *) hungary_png, sizeof(hungary_png)},
	{ "Polski", wxLANGUAGE_POLISH, (const unsigned char *) poland_png, sizeof(poland_png)},
	{ "Português", wxLANGUAGE_PORTUGUESE, (const unsigned char *) portugal_png, sizeof(portugal_png)},
	{ "Pyccкий", wxLANGUAGE_RUSSIAN, (const unsigned char *) russia_png, sizeof(russia_png)},
	{ "සිංහල", wxLANGUAGE_SINHALESE, (const unsigned char *) sri_lanka_png, sizeof(sri_lanka_png)},
	{ "Türkçe", wxLANGUAGE_TURKISH, (const unsigned char *) turkey_png, sizeof(turkey_png)},
	{ "简体中文", wxLANGUAGE_CHINESE_SIMPLIFIED, (const unsigned char *) china_png, sizeof(china_png)},
	{ "繁體中文", wxLANGUAGE_CHINESE_TRADITIONAL, (const unsigned char *) taiwan_png, sizeof(taiwan_png)},
};
