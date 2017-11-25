
#include "classes.hpp"
#include "mapdefs.hpp"

/**************************
 * Forest map
 * 
 **************************/

ForestMapGen::ForestMapGen (Voronoi* Voro) : PlainMapGen (Voro) {
    
    int n, SubN;
    
    // Generate random texture coordinates and wall heights at all corners
    for (n = 0; n < 3*NoOfSites; n++) {
        CornerWallHeight[n] = 2.0f + 6.0f * static_cast<float>(rand()) / RandMax;
        CornerTexS[n] = 0.1f + 0.2f*(float)(n % 5);
        CornerTexT[n] = 0.1f + 0.2f*(float)((n/5) % 5);
        CornerTe2S[n] = 0.1f + 0.2f*(float)((n+8) % 5);
        CornerTe2T[n] = 0.1f + 0.2f*(float)(((n+8)/5) % 5);
        CornerTe3S[n] = 0.1f + 0.2f*(float)((n+16) % 5);
        CornerTe3T[n] = 0.1f + 0.2f*(float)(((n+16)/5) % 5);
    }
    for (n = 0; n < NoOfSubsetSites; n++) {
        SiteTexS[n] = static_cast<float>(rand()) / RandMax;
        SiteTexT[n] = static_cast<float>(rand()) / RandMax;
    }
    
    // Set extra cell parameters
    //  0 or 1 to denote cliffs or lake for boundaries, and grass of water for traversible space
	ExtCellType = new int[NoOfSubsetSites];
	for (n = 0; n < NoOfSubsetSites; n++) {
		if (CellType[n] < 3) ExtCellType[n] = EXT_FLOOR;
		else if (CellType[n] == 3) ExtCellType[n] = EXT_CLIFF + (int)(3.0f * static_cast<float>(rand()) / RandMax);
		else ExtCellType[n] = EXT_RUBBLE + (int)(2.0f * static_cast<float>(rand()) / RandMax);
	}
    
    // Create extra data vectors
    ExtDecoCount = new int[NoOfSubsetSites];
    ExtDecoType = new int*[NoOfSubsetSites];
    ExtDecoParams = new float*[NoOfSubsetSites];
    for (n = 0; n < NoOfSubsetSites; n++) {
        if (CellType[n] < 3) {
            ExtDecoCount[n] = (int)(5.0f * (static_cast<float>(rand()) / RandMax)) + 1;
            ExtDecoType[n] = new int[ExtDecoCount[n]];
            ExtDecoParams[n] = new float[3*ExtDecoCount[n]];
            for (SubN = 0; SubN < ExtDecoCount[n]; SubN++) {
                ExtDecoType[n][SubN] = (int)(2.0f * (static_cast<float>(rand()) / RandMax));
                ExtDecoParams[n][SubN*3  ] = SubSiteX[n] - 3.0f + 6.0f * (static_cast<float>(rand()) / RandMax);
                ExtDecoParams[n][SubN*3+1] = SubSiteY[n] - 3.0f + 6.0f * (static_cast<float>(rand()) / RandMax);
                ExtDecoParams[n][SubN*3+2] = 360.0f * (static_cast<float>(rand()) / RandMax);
            }
        }
        else ExtDecoCount[n] = 0;
    }
    
}

ForestMapGen::~ForestMapGen () {
    
    int n;
    
    for (n = 0; n < NoOfSubsetSites; n++) {
        if (ExtDecoCount[n] > 0) {
            delete[] ExtDecoType[n];
            delete[] ExtDecoParams[n];
        }
    }
	delete[] ExtCellType;
    delete[] ExtDecoCount;
    delete[] ExtDecoType;
    delete[] ExtDecoParams;
    
}

int ForestMapGen::GetFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo) + PlainMapGen::GetCommonDecoNoOfFloats (CellNo, ExtDecoCount[CellNo], ExtDecoType[CellNo]);
    
}

int ForestMapGen::GetWalledFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo) + PlainMapGen::GetCommonReachableWallNoOfFloats (CellNo)
		 + PlainMapGen::GetCommonDecoNoOfFloats (CellNo, ExtDecoCount[CellNo], ExtDecoType[CellNo]);
    
}

