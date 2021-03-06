cmake_minimum_required(VERSION 2.8.7)
#predefined variables
set(SOURCES "")
set(HEADERS "")
set(RESOURCES "")
set(FORMS "")
set(INCLUDEPATH "")
set(LIBS "")
set(QMAKE_LFLAGS "")
set(CMAKE_CXX_STANDARD 17)
#-------------------------------------------------
#-------------------------------------------------
#
#
# Project created by QtCreator 2015-08-27T16:28:01
# Project created by QtCreator 2015-08-27T16:28:01
#
#
#-------------------------------------------------
#-------------------------------------------------
project(CubeModelConvertor)
#CONFIG += console
#CONFIG += console
set(TEMPLATE app)
#OpenGL
#OpenGL
###Application folder BEGIN###
include_directories(./Application)
###Application folder END###

####External library BEGIN####
include_directories(./External)

add_compile_definitions(WITH_OPENCV2)

#rapidJson
include_directories(./External/rapidjson/include)

#debugHelp
set(LIBS ${LIBS} Dbghelp)

#misc
set(LIBS ${LIBS} assimp )
include_directories(./External/Assimp)

####External library END####

set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}")

#SIMD
add_compile_options("/arch:AVX")



set(LINK_DIR ./libVC)
link_directories(${LINK_DIR})


macro(source_group_by_dir source_files)
    set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
    foreach(sgbd_file ${${source_files}})
        string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
        string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
        string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
        string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
        if(sgbd_nogroup)
            set(sgbd_group_name "\\")
        endif(sgbd_nogroup)
		#message("The Group: " ${sgbd_group_name})
        source_group(${sgbd_group_name} FILES ${sgbd_file})
    endforeach(sgbd_file)
endmacro(source_group_by_dir)

#copy res
#define
add_definitions(-DB3_USE_CLEW)

file(GLOB_RECURSE project_headers *.h *.hpp)
file(GLOB_RECURSE project_cpps *.cpp *.c)
set(all_files ${project_headers} ${project_cpps})
source_group_by_dir(all_files)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_BUILD_FILES_DIRECTORY}) # fixes syntax analysis in CLion
set(EngineSrcDir "./EngineSrc/")
INCLUDE_DIRECTORIES(${CMAKE_BUILD_FILES_DIRECTORY} ${EngineSrcDir})
set(TARGET ${CMAKE_PROJECT_NAME})
add_executable(${TARGET} ${all_files})
target_link_libraries(${TARGET} ${QMAKE_LFLAGS})
target_link_libraries(${TARGET} ${LIBS})

