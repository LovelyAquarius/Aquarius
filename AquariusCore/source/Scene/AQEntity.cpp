#include "AQPCH.h"
#include "AQEntity.h"


namespace Aquarius
{
	AQEntity::AQEntity(entt::entity handle, AQRef<AQScene>& scene)
		:m_EntityHandle(handle),m_Scene(scene){}

}
namespace Aquarius
{
}