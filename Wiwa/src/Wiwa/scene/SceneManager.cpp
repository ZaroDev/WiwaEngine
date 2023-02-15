#include <wipch.h>
#include "SceneManager.h"
#include <Wiwa/ecs/systems/MeshRenderer.h>
#include <Wiwa/events/Event.h>
#include <Wiwa/events/ApplicationEvent.h>
#include <Wiwa/audio/Audio.h>

namespace Wiwa {
	std::vector<Scene*> SceneManager::m_Scenes;
	SceneId SceneManager::m_ActiveScene = -1;
	bool SceneManager::m_PlayScene = true;

	std::vector<SceneId> SceneManager::m_RemovedSceneIds;

	void SceneManager::Awake()
	{
		m_Scenes[m_ActiveScene]->Awake();
	}

	void SceneManager::Init()
	{
		m_Scenes[m_ActiveScene]->Init();
	}

	void SceneManager::Update()
	{
		m_Scenes[m_ActiveScene]->Update();
	}

	void SceneManager::ModuleUpdate()
	{
		m_Scenes[m_ActiveScene]->ModuleUpdate();

		if (m_PlayScene) {
			Update();
		}
	}

	void SceneManager::CleanUp()
	{
		size_t scsize = m_Scenes.size();

		for (size_t i = 0; i < scsize; i++) {
			delete m_Scenes[i];
		}
	}

	SceneId SceneManager::CreateScene()
	{
		Scene* sc = new Scene();

		SceneId scene_id;

		if (m_RemovedSceneIds.size() > 0) {
			scene_id = m_RemovedSceneIds[m_RemovedSceneIds.size() - 1];
			m_RemovedSceneIds.pop_back();

			m_Scenes[scene_id] = sc;
		}
		else {
			scene_id = m_Scenes.size();
			m_Scenes.push_back(sc);
		}

		return scene_id;
	}

	void SceneManager::LoadEntity(File& scene_file, EntityId parent, EntityManager& em, bool is_parent)
	{
		size_t e_name_len;
		char* e_name_c;
		std::string e_name;

		// Read entity name
		scene_file.Read(&e_name_len, sizeof(size_t));
		e_name_c = new char[e_name_len];
		scene_file.Read(e_name_c, e_name_len);
		e_name = e_name_c;
		delete[] e_name_c;

		EntityId eid;

		if (is_parent) {
			eid = em.CreateEntity(e_name.c_str());
		}
		else {
			eid = em.CreateEntity(e_name.c_str(), parent);
		}

		size_t component_size;

		// Read component count
		scene_file.Read(&component_size, sizeof(size_t));

		// For each component in entity
		for (size_t i = 0; i < component_size; i++) {
			ComponentHash c_hash;
			size_t c_size;
			byte* data;

			// Read component hash, size and data
			scene_file.Read(&c_hash, sizeof(size_t));
			scene_file.Read(&c_size, sizeof(size_t));
			data = new byte[c_size];
			scene_file.Read(data, c_size);

			byte* component = em.AddComponent(eid, c_hash, data);
			delete[] data;

			size_t mesh_hash = em.GetComponentType(em.GetComponentId<Mesh>())->hash;

			if (c_hash == mesh_hash) {
				Mesh* mesh = (Mesh*)component;
				
				size_t meshpath_size = strlen(mesh->mesh_path);
				if(meshpath_size > 0) mesh->meshId = Resources::Load<Model>(mesh->mesh_path);

				size_t matpath_size = strlen(mesh->mat_path);
				if(matpath_size > 0) mesh->materialId = Resources::Load<Material>(mesh->mat_path);
			}
		}

		size_t system_count;
		
		// Read system count
		scene_file.Read(&system_count, sizeof(size_t));
		
		if (system_count > 0) {
			std::vector<SystemHash> system_hashes;
			system_hashes.resize(system_count);

			// Read system hashes
			scene_file.Read(&system_hashes[0], system_count * sizeof(SystemHash));

			for (size_t i = 0; i < system_count; i++) {
				em.ApplySystem(eid, system_hashes[i]);
			}
		}

		// Check for child entities
		size_t children_size;

		// Read children size (size >= 0)
		scene_file.Read(&children_size, sizeof(size_t));

		for (size_t i = 0; i < children_size; i++) {
			LoadEntity(scene_file, eid, em, false);
		}
	}

