// Copyright 2017-2020 Crytek GmbH / Crytek Group. All rights reserved.
#include "StdAfx.h"
#include "Player.h"
#include "GamePlugin.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>

namespace
{
	static void RegisterPlayerComponent(Schematyc::IEnvRegistrar& registrar)
	{
		Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
		{
			Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CPlayerComponent));
		}
	}

	CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterPlayerComponent);
}

void CPlayerComponent::Initialize()
{
	m_pCameraComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
	m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();
	m_pCharacterController = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
	m_pAdvancedAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
}

Cry::Entity::EventFlags CPlayerComponent::GetEventMask() const
{
	return Cry::Entity::EEvent::GameplayStarted| Cry::Entity::EEvent::Update | Cry::Entity::EEvent::Reset;
}

void CPlayerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case Cry::Entity::EEvent::GameplayStarted:
	{
		m_movementDelta = ZERO;
		InitInputs();
	}
	break;
	case Cry::Entity::EEvent::Update:
	{
		PlayerUpdateMovement();

		Ang3 rotationAngle = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));
		rotationAngle.x += m_mouseDeltaRotation.x * m_rotationSpeed;
		rotationAngle.y += m_mouseDeltaRotation.y * m_rotationSpeed;
		rotationAngle.z += 0;

		m_lookOrientation = Quat(CCamera::CreateOrientationYPR(rotationAngle));
		m_pEntity->SetRotation(m_lookOrientation);

	}
	break;
	case Cry::Entity::EEvent::Reset:
	{
		m_movementDelta = ZERO;
		m_mouseDeltaRotation = ZERO;
		m_lookOrientation = IDENTITY;


		Matrix34 camDefaultMatrix;
		camDefaultMatrix.SetTranslation(m_cameraDefaultPos);
		camDefaultMatrix.SetRotation33(Matrix33(m_pEntity->GetWorldRotation()));
		m_pCameraComponent->SetTransformMatrix(camDefaultMatrix);
	}
	break;
	}
}

void CPlayerComponent::InitInputs()
{
	m_pInputComponent->RegisterAction("player", "moveforward", [this](int activationMode, float value) {m_movementDelta.y = value; });
	m_pInputComponent->BindAction("player", "moveforward", eAID_KeyboardMouse, eKI_W);

	m_pInputComponent->RegisterAction("player", "movebackward", [this](int activationMode, float value) {m_movementDelta.y = -value; });
	m_pInputComponent->BindAction("player", "movebackward", eAID_KeyboardMouse, eKI_S);

	m_pInputComponent->RegisterAction("player", "moveright", [this](int activationMode, float value) {m_movementDelta.x = value; });
	m_pInputComponent->BindAction("player", "moveright", eAID_KeyboardMouse, eKI_D);

	m_pInputComponent->RegisterAction("player", "moveleft", [this](int activationMode, float value) {m_movementDelta.x = -value; });
	m_pInputComponent->BindAction("player", "moveleft", eAID_KeyboardMouse, eKI_A);

	m_pInputComponent->RegisterAction("player", "yaw", [this](int activationMode, float value) {m_mouseDeltaRotation.y = -value; });
	m_pInputComponent->BindAction("player", "yaw", eAID_KeyboardMouse, eKI_MouseY);

	m_pInputComponent->RegisterAction("player", "pitch", [this](int activationMode, float value) {m_mouseDeltaRotation.x = -value; });
	m_pInputComponent->BindAction("player", "pitch", eAID_KeyboardMouse, eKI_MouseX);
}

void CPlayerComponent::PlayerUpdateMovement()
{
	Vec3 velocity = Vec3(m_movementDelta.x, m_movementDelta.y, 0);
	velocity.normalize();
	m_pCharacterController->SetVelocity(m_pEntity->GetWorldRotation() * velocity * m_movementSpeed);
}
