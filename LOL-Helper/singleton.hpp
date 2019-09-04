#pragma once

template <class T>
class Singleton
{
public:
	static T* get_singleton_ptr(){
		static T* ptr= new T;
		return ptr;
	}
};