int ForestMapGen::GetBlockedCellNoOfFloats (int CellNo) {
    
    switch (ExtCellType[CellNo]) {
    case EXT_CLIFF:
        return PlainMapGen::GetCommonCliffNoOfFloats (CellNo);
    case EXT_POND:
        return PlainMapGen::GetCommonSunkenNoOfFloats (CellNo) + PlainMapGen::GetCommonUnreachableWallNoOfFloats (CellNo);
    case EXT_RUBBLE:
        return PlainMapGen::GetCommonFloorNoOfFloats (CellNo) + PlainMapGen::GetCommonRubbleNoOfFloats (CellNo)
            + PlainMapGen::GetCommonUnreachableWallNoOfFloats (CellNo);
    default:
        return 0;
    }
    
}

int ForestMapGen::GetEnclosedCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonBoulderNoOfFloats (CellNo);
    
}

int ForestMapGen::LoadFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    int Copied = 0;
    Copied += PlainMapGen::LoadCommonFloorFloats (Buffer, Index + Copied, CellNo);
    Copied += PlainMapGen::LoadCommonDecoFloats (Buffer, Index + Copied, CellNo, ExtDecoCount[CellNo], ExtDecoType[CellNo], ExtDecoParams[CellNo]);
    return Copied;
    
}

int ForestMapGen::LoadWalledFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    int Copied = 0;
    Copied += PlainMapGen::LoadCommonFloorFloats (Buffer, Index + Copied, CellNo);
    Copied += PlainMapGen::LoadCommonReachableWallFloats (Buffer, Index + Copied, CellNo);
    Copied += PlainMapGen::LoadCommonDecoFloats (Buffer, Index + Copied, CellNo, ExtDecoCount[CellNo], ExtDecoType[CellNo], ExtDecoParams[CellNo]);
    return Copied;
    
}

int ForestMapGen::LoadBlockedCellFloats (float* Buffer, int Index, int CellNo) {

    int Copied = 0;
    switch (ExtCellType[CellNo]) {
    case EXT_CLIFF:
        return PlainMapGen::LoadCommonCliffFloats (Buffer, Index, CellNo);
    case EXT_POND:
        Copied += PlainMapGen::LoadCommonSunkenFloats (Buffer, Index + Copied, CellNo);
        Copied += PlainMapGen::LoadCommonUnreachableWallFloats (Buffer, Index + Copied, CellNo);
        return Copied;
    case EXT_RUBBLE:
        Copied += PlainMapGen::LoadCommonFloorFloats (Buffer, Index + Copied, CellNo);
        Copied += PlainMapGen::LoadCommonRubbleFloats (Buffer, Index + Copied, CellNo);
        Copied += PlainMapGen::LoadCommonUnreachableWallFloats (Buffer, Index + Copied, CellNo);
        return Copied;
    default:
        return 0;
    }

}

int ForestMapGen::LoadEnclosedCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonBoulderFloats (Buffer, Index, CellNo);
    
}

/**************************
 * Lava map
 * 
 **************************/

LavaCaveMapGen::LavaCaveMapGen (Voronoi* Voro) : PlainMapGen (Voro) {
    
    int n;
    
    // Generate random texture coordinates and wall heights at all corners
    for (n = 0; n < 3*NoOfSites; n++) {
        CornerWallHeight[n] = 2.0f + 6.0f * static_cast<float>(rand()) / RandMax;
        CornerTexS[n] = static_cast<float>(rand()) / RandMax;
        CornerTexT[n] = static_cast<float>(rand()) / RandMax;
        CornerTe2S[n] = static_cast<float>(rand()) / RandMax;
        CornerTe2T[n] = static_cast<float>(rand()) / RandMax;
    }
    for (n = 0; n < NoOfSubsetSites; n++) {
        SiteTexS[n] = static_cast<float>(rand()) / RandMax;
        SiteTexT[n] = static_cast<float>(rand()) / RandMax;
    }
    
    // Set extra cell parameters
	ExtCellType = new int[NoOfSubsetSites];
	for (n = 0; n < NoOfSubsetSites; n++) {
		if (CellType[n] < 3) ExtCellType[n] = EXT_FLOOR;
		else if (CellType[n] == 3) ExtCellType[n] = EXT_CLIFF + (int)(2.0f * static_cast<float>(rand()) / RandMax);
		else ExtCellType[n] = EXT_BOULDER;
	}
    
}

