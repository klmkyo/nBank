set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

SET (SOURCE_FILES main.cpp 
Database/database.hpp 
Database/structures.hpp 
Repositories/user.hpp 
Repositories/user.cpp
Repositories/account.hpp
Repositories/account.cpp
Repositories/repos.hpp)
    
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)