	void SceneManager::SaveEntity(File& scene_file, EntityId eid, EntityManager& em)
	{
		const char* e_name = em.GetEntityName(eid);
		size_t e_name_len = strlen(e_name) + 1;

		// Save entity name
		scene_file.Write(&e_name_len, sizeof(size_t));
		scene_file.Write(e_name, e_name_len);

		std::map<ComponentId, size_t>& components = em.GetEntityComponents(eid);
		std::map<ComponentId, size_t>::iterator c_it;
		size_t component_size = components.size();

		// Save component count for this entity
		scene_file.Write(&component_size, sizeof(size_t));

		// For each component in entity
		for (c_it = components.begin(); c_it != components.end(); c_it++) {
			ComponentId c_id = c_it->first;
			const Type* c_type = em.GetComponentType(c_id);
			size_t c_size = c_type->size;
			byte* c_data = em.GetComponent(eid, c_id, c_size);

			// Save component hash, size and data
			scene_file.Write(&c_type->hash, sizeof(size_t));
			scene_file.Write(&c_size, sizeof(size_t));
			scene_file.Write(c_data, c_size);
		}

		// Save entity systems
		const std::vector<SystemHash>& system_list = em.GetEntitySystemHashes(eid);
		size_t system_count = system_list.size();

		// Save system count
		scene_file.Write(&system_count, sizeof(size_t));

		if (system_count > 0) {
			// Save system hashes
			scene_file.Write(system_list.data(), system_count * sizeof(SystemHash));
		}

		// Check for child entities
		std::vector<EntityId>* children = em.GetEntityChildren(eid);
		size_t children_size = children->size();

		// Save children size (size >= 0)
		scene_file.Write(&children_size, sizeof(size_t));
		
		// Recursively save each child entity
		for (size_t i = 0; i < children_size; i++) {
			SaveEntity(scene_file, children->at(i), em);
		}
	}

	bool SceneManager::_loadSceneImpl(Scene* scene, File& scene_file)
	{
		// Load cameras
		CameraManager& cm = scene->GetCameraManager();
		size_t camera_count;

		// Read camera count
		scene_file.Read(&camera_count, sizeof(size_t));

		for (size_t i = 0; i < camera_count; i++) {
			Wiwa::Camera::CameraType camtype;
			float fov;
			float ar;
			float nplane;
			float fplane;
			Vector3f pos;
			glm::vec3 rot;
			bool is_active;

			// Read camera active
			scene_file.Read(&is_active, 1);

			// Read camera type
			scene_file.Read(&camtype, sizeof(Wiwa::Camera::CameraType));

			// Read camera FOV
			scene_file.Read(&fov, sizeof(float));

			// Read camera aspect ratio
			scene_file.Read(&ar, sizeof(float));

			// Write camera near plane distance
			scene_file.Read(&nplane, sizeof(float));

			// Write camera far plane distance
			scene_file.Read(&fplane, sizeof(float));

			CameraId cam_id = -1;

			if (camtype == Wiwa::Camera::CameraType::PERSPECTIVE) {
				cam_id = cm.CreatePerspectiveCamera(fov, ar, nplane, fplane);
			}
			else {
				// TODO: LOAD ORTHOGRAPHIC
			}

			if (is_active) cm.setActiveCamera(cam_id);

			Camera* camera = cm.getCamera(cam_id);

			// Read camera cull
			scene_file.Read(&camera->cull, 1);

			// Read camera draw BB
			scene_file.Read(&camera->drawBoundingBoxes, 1);

			// Read camera draw frustums
			scene_file.Read(&camera->drawFrustrums, 1);

			// Read camera position
			scene_file.Read(&pos, sizeof(Vector3f));

			// Read camera rotation
			scene_file.Read(glm::value_ptr(rot), sizeof(glm::vec3));

			camera->setPosition(pos);
			camera->setRotation(rot);
		}

		// Load audio
		bool loaded_audio;

		// Save if audio loaded
		scene_file.Read(&loaded_audio, sizeof(bool));

		if (loaded_audio) {
			size_t n_len;
			std::string project_path;

			scene_file.Read(&n_len, sizeof(size_t));

			project_path.resize(n_len - 1);

			scene_file.Read(&project_path[0], n_len);

			Audio::LoadProject(project_path.c_str());

			size_t bank_size;
			std::string bank_path;

			// Save bank size
			scene_file.Read(&bank_size, sizeof(size_t));

			if (bank_size > 0) {
				for (size_t i = 0; i < bank_size; i++) {
					size_t n_size;

					scene_file.Read(&n_size, sizeof(size_t));

					bank_path.resize(n_size - 1);

					scene_file.Read(&bank_path[0], n_size);

					Audio::LoadBank(bank_path.c_str());
				}
			}

			size_t event_size;
			std::string event_path;

			// Save event size
			scene_file.Read(&event_size, sizeof(size_t));

			if (event_size > 0) {
				for (size_t i = 0; i < event_size; i++) {
					size_t n_size;
					scene_file.Read(&n_size, sizeof(size_t));

					event_path.resize(n_size - 1);

					scene_file.Read(&event_path[0], n_size);

					Audio::LoadEvent(event_path.c_str());
				}
			}
		}

		// Load entities
		EntityManager& em = scene->GetEntityManager();

		size_t entity_count;
		size_t p_entity_count;

		// Read total entity count
		scene_file.Read(&entity_count, sizeof(size_t));
		// Read parent entity count
		scene_file.Read(&p_entity_count, sizeof(size_t));

		em.ReserveEntities(entity_count);

		for (size_t i = 0; i < p_entity_count; i++) {
			LoadEntity(scene_file, i, em, true);
		}

		return true;
	}

