#pragma once

#include <vector>
#include <string>
#include <random>

using namespace std::literals::string_view_literals;

namespace bugle {


static const std::vector<std::string_view> kDogePrefix = {
    "wow",
    "such",
    "many",
    "great",
    "amazing",
    "fantastic",
    "awesome",
    "incredible",
    "impressive",
    "excellent",
    "superb",
    "phenomenal",
    "outstanding",
    "brilliant",
    "super",
    "fabulous",
    "terrific",
    "spectacular",
    "wonderful",
    "stunning"
};

static const std::vector<std::string_view> kDogeSuffix = {
    "success",
    "effort",
    "gain",
    "victory",
    "achieve",
    "happy",
    "triumph",
    "accomplishment",
    "win",
    "execution",
    "runtime",
    "compile",
    "improve",
    "stability",
    "features",
    "result",
    "work",
    "code",
    "function",
    "response",
    "clean",
    "debug",
    "optimize",
    "security",
    "flexibility",
    "updates",
    "fix",
    "maintenance",
    "simplicity",
    "progress",
    "logic",
    "design",
    "integration",
    "innovation",
    "compatibility",
    "modularity",
    "enhance",
    "dynamics",
    "sophistication",
    "functionality",
    "interface",
    "artistry",
    "potential",
    "adaptation",
    "customization",
    "power",
    "versatility",
    "usability",
    "smoothness",
    "dependability",
    "solutions",
    "performance"
};


static std::string randomDoge()
{
    static auto gen = std::mt19937{ std::random_device{}() };
    static auto rnd = std::uniform_real_distribution();

    std::vector<std::string> confs( 2 );
    std::ranges::sample( kDogeSuffix, confs.begin() + 1, 1, gen );
    std::ranges::sample( kDogePrefix, confs.begin(), 1, gen );

    if ( rnd( gen ) < 0.1f ) {
        return confs[ 0 ];
    }

    if ( rnd( gen ) < 0.1f ) {
        return confs[ 1 ];
    }

    return std::format( "{} {}", confs[ 0 ], confs[ 1 ] );
}


}   //  ::bugle