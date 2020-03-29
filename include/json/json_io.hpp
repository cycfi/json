/*=============================================================================
   Copyright (c) 2016-2018 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(JSON_IO_DECEMBER_18_2017)
#define JSON_IO_DECEMBER_18_2017

#include <json/json.hpp>
#include <filesystem>
#include <fstream>
#include <optional>

namespace cycfi { namespace json
{
   template <typename T>
   std::optional<T> load(std::filesystem::path path)
   {
      if (std::filesystem::exists(path))
      {
         std::ifstream file(path);
         std::string src(
            (std::istreambuf_iterator<char>(file))
         , std::istreambuf_iterator<char>());
         T data;
         auto f = src.begin();
         auto l = src.end();
         if (x3::phrase_parse(f, l, parser{}, x3::space, data))
            return { std::move(data) };
      }
      return {};
   }

   template <typename T>
   void save(std::filesystem::path path, T const& attr)
   {
      std::ofstream file(path);
      std::string json;
      printer pr(file);
      pr.print(attr);
   }
}}

#endif