	void SceneManager::SaveScene(SceneId scene_id, const char* scene_path)
	{
		File scene_file = FileSystem::Open(scene_path, FileSystem::OM_OUT | FileSystem::OM_BINARY);

		if (scene_file.IsOpen()) {
			Scene* sc = m_Scenes[scene_id];
			
			// Save scene data
			// TODO: Save scene info??
			// Scene name, etc

			// Save cameras
			CameraManager& cm = sc->GetCameraManager();
			std::vector<CameraId>& cameras = cm.getCameras();
			size_t camera_count = cameras.size();
			
			// Save camera count
			scene_file.Write(&camera_count, sizeof(size_t));

			CameraId active_cam = cm.getActiveCameraId();

			for (size_t i = 0; i < camera_count; i++) {
				CameraId cam_id = cameras[i];
				Camera* camera = cm.getCamera(cam_id);

				Wiwa::Camera::CameraType camtype = camera->GetCameraType();
				float fov = camera->getFOV();
				float ar = camera->getAspectRatio();
				float nplane = camera->getNear();
				float fplane = camera->getFar();
				glm::vec3 pos = camera->getPosition();
				glm::vec3 rot = camera->getRotation();

				bool is_active = cam_id == active_cam;

				// Write camera active
				scene_file.Write(&is_active, 1);

				// Write camera type
				scene_file.Write(&camtype, sizeof(Wiwa::Camera::CameraType));

				// Write camera FOV
				scene_file.Write(&fov, sizeof(float));

				// Write camera aspect ratio
				scene_file.Write(&ar, sizeof(float));

				// Write camera near plane distance
				scene_file.Write(&nplane, sizeof(float));

				// Write camera far plane distance
				scene_file.Write(&fplane, sizeof(float));

				// Write camera cull
				scene_file.Write(&camera->cull, 1);

				// Write camera draw BB
				scene_file.Write(&camera->drawBoundingBoxes, 1);

				// Write camera draw frustums
				scene_file.Write(&camera->drawFrustrums, 1);

				// Write camera position
				scene_file.Write(glm::value_ptr(pos), sizeof(glm::vec3));

				// Write camera rotation
				scene_file.Write(glm::value_ptr(rot), sizeof(glm::vec3));
			}

			// Save audio
			bool loaded_audio = Audio::LoadedProject();

			// Save if audio loaded
			scene_file.Write(&loaded_audio, sizeof(bool));

			if (loaded_audio) {
				// Save project path
				std::string project_path = Audio::GetProjectPath();

				size_t n_len = project_path.size() + 1;

				scene_file.Write(&n_len, sizeof(size_t));
				scene_file.Write(project_path.data(), n_len);

				const std::vector<Audio::BankData>& bank_list = Audio::GetLoadedBankList();

				size_t bank_size = bank_list.size();

				// Save bank size
				scene_file.Write(&bank_size, sizeof(size_t));

				if (bank_size > 0) {
					for (size_t i = 0; i < bank_size; i++) {
						size_t n_size = bank_list[i].path.size() + 1;

						scene_file.Write(&n_size, sizeof(size_t));
						scene_file.Write(bank_list[i].path.data(), n_size);
					}
				}

				const std::vector<Audio::EventData>& event_list = Audio::GetLoadedEventList();

				size_t event_size = event_list.size();

				// Save event size
				scene_file.Write(&event_size, sizeof(size_t));

				if (event_size > 0) {
					for (size_t i = 0; i < event_size; i++) {
						size_t n_size = event_list[i].name.size() + 1;

						scene_file.Write(&n_size, sizeof(size_t));
						scene_file.Write(event_list[i].name.data(), n_size);
					}
				}
			}
			
			// Save entities
			EntityManager& em = sc->GetEntityManager();
			std::vector<EntityId>* pentities = em.GetParentEntitiesAlive();
			size_t p_entity_count = pentities->size();

			size_t entity_count = em.GetEntityCount();

			// Save total entity count
			scene_file.Write(&entity_count, sizeof(size_t));
			// Save parent entity count
			scene_file.Write(&p_entity_count, sizeof(size_t));

			// Save each entity
			for (size_t i = 0; i < p_entity_count; i++) {
				EntityId eid = pentities->at(i);
				SaveEntity(scene_file, eid, em);
			}

			WI_CORE_INFO("Saved scene in file \"{0}\" successfully!", scene_path);
		}
		else {
			WI_CORE_WARN("Couldn't open file \"{0}\"", scene_path);
		}

		
		scene_file.Close();
	}

