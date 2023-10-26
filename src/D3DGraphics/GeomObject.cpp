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

	this->Localize(_graphicsEngine);

	for (auto& c : this->children)
	{
		c->Initalize(_graphicsEngine);
	}
}

void GeomObject::Localize(GraphicsEngine* _graphicsEngine)
{
	if (this->parent)
	{
		DirectX::XMVECTOR pDet = DirectX::XMMatrixDeterminant(this->parent->nodeTM);
		DirectX::XMMATRIX pInvers = DirectX::XMMatrixInverse(&pDet, this->parent->nodeTM);

		localTM = nodeTM * pInvers;
	}
	else
	{
		localTM = nodeTM;
	}

	for (auto& m : this->meshes)
	{
		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(nodeTM);
		DirectX::XMMATRIX invers = DirectX::XMMatrixInverse(&det, nodeTM);
		DirectX::XMMATRIX tarnspose = DirectX::XMMatrixTranspose(nodeTM);
		invers.r[3].m128_f32[3] = 1.0f;
		tarnspose.r[3].m128_f32[3] = 1.0f;
		for (int i = 0; i < m->vertexList.size(); i++)
		{
			DirectX::XMFLOAT4 meshWorldPosition;
			meshWorldPosition.x = m->worldVertexes[i].position.x;
			meshWorldPosition.y = m->worldVertexes[i].position.y;
			meshWorldPosition.z = m->worldVertexes[i].position.z;
			meshWorldPosition.w = 1.0f;

			DirectX::XMFLOAT4 meshWorldNoraml;
			meshWorldNoraml.x = m->worldVertexes[i].normal.x;
			meshWorldNoraml.y = m->worldVertexes[i].normal.y;
			meshWorldNoraml.z = m->worldVertexes[i].normal.z;
			meshWorldNoraml.w = 1.0f;

			DirectX::XMVECTOR positoin = DirectX::XMLoadFloat4(&meshWorldPosition);
			positoin = DirectX::XMVector4Transform(positoin, invers);
			DirectX::XMStoreFloat3(&m->localVertexes[i].position, positoin);

			DirectX::XMVECTOR normal = DirectX::XMLoadFloat4(&meshWorldNoraml);
			normal = DirectX::XMVector3Normalize(DirectX::XMVector4Transform(normal, tarnspose));
			DirectX::XMStoreFloat3(&m->localVertexes[i].normal, normal);
		}
	}
}

void GeomObject::SetLocal(bool _isLocal)
{
	for (auto& m : this->meshes)
	{
		m->isLocal = _isLocal;
	}
	for (auto& c : this->children)
	{
		c->SetLocal(_isLocal);
	}
}

void GeomObject::Roate()
{

}
