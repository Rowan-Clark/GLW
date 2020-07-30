// AttributeLayout.h
//
// An header-only definition allowing users to more conveniently
// the define the attribute layout of vertex data with nested braces
// which can then be interpretted by GLW classes.
// 
// ---- Usage ----
// 
//    GLW::AttributeLayout attribLayout = {
//            {"position", 3},
//            {"normal", 3},
//            {"TexCoords", 2}
//    };
//

#ifndef _ATTRIBUTE_LAYOUT_H_
#define _ATTRIBUTE_LAYOUT_H_

#include <string>
#include <tuple>
#include <vector>

namespace GLW
{
	using AttributeLayout = std::vector<std::tuple<std::string, int>>;
}

#endif //_ATTRIBUTE_LAYOUT_H_