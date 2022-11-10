#pragma once
#include "AquariusCore.h"
#include "Utils/AQMonitor/AQMonitor.h"
#include <atomic>
#include <string>
namespace Aquarius
{
	class AQApplicationMonitor;
	class AQObject;



	enum class AQObjectType
	{
		NoneAQObJ = 0,
		AQObject = 1, AQScene=2, AQElement=3,
		//渲染组件
		AQShader = 10, AQGLShader = 11,
		AQVertexBuffer = 20, AQGLVertexBuffer = 21,
		AQElementBuffer = 30, AQGLElementBuffer = 31,
		AQVertexArray = 40, AQGLVertexArray = 41,
		AQTexture = 50, AQTexture2D = 51, AQGLTexture2D=52,
		AQSubTexture2D=60, AQGLSubTexture2D=61,
		AQFrameBuffer =70, AQGLFrameBuffer=71,
		//__________________________________________
		//几何图形组件
		AQQuadraticBezierCurve2D =100,
		AQQuadraticBezierShape2D=110,

		//__________________________________________
		//基本元素
		AQComponent=1000, AQTransformComponent=1001, AQTagComponent=1002, AQColorComponent=1003, AQCameraComponent=1004, AQNativeScriptComponent=1005,


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

		template <typename Q>
		AQreference(AQreference<Q>& r)
		{
			m_Pointer = dynamic_cast<T*>(r.Get());
			if (m_Pointer)
			{
				((AQObject*)m_Pointer)->ReferenceIncrease();
				Report();
			}
			else
			{
				AQ_CORE_WARN("AQreference Constuctor::For Some Reason the casting failed,please check the object and type.!");
			}
		}

		template <typename Q>
		AQreference(const AQreference<Q>& r)
		{
			m_Pointer = const_cast<T*>(dynamic_cast<const T*>(r.Get()));
			if (m_Pointer)
			{
				((AQObject*)m_Pointer)->ReferenceIncrease();
				Report();
			}
			else
			{
				AQ_CORE_WARN("AQreference Constuctor::For Some Reason the casting failed,please check the object and type.!");
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

		//从各个AQObject子类中的Create函数创建引用
		//template<typename ...Args>
		//AQreference(Args&&... args):m_Pointer (T::Create(std::forward<Args>(args)...))
		//{
		//	if (m_Pointer)
		//	{
		//		((AQObject*)m_Pointer)->ReferenceIncrease();
		//		Report();
		//	}
		//}




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




		T* Get() { return m_Pointer; }

		const T* Get() const { return m_Pointer; }

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


	template<typename T, typename Q>
	AQRef<T> AQRefCast(AQRef<Q>& origin)noexcept
	{
		T* p = dynamic_cast<T*>(origin.Get());
		if (!p)
			AQ_CORE_WARN("AQRefCast::For Some Reason the casting failed,please check the object and type.!");
		return p;
	}

	template<typename T, typename Q>
	const AQRef<T> AQRefCast(const AQRef<Q>& origin)noexcept
	{
		 T* p = const_cast<T*>(dynamic_cast<const T*>(origin.Get()));
		 if (!p)
			 AQ_CORE_WARN("AQRefCast::For Some Reason the casting failed,please check the object and type.!");
		return p;
	}










}

namespace Aquarius
{



	class AQUARIUS_API AQObject
	{
	public:
		static AQRef<AQObject> Create() { return new AQObject(); }
		static AQRef<AQObject> Create(const std::string& name) { return new AQObject(name); }
		static AQObjectType ClassType() { return AQObjectType::AQObject; }
	public:
		virtual AQRef<AQObject> Copy() { return new AQObject(*this); }//有些类不支持copy
		~AQObject() {}

		AQINT GetReferenceCount() const { return m_referenceCount; };
		//不要调用这个函数！
		/*auto SetReferenceCount(int count) { m_referenceCount = count; }*/
		void ReferenceIncrease() const { ++m_referenceCount; }
		void ReferenceDecrease(bool deallocated = true) const noexcept;



		const std::string GetName()const { return m_Name; }
		void SetName(std::string& newname) { m_Name = newname; }
		const AQObjectType GetType()const { return m_type; }

	protected:
		AQObject() :m_Name("Unnamed Object") { m_type = AQObjectType::AQObject; }
		AQObject(const std::string& name) :m_Name(name) { m_type = AQObjectType::AQObject; }
		AQObject(const AQObject& other) : m_referenceCount(0), m_Name(other.m_Name), m_type(AQObjectType::AQObject) {}

	private:
		mutable std::atomic<AQINT> m_referenceCount{ 0 };

	protected:
		std::string m_Name;
		AQObjectType m_type = AQObjectType::AQObject;
	public:

	};










}