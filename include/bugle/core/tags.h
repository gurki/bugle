#pragma once

#include <nlohmann/json.hpp>

namespace bugle {


typedef nlohmann::json tags_t;
tags_t filterTags( const nlohmann::json& tags );


}   //  ::bugle