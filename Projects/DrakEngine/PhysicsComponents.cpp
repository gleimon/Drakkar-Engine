#include <PrecompiledHeader/pch.hpp>
#include <PxPhysicsAPI.h>
#include <Engine\Components\PhysicsComponents.hpp>
#include <Engine\Physics\PhysicsSystem.hpp>
#include <Engine\Engine.hpp>
#include <Engine/Scene/LevelSystem.hpp>

using namespace drak;
using namespace components;

void drak::components::RigidBody::activate(bool value) {
	m_active = value;
	if (rigidActor == nullptr) {
		return;
	}
	if (core::Engine::Get().inEditorMode()) {
		physx::PxShape* shapes;
		physx::PxU32 arraySize;
		arraySize = rigidActor->getNbShapes();
		rigidActor->getShapes(&shapes, arraySize);
		for (U32 i = 0; i < arraySize; ++i) {
			shapes[i].setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, !value);
		}
	}
	else {
		rigidActor->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !value);
	}

}

void drak::components::RigidBody::deleteComponent() {
	core::Engine::Get().getPhysicsSystem().getPhysicsScene().removeActor(*rigidActor);
}

void drak::components::BoxCollider::deleteComponent() {
	if(core::Engine::Get().currentLevel().getGameObjects()[GameObjectID].getComponentFlag(ComponentType<RigidBody>::id))
		core::Engine::Get().currentLevel().getGameObjects()[GameObjectID].getComponent<RigidBody>()->rigidActor->detachShape(*shape);
}

void drak::components::RigidBody::initComponent(){
	rigidActor = nullptr;
}
