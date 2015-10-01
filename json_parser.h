#ifndef __JSON_PARSER__
#define __JSON_PARSER__

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/phoenix/object/new.hpp>
#include <boost/phoenix/object/construct.hpp>

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

namespace json_parser
{
	using namespace std;

	using boost::spirit::qi::double_;
	using boost::spirit::qi::bool_;
	using boost::spirit::qi::lexeme;
	using boost::spirit::qi::_1;
	using boost::spirit::qi::_2;
	using boost::spirit::qi::_val;
	using boost::spirit::qi::phrase_parse;
	using boost::spirit::qi::rule;
	using boost::spirit::qi::grammar;
	using boost::spirit::ascii::char_;
	using boost::spirit::ascii::space;
	using boost::spirit::ascii::space_type;
	using boost::phoenix::new_;
	using boost::phoenix::construct;

	typedef boost::make_recursive_variant<
		string, 
		double, 
		bool, 
		vector< boost::recursive_variant_ >, 
		map<string, boost::recursive_variant_ >
		>::type json_node;

	string vector2string(vector<char, allocator<char> > str)
	{ 
		string val;
		copy(str.begin(), str.end(), back_inserter(val));
		return val;
	}

	json_node make_json_node_from_pairs(vector<pair<string, json_node> > v)
	{
		map<string, json_node> table;
		for (vector<pair<string, json_node> >::iterator iter = v.begin(); iter != v.end(); ++iter)
		{
			table[iter->first] = iter->second;
		}
		return json_node(table);
	}

	BOOST_PHOENIX_ADAPT_FUNCTION(string, vector2string, json_parser::vector2string, 1);
	BOOST_PHOENIX_ADAPT_FUNCTION(json_node, make_json_node_from_pairs, json_parser::make_json_node_from_pairs, 1);

	template <typename Iterator>
	struct JsonGrammar : grammar<Iterator, json_node(), space_type>
	{
		JsonGrammar() : JsonGrammar::base_type(start)
		{
			start = value_rule.alias();
			object_rule = '{' >> (pair_rule  % char_(',')) [_val = make_json_node_from_pairs(_1)] >> '}' ;
			pair_rule = (string_key_rule >> ':' >> value_rule) [ _val = construct<pair<string, json_node> >(_1, _2) ];
			value_rule = 
						object_rule [_val = _1] | 
						array_rule [_val = _1] | 
						string_rule [_val = _1] | 
						double_ [_val = construct<json_node>(_1) ]  |
						bool_ [_val = construct<json_node>(_1) ] 
						;
			array_rule = '[' >> (value_rule  % char_(',') ) [ _val =  construct<json_node>(_1) ] >> ']';
			string_rule = lexeme[ '\"' >> *( char_ - '\"'  ) >> '\"' ] [_val = construct<json_node>(vector2string(_1))];
			string_key_rule = lexeme[ '\"' >> *( char_ - '\"'  ) >> '\"' ] [_val = vector2string(_1)];
		}

		rule <Iterator, json_node(), space_type > start;
		rule <Iterator, pair<string, json_node>, space_type > pair_rule;
		rule <Iterator, json_node(), space_type > object_rule;
		rule <Iterator, json_node(), space_type > value_rule;
		rule <Iterator, json_node(), space_type > array_rule;
		rule <Iterator, json_node(), space_type > string_rule;
		rule <Iterator, string(), space_type > string_key_rule;
	};

	template <typename Iterator>
	bool parse_json(Iterator first, Iterator last, json_node& node)
	{
		JsonGrammar<string::const_iterator> g;
		bool r = phrase_parse(first, last, g, space, node);

		if (!r || first != last) // fail if we did not get a full match
			return false;
		return r;
	}
}
#endif

