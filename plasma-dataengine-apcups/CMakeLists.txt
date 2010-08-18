# A name for the project
project(plasma-dataengine-apcups)

cmake_minimum_required(VERSION 2.6)

# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 
# We add our source code here
set(apcups_engine_SRCS apcupsengine.cpp apcupsc.cpp apcupscif.cpp)
 
# Now make sure all files get to the right place
kde4_add_plugin(plasma_engine_apcups ${apcups_engine_SRCS})

target_link_libraries(plasma_engine_apcups
                      ${QT_QTNETWORK_LIBRARY}
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_PLASMA_LIBS})
 
set (SS_ENGINE_DESKTOP_FILE plasma-engine-apcups.desktop)
configure_file (${SS_ENGINE_DESKTOP_FILE}.in ${CMAKE_CURRENT_BINARY_DIR}/${SS_ENGINE_DESKTOP_FILE})

install(TARGETS plasma_engine_apcups
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES ${SS_ENGINE_DESKTOP_FILE}
        DESTINATION ${SERVICES_INSTALL_DIR})
