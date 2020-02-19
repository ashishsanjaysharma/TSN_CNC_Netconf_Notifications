set (ENDAPP_SOURCES
    cnc_notifications.c
)

include_directories(
    "/usr/include/x86_64-linux-gnu"
    "/usr/local/include/libyang"
    )
link_directories(/usr/local/lib)

add_executable(cnc_notifications ${ENDAPP_SOURCES})
add_library(cnc_config_parser STATIC cnc_config_parser.c)
set_property(TARGET cnc_config_parser PROPERTY COMPILE_FLAGS "-fPIC")
target_link_libraries(cnc_config_parser yang)
target_link_libraries(cnc_notifications cnc_config_parser)
target_link_libraries(cnc_notifications sysrepo)

install(TARGETS cnc_notifications DESTINATION ${CMAKE_INSTALL_BINDIR})
