

#pragma warning(disable: 4324)
#include "OVR_CAPI_D3D.h" // Oculus SDK
#include "openvr.h"
#include <vector>
#include <thread>
#include <chrono>


class Guardian2Chaperone
{
public:
    void Start();


};





void Guardian2Chaperone::Start()
{

	ovrSession mSession = nullptr;


    ovrResult result;
    result = ovr_Initialize(nullptr);
    if (!OVR_SUCCESS(result)) {
        printf("ovr_Initialize failed"); exit(-1);
    }

    ovrGraphicsLuid luid;
    result = ovr_Create(&mSession, &luid);
    if (!OVR_SUCCESS(result)) {
        printf("ovr_Create failed"); exit(-1);
    }


	
	std::vector<ovrVector3f> playPoints;



    // Use Eye level origin -- seems to correspond with SteamVR "raw" origin
	ovr_SetTrackingOriginType(mSession, ovrTrackingOrigin_EyeLevel);

	int numOfPlayPoints;
	if (!OVR_SUCCESS(ovr_GetBoundaryGeometry(mSession, ovrBoundaryType::ovrBoundary_PlayArea, NULL, &numOfPlayPoints))) {
		printf("Getting number of boundary points failed"); exit(-1);
	}

	playPoints.resize(numOfPlayPoints);
	if (!OVR_SUCCESS(ovr_GetBoundaryGeometry(mSession, ovrBoundaryType::ovrBoundary_PlayArea, playPoints.data(), &numOfPlayPoints))) {
		printf("Getting boundary points failed"); exit(-1);
	}

	int numOfGuardianPoints;
	std::vector<ovrVector3f> guardianPoints;
	if (!OVR_SUCCESS(ovr_GetBoundaryGeometry(mSession, ovrBoundaryType::ovrBoundary_Outer, NULL, &numOfGuardianPoints))) {
		printf("Getting number of guardian points failed"); exit(-1);
	}

	guardianPoints.resize(numOfGuardianPoints);

	if (!OVR_SUCCESS(ovr_GetBoundaryGeometry(mSession, ovrBoundaryType::ovrBoundary_Outer, guardianPoints.data(), &numOfGuardianPoints))) {
		printf("Getting guardian points failed"); exit(-1);
	}

	ovrVector3f dimensions;
	if (!OVR_SUCCESS(ovr_GetBoundaryDimensions(mSession, ovrBoundaryType::ovrBoundary_PlayArea, &dimensions))) {
		printf("Getting boundary dimensions failed"); exit(-1);
	}

	
	ovr_Destroy(mSession);

	ovr_Shutdown();

	ovrVector3f origin;

	origin.x = 0;
	origin.y = 0;
	origin.z = 0;

	for (unsigned int i = 0; i < playPoints.size(); ++i) {
		origin.x += playPoints[i].x;
		origin.y += playPoints[i].y;
		origin.z += playPoints[i].z;
	}

	int numOfPoints = playPoints.size();
	origin.x = origin.x / (float)numOfPoints;
	origin.y = origin.y / (float)numOfPoints;
	origin.z = origin.z / (float)numOfPoints;



	vr::EVRInitError initError;
	vr::VR_Init(&initError, vr::EVRApplicationType::VRApplication_Scene);



	vr::VRChaperone()->GetCalibrationState(); // REQUIRED in order to do any chaperone setup


	vr::VRChaperoneSetup()->RevertWorkingCopy();
	vr::HmdMatrix34_t standingZero = {};

	//standingZero.m[1][3] = 5;
	// Rotation to 0, 1s along diagonal I think
	standingZero.m[0][0] = 1;
	standingZero.m[1][1] = 1;
	standingZero.m[2][2] = 1;

	//Position
	standingZero.m[0][3] = origin.x;
	standingZero.m[1][3] = origin.y;
	standingZero.m[2][3] = origin.z;

	uint32_t quadCount = 0;
	vr::VRChaperoneSetup()->GetWorkingCollisionBoundsInfo(NULL, &quadCount);

	std::vector<vr::HmdQuad_t> quads(numOfGuardianPoints);

	for (int i = 0; i < numOfGuardianPoints; i++) {
		int j = (i + 1) % numOfGuardianPoints;
		quads[i].vCorners[0].v[0] = guardianPoints[i].x - origin.x;
		quads[i].vCorners[0].v[1] = guardianPoints[i].y - origin.y;
		quads[i].vCorners[0].v[2] = guardianPoints[i].z - origin.z;

		quads[i].vCorners[1].v[0] = guardianPoints[i].x - origin.x;
		quads[i].vCorners[1].v[1] = guardianPoints[i].y - origin.y + 2.43f;
		quads[i].vCorners[1].v[2] = guardianPoints[i].z - origin.z;

		quads[i].vCorners[2].v[0] = guardianPoints[j].x - origin.x;
		quads[i].vCorners[2].v[1] = guardianPoints[j].y - origin.y + 2.43f;
		quads[i].vCorners[2].v[2] = guardianPoints[j].z - origin.z;

		quads[i].vCorners[3].v[0] = guardianPoints[j].x - origin.x;
		quads[i].vCorners[3].v[1] = guardianPoints[j].y - origin.y;
		quads[i].vCorners[3].v[2] = guardianPoints[j].z - origin.z;
		
	}


	
	vr::VRChaperoneSetup()->SetWorkingStandingZeroPoseToRawTrackingPose(&standingZero);
	vr::VRChaperoneSetup()->SetWorkingPlayAreaSize(dimensions.x, dimensions.z);
	vr::VRChaperoneSetup()->SetWorkingCollisionBoundsInfo(quads.data(), numOfGuardianPoints);
	vr::VRChaperoneSetup()->CommitWorkingCopy(vr::EChaperoneConfigFile_Live);
	vr::VRSettings()->SetInt32(vr::k_pch_CollisionBounds_Section, vr::k_pch_CollisionBounds_ColorGammaA_Int32, 0);
	vr::VRSettings()->Sync();

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	vr::VR_Shutdown();




    



}













int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    Guardian2Chaperone* instance = new (_aligned_malloc(sizeof(Guardian2Chaperone), 16)) Guardian2Chaperone();
    instance->Start();
    return 0;
}
