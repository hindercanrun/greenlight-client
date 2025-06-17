#pragma once

namespace loader
{
	class module
	{
	public:
		module() {};
		virtual ~module() {};

		virtual void pre_destroy() {}
	};

	void load();
}