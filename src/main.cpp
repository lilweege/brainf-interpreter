#include "bf.hpp"

#include <fstream>
#include <iostream>

auto main(int argc, const char** argv) -> int {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " filename\n";
		return EXIT_FAILURE;
	}

	std::ifstream is(argv[1]);
	if (is.fail()) {
		std::cerr << "Error: Failed to open file '" << argv[1] << "'\n";
		return EXIT_FAILURE;
	}

	using isit = std::istreambuf_iterator<char>;
	std::string source(isit(is.rdbuf()), isit());
	is.close();

	try {
		auto program = bf::compile(source);
		bf::execute(program);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}