#include "pch.hpp"
#include "include/Core/WorkerThread.hpp"
#include "include/Core/Log.hpp"

void eg::WorkerThread::start()
{
    m_running = false;
    m_cv.notify_one();
    if (m_thread.joinable())
        m_thread.join();
    m_running = true;
    m_thread = std::jthread([this] { run(); });
}

void eg::WorkerThread::assignJob(std::function<void()> job)
{
    m_job = job;
    m_have_job     = true;
    m_job_complete = false;
    m_cv.notify_one();
}

void eg::WorkerThread::stop()
{
    m_running = false;
    m_cv.notify_all();
}

bool eg::WorkerThread::waitUntilJobComplete()
{
    {
        std::unique_lock lock(m_job_mutex);
        m_cv.wait(lock, [this] { return m_job_complete || !m_running; });
    }
    return m_running;
}

void eg::WorkerThread::join()
{
    if (m_thread.joinable())
        m_thread.join();
}

void eg::WorkerThread::run()
{
    while (m_running)
    {
        {
            std::unique_lock lock(m_mutex);
            m_cv.wait(lock, [this] { return m_have_job || !m_running; });
        }
        if (!m_running)
            break;
        m_job();
        m_job_complete = true;
        m_cv.notify_all();
        m_have_job = false;
    }
}
