#include "abstract_iterator.h"

#include <string>
#include <iostream>

#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <span>


static auto AbstractIterLoopFunc(const abstract_iterator<std::string>& begin, const abstract_iterator<std::string>& end)
{
	//	std::distance(begin, end);
	//	std::ranges::for_each(begin, end, [](const auto& arg) { std::cout << arg << '\n'; });

	for (auto it = begin; it != end; ++it)
		std::cout << *it << '\n';
}


static auto PerformTest()
{
	std::vector<std::string> container1 = { "STRINGS", "FROM", "VECTOR" };
	std::list<std::string> container2 = { "STRINGS", "FROM", "LIST" };
	std::set<std::string> container3 = { "STRINGS", "FROM", "SET" };
	std::unordered_set<std::string> container4 = { "STRINGS", "FROM", "UNORDERED", "SET" };
	std::span<std::string> container5 = container1;

	AbstractIterLoopFunc(std::begin(container1), std::end(container1));
	std::cout << "===============================\n";

	AbstractIterLoopFunc(std::begin(container2), std::end(container2));
	std::cout << "===============================\n";

	AbstractIterLoopFunc(std::begin(container3), std::end(container3));
	std::cout << "===============================\n";

	AbstractIterLoopFunc(std::begin(container4), std::end(container4));
	std::cout << "===============================\n";

	AbstractIterLoopFunc(std::begin(container5), std::end(container5));
}


int main()
{
	PerformTest();

	return 0;
}
