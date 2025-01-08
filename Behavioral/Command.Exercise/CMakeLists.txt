#----------------------------------------
# Target
#----------------------------------------
get_filename_component(DIRECTORY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" TARGET_MAIN ${DIRECTORY_NAME})

#----------------------------------------
# Boost.DI
#----------------------------------------
# include(FetchContent)
# FetchContent_Declare(
#   boost_di
#   GIT_REPOSITORY https://github.com/boost-ext/di.git
#   GIT_TAG        v1.3.0
# )

# FetchContent_MakeAvailable(boost_di)
# FetchContent_GetProperties(boost_di)

#----------------------------------------
# Library - src
#----------------------------------------
add_subdirectory(src)

#----------------------------------------
# Tests
#----------------------------------------
# enable_testing()
# add_subdirectory(gtests)

#----------------------------------------
# Main app
#----------------------------------------
include_directories(src)  
add_executable(${TARGET_MAIN} main.cpp)
target_compile_features(${TARGET_MAIN} PUBLIC cxx_std_17)
target_link_libraries(${TARGET_MAIN} PUBLIC ${PROJECT_LIB})

#----------------------------------------
# Boost DI - includes
#----------------------------------------
# target_include_directories(${TARGET_MAIN} PRIVATE ${boost_di_SOURCE_DIR}/include) 


# #----------------------------------------
# # set compiler options
# #----------------------------------------
# if (MSVC)
#     target_compile_definitions(${PROJECT_NAME} PRIVATE "/std:c++latest") 
# else ()
#     target_compile_definitions(${PROJECT_NAME} PRIVATE "-Wall") 
# endif()