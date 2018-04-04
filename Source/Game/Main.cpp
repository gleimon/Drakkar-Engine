#include <Core/Core.hpp>
#include <Engine/Engine.hpp>
#include <Core/Components/AGameObject.h>
#include <Engine/Components/Components.h>
#include <Engine/Scene/SceneSystem.h>
#include <Engine/Physics/SimulationEvent.h>
#include <Math/Matrix4x4.hpp>
#include <PxPhysicsAPI.h>
#include <string>

using namespace drak;
using namespace core;
using namespace components;
using namespace events;
using namespace function;
DK_IMPORT(drak::math)

class Player : public AGameObject {
	int counter = 0;
	virtual void Update() override {
		if (counter <= 1000) {
		}
		else {
			Engine::stopGame();
		}
		counter++;
	}

	virtual void Start() override {
		RigidBody* rb = myScene->getComponentByHandle<RigidBody>(myScene->getComponentByHandle<Transform>(transformIDX)->m_handlesToComponents[ComponentType<RigidBody>::id]);
		Engine::Get().getPhysicsSystem().AddCollisionCallback(
			rb,
			PhysicsEventDispatcher::COLLISION_IN,
			new MemberFunction<Player, void, const Event*>
			(this, &Player::OnCollisionEnter));
		Engine::Get().getPhysicsSystem().AddCollisionCallback(
			rb,
			PhysicsEventDispatcher::COLLISION_OUT,
			new MemberFunction<Player, void, const Event*>
			(this, &Player::OnCollisionExit));
		Engine::Get().getPhysicsSystem().AddCollisionCallback(
			rb,
			PhysicsEventDispatcher::COLLISION_STAY,
			new MemberFunction<Player, void, const Event*>
			(this, &Player::OnCollisionStay));
	}

	void Player::OnCollisionEnter(const Event* pEvent) {
		Model* model = myScene->getComponentByHandle<Model>(myScene->getComponentByHandle<Transform>(transformIDX)->m_handlesToComponents[ComponentType<Model>::id]);
		model->albedo.r = 0.0f;
		model->albedo.g = 0.0f;
		model->albedo.b = 0.1f*(F32)id;
	}

	void Player::OnCollisionExit(const Event* pEvent) {
		Model* model = myScene->getComponentByHandle<Model>(myScene->getComponentByHandle<Transform>(transformIDX)->m_handlesToComponents[ComponentType<Model>::id]);
		model->albedo.r = 0.5f;
		model->albedo.g = 0.1f*(F32)id;
		model->albedo.b = 0.f;
	}

	void Player::OnCollisionStay(const Event* pEvent) {
		Model* model = myScene->getComponentByHandle<Model>(myScene->getComponentByHandle<Transform>(transformIDX)->m_handlesToComponents[ComponentType<Model>::id]);
		model->albedo.r = 0.1f*(F32)id;;
		model->albedo.g = 0.5f;
		model->albedo.b = 0.5f;
	}

};

class Cube : public AGameObject {
	virtual void Update() override {
	}

	virtual void Start() override {
	}
};

class MainScene : public IManualSceneBlueprint {
	// Inherited via IManualSceneBlueprint
	virtual void build(Scene & scene) override
	{
		physx::PxMaterial* mat =  Engine::Get().getPhysicsSystem().getPhysics()->createMaterial(0.5, 0.5, 0.5);
		physx::PxShape* cube = Engine::Get().getPhysicsSystem().getPhysics()->createShape(physx::PxBoxGeometry(2.5, 2.5, 2.5), *mat);

		for (int i = 0; i < 10; ++i) {
			Player* p1 = (Player*)scene.addGameObject<Player>();
			scene.getComponentByHandle<Transform>(p1->transformIDX)->position = math::Vec3f(0, i*10, 0);
			scene.getComponentByHandle<Transform>(p1->transformIDX)->rotation = math::Vec4f(0, 0, 0, 0);
			scene.getComponentByHandle<Transform>(p1->transformIDX)->scale = math::Vec3f(5, 5, 5);

			scene.addComponentToGameObject<Model>(p1);
			Model* model = scene.getComponentByHandle<Model>(scene.getComponentByHandle<Transform>(p1->transformIDX)->m_handlesToComponents[ComponentType<Model>::id]);
			model->albedo.r = 0.1f*(F32)p1->id;;
			model->albedo.g = 0.5f;
			model->albedo.b = 0.5f;

			scene.addComponentToGameObject<RigidBody>(p1);
			RigidBody* rigid = scene.getComponentByHandle<RigidBody>(scene.getComponentByHandle<Transform>(p1->transformIDX)->m_handlesToComponents[ComponentType<RigidBody>::id]);
			rigid->rigidActor = Engine::Get().getPhysicsSystem().getPhysics()->createRigidDynamic(physx::PxTransform(
				scene.getComponentByHandle<Transform>(p1->transformIDX)->position.x,
				scene.getComponentByHandle<Transform>(p1->transformIDX)->position.y,
				scene.getComponentByHandle<Transform>(p1->transformIDX)->position.z));
			rigid->rigidActor->attachShape(*cube);
			std::string* a = new std::string;
			*a = std::to_string(p1->id);
			rigid->rigidActor->userData = a;
			physx::PxRigidBodyExt::updateMassAndInertia(*(physx::PxRigidDynamic*)rigid->rigidActor, 10.f);
			scene.m_pPhysXScene->addActor(*rigid->rigidActor);
		}

		Cube* ground = scene.addGameObject<Cube>();
		scene.getComponentByHandle<Transform>(ground->transformIDX)->position = math::Vec3f(0, -10, 0);
		scene.getComponentByHandle<Transform>(ground->transformIDX)->rotation = math::Vec4f(0, 0, 0, 0);
		scene.getComponentByHandle<Transform>(ground->transformIDX)->scale = math::Vec3f(100, 5, 100);
		
		scene.addComponentToGameObject<RigidBody>(ground);
		RigidBody* rigid = scene.getComponentByHandle<RigidBody>(scene.getComponentByHandle<Transform>(ground->transformIDX)->m_handlesToComponents[ComponentType<RigidBody>::id]);
		rigid->rigidActor = Engine::Get().getPhysicsSystem().getPhysics()->createRigidStatic(physx::PxTransform(scene.getComponentByHandle<Transform>(ground->transformIDX)->position.x,
			scene.getComponentByHandle<Transform>(ground->transformIDX)->position.y, scene.getComponentByHandle<Transform>(ground->transformIDX)->position.z));
		rigid->material = Engine::Get().getPhysicsSystem().getPhysics()->createMaterial(0.5, 0.5, 0.5);
		rigid->rigidActor->createShape(physx::PxBoxGeometry(scene.getComponentByHandle<Transform>(ground->transformIDX)->scale.x / 2.f,
			scene.getComponentByHandle<Transform>(ground->transformIDX)->scale.y / 2.f, scene.getComponentByHandle<Transform>(ground->transformIDX)->scale.z / 2.f), *rigid->material);
		std::string* a = new std::string;
		*a = std::to_string(ground->id);
		rigid->rigidActor->userData = a;
		scene.m_pPhysXScene->addActor(*rigid->rigidActor);

	}
};

int main(int argc, char** argv) {
	Engine::Get().startup();
	MainScene scene;
	Engine::Get().loadScene(scene);
	Engine::Get().startLoop();
	Engine::Get().shutdown();
}