#ifndef __JSON_PARSER__
#define __JSON_PARSER__

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/std_pair.hpp>

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

namespace json_parser
{
	namespace sp = boost::spirit;
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	typedef boost::make_recursive_variant<
		std::string,
		double,
		bool,
		std::vector< boost::recursive_variant_ >,
		std::map<const std::string, boost::recursive_variant_ >
		>::type json_node;
		typedef std::vector<json_node> json_array;
		typedef std::map<const std::string, json_node> json_map;

	template <typename Iterator, typename Node, typename Map, typename Array>
	struct JsonGrammar : qi::grammar<Iterator, Node(), ascii::space_type>
	{
		JsonGrammar() : JsonGrammar::base_type(start)
		{
			start = value_rule.alias();
			value_rule %=
						object_rule |
						array_rule |
						string_rule |
						qi::double_ |
						qi::bool_
						;
			object_rule = '{' >> -((string_rule >> ':' >> value_rule) % ',') >> '}';
			array_rule  = '[' >> -(value_rule % ',') >> ']';
			string_rule = qi::lexeme[ '\"' >> *( ascii::char_ - '\"'  ) >> '\"' ]  |
							qi::lexeme[ '\'' >> *( ascii::char_ - '\''  ) >> '\'' ]  ;
		}

		qi::rule <Iterator, Node(), ascii::space_type > start;
		qi::rule <Iterator, Node(), ascii::space_type > value_rule;
		qi::rule <Iterator, Array(), ascii::space_type > array_rule;
		qi::rule <Iterator, std::string(), ascii::space_type > string_rule;
		qi::rule <Iterator, Map(), ascii::space_type > object_rule;

	};

	template <typename Iterator>
	bool parse_json(Iterator first, Iterator last, json_node& node)
	{
		JsonGrammar<std::string::const_iterator, json_node, json_map, json_array> g;
		bool r = qi::phrase_parse(first, last, g, ascii::space, node);
		if (!r || first != last) // fail if we did not get a full match
			return false;
		return r;
	}
}
#endif
