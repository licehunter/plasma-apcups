project(plasma-applet-apcups)
 
cmake_minimum_required(VERSION 2.6)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 
set(apcups_SRCS apcupsplasmoid.cpp apcupsplasmoidwidget.cpp)

kde4_add_ui_files(apcups_SRCS ApcUpsPlasmoidConfiguration.ui)
kde4_add_plugin(plasma-applet-apcups ${apcups_SRCS})
target_link_libraries(plasma-applet-apcups 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS})
 
install(TARGETS plasma-applet-apcups
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-apcups.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})

# Install icons
add_subdirectory (img)
