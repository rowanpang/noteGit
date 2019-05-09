INCLUDE (CheckCSourceCompiles)
INCLUDE (CheckCXXSourceCompiles)
INCLUDE (CheckStructHasMember)
INCLUDE (CheckLibraryExists)
INCLUDE (CheckFunctionExists)
INCLUDE (CheckCCompilerFlag)
INCLUDE (CheckCSourceRuns)
INCLUDE (CheckSymbolExists)

INCLUDE (CheckIncludeFiles)
INCLUDE (CheckIncludeFileCXX)


option(FOO_ENABLE "Enable Foo" ON)

IF(FOO_ENABLE)
    set(FOO_STRING "fooValuedinCMakeList.txt")
ENDIF()

configure_file(foo.h.cmake foo.h @ONLY)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES("./")

CHECK_INCLUDE_FILES (malloc.h HAVE_MALLOC_H)
CHECK_INCLUDE_FILES (stdio.h  HAVE_STDIO_H)

add_executable(demo main.c hello.c)
