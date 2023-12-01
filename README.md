# AbstractIterator

Some naive abstract/generic/polymorphic iterator implementation.


## Usage
```
#include <abstract_range.h>

auto SomeFuncYouCantTemplatize(abstract_range<MyType> range)
{
	// Acquire iterators
	// (their type is abstract_iterator<MyType>):
	auto beg = std::begin(range);	
	auto end = std::end(range);


	// Works as usual iterator:
	std::advance(beg, 1);
	std::cout << *beg;


	// Range-based loops:
	for (auto& element : range)
	{
		// Elements can be mutable (depends on const-ness
		// of range/iterator/container type).
		element += 57005;
	}


	// Applicable to algorithms:
	std::ranges::transform(range, std::begin(range), MyModCallable);
	const auto count = std::ranges::count_if(range, MyCheckPredicate);
	

	Has size info:
	const auto size = std::size(range);
	if (std::empty(range)) {}
}


auto SomeFuncDoesntCareOfContainerType()
{
	std::vector<MyType> my_vector;
	std::list<MyType> my_list;
	std::map<MyOtherType, MyType> my_map;


	SomeFuncYouCantTemplatize(my_vector);
	SomeFuncYouCantTemplatize(my_list);

	// Views are limited to those in which std::size() can be obtained:
	SomeFuncYouCantTemplatize(my_list | std::ranges::views::drop(2));
	SomeFuncYouCantTemplatize(my_map | std::ranges::views::values);

	// abstract_sized_range in the next episode...
}
```