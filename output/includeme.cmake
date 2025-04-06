# =============================================
set(library_name "qtgllogger")
if (WIN32)
    set(${library_name}_include ${CMAKE_CURRENT_SOURCE_DIR}/../${library_name}/output/include)
    include_directories(${${library_name}_include})
    set(${library_name}_platform "win32msvc")
    set(${library_name}_lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/../${library_name}/output/lib/${${library_name}_platform}/${library_name})
    set(${library_name}_lib1 ${${library_name}_lib_dir}/${library_name}.lib)
    target_link_directories(${PROJECT_NAME} PUBLIC ${${library_name}_lib_dir})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${${library_name}_lib1})
elseif (UNIX AND NOT APPLE AND NOT WIN32 AND NOT ANDROID)
    set(${library_name}_include ${CMAKE_CURRENT_SOURCE_DIR}/../${library_name}/output/include)
    include_directories(${${library_name}_include})
    set(${library_name}_platform "linux64")
    set(${library_name}_lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/../${library_name}/output/lib/${${library_name}_platform}/${library_name})
    set(${library_name}_lib1 ${${library_name}_lib_dir}/lib${library_name}.so)
    target_link_directories(${PROJECT_NAME} PUBLIC ${${library_name}_lib_dir})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${${library_name}_lib1})
endif()
# =============================================


