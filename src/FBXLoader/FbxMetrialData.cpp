#include "FbxMetrialData.h"

FbxMetrialData::FbxMetrialData()
	: ambientFile("")
	, emissiveFile("")
	, diffuseFile("")
	, specularFile("")
	, normalMapFile("")
	, index(0)
{

}

FbxMetrialData::FbxMetrialData(UINT _index, FbxSurfaceMaterial* _matData)
	: index(_index)
{
	FbxProperty	prop = _matData->FindProperty(FbxSurfaceMaterial::sDiffuse);
	int layerMatCount = prop.GetSrcObjectCount<FbxLayeredTexture>();
	if (layerMatCount > 0)
	{
		for (int j = 0; j < layerMatCount; j++)
		{
			FbxLayeredTexture* layered_texture = prop.GetSrcObject<FbxLayeredTexture>(j);
			int lcount = layered_texture->GetSrcObjectCount<FbxFileTexture>();
			for (int k = 0; k < lcount; k++)
			{
				FbxFileTexture* texture = FbxCast<FbxFileTexture>(layered_texture->GetSrcObject<FbxFileTexture>(k));
				std::string texture_name = texture->GetRelativeFileName();
				this->diffuseFile = texture_name;
			}
		}
	}
	else
	{
		int texCount = prop.GetSrcObjectCount<FbxFileTexture>();
		for (int j = 0; j < texCount; j++)
		{
			const FbxFileTexture* texture = FbxCast<FbxFileTexture>(prop.GetSrcObject<FbxFileTexture>(j));
			// Then, you can get all the properties of the texture, include its name
			std::string texture_name = texture->GetRelativeFileName();
			this->diffuseFile = texture_name;
		}
	}
}

FbxMetrialData::~FbxMetrialData()
{

}
