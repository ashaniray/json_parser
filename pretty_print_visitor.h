#ifndef __PRETTY_PRINT_VISITOR__
#define __PRETTY_PRINT_VISITOR__

#include <iostream>
#include "json_parser.h"

using namespace std;
using namespace json_parser;

class pretty_print_visitor : public boost::static_visitor<>
{
public:
	pretty_print_visitor(ostream& os, int indent_depth = 0) : os_(os), indent_depth_(indent_depth) {}

	void operator() (const string& str) const
	{
		os_ << "\"" << str << "\"" ;
	}

	void operator() (const map<string, json_node>& node) const
	{
		os_ << endl;
		indent(indent_depth_);
		os_ << "{" << endl;
		for (map<string, json_node>::const_iterator iter = node.begin(); iter != node.end(); ++iter)
		{
			string key = iter->first;
			json_node node = iter->second;
			indent(indent_depth_);
			os_ << "\"" << key.c_str() << "\" : " ;
			apply_visitor(pretty_print_visitor(os_, indent_depth_ + 1), node);
			os_ << endl;
		}
		indent(indent_depth_);
		os_ << "}" << endl;
	}

	void operator() (const vector<json_node>& nodes) const
	{
		for (vector<json_node>::const_iterator iter = nodes.begin(); iter != nodes.end(); ++iter)
		{
			apply_visitor(pretty_print_visitor(os_, indent_depth_ + 1), *iter);
			os_ << ",";
		}
	}

	template<typename T> void operator() (const T & operand) const
	{
		os_ << operand;
	}

private:
	void indent(const int depth) const
	{
		static const string INDENT = "  ";
		for (int i = 0; i < depth; i++)
		{
			os_ << INDENT; 			
		}
	}
	
private:
	ostream& os_;
	int indent_depth_;
};
#endif

