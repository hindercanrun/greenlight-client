#include "../std_include.hpp"
#include "scheduler.hpp"

#include <functional>

namespace utils
{
	namespace scheduler
	{
		void loop::start(task task, DWORD interval_ms)
		{
			if (running_)
				return;

			task_ = task;
			interval_ms_ = interval_ms;
			running_ = true;

			handle_ = CreateThread(nullptr, 0, &thread_entry, this, 0, nullptr);
		}

		void loop::stop()
		{
			if (!running_)
				return;

			running_ = false;

			if (handle_)
			{
				WaitForSingleObject(handle_, INFINITE);
				CloseHandle(handle_);
				handle_ = nullptr;
			}
		}

		void loop::run()
		{
			while (running_)
			{
				if (task_)
					task_();
				Sleep(interval_ms_);
			}
		}
	}
}