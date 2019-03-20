#pragma once
#include <memory>
#include "../GameObject/GameObject.h"
#include "../../Util/Math.h"

namespace K3D12 {

	class PipelineStateObject;
	class GraphicsCommandList;
	class LightManager;
	class Resource;

	struct LightInfo {
		Vector3 color;
		Vector3 pos;
		Vector3 direction;
		float range;
		float attenuation;
		float intensity;
		int type; // 0 = point,1 = Direction,2 = spot

		LightInfo() :
			color(Vector3(1.0f, 1.0f, 1.0f)), pos(Vector3(0.0f, 0.0f, 10.0f)), direction(Vector3(0.0f, -1.0f, 0.0f))
			, range(10.0f), attenuation(0.98f), intensity(1.0f), type(0)
		{


		};
		~LightInfo() {
		};

	};



	class Light :
		public GameObject
	{
		friend class LightManager;
	private:
		LightInfo _info;
		//ÉNÉâÉXÉ^î‘çÜ
		unsigned int _clusterIndex;
	public:

	private:

	public:
		void		     Update()override;
		void			 SetClusterIndex(unsigned int clusterIndex);
		unsigned int	 GetClusterIndex();
		const LightInfo& GetInfo();
		void			 SetInfo(LightInfo& info);
		void			 WriteToResourceOffset(Resource* resource, unsigned int offset);
		void			 WriteToResourceIndex(Resource* resource, unsigned int index);
		void			 WriteToResource256ByteAlignment(Resource* resource, unsigned int offset);

		Light(LightInfo lightInfo);
		Light();
		~Light();
	};

}