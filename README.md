__________
## Library:
Library provides facilities to make indirection more uniformly:

- **value** - performs one indirection.
```cpp
...
int * ptr = &i;
value( ptr ) == *ptr;
```

- **recursive_value** - perform multiple indirections until no more indirections are possible.
```cpp
...
boost::optional< std::reference_wrapper< int > > opt = &ref;
value( opt ) == ( *opt ).get();
```

- **recursive_value_trait_t** - metafunction that returns type of **recursive_value** expression.
```cpp
...
using opt_ref_t = boost::optional< std::reference_wrapper< int > >;
static_assert( std::is_same_v< 
    recursive_value_trait_t< opt_ref_t >, 
    int& > );
```

__________
## Motivation:

Today we have several _types_ which we use as wrappers for our types, like **std::optional**, **std::reference_wrapper**, **iterators/pointers** and in some cases for **std::pair** we want to get its value i.e. **.second**. And there is no unified method to obtain their _underlying values_. 
Thus my library provides unified **access** to _underlying values_ as long as _type_ has:
- **operator***, or
- **get()**

or **is**:
- **std::pair**

# Examples:

## _Value_
``` cpp
	// #1 ===============================
    std::reference_wrapper< std::string > str_ref = "Hi, there ref!"_s;
    std::cout << value( str_ref ) << "\n";
	// #2 ===============================
    std::optional< std::string > opt_str = "Hi, there optional!"_s;
    std::cout << value( opt_str ) << "\n";
	// #3 ===============================
	auto str = "Hi, there optional!"_s;
    std::string * ptr_str = &str;
    std::cout << value( ptr_str ) << "\n";
    // #4 ===============================
	auto pair = std::make_pair( 1, "Hi, there std::pair!"_s )
    std::cout << value( pair ) << "\n";
    // #5 ===============================
    std::vector< std::string > vs{ "Hi, there std::vector::iterator!"_s };
    std::cout << value( std::begin( vs ) ) << "\n";

```
-  #### Output:
    ```
    Hi, there ref!
    Hi, there optional!
    Hi, there std::pair!
    Hi, there std::vector::iterator!
	```

## _Recursive Value_
``` cpp
	// #1 ===============================
    auto str = "Hi, there underlying opt_ref!"_s;
    boost::optional< std::reference_wrapper< std::string > > opt_ref_str = std::ref( str );
    std::cout << recursive_value( opt_ref_str ) << "\n";

	// #2 ===============================
    using underlying_type_t = boost::optional< std::reference_wrapper< std::pair< int, std::string > > >;
    
    auto pair = std::make_pair( 1, "Hi, there underlying opt_ref_pair!"_s );    
    std::vector< underlying_type_t > vec{ std::make_optional( std::ref( pair ) ) };
    std::cout << recursive_value( std::begin( vec ) ) << "\n";
    
```
-  #### Output:
    ```
    Hi, there underlying opt_ref!
    Hi, there underlying opt_ref_pair!
	```
_______________

## Minimum C++ standard:
C++14
_______________

## Supported Compilers:

Should work on all major compilers which support **C++14**.<br/>
I personally tested on following:

- **clang++-10**  (or later)
- **g++-9** (or later)
- **Visual Studio** **2019**.


## How to build:

**release:** *./build.sh < compiler_name >* // where compiler_name=**clang\+\+ | g\+\+ | ...**, but *not* **clang | gcc**.

**debug:**  *./build_dev.sh < compiler_name >*
_______________
## License:
Boost Software License( Version 1.0 )
_______________

