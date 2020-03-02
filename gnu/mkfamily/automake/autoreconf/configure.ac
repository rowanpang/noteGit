AC_INIT([hello], [2.0], [bug-automake@gnu.org])
AC_CONFIG_AUX_DIR(build-aux)
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    Makefile
    src/Makefile
])
AC_CANONICAL_BUILD
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AX_CHECK_OPENSSL([AC_DEFINE(HAVE_OPENSSL,1,'if exit')])
AC_OUTPUT
