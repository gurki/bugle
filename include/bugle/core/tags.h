#pragma once

#include <nlohmann/json.hpp>
#include <unordered_set>

namespace bugle {


using tags_t = std::unordered_set<std::string>;
using attributes_t = std::unordered_map<std::string, nlohmann::json>;

// tags_t filterTags( const nlohmann::json& tags );


}   //  ::bugle