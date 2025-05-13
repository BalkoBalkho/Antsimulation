cmake_minimum_required(VERSION 3.10)  

# Enable Hot Reload for MSVC compilers if supported.
if (TRUE)
  set(CMAKE_CXX_STANDARD 20)
    
    cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

#project ("Antsimulation")

# Include sub-projects.
#add_subdirectory ("Antsimulation")
project("AntSimulation")  

# Tell CMake where SFML is (LOCAL PATH, NO VCPKG)  
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML-2.6.2-win")  

# Add all .cpp files in /src  
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/*.cpp")  
# Create executable  
add_executable(AntSimulation ${SOURCES}  "Simulation.cpp" "hashgrid.hpp")  

# Link SFML (STATICALLY, NO DLL HELL)  
target_include_directories(AntSimulation PRIVATE "${SFML_DIR}/include")  
target_link_directories(AntSimulation PRIVATE "${SFML_DIR}/lib")  
target_link_libraries(AntSimulation  
    sfml-graphics  
    sfml-window  
    sfml-system  
    sfml-audio
)  

# Copy SFML DLLs to build folder (ONLY IF DYNAMIC)  
if(TRUE)  
    add_custom_command(TARGET AntSimulation POST_BUILD  
        COMMAND ${CMAKE_COMMAND} -E copy  
            "${SFML_DIR}/bin/sfml-graphics-2.dll"  
            "${CMAKE_BINARY_DIR}/sfml-graphics-2.dll"  
        COMMAND ${CMAKE_COMMAND} -E copy  
            "${SFML_DIR}/bin/sfml-window-2.dll"  
            "${CMAKE_BINARY_DIR}/sfml-window-2.dll"  
        COMMAND ${CMAKE_COMMAND} -E copy  
            "${SFML_DIR}/bin/sfml-system-2.dll"  
            "${CMAKE_BINARY_DIR}/sfml-system-2.dll"
        COMMAND ${CMAKE_COMMAND} -E copy  
            "${SFML_DIR}/bin/sfml-audio-2.dll"  
            "${CMAKE_BINARY_DIR}/sfml-audio-2.dll"
        COMMAND ${CMAKE_COMMAND} -E copy  
            "${SFML_DIR}/bin/openal32.dll"  
            "${CMAKE_BINARY_DIR}/openal32.dll"
    )  
endif()  