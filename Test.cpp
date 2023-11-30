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
#include <unordered_map>


static auto AbstractRangeLoopFunc(const abstract_range<const std::string> range)	// The key point is that function is non-template.
{
	std::ranges::copy(range, std::ostream_iterator<std::string>(std::cout, "\n"));
}


static auto PerformTest()
{
	std::vector<std::string> container1 = { "STRINGS", "FROM", "VECTOR" };
	std::list<std::string> container2 = { "STRINGS", "FROM", "LIST" };
	std::set<std::string> container3 = { "STRINGS", "FROM", "SET" };
	std::unordered_set<std::string> container4 = { "STRINGS", "FROM", "UNORDERED", "SET" };
	std::span<const std::string> container5 = container1;
	std::unordered_map<std::string, int> container6 = { { "STRINGS", 0 }, { "TAKEN", 6 }, { "FROM", 2 }, { "UNORDERED", 15 }, { "MAP", 7 }, { "KEYS", 3 } };

	AbstractRangeLoopFunc(container1);
	std::cout << "===============================\n";

	AbstractRangeLoopFunc(container2);
	std::cout << "===============================\n";

	AbstractRangeLoopFunc(container3);
	std::cout << "===============================\n";

	AbstractRangeLoopFunc(container4);
	std::cout << "===============================\n";

	AbstractRangeLoopFunc(container5);
	std::cout << "===============================\n";

	AbstractRangeLoopFunc(std::views::keys(container6));
}


int main()
{
	PerformTest();

	return 0;
}