	SceneId SceneManager::LoadScene(const char* scene_path, int flags)
	{
		if (!FileSystem::Exists(scene_path)) return -1;

		SceneId sceneid = -1;

		if (flags & UNLOAD_CURRENT) {
			UnloadScene(m_ActiveScene, flags & UNLOAD_RESOURCES);
		}

		if (flags & LOAD_SEPARATE) {
			sceneid = CreateScene();
		}
		else if (flags & LOAD_APPEND) {
			sceneid = m_ActiveScene;
		}
		
		File scene_file = FileSystem::Open(scene_path, FileSystem::OM_IN | FileSystem::OM_BINARY);
		if (scene_file.IsOpen()) {
			Scene* sc = m_Scenes[sceneid];

			_loadSceneImpl(sc, scene_file);

			std::filesystem::path path = scene_path;
			sc->ChangeName(path.filename().stem().string().c_str());

			if (flags & LOAD_SEPARATE) {
				SetScene(sceneid);
			}
			
			WI_CORE_INFO("Loaded scene in file \"{0}\" successfully!", scene_path);
		}
		else {
			WI_CORE_WARN("Couldn't open file \"{0}\".", scene_path);
		}

		scene_file.Close();

		return sceneid;
	}

	void SceneManager::UnloadScene(SceneId scene_id, bool unload_resources)
	{
		if (!m_Scenes[scene_id]) return;

		m_Scenes[scene_id]->Unload(unload_resources);
		delete m_Scenes[scene_id];
		m_Scenes[scene_id] = NULL;

		m_RemovedSceneIds.push_back(scene_id);
	}

	void SceneManager::SetScene(SceneId sceneId) {
		m_ActiveScene = sceneId;

		SceneChangeEvent event(sceneId);
		Action<Wiwa::Event&> act = { &Application::OnEvent, &Application::Get() };
		act(event);
	}

	void SceneManager::ChangeScene(SceneId sceneId)
	{
		SceneChangeEvent event(sceneId);
		Action<Wiwa::Event&> act = { &Application::OnEvent, &Application::Get()};
		act(event);
		m_Scenes[m_ActiveScene]->Unload();
		m_ActiveScene = sceneId;
		m_Scenes[m_ActiveScene]->Load();
	}

	void SceneManager::StartChangeScene(SceneId sceneId)
	{
		m_Scenes[m_ActiveScene]->ChangeScene(sceneId);
	}
}