find_package(Boost REQUIRED COMPONENTS program_options)

configure_file(./config.hpp.in ./config.hpp)

set(TARGET hedwig)
set(SOURCES
    main.cpp
    core.hpp
    core.cpp

    playground.hpp
    playground.cpp
)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} PRIVATE Boost::program_options)
