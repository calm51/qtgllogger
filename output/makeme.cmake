
# =============================================
if (WIN32)
    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND python "${CMAKE_SOURCE_DIR}/../scripts/make-output/cpp-lib.py" win32msvc ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR})
elseif (ANDROID)
    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND python3 "${CMAKE_SOURCE_DIR}/../scripts/make-output/cpp-lib.py" android ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR})
else()
    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND python3 "${CMAKE_SOURCE_DIR}/../scripts/make-output/cpp-lib.py" linux64 ${PROJECT_NAME} ${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR})
endif()
# =============================================




