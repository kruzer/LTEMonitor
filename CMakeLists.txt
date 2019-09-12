
cmake_minimum_required(VERSION 3.2)
project(LTEMonitor)


set (lte_monitor_VERSION_MAJOR 0)
set (lte_monitor_VERSION_MINOR 13)
 
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

if(NOT MSVC)
    add_compile_options(-Wall -Wextra -Wsign-conversion)
else()
    add_compile_options(/W1)
endif()
SET(Boost_USE_STATIC_LIBS ON)
add_subdirectory(Simple-Web-Server)

find_package(Boost COMPONENTS log program_options REQUIRED)
include_directories( ${Boost_INCLUDE_DIRS} ${PROJECT_BINARY_DIR})


configure_file (
  "${PROJECT_SOURCE_DIR}/Version.h.in"
  "${PROJECT_BINARY_DIR}/Version.h"
)

add_library(LTEClasses ProgramOptions.cpp HuaweiClient.cpp AngularServer.cpp )
target_link_libraries(LTEClasses INTERFACE simple-web-server ${Boost_LIBRARIES})
add_executable(LTEMonitor main.cpp)

target_link_libraries(LTEMonitor LTEClasses)

enable_testing()
add_subdirectory(tests)

# Creates C resources file from files in given directory

function(create_resources dir output)
	# Create empty output file
	file(WRITE ${output} "")
	# Collect input files
	file(GLOB bins ${dir}/*)
	# Iterate through input files
	foreach(bin ${bins})
		# Get short filename
		string(REGEX MATCH "([^/]+)$" filename ${bin})
		set(nameOnly ${filename})
		# Replace filename spaces & extension separator for C compatibility
		string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
		# Read hex data from file
		file(READ ${bin} filedata HEX)
		# Convert hex data for C compatibility
		string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
		# Append data to output file
		file(APPEND ${output} "const unsigned char x${filename}[] = {${filedata}};\nconst unsigned x${filename}_size = sizeof(x${filename});\n")
		string(APPEND resmap "{ \"${nameOnly}\" , { x${filename}_size, x${filename}} },\n")
	endforeach()
	file(APPEND ${output} "std::map <std::string, std::pair <const unsigned,const unsigned char *> > ResourcesMap = {\n")
	file(APPEND ${output} ${resmap})
	file(APPEND ${output} "};\n")
endfunction()
create_resources("${PROJECT_SOURCE_DIR}/lmgui/dist/lmgui" AngularResources.h) 
