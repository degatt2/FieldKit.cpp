/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/11/2010.
 */

#include "fieldkit/script/module/Logger.h"
#include "fieldkit/Logger.h"

using namespace fieldkit::script;

// Prints its arguments on stdout separated by spaces and ending with a newline.
v8::Handle<Value> LogPrint(Arguments const& args) 
{
    HandleScope handleScope;
	bool first = true;
	for (int i = 0; i < args.Length(); i++) {
		if (first) {
			first = false;
		} else {
			printf(" ");
		}
		v8::String::Utf8Value str(args[i]);
        LOG_OUT(ToCString(str));
	}
    LOG_INST->flush();
	return v8::Undefined();
}

v8::Handle<Value> LogInfo(Arguments const& args) 
{	
    HandleScope handleScope;
	std::stringstream ss;	
	for (int i = 0; i < args.Length(); i++) {
		v8::String::Utf8Value str(args[i]);
		ss << ToCString(str);
	}
	LOG_INFO(ss.str());	
	return v8::Undefined();
}

v8::Handle<Value> LogWarn(Arguments const& args) 
{	
    HandleScope handleScope;
	std::stringstream ss;	
	for (int i = 0; i < args.Length(); i++) {
		v8::String::Utf8Value str(args[i]);
		ss << ToCString(str);
	}
	LOG_WARN(ss.str());	
	return v8::Undefined();
}

v8::Handle<Value> LogError(Arguments const& args) 
{	
    HandleScope handleScope;
	std::stringstream ss;	
	for (int i = 0; i < args.Length(); i++) {
		v8::String::Utf8Value str(args[i]);
		ss << ToCString(str);
	}
	LOG_ERROR(ss.str());	
	return v8::Undefined();
}


void Logger::Initialize(v8::Handle<Object> target) 
{
    SET_METHOD(target, "print", LogPrint);
    SET_METHOD(target, "info", LogInfo);
    SET_METHOD(target, "warn", LogWarn);
    SET_METHOD(target, "error", LogError);
}

