#include "GeomObject.h"
#include "Mesh.h"

GeomObject::GeomObject()
	: name{}
	, nodeTM{}
	, isHelper(false)
	, meshes(std::vector<Mesh*>())
	, parent(nullptr)
	, children(std::vector<GeomObject*>())
	, localTM{}
{
	this->nodeTM = DirectX::XMMatrixIdentity();
	this->localTM = DirectX::XMMatrixIdentity();
}

void GeomObject::AddMesh(Mesh* _mesh)
{
	meshes.push_back(_mesh);
}

void GeomObject::AddChild(GeomObject* _child)
{
	children.push_back(_child);
}

void GeomObject::SetParent(GeomObject* _parent)
{
	this->parent = _parent;
}

void GeomObject::Render(GraphicsEngine* _graphicsEngine)
{
	for (auto& m : this->meshes)
	{


			m->Render(_graphicsEngine);
		

	}
	for (auto& c : this->children)
	{
		c->Render(_graphicsEngine);
	}
}

void GeomObject::Initalize(GraphicsEngine* _graphicsEngine)
{
	for (auto& m : this->meshes)
	{
		m->CreatePipeline(_graphicsEngine, this->path, L" ");
	}
	for (auto& c : this->children)
	{
		c->Initalize(_graphicsEngine);
	}

	if (this->parent)
	{
		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(this->parent->nodeTM);
		DirectX::XMMATRIX invers = DirectX::XMMatrixInverse(&det, this->parent->nodeTM);

		localTM = nodeTM * invers;
	}
}

void GeomObject::Localize(GraphicsEngine* _graphicsEngine, DirectX::XMMATRIX _parent)
{
	for (auto& m : this->meshes)
	{
		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(nodeTM);
		DirectX::XMMATRIX invers = DirectX::XMMatrixInverse(&det, nodeTM);
		invers.r[3].m128_f32[3] = 1.0f;
		for (int i = 0; i < m->vertexList.size(); i++)
		{
			DirectX::XMFLOAT4 temp;
			temp.x = m->vertexes[i].position.x;
			temp.y = m->vertexes[i].position.y;
			temp.z = m->vertexes[i].position.z;
			temp.w = 1.0f;
			DirectX::XMVECTOR positoin = DirectX::XMLoadFloat4(&temp);
			positoin = DirectX::XMVector4Transform(positoin, invers);
			DirectX::XMStoreFloat3(&m->vertexes[i].position, positoin);
			m->vertexes[i].position;
		}
	}

	for (auto& c : children)
	{
		c->Localize(_graphicsEngine, nodeTM);
	}
}
