#include "abstract_range.h"

#include <string>
#include <iostream>
#include <ranges>
#include <algorithm>

#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <span>


static auto AbstractIterLoopFunc(abstract_range<std::string> range)
{
	std::ranges::copy(range, std::ostream_iterator<std::string>(std::cout, "\n"));
}


static auto PerformTest()
{
	std::vector<std::string> container1 = { "STRINGS", "FROM", "VECTOR" };
	std::list<std::string> container2 = { "STRINGS", "FROM", "LIST" };
	std::set<std::string> container3 = { "STRINGS", "FROM", "SET" };
	std::unordered_set<std::string> container4 = { "STRINGS", "FROM", "UNORDERED", "SET" };
	std::span<std::string> container5 = container1;

	AbstractIterLoopFunc(container1);
	std::cout << "===============================\n";

	AbstractIterLoopFunc(container2);
	std::cout << "===============================\n";

	AbstractIterLoopFunc(container3);
	std::cout << "===============================\n";

	AbstractIterLoopFunc(container4);
	std::cout << "===============================\n";

	AbstractIterLoopFunc(container5);
}


int main()
{
	PerformTest();

	return 0;
}
