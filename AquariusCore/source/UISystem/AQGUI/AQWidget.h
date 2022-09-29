#pragma once

#include "AQGUI.h"
#include "AQLayout.h"

#include <vector>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>




namespace Aquarius
{
	 template class reference <AQLayout> ;





	class AQUARIUS_API AQWidget:public AQGUI

	{
	public:
		AQWidget(const std::string widgetname, AQWidget* parent);
		
	public:
		//属性访问方法
		const glm::i32vec2& GetPosition() const { return m_Position; }
		void SetPosition(const glm::i32vec2& position) { m_Position = position; }

		void SetFixedSize(const glm::i32vec2& fixedSize) { m_FixedSize = fixedSize; }
		const glm::i32vec2& GetFixedSize() const { return m_FixedSize; }
		int   GetFixedWidth() const { return m_FixedSize.x; }
		void SetFixedWidth(int width) { m_FixedSize.x = width; }
		int   GetFixedHeight() const { return m_FixedSize.y; }
		void SetFixedHeight(int height) { m_FixedSize.y = height; }

		const glm::i32vec2& GetSize() const { return m_Size; }
		void SetSize(const glm::i32vec2& size) { m_Size = size; }
		int   GetWidth() const { return m_Size.x; }
		void SetWidth(int width) { m_Size.x = width; }
		int   GetHeight() const { return m_Size.y; }
		void SetHeight(int height) { m_Size.y = height; }



		AQLayout* GetLayout() { return m_Layout; }
		const AQLayout* SetLayout() const { return m_Layout.get(); }
		void SetLayout(AQLayout* layout) { m_Layout = layout; }


	public:
		//访问父子
		AQWidget* GetParent() { return m_Parent; }
		const AQWidget* GetParent() const { return m_Parent; }
		void SetParent(AQWidget* parent) { m_Parent = parent; }

		int ChildCount() const { return (int)m_Children.size(); }
		virtual void AddChild(int index, AQWidget* widget);
		void AddChild(AQWidget* widget);


	public:
		//设置

	protected:
		//管理父级和子级
		AQWidget* m_Parent;
		std::vector<AQWidget*> m_Children;
		//物件的自身属性
		glm::i32vec2 m_Position;
		glm::i32vec2 m_Size;
		glm::i32vec2 m_FixedSize;



		//物件附加属性
		reference<AQLayout> m_Layout;

		

	};




}