LavaCaveMapGen::~LavaCaveMapGen () {
    
    delete[] ExtCellType;

}

int LavaCaveMapGen::GetFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo);
    
}

int LavaCaveMapGen::GetWalledFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo) + PlainMapGen::GetCommonReachableWallNoOfFloats (CellNo);
    
}

int LavaCaveMapGen::GetBlockedCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonSunkenNoOfFloats (CellNo) + PlainMapGen::GetCommonUnreachableWallNoOfFloats (CellNo);
    
}

int LavaCaveMapGen::GetEnclosedCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonBoulderNoOfFloats (CellNo);
    
}

int LavaCaveMapGen::LoadFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonFloorFloats (Buffer, Index, CellNo);
    
}

int LavaCaveMapGen::LoadWalledFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    int Copied = 0;
    Copied += PlainMapGen::LoadCommonFloorFloats (Buffer, Index + Copied, CellNo);
    Copied += PlainMapGen::LoadCommonReachableWallFloats (Buffer, Index + Copied, CellNo);
    return Copied;
    
}

int LavaCaveMapGen::LoadBlockedCellFloats (float* Buffer, int Index, int CellNo) {
    
    int Copied = 0;
    Copied += PlainMapGen::LoadCommonSunkenFloats (Buffer, Index + Copied, CellNo);
    Copied += PlainMapGen::LoadCommonUnreachableWallFloats (Buffer, Index + Copied, CellNo);
    return Copied;
    
}

int LavaCaveMapGen::LoadEnclosedCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonBoulderFloats (Buffer, Index, CellNo);
    
}

/**************************
 * Lake map
 * 
 **************************/

LakeMapGen::LakeMapGen (Voronoi* Voro) : PlainMapGen (Voro) {
    
    int n;
    
    // Generate random texture coordinates and wall heights at all corners
    for (n = 0; n < 3*NoOfSites; n++) {
        CornerWallHeight[n] = 2.0f + 6.0f * static_cast<float>(rand()) / RandMax;
        CornerTexS[n] = static_cast<float>(rand()) / RandMax;
        CornerTexT[n] = static_cast<float>(rand()) / RandMax;
        CornerTe2S[n] = static_cast<float>(rand()) / RandMax;
        CornerTe2T[n] = static_cast<float>(rand()) / RandMax;
    }
    for (n = 0; n < NoOfSubsetSites; n++) {
        SiteTexS[n] = static_cast<float>(rand()) / RandMax;
        SiteTexT[n] = static_cast<float>(rand()) / RandMax;
    }
    
    // Set extra cell parameters
    //  0 or 1 to denote cliffs or lake for boundaries, and grass of water for traversible space
	ExtCellType = new int[NoOfSubsetSites];
	for (n = 0; n < NoOfSubsetSites; n++) {
		if (CellType[n] < 3) ExtCellType[n] = EXT_FLOOR;
		else if (CellType[n] == 3) ExtCellType[n] = EXT_CLIFF + (int)(3.0f * static_cast<float>(rand()) / RandMax);
		else ExtCellType[n] = EXT_RUBBLE + (int)(2.0f * static_cast<float>(rand()) / RandMax);
	}
    
}

LakeMapGen::~LakeMapGen () {
    
	delete[] ExtCellType;

}

int LakeMapGen::GetFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo);
    
}

int LakeMapGen::GetWalledFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo) + PlainMapGen::GetCommonReachableWallNoOfFloats (CellNo);
    
}

int LakeMapGen::GetBlockedCellNoOfFloats (int CellNo) {
    
    if (ExtCellType[CellNo] == 0)
        return PlainMapGen::GetCommonCliffNoOfFloats (CellNo);
    else 
        return PlainMapGen::GetCommonSunkenNoOfFloats (CellNo) + PlainMapGen::GetCommonUnreachableWallNoOfFloats (CellNo);
    
}

int LakeMapGen::GetEnclosedCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonBoulderNoOfFloats (CellNo);
    
}

int LakeMapGen::LoadFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonFloorFloats (Buffer, Index, CellNo);
    
}

