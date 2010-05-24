/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   Space.h
 *	 Created by Marcus Wendt on 21/05/2010.
 */

#pragma once

#include "AABB.h"
#include "Spatial.h"
#include "BoundingVolume.h"

namespace fk { namespace physics {
	class Space : public fk::math::AABB {
	public:
		Space();
		Space(float w, float h, float d);
		
		virtual void clear();
		virtual void insert(Spatial* s);
		virtual void select(fk::math::BoundingVolume* volume);
	};
} } // namespace fk::physics