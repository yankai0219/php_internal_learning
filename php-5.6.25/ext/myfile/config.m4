dnl $Id$
dnl config.m4 for extension myfile

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(myfile, for myfile support,
dnl Make sure that the comment is aligned:
dnl [  --with-myfile             Include myfile support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(myfile, whether to enable myfile support,
Make sure that the comment is aligned:
[  --enable-myfile           Enable myfile support])

if test "$PHP_MYFILE" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-myfile -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/myfile.h"  # you most likely want to change this
  dnl if test -r $PHP_MYFILE/$SEARCH_FOR; then # path given as parameter
  dnl   MYFILE_DIR=$PHP_MYFILE
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for myfile files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       MYFILE_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$MYFILE_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the myfile distribution])
  dnl fi

  dnl # --with-myfile -> add include path
  dnl PHP_ADD_INCLUDE($MYFILE_DIR/include)

  dnl # --with-myfile -> check for lib and symbol presence
  dnl LIBNAME=myfile # you may want to change this
  dnl LIBSYMBOL=myfile # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $MYFILE_DIR/$PHP_LIBDIR, MYFILE_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_MYFILELIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong myfile lib version or lib not found])
  dnl ],[
  dnl   -L$MYFILE_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(MYFILE_SHARED_LIBADD)

  PHP_NEW_EXTENSION(myfile, myfile.c, $ext_shared)
fi
