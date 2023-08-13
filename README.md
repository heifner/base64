# base64 (C++)

C++ Base64 encoding and decoding

* Header Only
* Templated for `std::string`, `std::string_view`, `std::vector<char>` and  other container of `char` types
* Performance improvements

## Examples
```c++
// Interface:
// Defaults allow for use:
   std::string s = "foobar";
   std::string s_encoded = base64_encode(s);
   std::string_view sv = "foobar";
   std::string sv_encoded = base64_encode(sv);
   std::vector<char> vc = {'f', 'o', 'o'};
   std::string vc_encoded = base64_encode(vc);

// Also allows for user provided char containers and specified return types:
   std::vector<char> encoded = base64_encode<std::vector<char>>(s);
```

## Based on

Fork of https://github.com/ReneNyffenegger/cpp-base64 with additions.
