#include <PrecompiledHeader/pch.hpp>
#include <ResourceManager/Manager/SkinnedMeshManager.hpp>
#include <Converter/ResourceDefinitions.hpp>
#include <Animation/Skeleton/Skeleton.hpp>

namespace drak {

using namespace definition;

void SkinnedMeshManager::preload(const std::string& names, const std::string& filename) {
	Base::preload(names, filename);
}

void SkinnedMeshManager::preload(const ResourceName& rName, const std::string& filename) {
	for (auto name : rName.names) {
		if ((name.second & EFileType::SKINNEDMESH) == EFileType::SKINNEDMESH)
			m_map[name.first] = SkinnedMeshPtr(new Resource<geom::SkinnedMesh>(filename));
	}
}

void SkinnedMeshManager::load(const std::string& filename, std::vector<SkinnedMesh>& inSkinnedMeshes) {
	for (auto& skMesh : inSkinnedMeshes) {
		if (!m_map[skMesh.name].get())
			m_map[skMesh.name].reset(new Resource<geom::SkinnedMesh>(filename));
		animation::Skeleton skeleton;
		for (auto& bone : skMesh.skeleton.bones)
			skeleton.m_boneList[bone.first] = *reinterpret_cast<animation::Bone*>(&bone.second);
		skeleton.m_animList =
			*reinterpret_cast<std::map<std::string, animation::Animation>*>(&skMesh.skeleton.animations);
		skeleton.m_base = skMesh.skeleton.base;
		skeleton.m_invGlobalPos = skMesh.skeleton.invGlobalPos;
		new (m_map[skMesh.name].get())
			Resource<geom::SkinnedMesh>(filename, geom::SkinnedMesh(skMesh.name,
				std::move(*reinterpret_cast<std::vector<geom::Vertex1P1N1UV1B1W>*>(&skMesh.vertices)),
				std::move(skMesh.indices),
				std::move(skeleton)));

		m_map[skMesh.name]->loadState(Resource<geom::SkinnedMesh>::ELoadState::READY);
	}
}

void SkinnedMeshManager::reload(std::vector<SkinnedMesh>& inSkinnedMeshes) {
	for (auto& skMesh : inSkinnedMeshes) {
		m_map[skMesh.name]->loadState(Resource<geom::SkinnedMesh>::ELoadState::LOADING);
		new (&m_map[skMesh.name]->m_resource)
			geom::SkinnedMesh(skMesh.name,
				std::move(*reinterpret_cast<std::vector<geom::Vertex1P1N1UV1B1W>*>(&skMesh.vertices)),
				std::move(skMesh.indices),
				std::move(*reinterpret_cast<animation::Skeleton*>(&skMesh.skeleton)));
		m_map[skMesh.name]->loadState(Resource<geom::SkinnedMesh>::ELoadState::READY);
	}
}

}