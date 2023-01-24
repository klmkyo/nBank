set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

SET (SOURCE_FILES main.cpp 
Database/Database.hpp 
Database/Account.hpp
Database/Account.cpp
Database/CreditCard.hpp
Database/User.hpp
Database/TransactionData.hpp
Database/TransactionData.cpp
Database/Repo.hpp
Utils/sha256.hpp
Utils/sha256.cpp
Utils/Utils.hpp
Utils/Utils.cpp
Logic/transaction.hpp
Logic/transaction.cpp
Logic/transfer.cpp
Logic/transfer.hpp
Logic/payments.hpp
Logic/logger.hpp
UI/ui.hpp
UI/ui.cpp
UI/transferui.hpp
UI/transferui.cpp)
    
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)
