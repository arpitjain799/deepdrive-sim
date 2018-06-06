
#include "DeepDrivePluginPrivatePCH.h"
#include "Private/Simulation/Agent/Controllers/LocalAI/States/DeepDriveAgentPullBackInState.h"
#include "Public/Simulation/Agent/Controllers/DeepDriveAgentLocalAIController.h"
#include "Private/Simulation/Agent/Controllers/DeepDriveAgentSpeedController.h"
#include "Private/Simulation/Agent/Controllers/DeepDriveAgentSteeringController.h"


DeepDriveAgentPullBackInState::DeepDriveAgentPullBackInState(DeepDriveAgentLocalAIStateMachine &stateMachine)
	: DeepDriveAgentLocalAIStateBase(stateMachine, "PullBackIn")
{

}


void DeepDriveAgentPullBackInState::enter(DeepDriveAgentLocalAIStateMachineContext &ctx)
{
	m_remainingPullInTime = ctx.configuration.ChangeLaneDuration;
	m_curOffset = ctx.side_offset;
	m_deltaOffsetFac = ctx.configuration.OvertakingOffset / m_remainingPullInTime;
	UE_LOG(LogDeepDriveAgentLocalAIController, Log, TEXT("Agent %d Pulling back In"), ctx.agent.getAgentId());
}

void DeepDriveAgentPullBackInState::update(DeepDriveAgentLocalAIStateMachineContext &ctx, float dT)
{
	m_remainingPullInTime -= dT;
	m_curOffset -= dT * m_deltaOffsetFac;
	if (m_remainingPullInTime <= 0.0f)
	{
		m_StateMachine.setNextState("Cruise");
	}

	float desiredSpeed = ctx.local_ai_ctrl.getDesiredSpeed();
	desiredSpeed = ctx.speed_controller.limitSpeedByTrack(desiredSpeed, ctx.configuration.SpeedLimitFactor);
	desiredSpeed = ctx.speed_controller.limitSpeedByNextAgent(desiredSpeed);

	ctx.speed_controller.update(dT, desiredSpeed);
	ctx.steering_controller.update(dT, desiredSpeed, m_curOffset);

}

void DeepDriveAgentPullBackInState::exit(DeepDriveAgentLocalAIStateMachineContext &ctx)
{
	ctx.agent_to_overtake = 0;
	ctx.wait_time_before_overtaking = 2.0f;
}