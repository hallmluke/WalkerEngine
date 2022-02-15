#include "walkerpch.h"
#include "Entity.h"

namespace Walker {
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
}
