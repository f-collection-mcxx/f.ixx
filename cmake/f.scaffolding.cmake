option(BUILD_TEST "build test" ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(fetch_files name)
    file(GLOB_RECURSE cpp src/*.cpp)
    file(GLOB_RECURSE ixx src/*.ixx)
    file(GLOB_RECURSE test src/*.test.cpp)

    # 排除测试文件，避免链接冲突
    if(test)
        list(REMOVE_ITEM cpp ${test})
    endif()

    set(${name}_cpp ${cpp} PARENT_SCOPE)
    set(${name}_ixx ${ixx} PARENT_SCOPE)
    set(${name}_test ${test} PARENT_SCOPE)
endfunction()

function(configure_target target)
    fetch_files(${target})

    target_sources(${target}
            PRIVATE ${${target}_cpp}
            PUBLIC
            FILE_SET cxx_modules
            TYPE CXX_MODULES
            FILES ${${target}_ixx})

    if (BUILD_TEST)
        foreach (file ${${target}_test})
            get_filename_component(name ${file} NAME_WLE)
            get_filename_component(name ${name} NAME_WLE)

            set(test_exe_name "test-${target}-${name}")
            add_executable(${test_exe_name} ${file})
            target_link_libraries(${test_exe_name} PRIVATE ${target})
        endforeach ()
    endif ()
endfunction()