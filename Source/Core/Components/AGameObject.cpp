#include <Core/Components/AGameObject.h>

DK_LOG_CATEGORY_DEFINE(GameObjectLog)
DK_LOG_CATEGORY_DECLARE(GameObjectLog, ELoggerVerbosity::DEBUG)

void AGameObject::setComponentFlag(int id, bool value) {
	m_componentFlags = value ? m_componentFlags | (1LL << id) : m_componentFlags ^ (1LL << id);
}

bool AGameObject::getComponentFlag(int id){
	return m_componentFlags&(1LL << id);
}

void AGameObject::setHandleIDPair(int id, int handle) {
	m_handlesToComponents.insert(std::make_pair(id, handle));
}

int AGameObject::getHandle(int id) {
	return m_handlesToComponents[id];
}

drak::core::Scene * AGameObject::CurrentScene()
{
	return myScene;
}