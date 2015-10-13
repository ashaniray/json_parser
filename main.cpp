#include <iostream>

#include "json_parser.h"
#include "pretty_print_visitor.h"

using namespace std;
using namespace json_parser;

int main()
{
	const std::string str =
	" {'txn_id':'pay_c32ff0b257a20b3f','order_id':'M2308615032','amount':'53.41','currency':'INR','settled_amount':'0.00','status':'capture_settled','payment_mode':'credit','fees':'0.00','description':'Explicabo ut laborum repellendus quia vel.','created_at':'2015-08-26 16:22:03 UTC','entity':'payment'} "
	;

	string::const_iterator iterBegin = str.begin();
	string::const_iterator iterEnd = str.end();

	json_node root;
	if (parse_json(iterBegin, iterEnd, root))
	{
		cout << "Parsing succeeded\n";
		apply_visitor(pretty_print_visitor(cout), root);
	}
	else
	{
		cout << "Parsing failed\n";
	}

	cout << "Bye... :-) \n\n";
	return 0;
}
