# json_spirit

Header only library for parsing JSON (using the Boost Spirit library).

The only file that needs to be included is "json_parser.h"

The files included in the project are:
- **json_parser.h** : Header file that parses the json. Only this file needs to be included for parsing JSON. Call the method **parse_json** to get your parsed JSON
- pretty_print_visitor.h : A visitor patter implemenation of the parsed JSON. This is a sample implemenation of a 
visitor pattern on the parsed JSON and may not be included.
- main.cpp : A sample application that reads a JSON string, parses it and prints the parse output 
to show how the json_parser.h may be used.

## Dependencies
json_parser.h depends heavily on Boost Spirit library.
Download and install [Boost (1.59.0)](http://www.boost.org/) to compile the code.
No additional include/linking is required.

### Prerequisties
- Boost (including Boost Spirit)

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
  const std::string str = "<your json string>";
  string::const_iterator iterBegin = str.begin();
  string::const_iterator iterEnd = str.end();

  json_node root;
  if (parse_json(iterBegin, iterEnd, root))
  {
    // The parsed JSON is in "node" now.
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
```

