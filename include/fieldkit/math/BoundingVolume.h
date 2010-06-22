/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   BoundingVolume.h
 *	 Created by Marcus Wendt on 23/05/2010.
 */
#pragma once

#include "fieldkit/FieldKit.h"

namespace fieldkit { namespace math {

class BoundingVolume {
	public:
		Vec3f position;
		virtual bool contains(Vec3f p) = 0;
};

} } // namespace fieldkit::math