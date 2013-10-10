# At this point, the Xcode project assumes the version string will be three
# integers separated by periods and surrounded by square brackets (e.g.
# "[1.0.1]"). It also asumes that there won't be any closing parenthesis
# between "AC_INIT(" and the closing ")" including comments and strings.
AC_INIT([C++ Client for Apache Kafka v0.8+],
        [0.3.0],
        [dt@dt.org],
        [libkafka])

# Provide various options to initialize the Autoconf and configure processes.
AC_PREREQ([2.59])
AC_CONFIG_SRCDIR([lib/src])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADERS([config/config.h])
AC_CONFIG_FILES([Makefile])

# Initialize Automake with various options. We require at least v1.9, prevent
# pedantic complaints about package files, and enable various distribution
# targets.
AM_INIT_AUTOMAKE([1.9 dist-bzip2 dist-zip foreign subdir-objects])

# Check for programs used in building libkafka
AC_PROG_CC
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_LIBTOOL
GTEST_LIB_CHECK(,AC_MSG_WARN(foo),AC_MSG_WARN(bar))

###########################
# gcov coverage reporting
###########################
AC_TDD_GCOV
AC_SUBST(COVERAGE_CFLAGS)
AC_SUBST(COVERAGE_CXXFLAGS)
AC_SUBST(COVERAGE_LDFLAGS)

AC_OUTPUT
