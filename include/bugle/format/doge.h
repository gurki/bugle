#pragma once

#include <vector>
#include <string>
#include <random>

namespace bugle {


static const std::vector<std::string> kDogePrefix = {
    "WOW",
    "SUCH",
    "MANY",
    "GREAT",
    "AMAZING",
    "FANTASTIC",
    "AWESOME",
    "INCREDIBLE",
    "IMPRESSIVE",
    "EXCELLENT",
    "SUPERB",
    "PHENOMENAL",
    "OUTSTANDING",
    "BRILLIANT",
    "SUPER",
    "FABULOUS",
    "TERRIFIC",
    "SPECTACULAR",
    "WONDERFUL",
    "STUNNING"
};

static const std::vector<std::string> kDogeSuffix = {
    "SUCCESS",
    "EFFORT",
    "GAIN",
    "VICTORY",
    "ACHIEVE",
    "HAPPY",
    "TRIUMPH",
    "ACCOMPLISHMENT",
    "WIN",
    "EXECUTION",
    "RUNTIME",
    "COMPILE",
    "IMPROVE",
    "STABILITY",
    "FEATURES",
    "RESULT",
    "WORK",
    "CODE",
    "FUNCTION",
    "RESPONSE",
    "CLEAN",
    "DEBUG",
    "OPTIMIZE",
    "SECURITY",
    "FLEXIBILITY",
    "UPDATES",
    "FIX",
    "MAINTENANCE",
    "SIMPLICITY",
    "PROGRESS",
    "LOGIC",
    "DESIGN",
    "INTEGRATION",
    "INNOVATION",
    "COMPATIBILITY",
    "MODULARITY",
    "ENHANCE",
    "DYNAMICS",
    "SOPHISTICATION",
    "FUNCTIONALITY",
    "INTERFACE",
    "ARTISTRY",
    "POTENTIAL",
    "ADAPTATION",
    "CUSTOMIZATION",
    "POWER",
    "VERSATILITY",
    "USABILITY",
    "SMOOTHNESS",
    "DEPENDABILITY",
    "SOLUTIONS",
    "PERFORMANCE",
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