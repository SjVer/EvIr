#include "ir/irbuilder.hpp"
#include <iostream>

int main()
{
	eviir::IRBuilder* builder = new eviir::IRBuilder(new eviir::Module("test"));
	builder->get_module()->add_metadata(new eviir::Metadata(
		vector<string>{"module", "filename"}, eviir::Value::new_string("test.evi")
	));
	std::cout << builder->get_module()->generate_ir();

	return 0;
}