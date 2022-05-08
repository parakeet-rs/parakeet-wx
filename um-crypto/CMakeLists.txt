project(um-crypto)

message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

find_package(Boost REQUIRED)
find_package(Botan REQUIRED)

set(umCrypto_LIBRARIES
  ${Boost_LIBRARIES}
  ${BOTAN_LIBRARIES}
)

set(umCrypto_INCLUDE_DIRS
  "include"
  ${BOOST_INCLUDE_DIRS}
  ${BOTAN_INCLUDE_DIRS}
)

# FIXME: Expose the following variables so it can be used
#        outside of this repo.
include_directories(${umCrypto_INCLUDE_DIRS})
link_libraries(${umCrypto_LIBRARIES})

if (!MSVC)
	link_libraries(m)
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")

file(GLOB_RECURSE SOURCES *.cpp *.h)

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

# Tests!
enable_testing()
find_package(GTest CONFIG)

if(TARGET GTest::gtest)
  file(GLOB_RECURSE TESTS_SOURCE *.test.cc)
  add_executable(${PROJECT_NAME}_test ${TESTS_SOURCE})
  target_link_libraries(${PROJECT_NAME}_test GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
  target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})

  include(GoogleTest)
  gtest_discover_tests(${PROJECT_NAME}_test)
endif()
