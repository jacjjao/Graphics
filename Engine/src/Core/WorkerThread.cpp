#include "pch.hpp"
#include "include/Core/WorkerThread.hpp"

WorkerThread::WorkerThread()
{
    m_thread = std::jthread([this] { run(); });
}

void WorkerThread::assignJob(std::function<void()> job)
{
    m_job = job;
    m_job_complete = false;
    m_cv.notify_all();
}

void WorkerThread::stop()
{
    m_running = false;
}

bool WorkerThread::waitUntilJobComplete()
{
    {
        std::unique_lock lock(m_job_mutex);
        m_cv.wait(lock, [this] { return m_job_complete || !m_running; });
    }
    return m_running;
}

void WorkerThread::run()
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
