/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 created by Marcus Wendt on 01/06/2010.
 */

#include "fieldkit/tweak/Model.h"

using namespace fieldkit::tweak;

void Model::addChild(Model* child) {
	child->setParent(this);
	children.push_back(child);
}

Model* Model::getChild(const string &name) {
	BOOST_FOREACH(Model* m, children) {	
		if(m->getName().compare(name))
			return m;
	}
	return NULL;
}

// -- Serialization ------------------------------------------------------------
void Model::fromXML(TiXmlElement* element) {
	
}

TiXmlElement* Model::toXML() {
	return 0;
}