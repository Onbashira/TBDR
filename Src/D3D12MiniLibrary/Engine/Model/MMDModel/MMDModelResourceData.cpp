#include "MMDModelResourceData.h"



K3D12::MMDModelResourceData::MMDModelResourceData()
{


}


K3D12::MMDModelResourceData::~MMDModelResourceData()
{
	_vertexes.clear();
	_indexList.list.clear();
	_textureHeaps.texturesHeap.reset();
	_materials.clear();
	_boneTree.reset();
	_texturePaths.clear();
}
