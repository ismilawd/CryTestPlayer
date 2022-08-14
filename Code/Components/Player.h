// Copyright 2017-2020 Crytek GmbH / Crytek Group. All rights reserved.
#pragma once

#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Input/InputComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>

////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CPlayerComponent final : public IEntityComponent
{
public:
	CPlayerComponent() = default;
	virtual ~CPlayerComponent() = default;

	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CPlayerComponent>& desc)
	{
		desc.SetGUID("{63F4C0C6-32AF-4ACB-8FB0-57D45DD14725}"_cry_guid);

		desc.AddMember(&CPlayerComponent::m_movementSpeed, 'pms', "playermovespeed", "Player Movement Speed", "Sets the player movement speed",ZERO);
		desc.AddMember(&CPlayerComponent::m_rotationSpeed, 'prs', "playerrotationspeed", "Player Rotation Speed", "Sets the player rotation speed", ZERO);
	}
private:
	//camera
	//input
	//Character Controller
	//Advanced Animation

	void InitInputs();
	void PlayerUpdateMovement();

	Cry::DefaultComponents::CCameraComponent* m_pCameraComponent;
	Cry::DefaultComponents::CInputComponent* m_pInputComponent;
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController;
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAdvancedAnimationComponent;

	Vec2 m_movementDelta;
	Vec2 m_mouseDeltaRotation;

	float m_movementSpeed;
	float m_rotationSpeed;
};
