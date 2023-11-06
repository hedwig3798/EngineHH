#include "GeomObject.h"
#include "Mesh.h"

GeomObject::GeomObject()
	: name{}
	, nodeTM(DirectX::XMMatrixIdentity())
	, isHelper(false)
	, meshes(std::vector<Mesh*>())
	, parent(nullptr)
	, children(std::vector<GeomObject*>())
	, localTM(DirectX::XMMatrixIdentity())
	, animationPositionTrack(std::vector<std::pair<int, DirectX::XMFLOAT3>>())
	, animationRotateTrack(std::vector<std::pair<int, DirectX::XMFLOAT4>>())
	, positionTrackIndex(0)
	, rotateTrackIndex(0)
	, nowTick(0)
	, oneTick(0.0001f * 10)
	, accTick(0.0f)
	, maxTick(0)
	, isAnimation(false)
	, localScale{}
	, localRotate{}
	, localPosition{}
	, animationTM(DirectX::XMMatrixIdentity())
	, nodePosition{}
	, nodeRotate{}
	, nodeScale{}
	, isNegative(false)
{
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
	assert(DirectX::XMMatrixDecompose(&this->nodeScale, &this->nodeRotate, &this->nodePosition, this->nodeTM) && "cannot decompose node Tm");

	DirectX::XMMATRIX nodeTemp = this->nodeTM;

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

	assert(DirectX::XMMatrixDecompose(&this->localScale, &this->localRotate, &this->localPosition, this->localTM) && "cannot decompose local Tm");

	for (size_t i = 0; i < this->animationRotateTrack.size(); i++)
	{
		DirectX::XMFLOAT4& data = this->animationRotateTrack[i].second;

		data.x = (float)sin(data.w / 2.0f) * data.x;
		data.y = (float)sin(data.w / 2.0f) * data.y;
		data.z = (float)sin(data.w / 2.0f) * data.z;
		data.w = (float)cos(data.w / 2.0f);

		if (i != 0)
		{
			DirectX::XMVECTOR dataVector = DirectX::XMLoadFloat4(&data);
			DirectX::XMVECTOR prevVector = DirectX::XMLoadFloat4(&this->animationRotateTrack[i - 1].second);
			dataVector = DirectX::XMQuaternionMultiply(prevVector, dataVector);
			DirectX::XMStoreFloat4(&data, dataVector);
		}
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
		this->nodeTM = this->localTM * this->parent->nodeTM;
	}
	else
	{
		this->nodeTM = this->localTM;
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
	this->accTick += _dt * 1;
	while (this->accTick >= this->oneTick)
	{
		this->accTick -= this->oneTick;
		this->nowTick++;
		if (this->nowTick > this->maxTick)
		{
			this->nowTick = 0;
			this->positionTrackIndex = 0;
			this->rotateTrackIndex = 0;
		}
	}

	this->localTM = DirectX::XMMatrixIdentity();
	this->localTM *= DirectX::XMMatrixScalingFromVector(this->localScale);

// 	if (!this->animationRotateTrack.empty())
// 	{
// 		if (this->rotateTrackIndex + 1 < (int)animationRotateTrack.size())
// 		{
// 			if (this->animationRotateTrack[this->rotateTrackIndex + 1].first <= this->nowTick)
// 			{
// 				this->rotateTrackIndex++;
// 				if (rotateTrackIndex == animationRotateTrack.size())
// 				{
// 					this->rotateTrackIndex = (int)this->animationRotateTrack.size() - 1;
// 				}
// 			}
// 		}
// 
// 		const DirectX::XMFLOAT4& nowRotate = this->animationRotateTrack[this->rotateTrackIndex].second;
// 		DirectX::XMVECTOR rotateNowVector = DirectX::XMLoadFloat4(&nowRotate);
// 
// 		if (this->rotateTrackIndex != (int)this->animationRotateTrack.size() - 1)
// 		{
// 			const DirectX::XMFLOAT4& nextRotate = this->animationRotateTrack[this->rotateTrackIndex + 1].second;
// 			DirectX::XMVECTOR rotateNextVector = DirectX::XMLoadFloat4(&nextRotate);
// 			int t1 = this->animationRotateTrack[this->rotateTrackIndex].first;
// 			int t2 = this->animationRotateTrack[this->rotateTrackIndex + 1].first;
// 
// 			rotateNowVector = DirectX::XMQuaternionSlerp(rotateNowVector, rotateNextVector, (float)nowTick / (float)(t1 + t2));
// 		}
// 
// 		this->localTM *= DirectX::XMMatrixRotationQuaternion(rotateNowVector);
// 	}
// 	else
// 	{
// 		this->localTM *= DirectX::XMMatrixRotationQuaternion(this->localRotate);
// 	}

	if (this->animationPositionTrack.size() > 0)
	{
		if (this->positionTrackIndex < animationPositionTrack.size())
		{
			if (this->animationPositionTrack[this->positionTrackIndex].first <= this->nowTick)
			{
				this->positionTrackIndex++;
				if (positionTrackIndex == animationPositionTrack.size())
				{
					this->positionTrackIndex = (int)this->animationPositionTrack.size() - 1;
				}
			}
		}
		DirectX::XMFLOAT3 nowPosition = this->animationPositionTrack[this->positionTrackIndex].second;

		if (this->positionTrackIndex != 0)
		{
			const DirectX::XMFLOAT3 prevPosition = this->animationPositionTrack[this->positionTrackIndex - 1].second;
			float t1 = this->animationPositionTrack[this->positionTrackIndex - 1].first;
			float t2 = this->animationPositionTrack[this->positionTrackIndex].first;
			float lerp = (nowTick - t1) / (t2 - t1);

			if (lerp >= 1.0f) 
			{
				lerp = 1.0f;
			}

			DirectX::XMVECTOR nowVector = DirectX::XMLoadFloat3(&nowPosition);
			DirectX::XMVECTOR prevVector = DirectX::XMLoadFloat3(&prevPosition);
			nowVector = DirectX::XMVectorLerp(prevVector, nowVector, lerp);

			DirectX::XMStoreFloat3(&nowPosition, nowVector);
		}
		this->localTM *= DirectX::XMMatrixTranslation(nowPosition.x, nowPosition.y, nowPosition.z);
	}
	else
	{
		this->localTM *= DirectX::XMMatrixTranslationFromVector(this->localPosition);
	}
}

