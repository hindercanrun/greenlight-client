#pragma once

namespace cmd
{
	void add(const char* name, void(WINAPIV* func)());

	int params();
	const char* param(int value);

	void changes();
}
