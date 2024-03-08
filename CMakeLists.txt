cmake_minimum_required(VERSION 3.20)
project(DSALibraries VERSION 1.0.0 LANGUAGES CXX)
include(FetchContent)
enable_testing()
include(CTest)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
    message(STATUS "This project is a top-level one")
endif()

FetchContent_Declare(
       googletest
       URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
       DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_library(${PROJECT_NAME} STATIC
        include/Containers/Stack/Stack.hpp
        include/Utilities/Exception.hpp
        include/Utilities/Allocator.hpp
        include/Containers/List/List.hpp
        include/Containers/List/ListIterator.hpp
        include/Containers/List/ListNode.hpp
        include/Containers/List/ListExceptions.hpp
        include/Containers/SList/SList.hpp
        include/Containers/SList/SListIterator.hpp
        include/Containers/SList/SListNode.hpp
        include/Containers/SList/SListExceptions.hpp
        include/Containers/DList/DList.hpp
        include/Containers/DList/DListIterator.hpp
        include/Containers/DList/DListNode.hpp
        include/Containers/DList/DListExceptions.hpp
        include/Containers/CList/CList.hpp
        include/Containers/CList/CListIterator.hpp
        include/Containers/CList/CListNode.hpp
        include/Containers/CList/CListExceptions.hpp)



target_sources(${PROJECT_NAME}
    PRIVATE
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/Containers>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/Containers/Stack>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/Containers/List>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/Containers/SList>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/Utilities>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)


add_executable(tests tests/test_defaults.cpp)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(tests gtest_main ${PROJECT_NAME})
gtest_discover_tests(tests)




# totally optional listing of include directories
get_property(inclds
    TARGET ${PROJECT_NAME}
    PROPERTY INCLUDE_DIRECTORIES
)
message(STATUS "Listing include directories for ${PROJECT_NAME}...")
# "dumb" listing with "raw" generator expressions on configuration
foreach(pth ${inclds})
    message(STATUS "  ${pth}")
endforeach()
# actually evaluated generator expressions printed to file on generation
string(JOIN "\n" includeDirectories ${inclds})
file(GENERATE
    OUTPUT "${CMAKE_BINARY_DIR}/include-directories.txt"
    CONTENT ${includeDirectories}
)

# without it public headers won't get installed
set(public_headers
    include/Containers/Stack/Stack.hpp
        include/Utilities/Exception.hpp
        include/Utilities/Allocator.hpp
        include/Containers/List/List.hpp
        include/Containers/List/ListIterator.hpp
        include/Containers/List/ListNode.hpp
        include/Containers/SList/ListExceptions.hpp
        include/Containers/SList/SList.hpp
        include/Containers/SList/SListIterator.hpp
        include/Containers/SList/SListNode.hpp
        include/Containers/SList/SListExceptions.hpp
        include/Containers/DList/DList.hpp
        include/Containers/DList/DListIterator.hpp
        include/Containers/DList/DListNode.hpp
        include/Containers/DList/DListExceptions.hpp
        include/Containers/CList/CList.hpp
        include/Containers/CList/CListIterator.hpp
        include/Containers/CList/CListNode.hpp
        include/Containers/CList/CListExceptions.hpp

)

# not for MSVC
if(CMAKE_COMPILER_IS_GNUCXX)
    # compile options for this target only
    # treat warnings as errors
    target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
endif()

# where to find our CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#message(STATUS "CMake module path: ${CMAKE_MODULE_PATH}")

include(Installing)