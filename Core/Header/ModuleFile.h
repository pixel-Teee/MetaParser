#pragma once

class Class;

struct ModuleFile
{
	std::string name;

	std::vector<std::shared_ptr<Class>> classes;
};