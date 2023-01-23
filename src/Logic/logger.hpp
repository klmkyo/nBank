#pragma once
#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>

// thread safe version of localtime()
inline std::tm localtime_xp(std::time_t timer)
{
    std::tm bt{};
#if defined(__unix__)
    localtime_r(&timer, &bt);
#elif defined(_MSC_VER)
    localtime_s(&bt, &timer);
#else
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    bt = *std::localtime(&timer);
#endif
    return bt;
}

// Abstract class
// derived classes should override GetLogMessage()
class LoggedOperation {
protected:
    virtual std::string GetLogMessage(bool success) = 0;

    void LogAction(bool success = true) {
        
        std::string path = success ? "operations_successful.log" : "operations_failed.log";
        try {
            std::ofstream fw(path, std::ios_base::out | std::ios_base::app);
            if (fw.is_open()){
                fw << time() << " " << GetLogMessage(success) << "\n";
            }
        } catch (const char* msg) {
            std::cout << "Error while writing log file: " << msg << std::endl;
        }
    };

    // helper functions for formatting log message

    inline std::string time() {
        auto t = std::time(nullptr);
        auto tm = localtime_xp(t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "[%d/%m/%Y %H:%M:%S]");
        return oss.str();
    }
};