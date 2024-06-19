// #include "bugle/core/tags.h"

// namespace bugle {


// //////////////////////////////////////////////////////////////////////////////////
// tags_t filterTags( const nlohmann::json& tags )
// {
//     static auto insert = []( tags_t& jobj, const nlohmann::json& tag ) -> void
//     {
//         //  ignore arrays, numbers and bool

//         if ( tag.is_string() ) {
//             jobj[ tag.get<std::string>() ] = {};
//         }

//         if ( tag.is_object() ) {
//             for ( auto it = tag.begin(); it != tag.end(); ++it ) {
//                 jobj[ it.key() ] = it.value();
//             }
//         }
//     };

//     //  only consider first level strings, objects and convertible arrays
//     //  ignore all numbers, bools and second-level non-convertible arrays

//     tags_t jobj;

//     if ( tags.is_array() )
//     {
//         for ( const auto& tag : tags )
//         {
//             if ( tag.is_array() && tags.size() == 1 ) {
//                 for ( const auto& sub : tag ) {
//                     insert( jobj, sub );
//                 }
//             }
//             // else if ( tag.is_array() && tag.size() == 2 ) {
//             //     if ( tag[0].is_string() ) {
//             //         jobj[ tag[0].get<std::string>() ] = tag[1];
//             //     }
//             // }
//             else {
//                 insert( jobj, tag );
//             }
//         }
//     }
//     else {
//         insert( jobj, tags );
//     }

//     return jobj;
// }


// }   //  ::bugle