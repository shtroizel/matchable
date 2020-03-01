# MATCHABLE

Usage: MATCHABLE(type, variant...)

Where: variant... is: in [0..108] comma separated variants

## Getting Started

This compile time library consists of a single header file, "matchable.h".
Just copy src/matchable.h to somewhere in your project and then do something like:
```
#include <iostream>
#include "matchable.h"

MATCHABLE(Rating, Low, BelowAverage, Average, AboveAverage, High)

MATCHABLE(Status_foo, Ok, Err)

Status_foo::var foo()
{
    Rating::var r;
    for (Rating::var const & v: Rating::variants())
    {
        r = Rating::from_string(v.as_string());
        if (r != v)
            return Status_foo::Err::grab();
        std::cout << v << std::endl;
    }
    return Status_foo::Ok::grab();
}

int main()
{
    foo().match({
        { Status_foo::Ok::grab(), [&]() { std::cout << "foo() finished." << std::endl; } },
        { Status_foo::Err::grab(), [&]() { std::cout << "error!" << std::endl; exit(-1); } }
    });
    return 0;
}

```

### Prerequisites

clang >= 7.0.1-8  
or  
g++ >= 8.3.0

When building the examples CMakeLists.txt:8 forces clang, just comment out or set as desired

### Installing
Example workflow from project root:
```
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install ..
make install
cd ..
```

### Running Tests / Examples
Assuming workflow above with install directory under the project root (modify paths accordingly for your workflow).
<br/>
<br/>

List all tests:
```
install/test/bin/list.sh
```
Run Single Test (for example "max_variants"):
```
install/test/bin/run_test.sh max_variants
```
Run Quietly:
```
install/test/bin/run_test.sh max_variants quietly
```
Run All Tests:
```
install/test/bin/run_all.sh
```
Run All Tests Quietly
```
install/test/bin/run_all.sh quietly
```

### Example Source Code
Example code can be found under these directories:
```
test/programs
test/include
```

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/shtroizel/matchable/tags).

## Authors

* **Eric Hyer**

See also the list of [contributors](https://github.com/shtroizel/matchable/contributors) who participated in this project.

## License

This project is licensed under the "BSD 3-Clause License" - see the [LICENSE](LICENSE) file for details
