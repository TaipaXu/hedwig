find_package(PkgConfig REQUIRED)

pkg_check_modules(X11 REQUIRED x11)

set(TARGET hedwig)
set(SOURCES
    main.cpp
    core.hpp
    core.cpp
)

add_executable(${TARGET} ${SOURCES})

target_include_directories(${TARGET} PRIVATE ${X11_INCLUDE_DIRS})
target_link_libraries(${TARGET} PRIVATE ${X11_LIBRARIES})
