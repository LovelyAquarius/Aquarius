#pragma once
#include "AquariusCore.h"
#include "Utils/AQMonitor/AQMonitor.h"
#include <atomic>
#include <string>
namespace Aquarius
{
	class AQApplicationMonitor;

	enum class AQObjectType
	{
		NoneAQObJ = 0,
		AQObject = 1,
		AQShder = 10, AQGLShder = 11,
		AQVertexBuffer = 20, AQGLVertexBuffer = 21,
		AQElementBuffer = 30, AQGLElementBuffer = 31,
		AQVertexArray = 40, AQGLVertexArray = 41
	};

	//AQ对象类，AQ对象自带引用计数
	//目前在AQ程序中进行AQ自动管理的类有:
	//AQShder,  AQVertexBuffer, AQElementBuffer ,AQVertexArray
	class AQUARIUS_API AQObject
	{
	public:
		AQObject(){}
		AQObject(const AQObject&) : m_referenceCount(0) {}
		~AQObject() { }

		AQINT GetReferenceCount() const { return m_referenceCount; };
		void ReferenceIncrease() const { ++m_referenceCount; }
		void ReferenceDecrease(bool deallocated = true) const noexcept;
		
		const std::string GetName()const { return m_Name; }
		void SetName(std::string& newname) {m_Name = newname;}
	private:
		mutable std::atomic<AQINT> m_referenceCount{ 0 };
	protected:
		std::string m_Name="Unnamed Object";
	public:
		AQObjectType m_type = AQObjectType::AQObject;
	};

	//AQ对象引用计数的辅助类
	template <typename T> class AQUARIUS_API AQreference
	{
		
	public:
		AQreference() {}

		//AQ对象指针创建引用
		AQreference(T* pointer) : m_Pointer(pointer)
		{
			if (m_Pointer)
			{
				((AQObject*)pointer)->ReferenceIncrease();
				Report();
			}
				
		}

		//引用创建引用
		AQreference(const AQreference& r) : m_Pointer(r.m_Pointer)
		{
			if (m_Pointer)
			{
				((AQObject*)m_Pointer)->ReferenceIncrease();
				Report();
			}
		}

		//引用移动到新引用
		AQreference(AQreference&& r) noexcept : m_Pointer(r.m_Pointer)
		{
			r.m_Pointer = nullptr;
		}

		~AQreference()
		{
			if (m_Pointer)
				((AQObject*)m_Pointer)->ReferenceDecrease();
		}


		//引用移动到已创建的引用
		AQreference& operator=(AQreference&& r) noexcept
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
		AQreference& operator=(const AQreference& r) noexcept
		{
			if (m_Pointer != r.m_Pointer) 
			{
				if (r.m_Pointer)
				{
					((AQObject*)r.m_Pointer)->ReferenceIncrease();
					Report();
				}
				if (m_Pointer)
					((AQObject*)m_Pointer)->ReferenceDecrease();
				m_Pointer = r.m_Pointer;
			}
			return *this; 
		}

		//AQ对象指针覆盖到已创建的引用
		AQreference& operator=(T* pointer) noexcept
		{
			if (m_Pointer != pointer) 
			{
				if (pointer)
				{
					((AQObject*)pointer)->ReferenceIncrease();
					Report();
				}
				if (m_Pointer)
					((AQObject*)m_Pointer)->ReferenceDecrease();
				m_Pointer = pointer;
			}
			return *this;
		}

		//判断两个引用是否相同
		bool operator==(const AQreference& r) const { return m_Pointer == r.m_Pointer; }

		//判断两个引用是否不相同
		bool operator!=(const AQreference& r) const { return m_Pointer != r.m_Pointer; }

		//判断一个引用与一个AQ对象指针是否相同
		bool operator==(const T* ptr) const { return m_Pointer == ptr; }

		//判断一个引用与一个AQ对象指针是否不相同
		bool operator!=(const T* ptr) const { return m_Pointer != ptr; }

		T* operator->() { return m_Pointer; }

		const T* operator->() const { return m_Pointer; }

		T& operator*() { return *m_Pointer; }

		const T& operator*() const { return *m_Pointer; }

		 operator T* () { return m_Pointer; }

		operator const T* ()const { return m_Pointer; }

		T* get() { return m_Pointer; }

		const T* get() const { return m_Pointer; }

		operator bool() const { return m_Pointer != nullptr; }

    private:
		void Report()
		{
			if (AQApplicationMonitor::Enable&& m_Pointer)
			{
				if (!AQApplicationMonitor::s_Monitor->AQObjects.contains((void*)m_Pointer))
					AQApplicationMonitor::s_Monitor->AQObjects[m_Pointer] = (AQObject*)m_Pointer;
			}
		}
			


	private:
		T* m_Pointer = nullptr;
	};

	template<typename T>
		using AQRef = AQreference<T>;

}