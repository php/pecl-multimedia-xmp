dnl $Id$
dnl config.m4 for extension xmp

dnl Base file contributed by Boro Sitnikovski

PHP_ARG_WITH(xmp, for xmp support,
dnl Make sure that the comment is aligned:
[  --with-xmp               Enable xmp support])

if test "$PHP_XMP" != "no"; then
 # --with-xmp -> check with-path
 SEARCH_PATH="/usr/local /usr"     # you might want to change this
 SEARCH_FOR="/include/xmp.h"  # you most likely want to change this
 if test -r $PHP_XMP/$SEARCH_FOR; then # path given as parameter
   XMP_DIR=$PHP_XMP
 else # search default path list
   AC_MSG_CHECKING([for xmp files in default path])
   for i in $SEARCH_PATH ; do
     if test -r $i/$SEARCH_FOR; then
       XMP_DIR=$i
       AC_MSG_RESULT(found in $i)
     fi
   done
 fi

 if test -z "$XMP_DIR"; then
   AC_MSG_RESULT([not found])
   AC_MSG_ERROR([Please reinstall the xmp distribution])
 fi

 # --with-xmp -> add include path
 PHP_ADD_INCLUDE($XMP_DIR/include)

 # --with-xmp -> check for lib and symbol presence
 LIBNAME=xmp # you may want to change this
 LIBSYMBOL=xmp_create_context # you most likely want to change this 

 PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
 [
   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $XMP_DIR/$PHP_LIBDIR, XMP_SHARED_LIBADD)
   AC_DEFINE(HAVE_XMPLIB,1,[ ])
 ],[
   AC_MSG_ERROR([wrong xmp lib version or lib not found])
 ],[
   -L$XMP_DIR/$PHP_LIBDIR -lm
 ])

 PHP_SUBST(XMP_SHARED_LIBADD)
 CFLAGS="$CFLAGS -Wall"
 PHP_NEW_EXTENSION(xmp, php_xmp.c, $ext_shared)
fi
