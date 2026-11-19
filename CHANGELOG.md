# Changelog

All notable changes to matchable are listed here. The format follows
[Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and versions follow
[Semantic Versioning](https://semver.org/spec/v2.0.0.html). For a macro library, a removed or renamed
name, a changed signature or a new overload that stops a function being passed by name is a major change.
Each release is tagged vX.Y.Z.

## [Unreleased]

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
