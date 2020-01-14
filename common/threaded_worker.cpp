//
// Created by magicxqq on 2020/01/14.
//

#if defined(__GNUC__)
    #include <pthread.h>
#elif defined(_WIN32)
    #include <Windows.h>
#endif
#include "threaded_worker.hpp"

ThreadedWorker::~ThreadedWorker() {
    if (thread_.joinable()) {
        thread_.join();
    }
}

#if defined(__GNUC__) && !defined(__APPLE__)

static void SetThreadName(std::thread::native_handle_type thread_id, const std::string& name) {
    pthread_setname_np(reinterpret_cast<pthread_t>(thread_id), name.c_str());
}

#elif defined(_WIN32)

const DWORD kVCThreadNameException = 0x406D1388;

typedef struct tagTHREADNAME_INFO {
    DWORD dwType;  // Must be 0x1000.
    LPCSTR szName;  // Pointer to name (in user addr space).
    DWORD dwThreadID;  // Thread ID (-1=caller thread).
    DWORD dwFlags;  // Reserved for future use, must be zero.
} THREADNAME_INFO;

static void SetThreadName(std::thread::native_handle_type thread_id, const std::string& name) {
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = name.c_str();
    info.dwThreadID = reinterpret_cast<DWORD>(thread_id);
    info.dwFlags = 0;

    __try {
        RaiseException(kVCThreadNameException, 0, sizeof(info) / sizeof(DWORD), reinterpret_cast<ULONG_PTR*>(&info));
    } __except(EXCEPTION_EXECUTE_HANDLER) {
    }
}

#else

static void SetThreadName(std::thread::native_handle_type thread_id, const std::string& name) {
    // empty
}

#endif

void ThreadedWorker::Start() {
    thread_ = std::thread([this]() {
#ifdef __APPLE__
        pthread_setname_np(thread_name_.c_str());
#endif
        this->Run();
    });

    if (!thread_name_.empty()) {
        SetThreadName(thread_.native_handle(), thread_name_);
    }
}
