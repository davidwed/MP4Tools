dnl Process this file with autoconf to produce a configure script.

dnl initialization
AC_INIT([MP4Tools], [3.7], [http://www.mp4joiner.org])
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE

dnl options
AM_OPTIONS_WXCONFIG
AC_ARG_ENABLE(debug, [  --enable-debug          Enable debugging information], USE_DEBUG="$enableval", USE_DEBUG="no")

AC_MSG_CHECKING(for install location)
case "$prefix" in
NONE)	AC_CACHE_VAL(m_cv_prefix,m_cv_prefix=$ac_default_prefix);;
*)	m_cv_prefix=$prefix ;;
esac
AC_MSG_RESULT($m_cv_prefix)
case "$m_cv_prefix" in /*) ;; *)
	AC_MSG_WARN([--prefix=$prefix must be an absolute path name, using $ac_default_prefix])
	m_cv_prefix=$ac_default_prefix
esac
prefix=$m_cv_prefix

dnl Checks for programs.
AC_LANG_CPLUSPLUS
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CXXCPP
PKG_PROG_PKG_CONFIG()

dnl debug flags
DEBUG_FLAGS=""
if test "$USE_DEBUG" = "yes"; then
	DEBUG_FLAGS="-g -Wall -DDEBUG"
fi
CXXFLAGS="$CXXFLAGS $DEBUG_FLAGS -fno-strict-aliasing "

dnl check wxWidgets >= 2.8.7
WX_CONFIG_CHECK([2.8.7], [WXFOUND=1], [WXFOUND=0], [html,core,net,adv,media])
if test "$WXFOUND" != 1; then
    AC_MSG_ERROR([
        Please check that wx-config is in path, the directory
        where wxWidgets libraries are installed (returned by
        'wx-config --libs' command) is in LD_LIBRARY_PATH or
        equivalent variable and wxWidgets is version 2.8.7 or above.
    ])
fi
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS "
CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS "
LIBS="$LIBS $WX_LIBS "

dnl check libav/ffmpeg
PKG_CHECK_MODULES(LIBAV, [libavformat >= 53.20.0 libavcodec >= 53.34.0 libavutil libswscale libavfilter >= 2.15.0])
CXXFLAGS="$CXXFLAGS $LIBAV_CFLAGS"
LIBS="$LIBS $LIBAV_LIBS"
AC_CHECK_HEADER([libavformat/avformat.h],[],[AC_MSG_ERROR([*** missing libav (www.libav.org) or ffmpeg (www.ffmpeg.org) library])],[
#define __STDC_CONSTANT_MACROS
#define __STDC_LIMIT_MACROS
#include <stdint.h>])

dnl check for wxSVG
PKG_CHECK_MODULES(wxSVG, [libwxsvg >= 1.1.13], [LIBS="$LIBS $wxSVG_LIBS"], [AC_MSG_ERROR([*** missing wxSVG library >= 1.1.13 ***])])
AC_MSG_CHECKING([for wxSVG library with ffmpeg/libav support])
WXSVGLIBAV=no
CXXFLAGS="$CXXFLAGS $wxSVG_CFLAGS"
LIBS="$LIBS $wxSVG_LIBS"
AC_LINK_IFELSE([AC_LANG_PROGRAM(
    [[#include <wxSVG/mediadec_ffmpeg.h>]],
    [[wxFfmpegMediaDecoder *a = new wxFfmpegMediaDecoder;]])],
    [WXSVGLIBAV=yes], [WXSVGLIBAV=no])
AC_MSG_RESULT([$WXSVGLIBAV])
if test "x$WXSVGLIBAV" = x"no"; then
  AC_MSG_ERROR([
    MP4Joiner requires the wxSVG library with ffmpeg/libav support.
])
fi

dnl vars
AC_SUBST(LDFLAGS)
AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(WX_CONFIG_NAME)

dnl check msgfmt
AC_PATH_PROG(MSGFMT_PATH, msgfmt, "")
if test "x$MSGFMT_PATH" = x; then
  AC_MSG_ERROR([
    MP4Joiner requires gettext-tools.
    Please check that msgfmt is in path.
])
fi

AC_PATH_PROG(MP4BOX_PATH, MP4Box, "", [$PATH:/Applications/Osmo4.app/Contents/MacOS])
if test "x$MP4BOX_PATH" = x; then
  AC_MSG_ERROR([
    MP4Joiner requires MP4Box-tool (GPAC).
    Please check that MP4Box is in path.
])
fi

dnl check avconv
AC_PATH_PROG(AVCONV_PATH, avconv, "")
if test "x$AVCONV_PATH" = x; then
	dnl check ffmpeg
	AC_PATH_PROG(FFMPEG_PATH, ffmpeg, "")
	if test "x$FFMPEG_PATH" = x; then
	  AC_MSG_ERROR([
	    MP4Joiner requires libav or ffmpeg tools.
	    Please check that avconv or ffmpeg is in path.
	])
	fi
	CXXFLAGS="$CXXFLAGS -DUSE_FFMPEG=1"
fi

dnl create Makefiles
AC_CONFIG_FILES([
  Makefile
  Info.plist
  InfoSplitter.plist
  src/Makefile
  src/Version.h
  locale/Makefile
])
AC_OUTPUT
