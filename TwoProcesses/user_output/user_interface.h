#pragma once
#include <thread>
using namespace std;

typedef struct message_buffer{
    long message_type;
    char message_text[100];
};

class UserInterface
{
    public:
    enum user_request_t
    {
        ERROR_USER_COMMAND = 0,
        AVERAGE_OF_1000_RUNTIMES = 1,
        QUIT = 2
    };
    std::thread _thread_user_request_process;
    std::thread _second_thread;
    user_request_t check_new_user_request(string request);  
    void monitor_user_request_thread_proc();
    // UserInterface();
public:
    UserInterface();
    void second_proces();
    bool join_listener() {
        if(_thread_user_request_process.joinable()) {
            _thread_user_request_process.join();
            return true;
        }
        return false;
    }
    bool second_thread_join_listener() {
        if(_second_thread.joinable()) {
            _second_thread.join();
            return true;
        }
        return false;
    }
    static UserInterface &getInstance() {
        static UserInterface _instance;
        return _instance; 
    }
    
};
