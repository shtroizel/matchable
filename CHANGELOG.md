# Changelog

All notable changes to matchable are listed here. The format follows
[Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and versions follow
[Semantic Versioning](https://semver.org/spec/v2.0.0.html). For a macro library, a removed or renamed
name, a changed signature or a new overload that stops a function being passed by name is a major change.
Each release is tagged vX.Y.Z.

## [Unreleased]

### Added
- matchable::Variant, a base of every type's variants with as_string(), as_type_name(),
  as_by_string_index() and as_index()
- matchable::VariantRef, a reference to a variant of any type, or nil, usable as a property type
- matchable::Instance<type::variant>, one object per variant and its address. Code split into libraries
  can define instances in one and declare them in the rest
- matchable::variant_ref<type::variant>(), a variant's VariantRef: a constant that needs only a
  declaration of the variant
- **type**::from_variant(), which turns a VariantRef back into a typed variant, or nil
- MATCHABLE_INSTANCE_VISIBILITY, defined as `default`, `protected` or `hidden` when compiling the code
  that owns variants: each variant's Instance is defined where the variant is declared or grown, with
  that visibility under GCC and Clang
- MATCHABLE_EXTERN_VARIANTS(type, variant...), which declares variants that other code owns, so
  matchable::variant_ref refers to them without the type's declaration
- MatchableMaker::extern_variants lists such variants by type. save_as() declares them with
  MATCHABLE_EXTERN_VARIANTS before the matchables, 16 to a line, so property values can refer to
  variants of matchables in other files, and load() reads them back
- Escape codes `_x80_` through `_xff_` for bytes 128 to 255, so a variant name can hold any UTF-8 text,
  for example `caf_xc3__xa9_` for "café"
- MATCHABLE_SORTKEY(type, key), written before a type is declared, orders its variants by key(name) and
  then by name. Lookups and lt_by_string() follow that order
- VariantNames::lower_bound_by(less), which searches a type's names where they are with the caller's
  comparison, so a caller that knows the type's order looks a string up without building a key
- MatchableMaker: Matchable::sortkey names a key function, and save_as() in wrap mode writes
  MATCHABLE_SORTKEY once before the line that declares the type (always mode declares nothing).
  MatchableMaker::includes lists headers a saved file includes after the standard two, such as one
  declaring the key function. load() reads both back

### Changed
- By-string order (variants_by_string(), as_by_string_index(), lt_by_string() and, with
  MATCHABLE_OMIT_BY_INDEX, variants() and operator<) compares names as unsigned bytes, std::string's
  order, instead of putting symbols and digits before letters and comparing char values. It is now the
  same on every platform; before, names with bytes above 127 sorted differently where char is unsigned.
  Without MATCHABLE_OMIT_BY_INDEX, variants() and operator< keep declaration order
- Variant destructors are trivial and protected instead of virtual, and variant classes are final, so a
  referenced Instance needs no destruction at exit. Variants are still destroyed through shared_ptr

### Removed
- matchable::str_lt_str(). Compare names with std::string's operator< instead

## [15.1.0] - 2026-11-20

### Added
- MATCHABLE_UNLOADABLE: code compiled with it unregisters the variants its GROW_MATCHABLE lines added when
  it is unloaded (dlclose, or removal from a JIT), so a program can load, unload and swap code that grows
  its types. Without it, GROW_MATCHABLE works as before
- unregister_variants() on each type, which removes variants by as_index() in one pass

### Changed
- Registering a name that is already registered is refused, so lookups keep finding the first
- Once variants are removed, from_index() finds the rest by binary search, since as_index() values are
  not reused
- Registry tables, property storage, observers, nil values and the escapable tables are never destroyed.
  Rebuild everything compiled against an earlier version

### Fixed
- Storage that loaded code built first is no longer destroyed when that code is unloaded, so the program
  no longer reads freed memory afterward

## [15.0.0] - 2026-11-18

### Removed
- UNMATCHABLE, NAMESPACEx1_UNMATCHABLE through NAMESPACEx5_UNMATCHABLE and matchable::Unmatchable. Filter
  at the point of use instead, for example with MATCHABLE_INSTANCE_IN
- The separate per-variant slot: as_index() is now the one registration number, and MatchBox storage is
  keyed by it

### Added
- MATCHABLE_VERSION_MAJOR, MATCHABLE_VERSION_MINOR and MATCHABLE_VERSION_PATCH in matchable.h, usable in
  `#if` and `static_assert`
- match_strict(): when any non-nil variant lacks a handler, every call acts as nil (the nil handler if
  there is one, otherwise nothing). Both match() overloads have a match_strict() counterpart
- `-s` (`--sanitize`) in build_and_install.py, which sets the new SANITIZE CMake option for
  AddressSanitizer and UndefinedBehaviorSanitizer on GCC and Clang

### Changed
- as_index() exists in MATCHABLE_OMIT_BY_INDEX builds too
- from_string(), from_string_as_ref() and variants_by_string_index_of() gain `char const *` overloads, so
  they can no longer be passed by name. Wrap them in a lambda
- Flags equality compares the nil flag. Two Flags that differ only in nil now compare unequal
- currently_set() on Flags and MatchBox<M, T> includes nil, last, when nil is set. Its as_index() is -1
- A MatchBox built with a fill value starts nil at that value too, not at T{}
- During a notify, an observer added mid-notify waits for the next notify, a removed one no longer runs,
  and a replaced one runs its current function

### Fixed
- Registration is correct when a type is grown from several translation units: variants no longer go
  missing or share an index depending on link order
- An observer that removes itself or another observer during a notify no longer reads freed memory
- A nil-checked scalar property getter no longer returns a reference to a temporary under GCC 4.8
- Property reads and by-string lookups no longer allocate
- Startup with many variants per type no longer renumbers every variant through a virtual call
- MatchableMaker::save_as: dependency ordering looks up the dependency's matchable, not the property name;
  generated_matchable grows itself past 16 entries; a write error closes the file; flush errors at close
  are reported
- MatchableMaker::load splits arguments the way the preprocessor does, keeping spaces inside values and
  commas inside strings
- test_ok.h includes `<string>`, so the tests build with MSVC
- build_and_install.py builds on Windows with CMake's default generator
