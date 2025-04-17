# CMakeList.txt : CMake project for Antsimulation, include source and define
# project specific logic here.
#


include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

# Add source to this project's executable.
add_executable (Antsimulation "Antsimulation.cpp" "Antsimulation.h" "Ant.h" "World.h" "World.cpp" "Pherenome.h" "simplenoise.h" "Ant.cpp" "Job.cpp")

target_link_libraries(Antsimulation PRIVATE raylib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Antsimulation PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
