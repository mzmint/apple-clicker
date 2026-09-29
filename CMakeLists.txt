cmake_minimum_required(VERSION 3.20)

project(Apple_Clicker)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.1.0
)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(SFML json)

add_executable(Apple_Clicker
        main.cpp
        app.rc
)

target_link_libraries(Apple_Clicker PRIVATE
        SFML::Graphics
        SFML::Audio
        nlohmann_json::nlohmann_json
)

if(WIN32)
    add_custom_command(TARGET Apple_Clicker POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SFML::Graphics>
            $<TARGET_FILE_DIR:Apple_Clicker>

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SFML::Window>
            $<TARGET_FILE_DIR:Apple_Clicker>

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SFML::System>
            $<TARGET_FILE_DIR:Apple_Clicker>

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SFML::Audio>
            $<TARGET_FILE_DIR:Apple_Clicker>

            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:Apple_Clicker>/assets"
    )
endif()