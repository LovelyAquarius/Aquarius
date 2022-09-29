#include "AQPCH.h"
#include "AQWidget.h"



namespace Aquarius
{
    AQWidget::AQWidget(const std::string widgetname, AQWidget* parent)
        : AQGUI(std::string("AQWidget->") + widgetname),
        m_Parent(nullptr), m_Layout(nullptr),
        m_Position(glm::i32vec2(0)), m_Size(glm::i32vec2(0)),
        m_FixedSize(glm::i32vec2(0))
    {
        if (parent)
            parent->AddChild(this);
    }

    void AQWidget::AddChild(int index, AQWidget* widget)
    {
        assert(index <= ChildCount());
        m_Children.insert(m_Children.begin() + index, widget);
        widget->ReferenceIncrease();
        widget->SetParent(this);
    }

    void AQWidget::AddChild(AQWidget* widget)
    {
        AddChild(ChildCount(), widget);
    }

  






   












}