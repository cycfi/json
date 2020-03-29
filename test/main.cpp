/*=============================================================================
   Copyright (c) 2016-2018 Joel de Guzman

   Distributed under the MIT License (https://opensource.org/licenses/MIT)
=============================================================================*/
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <infra/doctest.hpp>

#include <json/json.hpp>
#include <json/json_io.hpp>

#include <boost/fusion/include/equal_to.hpp>

#include <sstream>
#include <iostream>

namespace json = cycfi::json;
namespace x3 = boost::spirit::x3;
namespace fusion = boost::fusion;

template <typename T>
void test_parser(json::parser const& jp, char const* in, T n)
{
   char const* f = in;
   char const* l = f + std::strlen(f);

   T attr;
   bool r = x3::parse(f, l, jp, attr);
   CHECK(r);
   CHECK_EQ(attr, n);
};

void test_string1(json::parser const& jp, char const* in)
{
   char const* f = in;
   char const* l = f + std::strlen(f);

   json::string<char const*> s;
   bool r = x3::parse(f, l, jp, s);
   CHECK(r);
   CHECK_EQ(s.begin(), in);
   CHECK_EQ(s.end(), l);
};

void test_string2(json::parser const& jp, char const* in, std::string s)
{
   char const* f = in;
   char const* l = f + std::strlen(f);

   std::string attr;
   bool r = x3::parse(f, l, jp, attr);
   CHECK(r);
   CHECK_EQ(attr, s);
};

void test_string3(json::parser const& jp, std::string in, std::string s)
{
   std::string::const_iterator f = in.begin();
   std::string::const_iterator l = in.end();

   std::string attr;
   bool r = x3::parse(f, l, jp, attr);
   CHECK(r);
   CHECK_EQ(attr, s);
};

template <typename C>
bool same(C const& a, C const& b)
{
   if (boost::size(a) != boost::size(b))
      return false;
   for (std::size_t i = 0; i != boost::size(a); ++i)
      if (a[i] != b[i])
         return false;
   return true;
}

template <typename Container>
void test_array(json::parser const& jp, char const* in, Container const& c)
{
   char const* f = in;
   char const* l = f + std::strlen(f);

   Container attr;
   bool r = x3::phrase_parse(f, l, jp, x3::space, attr);
   CHECK(r);
   CHECK(same(attr, c));
};

struct foo
{
   int i;
   double d;
   std::string s;
};

struct bar
{
   int ii;
   double dd;
   std::vector<int> vv;
   foo ff;
};

template <typename T>
void test_object(json::parser const& jp, char const* in, T const& obj)
{
   char const* f = in;
   char const* l = f + std::strlen(f);

   T attr;
   bool r = x3::phrase_parse(f, l, jp, x3::space, attr);
   CHECK(r);
   CHECK(attr == obj);
};

BOOST_FUSION_ADAPT_STRUCT(
   foo,
   (int, i)
   (double, d)
   (std::string, s)
)

BOOST_FUSION_ADAPT_STRUCT(
   bar,
   (int, ii)
   (double, dd)
   (std::vector<int>, vv)
   (foo, ff)
)

using fusion::operator==;
json::parser jp;

///////////////////////////////////////////////////////////////////////////////
// ints and bools
TEST_CASE("test_json_ints_and_bools, test_json")
{
   test_parser(jp, "1234", 1234);
   test_parser(jp, "1234.45", 1234.45);
   test_parser(jp, "true", true);
   test_parser(jp, "false", false);
}

///////////////////////////////////////////////////////////////////////////////
// strings
TEST_CASE("test_json_strings, test_json")
{
   test_string1(jp, "\"This is my string\"");
   test_string1(jp, "\"This is \\\"my\\\" string\"");

   test_string2(jp, "\"This is my string\"", "This is my string");
   test_string2(jp, "\"This is \\\"my\\\" string\"", "This is \"my\" string");
   test_string2(jp, "\"Sosa did fine.\\u263A\"", u8"Sosa did fine.\u263A");
   test_string2(jp, "\"Snowman: \\u2603\"", u8"Snowman: \u2603");

   test_string3(jp, "\"This is my string\"", "This is my string");
}

///////////////////////////////////////////////////////////////////////////////
// int vector
TEST_CASE("test_json_vector, test_json")
{
   std::vector<int> c = {1, 2, 3, 4};
   test_array(jp, "[1, 2, 3, 4]", c);

   // empty vector
   std::vector<int> c2;
   test_array(jp, "[]", c2);
}

///////////////////////////////////////////////////////////////////////////////
// int array
TEST_CASE("test_json_array, test_json")
{
   std::array<int, 4> c = {{1, 2, 3, 4}};
   test_array(jp, "[1, 2, 3, 4]", c);

   int c2[4] = {1, 2, 3, 4};
   test_array(jp, "[1, 2, 3, 4]", c2);

   // empty array
   std::array<int, 0> c3;
   test_array(jp, "[]", c3);
}

///////////////////////////////////////////////////////////////////////////////
// double vector
TEST_CASE("test_json_double_vector, test_json")
{
   std::vector<double> c = {1.1, 2.2, 3.3, 4.4};
   test_array(jp, "[1.1, 2.2, 3.3, 4.4]", c);
}

///////////////////////////////////////////////////////////////////////////////
// string vector
TEST_CASE("test_json_string_vector, test_json")
{
   std::vector<std::string> c = {"a", "b", "c", "d"};
   test_array(jp, "[\"a\", \"b\", \"c\", \"d\"]", c);
}

///////////////////////////////////////////////////////////////////////////////
// struct
TEST_CASE("test_json_struct, test_json")
{
   foo obj = {1, 2.2, "hey!"};
   bar obj2 = {8, 9.9, {1, 2, 3, 4}, obj};

   {
      char const* in = R"(
         {
            "i" : 1,
            "d" : 2.2,
            "s" : "hey!"
         }
      )";

      test_object(jp, in, obj);
   }

   {
      char const* in = R"(
         {
            "ii" : 8,
            "dd" : 9.9,
            "vv" : [1, 2, 3, 4],
            "ff" :
            {
               "i" : 1,
               "d" : 2.2,
               "s" : "hey!"
            }
         }
      )";

      test_object(jp, in, obj2);
   }

   // Round trip
   {
      std::stringstream ss;
      json::printer pr{ss, 3};
      pr.print(obj2);
      test_object(jp, ss.str().c_str(), obj2);

      // Just for fun!
      // std::cout << ss.str() << std::endl;
   }
}

///////////////////////////////////////////////////////////////////////////////
// test json_io save and load
TEST_CASE("test_json_io, test_json")
{
   std::vector<std::string> c = {"a", "b", "c", "d"};

   std::filesystem::path tmpname(std::tmpnam(nullptr));
   json::save(tmpname, c);
   CHECK(std::filesystem::exists(tmpname));

   auto loaded = json::load<std::vector<std::string>>(tmpname);
   CHECK(loaded);
   CHECK(same(c, *loaded));

   std::filesystem::remove(tmpname);
}


