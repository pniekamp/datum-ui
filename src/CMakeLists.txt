#
# datumui
#

set(CMAKE_CXX_STANDARD 14)

if(UNIX OR MINGW)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wfloat-conversion -Wno-unused-parameter -Wno-missing-field-initializers")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wfloat-conversion -Wno-unused-parameter -Wno-missing-field-initializers")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math -march=native")
endif(UNIX OR MINGW)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-braces -Wno-char-subscripts")
endif()

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4800 /wd4267 /wd4146 /wd4814")
endif(MSVC)

if(WIN32)
  add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX -D_CRT_SECURE_NO_WARNINGS -D_ENABLE_ATOMIC_ALIGNMENT_FIX)
endif(WIN32)

if(UNIX)
  add_definitions(-fpic)
endif(UNIX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

set(SRCS ${SRCS} typefont.h typefont.cpp)
set(SRCS ${SRCS} fontcatalog.h fontcatalog.cpp)
set(SRCS ${SRCS} spritecatalog.h spritecatalog.cpp)
set(SRCS ${SRCS} expression.h)
set(SRCS ${SRCS} context.h context.cpp)
set(SRCS ${SRCS} items.h items.cpp)
set(SRCS ${SRCS} scrollarea.h scrollarea.cpp)
set(SRCS ${SRCS} textarea.h textarea.cpp)
set(SRCS ${SRCS} control.h control.cpp)
set(SRCS ${SRCS} label.h label.cpp)
set(SRCS ${SRCS} button.h button.cpp)
set(SRCS ${SRCS} slider.h slider.cpp)
set(SRCS ${SRCS} lineedit.h lineedit.cpp)
set(SRCS ${SRCS} slideedit.h slideedit.cpp)
set(SRCS ${SRCS} listbox.h listbox.cpp)
set(SRCS ${SRCS} combobox.h combobox.cpp)
set(SRCS ${SRCS} groupbox.h groupbox.cpp)
set(SRCS ${SRCS} loader.h loader.cpp)

add_library(datumui STATIC ${SRCS})

set_target_properties(datumui PROPERTIES COMPILE_DEFINITIONS "DATUMUI")

target_link_libraries(datumui leap datum vulkan)

set(SRCS)
set(SRCS ${SRCS} loader.h loader.cpp)
set(SRCS ${SRCS} parser.h parser.cpp)
set(SRCS ${SRCS} generator.h generator.cpp)

add_library(datumuic STATIC ${SRCS})

set_target_properties(datumuic PROPERTIES COMPILE_DEFINITIONS "DATUMUIC")

target_link_libraries(datumuic leap datum vulkan)

set(SRCS)
set(SRCS ${SRCS} compiler.cpp)

add_executable(datumuicc ${SRCS})

target_link_libraries(datumuicc leap datumuic datumui datum vulkan)

if(MINGW)
  target_link_libraries(datumuicc mingw32)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif(MINGW)

#
# install
#

INSTALL(TARGETS datumui DESTINATION lib)
INSTALL(TARGETS datumuic DESTINATION lib)
INSTALL(TARGETS datumuicc DESTINATION bin)