int LakeMapGen::LoadWalledFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    int Copied = 0;
    Copied += PlainMapGen::LoadCommonFloorFloats (Buffer, Index + Copied, CellNo);
    Copied += PlainMapGen::LoadCommonReachableWallFloats (Buffer, Index + Copied, CellNo);
    return Copied;
    
}

int LakeMapGen::LoadBlockedCellFloats (float* Buffer, int Index, int CellNo) {
    
    int Copied = 0;
    if (ExtCellType[CellNo] == 0)
        return PlainMapGen::LoadCommonCliffFloats (Buffer, Index + Copied, CellNo);
    Copied += PlainMapGen::LoadCommonSunkenFloats (Buffer, Index + Copied, CellNo);
    Copied += PlainMapGen::LoadCommonUnreachableWallFloats (Buffer, Index + Copied, CellNo);
    return Copied;
    
}

int LakeMapGen::LoadEnclosedCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonBoulderFloats (Buffer, Index, CellNo);
    
}

/**************************
 * Arid map
 * 
 **************************/

AridMapGen::AridMapGen (Voronoi* Voro) : PlainMapGen (Voro) {
    
    int n;
    
    // Generate random texture coordinates and wall heights at all corners
    for (n = 0; n < 3*NoOfSites; n++) {
        CornerWallHeight[n] = 2.0f + 6.0f * static_cast<float>(rand()) / RandMax;
        CornerTexS[n] = static_cast<float>(rand()) / RandMax;
        CornerTexT[n] = static_cast<float>(rand()) / RandMax;
        CornerTe2S[n] = static_cast<float>(rand()) / RandMax;
        CornerTe2T[n] = static_cast<float>(rand()) / RandMax;
    }
    for (n = 0; n < NoOfSubsetSites; n++) {
        SiteTexS[n] = static_cast<float>(rand()) / RandMax;
        SiteTexT[n] = static_cast<float>(rand()) / RandMax;
    }
    
    // Set extra cell parameters
	ExtCellType = new int[NoOfSubsetSites];
	for (n = 0; n < NoOfSubsetSites; n++) {
		if (CellType[n] < 3) ExtCellType[n] = EXT_FLOOR;
		else if (CellType[n] == 3) ExtCellType[n] = EXT_CLIFF + (int)(2.0f * static_cast<float>(rand()) / RandMax);
		else ExtCellType[n] = EXT_BOULDER;
	}
    
}

AridMapGen::~AridMapGen () {
    
    delete[] ExtCellType;

}

int AridMapGen::GetFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo);
    
}

int AridMapGen::GetWalledFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo) + PlainMapGen::GetCommonReachableWallNoOfFloats (CellNo);
    
}

int AridMapGen::GetBlockedCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonCliffNoOfFloats (CellNo);
    
}

int AridMapGen::GetEnclosedCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonBoulderNoOfFloats (CellNo);
    
}

int AridMapGen::LoadFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonFloorFloats (Buffer, Index, CellNo);
    
}

int AridMapGen::LoadWalledFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    int Copied = 0;
    Copied += PlainMapGen::LoadCommonFloorFloats (Buffer, Index + Copied, CellNo);
    Copied += PlainMapGen::LoadCommonReachableWallFloats (Buffer, Index + Copied, CellNo);
    return Copied;
    
}

int AridMapGen::LoadBlockedCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonCliffFloats (Buffer, Index, CellNo);
    
}

int AridMapGen::LoadEnclosedCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonBoulderFloats (Buffer, Index, CellNo);
    
}

/**************************
 * Cave map
 * 
 **************************/

CaveMapGen::CaveMapGen (Voronoi* Voro) : PlainMapGen (Voro) {
    
    int n;
    
    // Generate random texture coordinates and wall heights at all corners
    for (n = 0; n < 3*NoOfSites; n++) {
        CornerWallHeight[n] = 2.0f + 6.0f * static_cast<float>(rand()) / RandMax;
        CornerTexS[n] = static_cast<float>(rand()) / RandMax;
        CornerTexT[n] = static_cast<float>(rand()) / RandMax;
        CornerTe2S[n] = static_cast<float>(rand()) / RandMax;
        CornerTe2T[n] = static_cast<float>(rand()) / RandMax;
    }
    for (n = 0; n < NoOfSubsetSites; n++) {
        SiteTexS[n] = static_cast<float>(rand()) / RandMax;
        SiteTexT[n] = static_cast<float>(rand()) / RandMax;
    }
    
    // Set extra cell parameters
	ExtCellType = new int[NoOfSubsetSites];
	for (n = 0; n < NoOfSubsetSites; n++) {
		if (CellType[n] < 3) ExtCellType[n] = EXT_FLOOR;
		else if (CellType[n] == 3) ExtCellType[n] = EXT_CLIFF + (int)(2.0f * static_cast<float>(rand()) / RandMax);
		else ExtCellType[n] = EXT_BOULDER;
	}
    
}

