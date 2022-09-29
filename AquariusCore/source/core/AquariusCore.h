#pragma once
#ifdef AQ_PLATFORM_WINDOWS
#endif
#define AQUARIUS_API

#define BIT(X) (1 << X)



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

#define AQ_BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)


//AQTYPE
#define AQINT int
#define AQUINT unsigned int





