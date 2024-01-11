#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <utility>
#include <future>
#include <atomic>

namespace eg
{

    class WorkerThread
    {
    public:
        void start();

        void assignJob(std::function<void()> job);
        
        bool waitUntilJobComplete();
    
        void stop();

        void join();
    
    private:
        void run();
    
        std::condition_variable m_cv;
        std::jthread            m_thread;
        std::atomic_bool        m_running  = false;
        std::atomic_bool        m_have_job = false;
        std::atomic_bool        m_job_complete = false;
        std::mutex              m_mutex, m_job_mutex;
    
        std::function<void()> m_job;
    };

} // namespace eg