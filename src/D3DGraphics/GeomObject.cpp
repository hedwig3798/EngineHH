#include "GeomObject.h"
#include "Mesh.h"

GeomObject::GeomObject()
	: name{}
	, parentname{}
	, nodeTM{}
	, inheritTM{}
	, isHelper(false)
	, meshes(std::vector<Mesh*>())
{

}

void GeomObject::AddMesh(Mesh* _mesh)
{
	meshes.push_back(_mesh);
}

void GeomObject::Render(GraphicsEngine* _graphicsEngine)
{
	for(auto& m : this->meshes) 
	{
		m->Render(_graphicsEngine);
	}
}

void GeomObject::Initalize(GraphicsEngine* _graphicsEngine)
{
	for (auto& m : this->meshes)
	{
		m->CreatePipeline(_graphicsEngine, this->path, L" ");
	}
}
