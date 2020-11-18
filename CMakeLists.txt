cmake_minimum_required(VERSION 3.13)
project(PolygonDivide_BD)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:/Qt/5.11.1/msvc2017_64/lib/cmake")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLibs/Modules)

set(TUFAO1_ROOT "C:/Program Files/tufao")
set(BOOST_ROOT "E:/DividePolygon/Tufao/boost_1_71_0/boost_1_71_0")

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Tufao1 REQUIRED)
find_package(BOOST REQUIRED)
find_package(CGAL REQUIRED)

set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE TRUE)

include( ${CGAL_USE_FILE} )

include_directories (${CGAL_INCLUDE_DIRS})

##Setting Include directories
include_directories(${TUFAO_INCLUDE_DIR})

set(HDR
    FPProcess.h
    RestApiHandler.h
    WebService.h
    Job.h
    DivideJob.h
    typedef.h
    Polygon.h
    JobTypes.h Utils.h)

set(SRC
    main.cpp
    FPProcess.cpp
    RestApiHandler.cpp
    WebService.cpp
    Job.cpp
    DivideJob.cpp
    Polygon.cpp
    JobTypes.cpp)

add_executable(PolygonDivide_BD ${HDR} ${SRC})

##TODO:Currently linking Debug library !! Will Change to Release
target_link_libraries(PolygonDivide_BD Qt5::Core Qt5::Network)

target_link_libraries(PolygonDivide_BD
                      debug ${TUFAO_DEBUG_LIB})

get_target_property(QMAKE_EXECUTABLE Qt5::qmake LOCATION)

function(QUERY_QMAKE VAR RESULT)
   exec_program("${QMAKE_EXECUTABLE}" ARGS "-query ${VAR}" RETURN_VALUE return_code OUTPUT_VARIABLE output)
   if (NOT return_code)
      file(TO_CMAKE_PATH "${output}" output)
      set(${RESULT} ${output} PARENT_SCOPE)
   endif (NOT return_code)
endfunction(QUERY_QMAKE)

query_qmake(QT_INSTALL_BINS QT_DLL_DIR)

set(QT_BINARIES ${QT_DLL_DIR}/Qt5Core.dll ${QT_DLL_DIR}/Qt5Network.dll)
set(QT_DEBUG_BINARIES ${QT_DLL_DIR}/Qt5Cored.dll ${QT_DLL_DIR}/Qt5Networkd.dll)

file(COPY ${QT_BINARIES} 
	 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${QT_DEBUG_BINARIES} 
	 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

message(${QT_DEBUG_BINARIES})