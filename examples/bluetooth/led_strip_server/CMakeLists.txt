cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(bluetooth_server)

target_include_directories(app PRIVATE src/include)

target_sources(app PRIVATE 
    src/main.c
    src/gap.c
    src/gatt.c
    src/led_strip.c
)
