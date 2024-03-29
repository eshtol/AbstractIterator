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


static auto AbstractRangeAccessLoopFunc(const abstract_range<const std::string> range)	// The key point is that function is non-template.
{
	std::ranges::copy(range, std::ostream_iterator<std::string>(std::cout, "\n"));
}


static auto AbstractRangeModifyingLoopFunc(const abstract_range<std::string> range)
{
	constexpr auto alternate_caps = [](auto& arg)
	{
		if (arg == "UNMODIFIED") arg = "MODIFIED";

		for (std::size_t i = 0; i < std::size(arg); i += 2)
			arg[i] = std::tolower(arg[i]);

		return arg;
	};

	std::ranges::for_each(range, alternate_caps);
}


static auto PerformAccessTest()
{
	std::vector<std::string> container1 = { "STRINGS", "FROM", "VECTOR" };
	std::list<std::string> container2 = { "STRINGS", "FROM", "LIST" };
	std::set<std::string> container3 = { "STRINGS", "FROM", "SET" };
	std::unordered_set<std::string> container4 = { "STRINGS", "FROM", "UNORDERED", "SET" };
	std::span<const std::string> container5 = container1;
	std::unordered_map<std::string, int> container6 = { { "STRINGS", 0 }, { "TAKEN", 6 }, { "FROM", 2 }, { "UNORDERED", 15 }, { "MAP", 7 }, { "KEYS", 3 } };

	std::cout << "===============================\n";
	AbstractRangeAccessLoopFunc(container1);
	std::cout << "===============================\n";

	AbstractRangeAccessLoopFunc(container2);
	std::cout << "===============================\n";

	AbstractRangeAccessLoopFunc(container3);
	std::cout << "===============================\n";

	AbstractRangeAccessLoopFunc(container4);
	std::cout << "===============================\n";

	AbstractRangeAccessLoopFunc(container5);
	std::cout << "===============================\n";

	AbstractRangeAccessLoopFunc(container6 | std::views::keys);
	std::cout << "===============================\n";
}


static auto PerformModifyingTest()
{
	constexpr auto print_container = AbstractRangeAccessLoopFunc;

	std::vector<std::string> container1 = { "THESE", "ARE", "UNMODIFIED", "STRINGS", "FROM", "VECTOR" };
	std::list<std::string> container2 = { "THESE", "ARE", "UNMODIFIED", "STRINGS", "FROM", "LIST" };

	std::cout << "===============================\n";
	print_container(container1);
	std::cout << "-------------------------------\n";
	AbstractRangeModifyingLoopFunc(container1);
	print_container(container1);

	std::cout << "===============================\n";

	print_container(container2);
	std::cout << "-------------------------------\n";
	AbstractRangeModifyingLoopFunc(container2);
	print_container(container2);
	std::cout << "===============================\n";
}


static auto PerformViewsTest()
{
	constexpr auto print_range = AbstractRangeAccessLoopFunc;

	std::set<std::string> container1 =
	{
		"STRING1",
		"STRING2",
		"STRING3",
		"STRING4",
		"STRING5",
		"STRING6",
		"STRING7"
	};

	std::unordered_map<std::string, std::string> container2 =
	{
		{ "KEY1", "VAL1" },
		{ "KEY2", "VAL2" },
		{ "KEY3", "VAL3" },
		{ "KEY4", "VAL4" },
		{ "KEY5", "VAL5" },
		{ "KEY6", "VAL6" },
		{ "KEY7", "VAL7" },
	};

	std::cout << "----empty view:\n";
	print_range(std::views::empty<std::string>);

	std::cout << "----single view:\n";
	print_range(std::views::single(std::string("SINGLE STRING")));

	std::cout << "----filter(STRING2 or STRING5) view:\n";
	print_range(container1 | std::views::filter([](const auto& arg) { return arg == "STRING2" || arg == "STRING5"; }));

	std::cout << "----all view:\n";
	print_range(container1 | std::views::all);

	std::cout << "----reverse view:\n";
	print_range(container1 | std::views::reverse);

	std::cout << "----take(3) view:\n";
	print_range(container1 | std::views::take(3));

	std::cout << "----drop(3) view:\n";
	print_range(container1 | std::views::drop(3));

	std::cout << "----keys view:\n";
	print_range(container2 | std::views::keys);

	std::cout << "----values view:\n";
	print_range(container2 | std::views::values);

	// todo : transform, (take|drop)_while, join, split, istream views
}


static auto PerformTest()
{
	std::cout << "Access test result:\n";
	PerformAccessTest();

	std::cout << "\n\n\n";

	std::cout << "Modifying test result:\n";
	PerformModifyingTest();

	std::cout << "\n\n\n";

	std::cout << "Views test result:\n";
	PerformViewsTest();
}


int main()
{
	PerformTest();

	return 0;
}