CaveMapGen::~CaveMapGen () {
    
    delete[] ExtCellType;

}

int CaveMapGen::GetFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo);
    
}

int CaveMapGen::GetWalledFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo) + PlainMapGen::GetCommonReachableWallNoOfFloats (CellNo);
    
}

int CaveMapGen::GetBlockedCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonCliffNoOfFloats (CellNo);
    
}

int CaveMapGen::GetEnclosedCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonBoulderNoOfFloats (CellNo);
    
}

int CaveMapGen::LoadFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonFloorFloats (Buffer, Index, CellNo);
    
}

int CaveMapGen::LoadWalledFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    int Copied = 0;
    Copied += PlainMapGen::LoadCommonFloorFloats (Buffer, Index + Copied, CellNo);
    Copied += PlainMapGen::LoadCommonReachableWallFloats (Buffer, Index + Copied, CellNo);
    return Copied;
    
}

int CaveMapGen::LoadBlockedCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonCliffFloats (Buffer, Index, CellNo);
    
}

int CaveMapGen::LoadEnclosedCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonBoulderFloats (Buffer, Index, CellNo);
    
}

/**************************
 * Snow map
 * 
 **************************/

SnowMapGen::SnowMapGen (Voronoi* Voro) : PlainMapGen (Voro) {
    
    int n;
    
    // Generate random texture coordinates and wall heights at all corners
    for (n = 0; n < 3*NoOfSites; n++) {
        CornerWallHeight[n] = 2.0f + 6.0f * static_cast<float>(rand()) / RandMax;
        CornerTexS[n] = static_cast<float>(rand()) / RandMax;
        CornerTexT[n] = static_cast<float>(rand()) / RandMax;
        CornerTe2S[n] = static_cast<float>(rand()) / RandMax;
        CornerTe2T[n] = static_cast<float>(rand()) / RandMax;
    }
    for (n = 0; n < NoOfSubsetSites; n++) {
        SiteTexS[n] = static_cast<float>(rand()) / RandMax;
        SiteTexT[n] = static_cast<float>(rand()) / RandMax;
    }
    
    // Set extra cell parameters
	ExtCellType = new int[NoOfSubsetSites];
	for (n = 0; n < NoOfSubsetSites; n++) {
		if (CellType[n] < 3) ExtCellType[n] = EXT_FLOOR;
		else if (CellType[n] == 3) ExtCellType[n] = EXT_CLIFF + (int)(2.0f * static_cast<float>(rand()) / RandMax);
		else ExtCellType[n] = EXT_BOULDER;
	}
    
}

SnowMapGen::~SnowMapGen () {
    
    delete[] ExtCellType;

}

int SnowMapGen::GetFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo);
    
}

int SnowMapGen::GetWalledFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo) + PlainMapGen::GetCommonReachableWallNoOfFloats (CellNo);
    
}

int SnowMapGen::GetBlockedCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonCliffNoOfFloats (CellNo);
    
}

int SnowMapGen::GetEnclosedCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonBoulderNoOfFloats (CellNo);
    
}

int SnowMapGen::LoadFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonFloorFloats (Buffer, Index, CellNo);
    
}

int SnowMapGen::LoadWalledFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    int Copied = 0;
    Copied += PlainMapGen::LoadCommonFloorFloats (Buffer, Index + Copied, CellNo);
    Copied += PlainMapGen::LoadCommonReachableWallFloats (Buffer, Index + Copied, CellNo);
    return Copied;
    
}

int SnowMapGen::LoadBlockedCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonCliffFloats (Buffer, Index, CellNo);
    
}

