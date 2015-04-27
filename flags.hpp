#ifndef flags_hpp_
#define flags_hpp_

#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

class Flags
{
public:
	Flags(std::string name, std::string version)
	: name(name), version(version)
	{
	}

	void add_option(std::string, std::string, std::string, std::function<void(std::string, std::string, void*)>);
	void add_option(std::string, std::string, std::string, std::function<void(std::string, std::string, void*)>, void*);
	void parse(int argc, char* argv[]);
	void print_usage();

private:
	std::string name;
	std::string version;

	std::map<std::string, std::function<void(std::string, std::string, void*)>> funcs;
	std::map<std::string, void*> ptrs;
	std::map<std::pair<std::string, std::string>, std::string> descs;
};

inline void Flags :: add_option(std::string long_opt,
	std::string opt, std::string desc,
	std::function<void(std::string, std::string, void*)> cb) {
	add_option(long_opt, opt, desc, cb, nullptr);
}

inline void Flags :: add_option(std::string long_opt,
	std::string opt, std::string desc,
	std::function<void(std::string, std::string, void*)> cb,
	void* p) {

	if (funcs.find(long_opt) != funcs.end()) {
		throw std::invalid_argument("redefined flag: " + long_opt);
	}

	if (funcs.find(opt) != funcs.end()) {
		throw std::invalid_argument("redefined flag: " + opt);
	}

	funcs[long_opt] = cb;
	ptrs[long_opt] = p;

	if (opt != "") {
		funcs[opt] = cb;
		ptrs[opt] = p;
	}

	descs[std::pair<std::string, std::string>(opt, long_opt)] = desc;
}

inline void Flags :: print_usage() {
	std::cout << name << " version " << version << " usage:" << std::endl << std::endl;
	for (const auto &i: descs) {
		std::string opts = "";

		if (i.first.first != "") {
			opts = i.first.first + ", " + i.first.second;
		} else {
			opts = i.first.second;
		}

		printf("  %-25s %s\n", opts.c_str(), i.second.c_str());
	}
}

inline void Flags :: parse(int argc, char* argv[]) {
	auto is_first = true;
	std::string arg;

	for (int i = 0; i < argc; i++) {
		if (!is_first) {
			auto val = funcs.find(arg);
			if (argv[i][0] == '-') {
				val->second(arg, "", ptrs.find(arg)->second);
				arg = "";
				break;
			}

			if (val != funcs.end()) {
				val->second(arg, argv[i], ptrs.find(arg)->second);
			}

			is_first = true;
			arg = "";

			continue;
		}

		is_first = true;
		arg = "";

		auto val = funcs.find(argv[i]);
		if (val != funcs.end()) {
			if (is_first) {
				arg = val->first;
				is_first = false;
				continue;
			}
		}
	}

	if (!is_first) {
		auto val = funcs.find(arg);
		if (val != funcs.end()) {
			val->second(arg, "", ptrs.find(arg)->second);
		}
	}
}

#endif
