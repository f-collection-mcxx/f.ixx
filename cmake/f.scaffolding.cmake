option(BUILD_TEST "build test" ON)
set(USE_CUDA OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


function(scan_target name)
    file(GLOB_RECURSE h         CONFIGURE_DEPENDS src/*.h)
    file(GLOB_RECURSE hpp       CONFIGURE_DEPENDS src/*.hpp)
    file(GLOB_RECURSE private_h CONFIGURE_DEPENDS src/*.private.h)
    file(GLOB_RECURSE private_hpp CONFIGURE_DEPENDS src/*.private.hpp)
    file(GLOB_RECURSE cpp       CONFIGURE_DEPENDS src/*.cpp)
    file(GLOB         main      CONFIGURE_DEPENDS src/main.cpp)
    file(GLOB_RECURSE ixx       CONFIGURE_DEPENDS src/*.ixx)
    file(GLOB_RECURSE test      CONFIGURE_DEPENDS src/*.test.cpp)

    file(GLOB_RECURSE cu        CONFIGURE_DEPENDS src/*.cu)
    file(GLOB_RECURSE test_cu   CONFIGURE_DEPENDS src/*.test.cu)

    if(cu)
        if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
            set(CMAKE_CUDA_ARCHITECTURES 75 86 89 90)
        endif()
        enable_language(CUDA)
        set(CMAKE_CUDA_STANDARD 20)
        set(CMAKE_CUDA_STANDARD_REQUIRED ON)
        set(CMAKE_CUDA_EXTENSIONS ON)
    endif ()


    list(REMOVE_ITEM h ${private_h})
    list(REMOVE_ITEM hpp ${private_hpp})
    list(REMOVE_ITEM cpp ${test} ${main})
    list(REMOVE_ITEM cu ${test_cu})



    set(${name}_header ${h} ${hpp} PARENT_SCOPE)
    set(${name}_main ${main} PARENT_SCOPE)
    set(${name}_src ${cu} ${cpp} PARENT_SCOPE)
    set(${name}_ixx ${ixx} PARENT_SCOPE)
    set(${name}_test ${test} ${test_cu} PARENT_SCOPE)
endfunction()

function(configure_target target)
    scan_target(${target})
    get_target_property(target_type ${target} TYPE)
    if (target_type STREQUAL "EXECUTABLE")
        set(lib ${target}_lib)
        target_sources(${target} PRIVATE ${${target}_main})
        add_library(${lib} STATIC)
        target_link_libraries(${target} PRIVATE ${lib})

    else ()
        set(lib ${target})
        list(APPEND ${target}_src ${${target}_main})
    endif ()

    if (${target}_src)
        target_sources(${lib}
                PRIVATE ${${target}_src})
    endif ()
    if (${target}_ixx)
        target_sources(${lib}
                PUBLIC FILE_SET ixx
                TYPE CXX_MODULES
                FILES ${${target}_ixx})
    endif ()
    if (${target}_header)
        if (target_type STREQUAL "INTERFACE_LIBRARY")
            target_include_directories(${lib} INTERFACE src)
            target_sources(${lib}
                    INTERFACE FILE_SET h
                    TYPE HEADERS
                    FILES ${${target}_header})
        else ()
            target_include_directories(${lib} PUBLIC src)
            target_sources(${lib}
                    PUBLIC FILE_SET h
                    TYPE HEADERS
                    FILES ${${target}_header})
        endif ()
    endif ()


    if (BUILD_TEST)
        foreach (file ${${target}_test})
            get_filename_component(name ${file} NAME_WLE)
            get_filename_component(name ${name} NAME_WLE)

            set(test_exe_name "${target}-test__${name}")
            add_executable(${test_exe_name} ${file})
            target_link_libraries(${test_exe_name} PRIVATE ${lib})
        endforeach ()
    endif ()
endfunction()