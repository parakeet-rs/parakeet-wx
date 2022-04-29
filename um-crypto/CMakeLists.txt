project(um-crypto)

include_directories(include)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")

file(GLOB SOURCES *.cpp **/*.cpp)

# Add source to this project's executable.
add_library (${PROJECT_NAME} OBJECT ${SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1)

# shared and static libraries built from the same object files
add_library(${PROJECT_NAME}_shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}>)
add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}>)

target_include_directories(${PROJECT_NAME}_shared PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_include_directories(${PROJECT_NAME}_static PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

if(WIN32)
	target_link_libraries(${PROJECT_NAME}_shared ws2_32)
	target_link_libraries(${PROJECT_NAME}_static ws2_32)
endif()

if (!MSVC)
	target_link_libraries(${PROJECT_NAME} m)
endif()
