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
	DirectX::XMVECTOR pDet;
	DirectX::XMMATRIX pInvers;
	if (this->parent)
	{
		pDet = DirectX::XMMatrixDeterminant(this->parent->nodeTM);
		pInvers = DirectX::XMMatrixInverse(&pDet, this->parent->nodeTM);

		this->localTM = this->nodeTM * pInvers;
	}
	else
	{
		localTM = nodeTM;
		pInvers = DirectX::XMMatrixIdentity();
	}

	DirectX::XMMATRIX invers = DirectX::XMMatrixInverse(nullptr, nodeTM);
	DirectX::XMMATRIX inversTarnspose = DirectX::XMMatrixTranspose(invers);

	DirectX::XMMATRIX inversTarnsposeInvers = DirectX::XMMatrixInverse(nullptr, inversTarnspose);
// 	invers.r[3].m128_f32[3] = 1.0f;
// 	tarnspose.r[3].m128_f32[3] = 1.0f;
	for (auto& m : this->meshes)
	{
		for (int i = 0; i < m->vertexList.size(); i++)
		{
			DirectX::XMFLOAT4 meshWorldPosition;
			meshWorldPosition.x = m->worldVertexes[i].position.x;
			meshWorldPosition.y = m->worldVertexes[i].position.y;
			meshWorldPosition.z = m->worldVertexes[i].position.z;
			meshWorldPosition.w = 1.0f;

			DirectX::XMVECTOR positoin = DirectX::XMLoadFloat4(&meshWorldPosition);
			positoin = DirectX::XMVector4Transform(positoin, invers);
			DirectX::XMStoreFloat3(&m->localVertexes[i].position, positoin);


			DirectX::XMFLOAT4 meshWorldNoraml;
			meshWorldNoraml.x = m->worldVertexes[i].normal.x;
			meshWorldNoraml.y = m->worldVertexes[i].normal.y;
			meshWorldNoraml.z = m->worldVertexes[i].normal.z;
			meshWorldNoraml.w = 1.0f;

			DirectX::XMVECTOR normal = DirectX::XMLoadFloat4(&meshWorldNoraml);
			normal = DirectX::XMVector3Normalize(DirectX::XMVector4Transform(normal, inversTarnsposeInvers));
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

void GeomObject::Update(float _dt)
{
	if (this->parent) 
	{
		this->nodeTM = this->localTM * this->parent->nodeTM;
	}
	else
	{
		this->nodeTM = this->localTM;
	}
	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(nodeTM);
	DirectX::XMMATRIX invers = DirectX::XMMatrixInverse(&det, nodeTM);
	DirectX::XMMATRIX inversTarnspose = DirectX::XMMatrixTranspose(invers);

	for (auto& m : this->meshes)
	{
		for (int i = 0; i < m->vertexList.size(); i++)
		{
			DirectX::XMFLOAT4 meshLocalPosition;
			meshLocalPosition.x = m->localVertexes[i].position.x;
			meshLocalPosition.y = m->localVertexes[i].position.y;
			meshLocalPosition.z = m->localVertexes[i].position.z;
			meshLocalPosition.w = 1.0f;

			DirectX::XMFLOAT4 meshLocalNoraml;
			meshLocalNoraml.x = m->localVertexes[i].normal.x;
			meshLocalNoraml.y = m->localVertexes[i].normal.y;
			meshLocalNoraml.z = m->localVertexes[i].normal.z;
			meshLocalNoraml.w = 1.0f;

			DirectX::XMVECTOR positoin = DirectX::XMLoadFloat4(&meshLocalPosition);
			positoin = DirectX::XMVector4Transform(positoin, nodeTM);
			DirectX::XMStoreFloat3(&m->worldVertexes[i].position, positoin);

			DirectX::XMVECTOR normal = DirectX::XMLoadFloat4(&meshLocalNoraml);
			normal = DirectX::XMVector3Normalize(DirectX::XMVector4Transform(normal, inversTarnspose));
			DirectX::XMStoreFloat3(&m->worldVertexes[i].normal, normal);
		}
	}

	for(auto& c : this->children) 
	{
		c->Update(_dt);
	}

}

void GeomObject::Translate(float _x, float _y, float _z)
{
	this->localTM *= DirectX::XMMatrixTranslation(_x, _y, _z);
}

void GeomObject::RoateBaseAxis(float _x, float _y, float _z)
{
	this->localTM *= DirectX::XMMatrixRotationX(_x);
	this->localTM *= DirectX::XMMatrixRotationY(_y);
	this->localTM *= DirectX::XMMatrixRotationZ(_z);
}

void GeomObject::Scale(float _x, float _y, float _z)
{

}
