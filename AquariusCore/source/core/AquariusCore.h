#pragma once
#ifdef AQ_PLATFORM_WINDOWS
#endif
#define AQUARIUS_API





#ifdef AQ_DEBUG
	#define AQ_ENABLE_ASSERTS
#endif // AQ_DEBUG

#ifdef AQ_ENABLE_ASSERTS

	#define AQ_ASSERT(x,...) {if( !(x) )  {AQ_CLIENT_ERROR("Assertion Failed:{0}",__VA_ARGS__);__debugbreak(); } }
	#define AQ_CORE_ASSERT(x,...) {if( !(x) ) {AQ_CORE_ERROR("Assertion Failed:{0}",__VA_ARGS__);__debugbreak();}  }

#else

	#define AQ_ASSERT(x,...)
	#define AQ_CORE_ASSERT(x,...)

#endif



#define BIT(X) (1 << X)
#define AQ_BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)



//AQObject Macro
#define AQ_INITIALIZE_AQOBJECT_TYPE(type) m_Name = "Unnamed "#type;\
m_type = AQObjectType::type
#define AQ_INITIALIZE_AQOBJECT_NAME_AND_TYPE(name,type) if(name.size())\
m_Name = name;\
else \
m_Name =  "Unnamed "#type;\
m_type = AQObjectType::type
#define AQ_COPY_AQOBJECT_NAME_AND_TYPE(other) m_Name = other.m_Name;\
m_type = other.m_type
//_____________________________________________________________________




//AQ commonly  used Macro
#define AQ_CORE_C_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))
#define AQ_CORE_TYPE_HASHCODE(type) typeid(type).hash_code()

















//AQTYPE Macro
#define AQCHAR char
#define AQINT int
#define AQUINT uint32_t
#define AQUINT64 uint64_t
#define AQFLOAT float
#define AQDOUBLE double
#define AQBOOL bool
//________________________________________________



//AQ_CORE_EIGEN Macro
#define AQ_CORE_EIGEN_IDENTITY_MATRIX4F(x,mat4)mat4<<x, 0.0f, 0.0f, 0.0f,\
0.0f,x, 0.0f, 0.0f,\
0.0f, 0.0f,x, 0.0f,\
0.0f, 0.0f, 0.0f, x







//AQ_CORE_IMGUI Macro
#define AQ_CORE_IMGUI_LEFT_LABEL(label,func, ...) ImGui::TextUnformatted(label); ImGui::SameLine(); func("##" label, __VA_ARGS__)
//func 的label不能置空字符串写成"##label"形式
#define AQ_CORE_IMGUI_LEFT_LABEL_IFSTRUCTURE(label,func, code) ImGui::TextUnformatted(label); ImGui::SameLine(); if(func) { code }ImGui::NextColumn()
#define AQ_CORE_IMGUI_COMPONENT_VAR_BYCOLUMN_IFSTRUCTURE(label,func, code) ImGui::TextUnformatted(label); ImGui::NextColumn(); ImGui::SetNextItemWidth(-1); if(func) { code } ImGui::NextColumn()
