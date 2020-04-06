/*=============================================================================
   Copyright (c) 2016-2018 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(JSON_IO_DECEMBER_18_2017)
#define JSON_IO_DECEMBER_18_2017

#include <json/json.hpp>
#include <infra/filesystem.hpp>
#include <optional>
#include <fstream>

namespace cycfi { namespace json
{
   template <typename T>
   std::optional<T> load(fs::path path)
   {
      if (fs::exists(path))
      {
         std::ifstream file(path.string().c_str(), std::ios::binary);
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
   void save(fs::path path, T const& attr)
   {
      std::ofstream file(path.string().c_str());
      std::string json;
      printer pr(file);
      pr.print(attr);
   }
}}

#endif
