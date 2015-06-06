
#include <cstdlib>
#include <iostream>

#include "flags.hpp"

void
show_help(std::string a, std::string b, void* d) {
	auto flags = reinterpret_cast<Flags*>(d);
	flags->print_usage();
	exit(0);
}

void
set_foo(std::string a, std::string b, void* d) {
	auto foo = reinterpret_cast<int*>(d);
	*foo = atoi(b.c_str());
}

void
show_version(std::string a, std::string b, void* d) {
	std::cout << *reinterpret_cast<std::string*>(d) << std::endl;
	exit(0);
}

int
main(int argc, char* argv[]) {
	int foo = -1;
	std::string version = "1.0.0";

	Flags flags("main", version);
	flags.add_option("--help", "-h", "show help", show_help, &flags);
	flags.add_option("--foo", "", "set foo", set_foo, &foo);
	flags.add_option("--version", "-v", "show version", show_version, &version);
	flags.parse(argc, argv);

	if (foo != -1) {
		std::cout << "foo was set to " << foo << std::endl;
	}

	return 0;
}
