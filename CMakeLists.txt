# CMakeList.txt : CMake project for Tplai, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_TOOLCHAIN_FILE "vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain file")

project ("Tplai")

find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)

# Add source to this project's executable.
add_executable (Tplai "Tplai.cpp" "Tplai.h"     "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Tplai PROPERTY CXX_STANDARD 20)
endif()	

target_link_libraries(Tplai PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)