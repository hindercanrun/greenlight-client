#pragma once

namespace utils
{
	namespace scheduler
	{
		class loop
		{
		public:
			typedef void (*task)();

			loop()
				: handle_(nullptr),
				  task_(nullptr),
				  interval_ms_(1000),
				  running_(false)
			{
			}

			~loop()
			{
				stop();
			}

			void start(task task, DWORD interval_ms);
			void stop();

		private:
			HANDLE handle_;
			task task_;
			DWORD interval_ms_;
			volatile bool running_;

			DWORD WINAPI thread_entry(LPVOID param)
			{
				static_cast<loop*>(param)->run();
				return 0;
			}

			void run();
		};
	}
}