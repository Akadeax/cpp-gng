#pragma once

#include <vector>
#include <type_traits>

class Enemy;

template<typename T>
class EnemyPool
{
public:
	static_assert(std::is_base_of_v<Enemy, T>, "T must inherit from Enemy.");

	T* GetPooledObject();
	void ReturnObject(T* object);

private:
	std::vector<T*> m_Objects;
};

// Gets object of type T and removes it from the pool; return later with `ReturnObject`
template<typename T>
T* EnemyPool<T>::GetPooledObject()
{
	if (m_Objects.empty()) 
	{
		return nullptr;
	}

	T* object = m_Objects.back();
	m_Objects.pop_back();
	return object;
}

template<typename T>
void EnemyPool<T>::ReturnObject(T* object)
{
	m_Objects.push_back(object);
}