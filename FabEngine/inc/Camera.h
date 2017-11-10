#pragma once

#include <DirectXComponentsPCH.h>
#include "IComponent.h"
#include "IEntity.h"

using namespace DirectX;

namespace Fab
{
	enum class CameraMode
	{
		WALK,
		FLY
	};

	class Camera : public IComponent, public IEntity
	{
	public:
		Camera();
		Camera(float fov, float nearZ, float farZ, float translationSpeed, float rotationSpeed, CameraMode mode);
		~Camera();
		void        Initialise() override;
		void        Draw() override;
		void        Update(float deltaTime, float totalTime) override;
		void        ComputeProjectionMatrix();

		void        RotateY(float angle);
		void        RotateX(float angle);
		void        RotateZ(float angle);
		void        Pitch(float angle);
		void        Yaw(float angle);
		void        Roll(float angle);
		void        Rotate(float x, float y);
		void        Fly(float distance);
		void        Walk(float distance);
		void        Move(float distanceX, float distanceY, float distanceZ);

		void        SetFov(float fov);
		void        SetNearZ(float nearZ);
		void        SetFarZ(float farZ);
		void 	    SetRotationSpeed(float rotationSpeed);
		void 	    SetTranslationSpeed(float translationSpeed);

		CameraMode  GetMode();

		XMFLOAT3&   GetPosition();
		XMFLOAT3&   GetEye();
		XMFLOAT3&   GetAt();
		XMFLOAT3&   GetUp();

		XMFLOAT4X4& GetView();
		XMFLOAT4X4& GetProjection();

	public:
		static const float      DefaultFov;
		static const float      DefaultNearZ;
		static const float      DefaultFarZ;
		static const float      DefaultTranslationSpeed;
		static const float      DefaultRotationSpeed;
		static const CameraMode DefaultMode;

	private:
		XMFLOAT3   _position;
		XMFLOAT3   _oldPosition;

		XMFLOAT3   _eye;
		XMFLOAT3   _at;
		XMFLOAT3   _up;

		float      _fov;
		float      _nearZ;
		float      _farZ;
		float      _rotationSpeed;
		float      _translationSpeed;

		float      _lastAngle;

		XMFLOAT4X4 _view;
		XMFLOAT4X4 _projection;

		CameraMode _mode;
	};
}
