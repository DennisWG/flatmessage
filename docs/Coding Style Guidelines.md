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
- [Naming Conventions](#naming-conventions)
- [Language Usage](#language-usage)

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
class Foo {
    void Empty()
    {
    }
    
    void SingleLine() {
        std::cout << std::endl;
    }
    
    void SingleLineOverLength() { std::cout << "this instruction is way too long and should be placed in its own line" << std:: endl; }
    
    void Foobar()
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
class Foo
{
    void Empty() {}
    void SingleLine() { std::cout << std::endl; }
    void SingleLineOverLength()
    {
        std::cout << "this instruction is way too long and should be placed in its own line" << std::endl;
    }

    void Foobar()
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
namespace Foo {
int Foo();
}

namespace Foo { namespace Bar {
int Foo();
}}
```

Example (do):
```cpp
namespace Foo
{
    int Foo();
}

namespace Foo
{
    namespace Bar
    {
        int Foo();
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
bool Calculate(int& outResult, int a, int b);
```

Example (do):
```cpp
bool Calculate(int a, int b, int& outResult);
```

### Variable Scope
Variables are to be declared in the scope they're used in as near to their point of initialization as possible. It's also advised to declare them close the point where they're being used.

Example (do not):
```cpp
void Foo(bar const& bar)
{
    Something woof;
    // ...
    int n = 0;
    for (auto& meow : bar.Iterate())
    {
        woof = meow.Something();
        // ...
        bar.SomethingElse(woof, n);
    }
    std::cout << n << std::endl;
}
```

Example (do):
```cpp
void Foo(bar const& bar)
{
    // ...
    int n = 0;
    for (auto& meow : bar.Iterate())
    {
        // ...
        auto woof = meow.Something();
        bar.SomethingElse(woof, n);
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
Functions and methods are to be named in CamelCase starting with an upper case letter.

Example (do not):
```cpp
void my_function();
void myFunction();
void My_Function();
```

Example (do):
```cpp
void MyFunction();
```

### Output Arguments
Output arguments of a function or method have to be prefixed with `out`.

Example (do):
```cpp
void DoStuff(int a, int b, int& outResult);
```

### Constants
Constants shall be written in all upper case letters with underscores seperating words. Constants listed within an `enum class` elements are to be written in CamelCase starting with an upper case character.
Enums (both `enum` and `enum class`) are to be named in singular and to be written in CamelCase.

Example (do not):
```cpp
constexpr int milliseconds = 1000;
constexpr int Seconds =  1 * milliseconds;
constexpr int MAXPINGDELTA = 20 * Seconds;

enum colors
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

enum Color
{
    CORNFLOWER_BLUE = 1,
    VAN_DYKE_BROWN = 2,
}

enum class Color
{
    CornflowerBlue = 1,
    VanDykeBrown = 2,
}
```

### Variables
Variable names have to be written in CamelCase starting with a lower case character.

Example (do):
```cpp
std::string myFancyString;
int justAnOrdinaryInteger;
```

### User Defined Types
User defined types have to be written in CamelCase starting with a upper case character.

Example (do):
```cpp
struct MyStruct;
class MyFancyClass;
```

### Namespaces
Namespaces have to be written in CamelCase starting with an upper case character.

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
namespace MyNamespace
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
#include "MyFile.hpp"
#include "RandomHeader.hpp"
#include <boost/range.hpp>
#include <vector>
```

Example (do):
```cpp
#include "MyFile.hpp"
#include "RandomHeader.hpp"

#include <boost/range.hpp>

#include <iostream>
#include <vector>
```

### Prefer `enum class` over `enum`
For type-safety purposes you should always prefer `enum class` over `enum`!

Example:
```cpp
enum BorderColor
{
    RED = 1,
    GREEN = 2,
    BLUE = 3,
}
enum BackgroundColor
{
    ORANGE = 1
    RED = 2,
}

void SetBackgroundColor(BackgroundColor color)
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
enum class BorderColor
{
    Red = 1,
    Green = 2,
    Blue = 3,
}
enum class BackgroundColor
{
    Orange = 1
    Red = 2,
}

void SetBackgroundColor(BackgroundColor color)
{
    switch(color)
    {
        case BackgroundColor::Red:
            // ...
    }
}

SetBackgroundColor(BackgroundColor::Red);
```

### Exceptions
Exceptions should only be thrown in exceptional cases to signal a fatal error. It is not to be used as flow-control to deal with invalid player input!

### Pointers
Never use naked new or delete! Always use unique_ptr or smart_ptr instead!
Do not pass around owning raw pointers!