int SnowMapGen::LoadEnclosedCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonBoulderFloats (Buffer, Index, CellNo);
    
}

/**************************
 * Volcano map
 * 
 **************************/

VolcanoMapGen::VolcanoMapGen (Voronoi* Voro) : PlainMapGen (Voro) {
    
    int n;
    
    // Generate random texture coordinates and wall heights at all corners
    for (n = 0; n < 3*NoOfSites; n++) {
        CornerWallHeight[n] = 2.0f + 6.0f * static_cast<float>(rand()) / RandMax;
        CornerTexS[n] = static_cast<float>(rand()) / RandMax;
        CornerTexT[n] = static_cast<float>(rand()) / RandMax;
        CornerTe2S[n] = static_cast<float>(rand()) / RandMax;
        CornerTe2T[n] = static_cast<float>(rand()) / RandMax;
    }
    for (n = 0; n < NoOfSubsetSites; n++) {
        SiteTexS[n] = static_cast<float>(rand()) / RandMax;
        SiteTexT[n] = static_cast<float>(rand()) / RandMax;
    }
    
    // Set extra cell parameters
	ExtCellType = new int[NoOfSubsetSites];
	for (n = 0; n < NoOfSubsetSites; n++) {
		if (CellType[n] < 3) ExtCellType[n] = EXT_FLOOR;
		else if (CellType[n] == 3) ExtCellType[n] = EXT_CLIFF + (int)(2.0f * static_cast<float>(rand()) / RandMax);
		else ExtCellType[n] = EXT_BOULDER;
	}
    
}

VolcanoMapGen::~VolcanoMapGen () {
    
    delete[] ExtCellType;

}

int VolcanoMapGen::GetFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo);
    
}

int VolcanoMapGen::GetWalledFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo) + PlainMapGen::GetCommonReachableWallNoOfFloats (CellNo);
    
}

int VolcanoMapGen::GetBlockedCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonCliffNoOfFloats (CellNo);
    
}

int VolcanoMapGen::GetEnclosedCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonBoulderNoOfFloats (CellNo);
    
}

int VolcanoMapGen::LoadFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonFloorFloats (Buffer, Index, CellNo);
    
}

int VolcanoMapGen::LoadWalledFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    int Copied = 0;
    Copied += PlainMapGen::LoadCommonFloorFloats (Buffer, Index + Copied, CellNo);
    Copied += PlainMapGen::LoadCommonReachableWallFloats (Buffer, Index + Copied, CellNo);
    return Copied;
    
}

int VolcanoMapGen::LoadBlockedCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonCliffFloats (Buffer, Index, CellNo);
    
}

int VolcanoMapGen::LoadEnclosedCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonBoulderFloats (Buffer, Index, CellNo);
    
}

/**************************
 * Flowers map
 * 
 **************************/

