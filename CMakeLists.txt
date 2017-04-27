cmake_minimum_required (VERSION 2.6)

project (DTN)

# The version number
#set (DTN_VERSION_MAJOR 0)
#set (DTN_VERSION_MINOR 1)

# Compiler settings
set (CXX11_FLAGS -std=gnu++11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")

# Set includes
include_directories (include)

# Set sources
file (GLOB SOURCES "src/*.cpp")

# Compilation output
add_custom_target ("${PROJECT_NAME}_SUCCESSFUL" ALL
                   DEPENDS ${PROJECT_NAME}
                   COMMAND ${CMAKE_COMMAND} -E echo ""
                   COMMAND ${CMAKE_COMMAND} -E echo "====================="
                   COMMAND ${CMAKE_COMMAND} -E echo "  Compile complete!"
                   COMMAND ${CMAKE_COMMAND} -E echo "====================="
                   COMMAND ${CMAKE_COMMAND} -E echo "${CMAKE_CURRENT_BINARY_DIR}"
                  )

add_executable (${PROJECT_NAME} ${SOURCES})
