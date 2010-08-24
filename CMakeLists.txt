project (plasma-apcups)
cmake_minimum_required (VERSION 2.6)

# Initialize project version
file (READ VERSION AU_PROJECT_VERSION)
add_definitions (-DPROJECT_VERSION="${AU_PROJECT_VERSION}")

add_subdirectory (plasma-dataengine-apcups)
add_subdirectory (plasma-applet-apcups)
add_subdirectory (po)
