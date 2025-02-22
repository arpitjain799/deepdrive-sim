
#include "Simulation/Traffic/Path/Annotations/DeepDrivePathManeuverAnnotations.h"
#include "Simulation/Traffic/Path/DeepDrivePartialPath.h"
#include "Simulation/Traffic/BehaviorTree/DeepDriveTrafficBlackboard.h"
#include "Simulation/Traffic/BehaviorTree/DeepDriveTrafficBehaviorTree.h"

DEFINE_LOG_CATEGORY(LogDeepDrivePathManeuverAnnotations);

void DeepDrivePathManeuverAnnotations::annotate(DeepDrivePartialPath &path, TDeepDrivePathPoints &pathPoints, TDeepDriveManeuvers &maneuvers)
{
	for(auto &maneuver : maneuvers)
	{
		if(maneuver.BehaviorTree)
		{
			UE_LOG(LogDeepDrivePathManeuverAnnotations, Log, TEXT("Binding behavior tree") );

			maneuver.BehaviorTree->getBlackboard().setManeuver(maneuver);
			maneuver.BehaviorTree->getBlackboard().setPartialPath(path);
			maneuver.BehaviorTree->bind(path, maneuver);

			float minEntryDist = TNumericLimits<float>::Max();
			float minExitDist = TNumericLimits<float>::Max();
			for(int32 ind = 0; ind < pathPoints.Num(); ind++)
			{
				SDeepDrivePathPoint &pathPoint = pathPoints[ind];
				const float curEntryDist = (pathPoint.Location - maneuver.EntryPoint).Size();
				if(curEntryDist < minEntryDist)
				{
					minEntryDist = curEntryDist;
					maneuver.EntryPointIndex = ind;
				}

				const float curExitDist = (pathPoint.Location - maneuver.ExitPoint).Size();
				if(curExitDist < minExitDist)
				{
					minExitDist = curExitDist;
					maneuver.ExitPointIndex = ind;
				}
			}

			maneuver.DirectionIndicationBeginIndex = path.rewind(maneuver.DirectionIndicationBeginIndex, 1500.0f);
			// maneuver.DirectionIndicationEndIndex = path.windForward(maneuver.DirectionIndicationEndIndex, 500.0f);

			UE_LOG(LogDeepDrivePathManeuverAnnotations, Log, TEXT("DirectionIndication %d %d | %d"), maneuver.DirectionIndicationBeginIndex, maneuver.DirectionIndicationEndIndex, maneuver.ExitPointIndex );

		}
		else
		{
			UE_LOG(LogDeepDrivePathManeuverAnnotations, Log, TEXT("No behavior tree to bind") );
		}
	}
}
