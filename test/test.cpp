#include "ir/irbuilder.hpp"
#include <iostream>

int main()
{
	eviir::IRBuilder* builder = new eviir::IRBuilder(new eviir::Module("test"));
	std::cout << builder->get_module()->to_string();

	return 0;
}