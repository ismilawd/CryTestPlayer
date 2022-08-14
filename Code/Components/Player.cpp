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

	m_pInputComponent->RegisterAction("player", "moveforward", [this](int activationMode, float value) {m_movementDelta.y = value; });
	m_pInputComponent->BindAction("player", "moveforward", eAID_KeyboardMouse, eKI_W);

	m_pInputComponent->RegisterAction("player", "movebackward", [this](int activationMode, float value) {m_movementDelta.y = -value; });
	m_pInputComponent->BindAction("player", "movebackward", eAID_KeyboardMouse, eKI_S);

	m_pInputComponent->RegisterAction("player", "moveright", [this](int activationMode, float value) {m_movementDelta.x = value; });
	m_pInputComponent->BindAction("player", "moveright", eAID_KeyboardMouse, eKI_D);

	m_pInputComponent->RegisterAction("player", "moveleft", [this](int activationMode, float value) {m_movementDelta.x = -value; });
	m_pInputComponent->BindAction("player", "moveleft", eAID_KeyboardMouse, eKI_A);

	m_pEntity->SetPos(m_pEntity->GetWorldPos() + Vec3(m_movementDelta.x, m_movementDelta.y, 0));
}
