#pragma once
#include <cassert>


template <typename T>
class Singleton
{
private:
	//static T* instance;
private:
	Singleton()
	{
		assert(instance == nullptr);
		instance = static_cast<T*>(this);
	}
	friend T;
public:
	Singleton& operator = (const Singleton&) = delete;
	Singleton& operator = (Singleton&&) = delete;

	static T& Create()
	{
		instance = new T();
		return *instance;
	}

	static T& Create(T* ptr)
	{
		instance = ptr;
		return *instance;
	}

	~Singleton()
	{
		instance = nullptr;
	}

	static T& GetSingleton()
	{
		return *instance;
	}

	static T* GetSingletonPtr()
	{
		return instance;
	}

	static void Delete()
	{
		delete instance;
	}

private:
	static inline T* instance = nullptr;
};

