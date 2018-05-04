#pragma once
#include <Engine\Components\ABehavior.hpp>
#include <Engine/Physics/SimulationEvent.hpp>

namespace drak {
namespace behavior {
class Cube : public ABehaviorVariables {
DK_SERIALIZED_OBJECT(Cube)
	function::MemberFunction<Cube, void, const events::Event*> updateBind;
	function::MemberFunction<Cube, void, const events::Event*> startBind;
	void bindUpdateToEngine();
	void bindStartToEngine();
public:
	Cube();
	~Cube();
	void init();
	void update(const events::Event* pEvent);
	void start(const events::Event* pEvent);

	void OnCollisionEnter(const events::Event* pEvent);
	void OnCollisionExit(const events::Event* pEvent);
	void OnCollisionStay(const events::Event* pEvent);
	template <typename T>
	T& getComponent() {
		return CurrentLevel.getGameObjects()[gameObjectID].getComponent<T>();
	}
};
BHVR_TYPE(Cube)
}
DK_METADATA_BEGIN(behavior::Cube)
DK_PUBLIC_FIELDS(gameObjectID, name)
DK_PUBLIC_FIELD_COMPLEMENT
DK_METADATA_END
}