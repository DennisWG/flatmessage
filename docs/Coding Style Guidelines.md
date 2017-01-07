# **QuickMessage Style Guidelines**
---
These guidelines aim to provide advice on both the formatting of code contributions as well as language usage rules of thumb to keep in mind.

## Table of Contents:
- [Code Formatting](#code-formatting)
  - [Line length](#line-length)
  - [Tabs vs Spaces and Indentation](#tabs-vs-spaces-and-indentation)
  - [Braces](#braces)
  - [Namespaces](#namespaces)
  - [Switch](#switch)
  - [Argument Ordering](#argument-ordering)
  - [Variable Scope](#variable-scope)
  - [CV Qualifiers, Pointers and References](#cv-qualifiers-pointers-and-references)
- [Naming Conventions](#naming-conventions)
  - [Functions and Methods](#functions-and-methods)
  - [Output Arguments](#output-arguments)
  - [Constants](#constants)
  - [Variables](#variables)
  - [User Defined Types](#user-defined-types)
  - [Namespaces](#namespaces)
- [Language Usage](#language-usage)
  - [Header Guards](#header-guards)
  - [Header Inclusion Order](#header-inclusion-order)
  - [Prefer enum class over enum](#prefer-enum-class-over-enum)
  - [Exceptions](#exceptions)
  - [Pointers](#pointers)

## Code Formatting
Coding styles vary from person to person and everyone has their own preferences. In order to maintain readability it's required to just pick one and then stick to it. To achieve this goal, a '.clang-format' file is attached to the project.

### Line length
It is advised to stick to lines that aren't longer than 100 characters and strictly forbidden to surpass the 120 characters mark. If you have to break in a long chain of operators or in the middle of a function call indent the next operator or argument to match the starting position of the starting operator or argument of the previous line.

Example (do not):
```cpp
std::cout << "this is a long chain of " << "operators aiming" << " to demonstrate" << " how it's not supposed to look " << "like in the final file" << std::endl;
```

Example (do):
```cpp
std::cout << "this is a long chain of " << "operators aiming" << " to demonstrate"
          << " how it's supposed to look " << "like in the final file" << std::endl;
```

### Tabs vs Spaces and Indentation
Indent every scope, including namespaces, with 4 spaces. Tabs may not be used!

### Braces
Braces always have to go into their own lines and indented to the same level as the scope that they're placed in. Class methods with empty bodies or bodies that only hold a single instruction that doesn't go over the line length limit, however, are to be placed in the same line that the method has been declared in.

Example (do not):
```cpp
class foo {
    void empty()
    {
    }
    
    void single_line() {
        std::cout << std::endl;
    }
    
    void single_line_over_length() { std::cout << "this instruction is way too long and should be placed in its own line" << std:: endl; }
    
    void foobar()
    {
        if(false) {
            std::cout << "oops!";
            return;
        } else {
            std::cout << "true!";
        }
    }
};
```

Example (do):
```cpp
class foo
{
    void empty() {}
    void single_line() { std::cout << std::endl; }
    void single_line_over_length()
    {
        std::cout << "this instruction is way too long and should be placed in its own line" << std::endl;
    }

    void foobar()
    {
        if (false)
        {
            std::cout << "oops!";
            return;
        }
        else
        {
            std::cout << "true!";
        }
    }
};
```
### Namespaces
Namespace bodies have to be indented and nested namespaces always start in a new line.

Example (do not):
```cpp
namespace foo {
int foo();
}

namespace foo { namespace bar {
int foo();
}}
```

Example (do):
```cpp
namespace foo
{
    int foo();
}

namespace foo
{
    namespace bar
    {
        int foo();
    }
}
```

### Switch
Indent each case and indent each case's body, including the `break` statement twice. A space between the keyword `switch` and the opening bracket is required.

Example (do not):
```cpp
switch(foo)
{
    case 5:
    std::cout << "test";
    break;
    case 6:
    break;
}
```

Example (do):
```cpp
switch (foo)
{
    case 5:
        std::cout << "test";
        break;
    case 6:
        break;
}
```

### Argument Ordering
Order the inputs first, then the outputs.

Example (do not):
```cpp
bool calculate(int& outResult, int a, int b);
```

Example (do):
```cpp
bool calculate(int a, int b, int& outResult);
```

### Variable Scope
Variables are to be declared in the scope they're used in as near to their point of initialization as possible. It's also advised to declare them close the point where they're being used.

Example (do not):
```cpp
void foo(bar const& bar)
{
    something woof;
    // ...
    int n = 0;
    for (auto& meow : bar.iterate())
    {
        woof = meow.something();
        // ...
        bar.something_else(woof, n);
    }
    std::cout << n << std::endl;
}
```

Example (do):
```cpp
void foo(bar const& bar)
{
    // ...
    int n = 0;
    for (auto& meow : bar.iterate())
    {
        // ...
        auto woof = meow.something();
        bar.something_else(woof, n);
    }
    std::cout << n << std::endl;
}
```

### CV Qualifiers, Pointers and References
CV Qualifiers, pointers and references are to be placed between a variable's or function's type and its name.

Example (do not):
```cpp
const std::string data("");
volatile const& std::string foo(data);
```

Example (do):
```cpp
std::string const data("");
std::string volatile const& foo(data);
```

## Naming Conventions

### Functions and Methods
Functions and methods are to be named in all lower case letters with different words sperated by a single underscore `_`.

Example (do not):
```cpp
void MyFunction();
void myFunction();
void My_Function();
```

Example (do):
```cpp
void my_function();
```

### Output Arguments
Output arguments of a function or method have to be prefixed with `out_`.

Example (do):
```cpp
void do_stuff(int a, int b, int& out_result);
```

### Constants
Constants shall be written in all upper case letters with a single underscore seperating words. Constants listed within an `enum class` elements are to be written in all lower case letters with different words sperated by a single underscore.
Enums (both `enum` and `enum class`) are to be named in singular and to be written in all lower case letters with different words sperated by a single underscore.

Example (do not):
```cpp
constexpr int milliseconds = 1000;
constexpr int Seconds =  1 * milliseconds;
constexpr int MAXPINGDELTA = 20 * Seconds;

enum Colors
{
    cornflowerBlue = 1,
    VanDykeBrown = 2,
}

enum class colors
{
    CORNFLOWER_BLUE = 1,
    Van_Dyke_Brown = 2,
}
```

Example (do):
```cpp
constexpr int MILLISECONDS = 1000;
constexpr int SECONDS =  1 * MILLISECONDS;
constexpr int MAX_PING_DELTA = 20 * SECONDS;

enum color
{
    CORNFLOWER_BLUE = 1,
    VAN_DYKE_BROWN = 2,
}

enum class color
{
    cornflower_blue = 1,
    van_dyke_brown = 2,
}
```

### Variables
Variable names have to be written in all lower case letters with different words sperated by a single underscore.

Example (do):
```cpp
std::string my_fancy_string;
int just_an_ordinary_integer;
```

### User Defined Types
User defined types have to be written in all lower case letters with different words sperated by a single underscore.

Example (do):
```cpp
struct my_struct;
class my_fancy_class;
```

### Namespaces
Namespaces have to be written in all lower case letters.

Example (do not):
```cpp
namespace my_namespace
{
    // ...
}
namespace myNamespace
{
    // ...
}
```

Example (do):
```cpp
namespace mynamespace
{
    // ...
}
```

## Language Usage

### Header Guards
Use the `#pragma once` preprocessor directive instead of the traditional `#ifndef` method.

Example (do not):
```cpp
#ifndef MY_HEADER_NAME
#define MY_HEADER_NAME
// ...
#endif
```

Example (do):
```cpp
#pragma once
// ...
```


### Header Inclusion Order
Headers have to be broken down in the following order:

1. This project's header files
2. Third party header files
3. STL header files
4. System header files

Header files in each of these groups have to be sorted alphabetically.

Example (do not):
```cpp
#include <iostream>
#include "my_file.hpp"
#include "random_header.hpp"
#include <boost/range.hpp>
#include <vector>
```

Example (do):
```cpp
#include "my_file.hpp"
#include "random_header.hpp"

#include <boost/range.hpp>

#include <iostream>
#include <vector>
```

### Prefer `enum class` over `enum`
For type-safety purposes, you should always prefer `enum class` over `enum`!

Example:
```cpp
enum border_color
{
    RED = 1,
    GREEN = 2,
    BLUE = 3,
}
enum background_color
{
    ORANGE = 1
    RED = 2,
}

void set_background_color(background_color color)
{
    switch(color)
    {
        case RED:       // this compiles fine!
            // ...
    }
}

SetBackgroundColor(RED); // Oops!
```
```cpp
enum class border_color
{
    red = 1,
    green = 2,
    blue = 3,
}
enum class background_color
{
    orange = 1
    red = 2,
}

void SetBackgroundColor(background_color color)
{
    switch(color)
    {
        case background_color::red:
            // ...
    }
}

SetBackgroundColor(background_color::red);
```

### Exceptions
Exceptions should only be thrown in exceptional cases to signal a fatal error. It is not to be used as flow-control to deal with invalid player input!

### Pointers
Never use naked new or delete! Always use unique_ptr or smart_ptr instead!
Do not pass around owning raw pointers!
