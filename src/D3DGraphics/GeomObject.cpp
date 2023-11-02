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
	, animationPositionTrack(std::vector<std::pair<int, DirectX::XMFLOAT3>>())
	, animationRotateTrack(std::vector<std::pair<int, DirectX::XMFLOAT4>>())
	, anmationTM()
	, positionTrackIndex(0)
	, rotateTrackIndex(0)
	, nowTick(0)
	, oneTick(1.0f)
	, accTick(0.0f)
	, maxTick(0)
	, isAnimation(false)
{
	this->nodeTM = DirectX::XMMatrixIdentity();
	this->localTM = DirectX::XMMatrixIdentity();
	this->anmationTM = DirectX::XMMatrixIdentity();
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
		DirectX::XMMATRIX pInvers;
		pInvers = DirectX::XMMatrixInverse(nullptr, this->parent->nodeTM);

		this->localTM = this->nodeTM * pInvers;
	}
	else
	{
		localTM = nodeTM;
	}

	DirectX::XMMATRIX invers = DirectX::XMMatrixInverse(nullptr, nodeTM);
	DirectX::XMMATRIX inversTarnspose = DirectX::XMMatrixTranspose(invers);

	for (auto& m : this->meshes)
	{
		for (int i = 0; i < m->vertexList.size(); i++)
		{
			DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&m->worldVertexes[i].position);
			position.m128_f32[3] = 1.0f;
			position = DirectX::XMVector4Transform(position, invers);
			DirectX::XMStoreFloat3(&m->localVertexes[i].position, position);

			DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&m->localVertexes[i].normal);
			normal.m128_f32[3] = 1.0f;
			normal = DirectX::XMVector3Normalize(DirectX::XMVector4Transform(normal, inversTarnspose));
			DirectX::XMStoreFloat3(&m->worldVertexes[i].normal, normal);
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
	UpdateAnimation(_dt);

	if (this->parent)
	{
		this->nodeTM = this->localTM * this->parent->nodeTM * this->anmationTM;
	}
	else
	{
		this->nodeTM = this->localTM * this->anmationTM;;
	}

	DirectX::XMMATRIX invers = DirectX::XMMatrixInverse(nullptr, nodeTM);
	DirectX::XMMATRIX inversTarnspose = DirectX::XMMatrixTranspose(invers);

	for (auto& m : this->meshes)
	{
		for (int i = 0; i < m->vertexList.size(); i++)
		{
			DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&m->localVertexes[i].position);
			position.m128_f32[3] = 1.0f;
			position = DirectX::XMVector4Transform(position, nodeTM);
			DirectX::XMStoreFloat3(&m->worldVertexes[i].position, position);

			DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&m->localVertexes[i].normal);
			normal.m128_f32[3] = 1.0f;
			normal = DirectX::XMVector3Normalize(DirectX::XMVector4Transform(normal, inversTarnspose));
			DirectX::XMStoreFloat3(&m->worldVertexes[i].normal, normal);
		}
	}

	for (auto& c : this->children)
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

void GeomObject::UpdateAnimation(float _dt)
{
	this->accTick += _dt / 100;
	if (this->accTick >= this->oneTick)
	{
		this->accTick -= this->oneTick;
		this->nowTick++;
		this->isAnimation = true;
		if (this->nowTick == this->maxTick)
		{
			this->nowTick = 0;
			this->positionTrackIndex = 0;
			this->rotateTrackIndex = 0;
		}
	}

	this->anmationTM = DirectX::XMMatrixIdentity();


	if (this->animationRotateTrack.size() > 0)
	{
		if (this->rotateTrackIndex + 1 < animationRotateTrack.size())
		{
			if (this->animationRotateTrack[this->rotateTrackIndex + 1].first >= this->nowTick)
			{
				this->rotateTrackIndex++;
				if (rotateTrackIndex == animationRotateTrack.size())
				{
					this->rotateTrackIndex = this->animationRotateTrack.size() - 1;
				}
			}
		}

		const DirectX::XMFLOAT4& nowRotate = this->animationRotateTrack[this->rotateTrackIndex].second;
		DirectX::XMFLOAT3 axis = { nowRotate.x, nowRotate.y, nowRotate.z };
		DirectX::XMVECTOR axisData = DirectX::XMLoadFloat3(&axis);
		this->anmationTM *= DirectX::XMMatrixRotationAxis(axisData, nowRotate.w);
	}

// 	if (this->animationPositionTrack.size() > 0)
// 	{
// 		if (this->positionTrackIndex < animationPositionTrack.size())
// 		{
// 			if (this->animationPositionTrack[this->positionTrackIndex].first >= this->nowTick)
// 			{
// 				this->positionTrackIndex++;
// 				if (positionTrackIndex == animationPositionTrack.size())
// 				{
// 					this->positionTrackIndex = this->animationPositionTrack.size() - 1;
// 				}
// 			}
// 		}
// 		const DirectX::XMFLOAT3& nowPosition = this->animationPositionTrack[this->positionTrackIndex].second;
// 
// 		this->anmationTM *= DirectX::XMMatrixTranslation(nowPosition.x, nowPosition.y, nowPosition.z);
// 	}



}
