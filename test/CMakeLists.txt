
add_compile_options(-Wall -Wextra -Werror)

add_executable(ws2812_test
    ws2812_test.cpp
)

pico_enable_stdio_uart(ws2812_test 0)
pico_enable_stdio_usb(ws2812_test 1)

target_include_directories(ws2812_test PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(ws2812_test PRIVATE
    pico_stdlib
    pico_stdio_usb
    ws2812
)

pico_add_extra_outputs(ws2812_test)