FlowersMapGen::FlowersMapGen (Voronoi* Voro) : PlainMapGen (Voro) {
    
    int n, SubN;
    
    // Generate random texture coordinates and wall heights at all corners
    for (n = 0; n < 3*NoOfSites; n++) {
        CornerWallHeight[n] = 2.0f + 6.0f * static_cast<float>(rand()) / RandMax;
        CornerTexS[n] = static_cast<float>(rand()) / RandMax;
        CornerTexT[n] = static_cast<float>(rand()) / RandMax;
        CornerTe2S[n] = static_cast<float>(rand()) / RandMax;
        CornerTe2T[n] = static_cast<float>(rand()) / RandMax;
    }
    for (n = 0; n < NoOfSubsetSites; n++) {
        SiteTexS[n] = static_cast<float>(rand()) / RandMax;
        SiteTexT[n] = static_cast<float>(rand()) / RandMax;
    }
    
    // Set extra cell parameters
    //  0 or 1 to denote cliffs or lake for boundaries, and grass of water for traversible space
	ExtCellType = new int[NoOfSubsetSites];
	for (n = 0; n < NoOfSubsetSites; n++) {
		if (CellType[n] < 3) ExtCellType[n] = EXT_FLOOR;
		else if (CellType[n] == 3) ExtCellType[n] = EXT_CLIFF + (int)(3.0f * static_cast<float>(rand()) / RandMax);
		else ExtCellType[n] = EXT_RUBBLE + (int)(2.0f * static_cast<float>(rand()) / RandMax);
	}
    
    // Create extra data vectors
    // ExtCellType contains the number of deco items in each cell
	// ExtIntVector contains deco types
    // ExtFloatVector contains position and angle data
    ExtDecoCount = new int[NoOfSubsetSites];
    ExtDecoType = new int*[NoOfSubsetSites];
    ExtDecoParams = new float*[NoOfSubsetSites];
    for (n = 0; n < NoOfSubsetSites; n++) {
        if (CellType[n] < 3) {
            ExtDecoCount[n] = (int)(5.0f * (static_cast<float>(rand()) / RandMax)) + 1;
            ExtDecoType[n] = new int[ExtDecoCount[n]];
            ExtDecoParams[n] = new float[3*ExtDecoCount[n]];
            for (SubN = 0; SubN < ExtDecoCount[n]; SubN++) {
                ExtDecoType[n][SubN] = (int)(2.0f * (static_cast<float>(rand()) / RandMax)) + 2;
                ExtDecoParams[n][SubN*3  ] = SubSiteX[n] - 3.0f + 6.0f * (static_cast<float>(rand()) / RandMax);
                ExtDecoParams[n][SubN*3+1] = SubSiteY[n] - 3.0f + 6.0f * (static_cast<float>(rand()) / RandMax);
                ExtDecoParams[n][SubN*3+2] = 360.0f * (static_cast<float>(rand()) / RandMax);
            }
        }
        else ExtDecoCount[n] = 0;
    }
    
}

FlowersMapGen::~FlowersMapGen () {
    
    int n;
    
    for (n = 0; n < NoOfSubsetSites; n++) {
        if (ExtDecoCount[n] > 0) {
            delete[] ExtDecoType[n];
            delete[] ExtDecoParams[n];
        }
    }
    delete[] ExtCellType;
    delete[] ExtDecoCount;
    delete[] ExtDecoType;
    delete[] ExtDecoParams;
    
}

int FlowersMapGen::GetFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo) + PlainMapGen::GetCommonDecoNoOfFloats (CellNo, ExtDecoCount[CellNo], ExtDecoType[CellNo]);
    
}

int FlowersMapGen::GetWalledFloorCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonFloorNoOfFloats (CellNo) + PlainMapGen::GetCommonReachableWallNoOfFloats (CellNo)
    + PlainMapGen::GetCommonDecoNoOfFloats (CellNo, ExtDecoCount[CellNo], ExtDecoType[CellNo]);
    
}

int FlowersMapGen::GetBlockedCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonCliffNoOfFloats (CellNo);
    
}

int FlowersMapGen::GetEnclosedCellNoOfFloats (int CellNo) {
    
    return PlainMapGen::GetCommonBoulderNoOfFloats (CellNo);
    
}

int FlowersMapGen::LoadFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    int Copied = 0;
    Copied += PlainMapGen::LoadCommonFloorFloats (Buffer, Index + Copied, CellNo);
    Copied += PlainMapGen::LoadCommonDecoFloats (Buffer, Index + Copied, CellNo, ExtDecoCount[CellNo], ExtDecoType[CellNo], ExtDecoParams[CellNo]);
    return Copied;
    
}

int FlowersMapGen::LoadWalledFloorCellFloats (float* Buffer, int Index, int CellNo) {
    
    int Copied = 0;
    Copied += PlainMapGen::LoadCommonFloorFloats (Buffer, Index + Copied, CellNo);
    Copied += PlainMapGen::LoadCommonReachableWallFloats (Buffer, Index + Copied, CellNo);
    Copied += PlainMapGen::LoadCommonDecoFloats (Buffer, Index + Copied, CellNo, ExtDecoCount[CellNo], ExtDecoType[CellNo], ExtDecoParams[CellNo]);
    return Copied;
    
}

int FlowersMapGen::LoadBlockedCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonCliffFloats (Buffer, Index, CellNo);
    
}

int FlowersMapGen::LoadEnclosedCellFloats (float* Buffer, int Index, int CellNo) {
    
    return PlainMapGen::LoadCommonBoulderFloats (Buffer, Index, CellNo);
    
}
