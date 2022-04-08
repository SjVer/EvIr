#include "ir/irbuilder.hpp"
#include <iostream>

int main()
{
	eviir::IRBuilder* builder = new eviir::IRBuilder(new eviir::Module("test"));
	std::cout << builder->get_module()->generate_ir();

	return 0;
}