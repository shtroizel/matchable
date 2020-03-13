# MATCHABLE

Usage: MATCHABLE(type, variant...)

Where: variant... is: in [0..108] comma separated variants

## Getting Started

This compile time library consists of a single header file, "matchable.h".
Just copy src/matchable.h to somewhere in your project. To get started, take a look
at the examples under test/programs.

### Prerequisites

clang >= 7.0.1-8
or
g++ >= 8.3.0

### Running Tests / Examples

Example workflow from project root:
```
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER="/usr/bin/clang++" -DCMAKE_INSTALL_PREFIX=../install ..
make install
cd ..
```

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
Example code can be found under:
```
test/programs
```

## Versioning

For the versions available, see the [tags on this repository](https://github.com/shtroizel/matchable/tags).

## Authors

* **Eric Hyer**

See also the list of [contributors](https://github.com/shtroizel/matchable/contributors) who participated in this project.

## License

This project is licensed under the "BSD 3-Clause License" - see the [LICENSE](LICENSE) file for details
