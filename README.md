# json_parser

**json_parser** is a C++ header only library for parsing JSON based on the **Boost Spirit 2.x** library.

The only file that needs to be included is "json_parser.h"

The files included in the project are:
- **json_parser.h** : Header file that parses the json. Only this file needs to be included for parsing JSON. Call the method **parse_json** to get your parsed JSON
- pretty_print_visitor.h : A visitor pattern implemenation of the parsed JSON. This is a sample implemenation of a 
visitor pattern on the parsed JSON and may not be included.
- main.cpp : A sample application that reads a JSON string, parses it and prints the parse output 
to show how the json_parser.h may be used.

## Dependencies
json_parser.h depends heavily on Boost Spirit library.
Download and install [Boost (1.59.0)](http://www.boost.org/) to compile the code.

No additional include/linking is required.

### Prerequisties
- [Boost 1.59](http://www.boost.org/) (including [Boost Spirit 2.x](http://www.boost.org/doc/libs/1_59_0/libs/spirit/doc/html/index.html) )

## Usage

- Include "json_parer.h"
- Create and instance of json_parser::json_node and call **json_parser::parse_json**

The parsed json is returned in the **json_node** supplied to parse_json

json_node is defined as:

```cpp
typedef boost::make_recursive_variant<
        string, 
        double, 
        bool, 
        vector< boost::recursive_variant_ >, 
        map<string, boost::recursive_variant_ >
        >::type json_node;

```
A json_node at the root can contain a map of [string, json_node]. It is a recursive structure.

Sample code to use the parser is given below:

```c++
#include <iostream>
#include "json_parser.h"
#include "pretty_print_visitor.h"

using namespace std;
using namespace json_parser;

int main()
{
  string str = "<your json string>";
  string::const_iterator iterBegin = str.begin();
  string::const_iterator iterEnd = str.end();

  json_node root;
  if (parse_json(iterBegin, iterEnd, root))
  {
    // The parsed JSON is in "root" now.
    cout << "Parsing succeeded\n";
    // You application/logic for processing the JSON may 
    // be implemented as a visitor pattern
    apply_visitor(pretty_print_visitor(cout), root);
  }
  else
  {
    cout << "Parsing failed\n";
  }
  cout << "Bye... :-) \n\n";
  return 0;
}
```

