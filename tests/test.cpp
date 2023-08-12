#include "base64.hpp"
#include <iostream>

size_t error_count = 0;

void report_error(const char* assertion, const char* file, int line) {
    if(error_count <= 20) {
       printf("%s:%d: failed %s\n", file, line, assertion);
    }
    ++error_count;
}

#define CHECK(...) do { if(__VA_ARGS__) {} else { report_error(#__VA_ARGS__, __FILE__, __LINE__); } } while(0)

int main() {

  using namespace code;
  using namespace std::string_literals;

  // Additional tests from

  // Additional tests from https://github.com/AntelopeIO/leap/blob/main/libraries/libfc/test/test_base64.cpp
  {
     auto input = "abc123$&()'?\xb4\xf5\x01\xfa~a"s;
     auto expected_output = "YWJjMTIzJCYoKSc/tPUB+n5h"s;

     CHECK(expected_output == base64_encode(input));
  }
  {
     auto input = "abc123$&()'?\xb4\xf5\x01\xfa~a"s;
     auto expected_output = "YWJjMTIzJCYoKSc_tPUB-n5h"s;

     CHECK(expected_output == base64_encode(input, true));
  }
  {
     auto input = "YWJjMTIzJCYoKSc/tPUB+n5h"s;
     auto expected_output = "abc123$&()'?\xb4\xf5\x01\xfa~a"s;

     CHECK(expected_output == base64_decode(input));
  }
  {
     auto input = "YWJjMTIzJCYoKSc_tPUB-n5h"s;
     auto expected_output = "abc123$&()'?\xb4\xf5\x01\xfa~a"s;

     CHECK(expected_output == base64_decode(input, true));
  }
  {
     auto input = "YWJjMTIzJCYoKSc/tPUB+n5h========="s;
     auto expected_output = "abc123$&()'?\xb4\xf5\x01\xfa~a"s;

     try {
       base64_decode(input);
       CHECK(false);
     } catch(const std::exception&) {}
  }
  {
     auto input = "YWJjMTIzJCYoKSc/tPU$B+n5h="s;
     try {
       base64_decode(input);
       CHECK(false);
     } catch(const std::exception&) {}
  }

   //
   // Note: this file must be encoded in UTF-8
   // for the following test, otherwise, the test item
   // fails.
   //
   const std::string orig =
      "René Nyffenegger\n"
      "http://www.renenyffenegger.ch\n"
      "passion for data\n";

   std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(orig.c_str()), orig.length());
   std::string decoded = base64_decode(encoded);

   CHECK(encoded == "UmVuw6kgTnlmZmVuZWdnZXIKaHR0cDovL3d3dy5yZW5lbnlmZmVuZWdnZXIuY2gKcGFzc2lvbiBmb3IgZGF0YQo=");
   CHECK(decoded == orig);

   // Test all possibilites of fill bytes (none, one =, two ==)
   // References calculated with: https://www.base64encode.org/

   std::string rest0_original = "abc";
   std::string rest0_reference = "YWJj";

   std::string rest0_encoded = base64_encode(reinterpret_cast<const unsigned char*>(rest0_original.c_str()),
                                             rest0_original.length());
   std::string rest0_decoded = base64_decode(rest0_encoded);

   CHECK(rest0_decoded == rest0_original);
   CHECK(rest0_reference == rest0_encoded);

   std::string rest1_original = "abcd";
   std::string rest1_reference = "YWJjZA==";

   std::string rest1_encoded = base64_encode(reinterpret_cast<const unsigned char*>(rest1_original.c_str()),
                                             rest1_original.length());
   std::string rest1_decoded = base64_decode(rest1_encoded);

   CHECK(rest1_decoded == rest1_original);
   CHECK(rest1_reference == rest1_encoded);

   std::string rest2_original = "abcde";
   std::string rest2_reference = "YWJjZGU=";

   std::string rest2_encoded = base64_encode(reinterpret_cast<const unsigned char*>(rest2_original.c_str()),
                                             rest2_original.length());
   std::string rest2_decoded = base64_decode(rest2_encoded);

   CHECK(rest2_decoded == rest2_original);
   CHECK(rest2_reference == rest2_encoded);

   // --------------------------------------------------------------
   //
   // Data that is 17 bytes long requires one padding byte when
   // base-64 encoded. Such an encoded string could not correctly
   // be decoded when encoded with «url semantics». This bug
   // was discovered by https://github.com/kosniaz. The following
   // test checks if this bug was fixed:
   //
   std::string a17_orig    = "aaaaaaaaaaaaaaaaa";
   std::string a17_encoded     = base64_encode(a17_orig);
   std::string a17_encoded_url = base64_encode(a17_orig, true);

   CHECK(a17_encoded == "YWFhYWFhYWFhYWFhYWFhYWE=");
   CHECK(a17_encoded_url == "YWFhYWFhYWFhYWFhYWFhYWE.");
   CHECK(base64_decode(a17_encoded_url) == a17_orig);
   CHECK(base64_decode(a17_encoded) == a17_orig);

   // --------------------------------------------------------------

   // characters 63 and 64 / URL encoding

   std::string s_6364 = "\x03" "\xef" "\xff" "\xf9";

   std::string s_6364_encoded     = base64_encode(s_6364);
   std::string s_6364_encoded_url = base64_encode(s_6364, true);

   CHECK(s_6364_encoded == "A+//+Q==");
   CHECK(s_6364_encoded_url == "A-__-Q..");
   CHECK(base64_decode(s_6364_encoded) == s_6364);
   CHECK(base64_decode(s_6364_encoded_url) == s_6364);

   // ----------------------------------------------

   std::string unpadded_input   = "YWJjZGVmZw"; // Note the 'missing' "=="
   std::string unpadded_decoded = base64_decode(unpadded_input);
   CHECK(unpadded_decoded == "abcdefg");

   unpadded_input   = "YWJjZGU"; // Note the 'missing' "="
   unpadded_decoded = base64_decode(unpadded_input);
   CHECK(unpadded_decoded == "abcde");

   unpadded_input   = "";
   unpadded_decoded = base64_decode(unpadded_input);
   CHECK(unpadded_decoded == "");

   unpadded_input   = "YQ";
   unpadded_decoded = base64_decode(unpadded_input);
   CHECK(unpadded_decoded == "a");

   unpadded_input   = "YWI";
   unpadded_decoded = base64_decode(unpadded_input);
   CHECK(unpadded_decoded == "ab");

   // --------------------------------------------------------------
   //
   //    2022-11-01
   //       Replace
   //          encoded_string[…] with encoded_sring.at(…)
   //       in
   //          decode()
   //
   std::string not_null_terminated = std::string(1, 'a');
   try {
      base64_decode(not_null_terminated);
      CHECK(false);
   } catch (std::out_of_range const&) {}

   // --------------------------------------------------------------
   //
   // Test the string_view interface (which required C++17)
   //
   std::string_view sv_orig    = "foobarbaz";
   std::string sv_encoded = base64_encode(sv_orig);

   CHECK(sv_encoded == "Zm9vYmFyYmF6");

   std::string sv_decoded = base64_decode(sv_encoded);

   CHECK(sv_decoded == sv_orig);

   if (error_count == 0)
      std::cout << "Success" << std::endl;

   return error_count;
}
