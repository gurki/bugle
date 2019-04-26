#pragma once

#include <nlohmann/json.hpp>

namespace mc {


typedef nlohmann::json::object_t tags_t;
tags_t filterTags( const nlohmann::json& tags );


}   //  ::mc