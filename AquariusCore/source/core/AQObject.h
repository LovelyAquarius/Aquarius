#pragma once
#include "AquariusCore.h"
#include <atomic>

namespace Aquarius
{
	//AQ对象类，AQ对象自带引用计数
	class AQUARIUS_API AQObject
	{
	public:
		AQObject(){}
		AQObject(const AQObject&) : m_referenceCount(0) {}
		~AQObject() { }

		AQINT GetReferenceCount() const { return m_referenceCount; };
		void ReferenceIncrease() const { ++m_referenceCount; }

		void ReferenceDecrease(bool deallocated = true) const noexcept;
	
	
	private:
		mutable std::atomic<AQINT> m_referenceCount{ 0 };

	};

	//AQ对象引用计数的辅助类
	template <typename T> class AQUARIUS_API reference
	{
	public:
		reference() {}

		//AQ对象指针创建引用
		reference(T* pointer) : m_Pointer(pointer) 
		{
			if (m_Pointer)
				((AQObject*)pointer)->ReferenceIncrease();
		}

		//引用创建引用
		reference(const reference& r) : m_Pointer(r.m_Pointer) 
		{
			if (m_Pointer)
				((AQObject*)m_Pointer)->ReferenceIncrease();
		}

		//引用移动到新引用
		reference(reference&& r) noexcept : m_Pointer(r.m_Pointer) 
		{
			r.m_Pointer = nullptr;
		}

		~reference() 
		{
			if (m_Pointer)
				((AQObject*)m_Pointer)->ReferenceDecrease();
		}


		//引用移动到已创建的引用
		reference& operator=(reference&& r) noexcept
		{
			if (&r != this)
			{
				if (m_Pointer)
					((AQObject*)m_Pointer)->ReferenceDecrease();
				m_Pointer = r.m_Pointer;
				r.m_Pointer = nullptr;
			}
			return *this;
		}

		//引用覆盖到已创建的引用
		reference& operator=(const reference& r) noexcept
		{
			if (m_Pointer != r.m_Pointer) {
				if (r.m_Pointer)
					((AQObject*)r.m_Pointer)->ReferenceIncrease();
				if (m_Pointer)
					((AQObject*)m_Pointer)->ReferenceDecrease();
				m_Pointer = r.m_Pointer;
			}
			return *this;
		}

		//AQ对象指针覆盖到已创建的引用
		reference& operator=(T* pointer) noexcept 
		{
			if (m_Pointer != pointer) {
				if (pointer)
					((AQObject*)pointer)->ReferenceIncrease();
				if (m_Pointer)
					((AQObject*)m_Pointer)->ReferenceDecrease();
				m_Pointer = pointer;
			}
			return *this;
		}

		//判断两个引用是否相同
		bool operator==(const reference& r) const { return m_Pointer == r.m_Pointer; }

		//判断两个引用是否不相同
		bool operator!=(const reference& r) const { return m_Pointer != r.m_Pointer; }

		//判断一个引用与一个AQ对象指针是否相同
		bool operator==(const T* ptr) const { return m_Pointer == ptr; }

		//判断一个引用与一个AQ对象指针是否不相同
		bool operator!=(const T* ptr) const { return m_Pointer != ptr; }

		T* operator->() { return m_Pointer; }

		const T* operator->() const { return m_Pointer; }

		T& operator*() { return *m_Pointer; }

		const T& operator*() const { return *m_Pointer; }

		 operator T* () { return m_Pointer; }

		T* get() { return m_Pointer; }

		const T* get() const { return m_Pointer; }

		operator bool() const { return m_Pointer != nullptr; }

	private:
		T* m_Pointer = nullptr;
	};







}