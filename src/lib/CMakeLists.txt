cmake_minimum_required(VERSION 3.0.0)

file(GLOB lib
    "*.cpp"
	"parser/*.cpp"
	"ast/*.cpp"
	"generator/*.cpp"
)

file(GLOB lua
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lapi.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lauxlib.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lbaselib.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lcode.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lcorolib.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lctype.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/ldblib.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/ldebug.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/ldo.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/ldump.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lfunc.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lgc.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/linit.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/liolib.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/llex.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lmathlib.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lmem.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/loadlib.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lobject.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lopcodes.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/loslib.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lparser.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lstate.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lstring.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lstrlib.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/ltable.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/ltablib.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/ltm.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lundump.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lvm.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lzio.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/print.c"
    "${PROJECT_SOURCE_DIR}/contrib/lua-5.3.4/src/lutf8lib.c"
)

add_library (${PROJECT_NAME} ${lib} ${lua})