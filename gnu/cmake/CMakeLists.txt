option(FOO_ENABLE "Enable Foo" ON)
IF(FOO_ENABLE)
    set(FOO_STRING "fooValuedinCMakeList.txt")
ENDIF()
configure_file(foo.h.cmake foo.h @ONLY)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES("./")

add_executable(demo main.c hello.c)
