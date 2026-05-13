
add_library(ws2812 INTERFACE) 

target_sources(ws2812 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.cpp
)

pico_generate_pio_header(ws2812
    ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio
)

target_include_directories(ws2812 INTERFACE 
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ws2812 INTERFACE
    pico_stdlib
    hardware_gpio
    hardware_pio
)

add_subdirectory(test)
