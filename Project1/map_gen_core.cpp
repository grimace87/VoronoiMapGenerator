
#include "classes.hpp"
#include "mapdefs.hpp"

// Unique and shared map features
//  Forest
//   1 - Mossy rock cliff faces
//   2 - Ponds, sometimes fed by waterfalls
//   3 - Mossy rocks on the ground
//   4 - Trees, out of bounds but overhanging walkable space, often covered with moss.
//   5 - Autumn-like leaf cover
//     - Rocky cliffs bearing shrubs with scarce foliage (shared with lake, arid, volcano)
//     - Ponds with dirt or rock shorelines
//     - Stone rubble
//     - Large boulders
//     - Puddles
//  Lava cave
//   1 - Charred stone rubble
//   2 - Sloped lava rivers feeding pools of lava
//   3 - Arching stone formations
//   4 - Tall walls, occasionally with overhanging rock protrusions
//   5 - Dark blue stone pillars with brighter highlights, for more colour
//     - 
//     - 
//     - 
//     - 
//     - 
//  Lake
//   1 - Beach, stone and autumn-leaf shorelines
//   2 - Lily pad clusters near the shore
//   3 - Jetties and fishing platforms
//   4 - 
//   5 - 
//     - Rocky cliffs bearing shrubs with scarce foliage (shared with forest, arid, volcano)
//     - 
//     - 
//     - 
//     - 
//  Arid
//   1 - Dead logs
//   2 - Flower-covered rocks in out-of-bounds
//   3 - Shallow creeks
//   4 - Used farm allocations
//   5 - 
//     - Rocky cliffs bearing shrubs with scarce foliage (shared with forest, lake, volcano)
//     - 
//     - 
//     - 
//     - 
//  Snow
//   1 - Snow-covered rock cliffs
//   2 - Frozen lakes
//   3 - Elevated platforms over sunken areas
//   4 - Runoffs through rocky crevices, into unfrozen lakes
//   5 - 
//     - 
//     - 
//     - 
//     - 
//     - 
//  Cave
//   1 - Unaccessible, bottomless pits
//   2 - Shallow waters
//   3 - Rocky roof cover overhead, with occasional small openings letting light seep in
//   4 - Broken rocks on ground
//   5 - Cliffs with coloured highlights of red tarnish and green moss
//     - 
//     - 
//     - 
//     - 
//     - 
//  Volcano
//   1 - Exotic palm trees
//   2 - Dark stone patches with blackened logs and narrow lava creeks
//   3 - Pools with hot springs
//   4 - 
//   5 - 
//     - Rocky cliffs bearing shrubs with scarce foliage (shared with forest, lake, arid)
//     - 
//     - 
//     - 
//     - 
//  Flowers
//   1 - Tall flower trees
//   2 - Large fields of multi-coloured flowers
//   3 - Rivers with wooden bridges passing over, strewn with ivy
//   4 - Patches of a dark, grainy soil
//   5 - Moderately dense canopy with mildly colourful petals
//     - 
//     - 
//     - 
//     - 
//     - 

// Cell types
//  0 - Unused cell (should not exist in subset)
//  1 - Floor only
//  2 - Floor with walls
//  3 - Next to floor, untraversible ("blocked")
//  4 - Untraversible area (one or more cells) enclosed within floor space ("enclosed")

/**************************
 * Interface functions.
 * Create new map and get data
 **************************/

float* PlainMapGen::GenerateMapData (int* ReturnNoOfVertices, int TypeMode, Voronoi* Voro, char* ReturnTextureFilePath) {
    
    // Create new blank map object
    PlainMapGen *NewMap;
    float GetRand = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    switch (TypeMode) {
        case 0:
			if (GetRand > 0.875f) {
				NewMap = new ForestMapGen(Voro);
				strcpy(ReturnTextureFilePath, "\\res\\Forest.bmp");
			}
			else if (GetRand > 0.75f) {
				NewMap = new LavaCaveMapGen(Voro);
				strcpy(ReturnTextureFilePath, "\\res\\LavaCave.bmp");
			}
			else if (GetRand > 0.625f) {
				NewMap = new LakeMapGen(Voro);
				strcpy(ReturnTextureFilePath, "\\res\\Lake.bmp");
			}
			else if (GetRand > 0.5f) {
				NewMap = new AridMapGen(Voro);
				strcpy(ReturnTextureFilePath, "\\res\\Arid.bmp");
			}
			else if (GetRand > 0.375f) {
				NewMap = new CaveMapGen(Voro);
				strcpy(ReturnTextureFilePath, "\\res\\Cave.bmp");
			}
			else if (GetRand > 0.25f) {
				NewMap = new SnowMapGen(Voro);
				strcpy(ReturnTextureFilePath, "\\res\\Snow.bmp");
			}
			else if (GetRand > 0.125f) {
				NewMap = new VolcanoMapGen(Voro);
				strcpy(ReturnTextureFilePath, "\\res\\Volcano.bmp");
			}
			else {
				NewMap = new FlowersMapGen(Voro);
				strcpy(ReturnTextureFilePath, "\\res\\Flowers.bmp");
			}
            break;
        case 1:
			NewMap = new ForestMapGen(Voro);
			strcpy(ReturnTextureFilePath, "\\res\\Forest.bmp");
            break;
        case 2:
			NewMap = new LavaCaveMapGen(Voro);
			strcpy(ReturnTextureFilePath, "\\res\\LavaCave.bmp");
            break;
        case 3:
			NewMap = new LakeMapGen(Voro);
			strcpy(ReturnTextureFilePath, "\\res\\Lake.bmp");
            break;
        case 4:
			NewMap = new AridMapGen(Voro);
			strcpy(ReturnTextureFilePath, "\\res\\Arid.bmp");
            break;
        case 5:
			NewMap = new CaveMapGen(Voro);
			strcpy(ReturnTextureFilePath, "\\res\\Cave.bmp");
            break;
        case 6:
			NewMap = new SnowMapGen(Voro);
			strcpy(ReturnTextureFilePath, "\\res\\Snow.bmp");
            break;
        case 7:
			NewMap = new VolcanoMapGen(Voro);
			strcpy(ReturnTextureFilePath, "\\res\\Volcano.bmp");
            break;
        case 8:
			NewMap = new FlowersMapGen(Voro);
			strcpy(ReturnTextureFilePath, "\\res\\Flowers.bmp");
            break;
		default:
			NewMap = new ForestMapGen(Voro);
			strcpy(ReturnTextureFilePath, "\\res\\Forest.bmp");
            break;
    }
    
    // Perform some finishing touches on the map object
    NewMap->FinishObject();

    int TotalFloats = NewMap->GetTotalFloats ();
    *ReturnNoOfVertices = TotalFloats / 5;
    float* ReturnBuffer = NewMap->GetBuffer (TotalFloats);
    
    delete NewMap;
    return ReturnBuffer;
    
}

int PlainMapGen::GetTotalFloats () {
    
    // Calculate buffer size required and allocate the space
    int n;
    int TotalFloats = 0;
    for (n = 0; n < NoOfSubsetSites; n++) {
        // Floor only
        if (CellType[n] == 1) {
            TotalFloats += GetFloorCellNoOfFloats (n);
        }
        // Floor with walls around
        else if (CellType[n] == 2) {
            TotalFloats += GetWalledFloorCellNoOfFloats (n);
        }
        // Blocked area around the edges
        else if (CellType[n] == 3) {
            TotalFloats += GetBlockedCellNoOfFloats (n);
        }
        // Enclosed area within the floor space
        else if (CellType[n] == 4) {
            TotalFloats += GetEnclosedCellNoOfFloats (n);
        }
    }
    
    return TotalFloats;
    
}

float* PlainMapGen::GetBuffer (int NoOfFloats) {
    
    float *Buffer = new float[NoOfFloats];

    // Fill the buffer
    int n;
    int BuffIndex = 0;
    
    for (n = 0; n < NoOfSubsetSites; n++) {
        // Floor only
        if (CellType[n] == 1) {
            BuffIndex += LoadFloorCellFloats (Buffer, BuffIndex, n);
        }
        // Floor with walls around
        else if (CellType[n] == 2) {
            BuffIndex += LoadWalledFloorCellFloats (Buffer, BuffIndex, n);
        }
        // Blocked area around the edges
        else if (CellType[n] == 3) {
            BuffIndex += LoadBlockedCellFloats (Buffer, BuffIndex, n);
        }
        // Enclosed area within the floor space
        else if (CellType[n] == 4) {
            BuffIndex += LoadEnclosedCellFloats (Buffer, BuffIndex, n);
        }
    }
    
    return Buffer;
    
}

void PlainMapGen::FinishObject() {

    // Generate the extended cell types at corners
    int CellNo;
    int LineNo, Try;
    int GetCW, GetThis, GetCCW;
    for (CellNo = 0; CellNo < NoOfSubsetSites; CellNo++) {
        // Get initial codes for the last line
        Try = SubAdjacentSite[CellNo][SubLinesForThisSite[CellNo] - 1];
        if (Try < 0) GetThis = EXT_NONE;
        else GetThis = ExtCellType[Try];
        Try = SubAdjacentSite[CellNo][0];
        if (Try < 0) GetCW = EXT_NONE;
        else GetCW = ExtCellType[Try];
        // Go through all lines
        for (LineNo = 0; LineNo < SubLinesForThisSite[CellNo]; LineNo++) {
            GetCCW = GetThis;
            GetThis = GetCW;
            Try = LineNo + 1;
            if (Try >= SubLinesForThisSite[CellNo]) Try = 0;
            Try = SubAdjacentSite[CellNo][Try];
            if (Try < 0) GetCW = EXT_NONE;
            else GetCW = ExtCellType[Try];
            AdjExtCW[CellNo][LineNo] = GetCW;
            AdjExtCCW[CellNo][LineNo] = GetCCW;
        }
    }

}

PlainMapGen::PlainMapGen (Voronoi* Voro) {
    
    NoOfSubsetSites = Voro->NoOfSubsetSites;
    CellType = Voro->CellType;
    SubSiteX = Voro->SubSiteX;
    SubSiteY = Voro->SubSiteY;
    SubLinesForThisSite = Voro->SubLinesForThisSite;
    SubAdjacentSite = Voro->SubAdjacentSite;
    SubStartCorner = Voro->SubStartCorner;
    SubEndCorner = Voro->SubEndCorner;
    SubStartX = Voro->SubStartX;
    SubStartY = Voro->SubStartY;
    SubEndX = Voro->SubEndX;
    SubEndY = Voro->SubEndY;
    
}

PlainMapGen::~PlainMapGen () {
    
}

/**************************
 * Blank base functions.
 * Overridden and never used.
 **************************/

int PlainMapGen::GetFloorCellNoOfFloats (int CellNo) {return 0;}
int PlainMapGen::GetWalledFloorCellNoOfFloats (int CellNo) {return 0;}
int PlainMapGen::GetBlockedCellNoOfFloats (int CellNo) {return 0;}
int PlainMapGen::GetEnclosedCellNoOfFloats (int CellNo) {return 0;}

int PlainMapGen::LoadFloorCellFloats (float* Buffer, int Index, int CellNo) {return 0;}
int PlainMapGen::LoadWalledFloorCellFloats (float* Buffer, int Index, int CellNo) {return 0;}
int PlainMapGen::LoadBlockedCellFloats (float* Buffer, int Index, int CellNo) {return 0;}
int PlainMapGen::LoadEnclosedCellFloats (float* Buffer, int Index, int CellNo) {return 0;}

/**************************
 * Common functions.
 * Generate common structures.
 **************************/

int PlainMapGen::GetCommonFloorNoOfFloats (int CellNo) {
    
    return 15*SubLinesForThisSite[CellNo];
    
}

int PlainMapGen::GetCommonReachableWallNoOfFloats (int CellNo) {
    
    int TotalFloats = 0;
    int LineNo;
    
    for (LineNo = 0; LineNo < SubLinesForThisSite[CellNo]; LineNo++) {
        
        if (SubAdjacentSite[CellNo][LineNo] >= 0) {
            if (CellType[SubAdjacentSite[CellNo][LineNo]] == 1) continue;
            if (CellType[SubAdjacentSite[CellNo][LineNo]] == 2) continue;
            if (CellType[SubAdjacentSite[CellNo][LineNo]] == 3) continue;
        }
        
        TotalFloats += 30;
        
    }
    
    return TotalFloats;
    
}

int PlainMapGen::GetCommonUnreachableWallNoOfFloats (int CellNo) {
    
    int TotalFloats = 0;
    int LineNo;
    int Try;
    int ExtCW, ExtCCW;
    
    for (LineNo = 0; LineNo < SubLinesForThisSite[CellNo]; LineNo++) {
        if (SubAdjacentSite[CellNo][LineNo] >= 0) {
            if (ExtCellType[SubAdjacentSite[CellNo][LineNo]] == EXT_CLIFF) {
                Try = LineNo - 1;
                if (Try < 0) Try += SubLinesForThisSite[CellNo];
                if (SubAdjacentSite[CellNo][Try] >= 0)
                    ExtCCW = ExtCellType[SubAdjacentSite[CellNo][Try]];
                else ExtCCW = EXT_NONE;
                Try = LineNo + 1;
		        if (Try >= SubLinesForThisSite[CellNo]) Try -= SubLinesForThisSite[CellNo];
                if (SubAdjacentSite[CellNo][Try] >= 0)
                    ExtCW = ExtCellType[SubAdjacentSite[CellNo][Try]];
                else ExtCW = EXT_NONE;
                if ((ExtCCW == EXT_FLOOR) && (ExtCW == EXT_FLOOR)) continue;
                else if ((ExtCCW == EXT_FLOOR) || (ExtCW == EXT_FLOOR)) TotalFloats += 15;
                else TotalFloats += 30;
            }
        }
        else TotalFloats += 30;
    }
    return TotalFloats;
}

int PlainMapGen::GetCommonCliffNoOfFloats(int CellNo) {

	int LineNo;
	int TotalFloats = 0;
	int Try, TryCell;

	int Sig, SigCW, SigCCW;
	int Ext, ExtCW, ExtCCW;

	// Get signatures for LineNo = -1
	TryCell = SubAdjacentSite[CellNo][SubLinesForThisSite[CellNo] - 1];
	if (TryCell >= 0) {
		Sig = CellType[TryCell];
		Ext = ExtCellType[TryCell];
	}
	else {
		Sig = -1;
		Ext = EXT_NONE;
	}
	TryCell = SubAdjacentSite[CellNo][0];
	if (TryCell >= 0) {
		SigCW = CellType[TryCell];
		ExtCW = ExtCellType[TryCell];
	}
	else {
		SigCW = -1;
		ExtCW = EXT_NONE;
	}
    
    for (LineNo = 0; LineNo < SubLinesForThisSite[CellNo]; LineNo++) {

		// Shift signature codes across to new LineNo
		SigCCW = Sig;
		ExtCCW = Ext;
		Sig = SigCW;
		Ext = ExtCW;
		Try = LineNo + 1;
		if (Try >= SubLinesForThisSite[CellNo]) Try -= SubLinesForThisSite[CellNo];
        TryCell = SubAdjacentSite[CellNo][Try];
		if (TryCell >= 0) {
			SigCW = CellType[TryCell];
			ExtCW = ExtCellType[TryCell];
		}
		else {
			SigCW = -1;
			ExtCW = EXT_NONE;
		}
        
		// Get floats required depending on signatures around this line
		switch (Sig) {
		case -1:
			TotalFloats += 15;
            break;
		case 1:
		case 2:
			if ((ExtCW == EXT_NONE) && (ExtCCW == EXT_NONE))
				TotalFloats += 45;
			else if (ExtCCW == EXT_NONE)
				TotalFloats += 30;
			else if (ExtCW == EXT_NONE)
				TotalFloats += 30;
			else
				TotalFloats += 15;
            break;
		case 3:
		case 4:
			if ((ExtCW == EXT_FLOOR) && (ExtCCW == EXT_FLOOR))
				TotalFloats += 15;
            else if (ExtCCW == EXT_FLOOR)
                TotalFloats += 15;
            else if (ExtCW == EXT_FLOOR)
                TotalFloats += 15;
			else
				TotalFloats += 15;
			break;
		}
		
    }
    
    return TotalFloats;
    
}

int PlainMapGen::GetCommonBoulderNoOfFloats (int CellNo) {
    
    return 15*SubLinesForThisSite[CellNo];
    
}

int PlainMapGen::GetCommonSunkenNoOfFloats (int CellNo) {
    
    int LineNo;
    int TotalFloats = 0;
    int AdjSite;
    int Try;
    
    for (LineNo = 0; LineNo < SubLinesForThisSite[CellNo]; LineNo++) {
        // Standard is 45 floats
        //  For adjacent sunken cells, this is for the base to the boundary and bank segments on the sides.
        //  For no adjacent sunken cells, this is for the base to the bank, and for the bank itself.
        // Exception is where there are adjacent sunken cells, as well as more on either side (or both) of the boundary.
        AdjSite = SubAdjacentSite[CellNo][LineNo];
        if (AdjSite >= 0) {
            if (ExtCellType[AdjSite] == EXT_POND) {
                TotalFloats += 15;
                Try = LineNo - 1; if (Try < 0) Try += SubLinesForThisSite[CellNo];
                if (SubAdjacentSite[CellNo][Try] >= 0) {
                    if (ExtCellType[SubAdjacentSite[CellNo][Try]] != EXT_POND)
                        TotalFloats += 15;
                }
                else TotalFloats += 15;
                Try = LineNo + 1; if (Try >= SubLinesForThisSite[CellNo]) Try -= SubLinesForThisSite[CellNo];
                if (SubAdjacentSite[CellNo][Try] >= 0) {
                    if (ExtCellType[SubAdjacentSite[CellNo][Try]] != EXT_POND)
                        TotalFloats += 15;
                }
                else TotalFloats += 15;
            }
            else
                TotalFloats += 45;
        }
        else
            TotalFloats += 45;
    }
    
    return TotalFloats;
    
}

int PlainMapGen::GetCommonRubbleNoOfFloats (int CellNo) {
    
    int TotalFloats = 0;

    int n;
    float HeightCW, HeightCCW;
    float MaxHeight;
    float Len, dX, dY;

    for (n = 0; n < SubLinesForThisSite[CellNo]; n++) {
        
        TotalFloats += 15;
        if (SubAdjacentSite[CellNo][n] < 0)  continue;
        if (ExtCellType[SubAdjacentSite[CellNo][n]] != EXT_FLOOR) continue;

        HeightCCW = CornerWallHeight[SubStartCorner[CellNo][n]];
        HeightCW = CornerWallHeight[SubEndCorner[CellNo][n]];

        if (HeightCW < HeightCCW) MaxHeight = HeightCCW;
        else MaxHeight = HeightCW;
        
        dX = SubEndX[CellNo][n] - SubStartX[CellNo][n];
        dY = SubEndY[CellNo][n] - SubStartY[CellNo][n];
        Len = (float)sqrt((double)(dX*dX + dY*dY));

        if ((MaxHeight > 3.0f) && (Len > 8.0f))
            TotalFloats += 510;
        else if ((MaxHeight > 5.5f) && (Len > 8.0f))
            TotalFloats += 210;
        else
            TotalFloats += 210;

    }

    return TotalFloats;
    
}

int PlainMapGen::GetCommonDecoNoOfFloats (int CellNo, int NoOfDecos, int* DecoTypes) {
    
    int TotalFloats = 0;
    int n;
    
    for (n = 0; n < NoOfDecos; n++) {
        switch (DecoTypes[n]) {
            case 0: TotalFloats += 45; break;
            case 1: TotalFloats += 90; break;
        }
    }
    
    return TotalFloats;
    
}

int PlainMapGen::LoadCommonFloorFloats (float* Buffer, int Index, int CellNo) {
    
    int LineNo;
    int BIndex;
    
    for (LineNo = 0; LineNo < SubLinesForThisSite[CellNo]; LineNo++) {
        
        BIndex = Index + 15*LineNo;
        
        // Draw floor polygon
        
        Buffer[BIndex   ] = SubSiteX[CellNo];
        Buffer[BIndex+1 ] = SubSiteY[CellNo];
        Buffer[BIndex+2 ] = 0.0f;
        Buffer[BIndex+3 ] = GrndTexS + SiteTexS[CellNo]*GrndTexW;
        Buffer[BIndex+4 ] = GrndTexT + SiteTexT[CellNo]*GrndTexH;
        
        Buffer[BIndex+5 ] = SubStartX[CellNo][LineNo];
        Buffer[BIndex+6 ] = SubStartY[CellNo][LineNo];
        Buffer[BIndex+7 ] = 0.0f;
        Buffer[BIndex+8 ] = GrndTexS + CornerTexS[SubStartCorner[CellNo][LineNo]]*GrndTexW;
        Buffer[BIndex+9 ] = GrndTexT + CornerTexT[SubStartCorner[CellNo][LineNo]]*GrndTexH;
        
        Buffer[BIndex+10] = SubEndX[CellNo][LineNo];
        Buffer[BIndex+11] = SubEndY[CellNo][LineNo];
        Buffer[BIndex+12] = 0.0f;
        Buffer[BIndex+13] = GrndTexS + CornerTexS[SubEndCorner[CellNo][LineNo]]*GrndTexW;
        Buffer[BIndex+14] = GrndTexT + CornerTexT[SubEndCorner[CellNo][LineNo]]*GrndTexH;
        
    }
    
    return 15*SubLinesForThisSite[CellNo];
    
}

int PlainMapGen::LoadCommonReachableWallFloats (float* Buffer, int Index, int CellNo) {
    
    int LineNo;
    int BIndex = Index;
    int TotalFloats = 0;
    
    for (LineNo = 0; LineNo < SubLinesForThisSite[CellNo]; LineNo++) {
        
        if (SubAdjacentSite[CellNo][LineNo] >= 0) {
            if (CellType[SubAdjacentSite[CellNo][LineNo]] == 1) continue;
            if (CellType[SubAdjacentSite[CellNo][LineNo]] == 2) continue;
            if (CellType[SubAdjacentSite[CellNo][LineNo]] == 3) continue;
        }
        
        Buffer[BIndex   ] = SubStartX[CellNo][LineNo];
        Buffer[BIndex+1 ] = SubStartY[CellNo][LineNo];
        Buffer[BIndex+2 ] = 0.0f;
        Buffer[BIndex+3 ] = WallTexS + CornerTexS[SubStartCorner[CellNo][LineNo]]*WallTexW;
        Buffer[BIndex+4 ] = WallTexT + CornerTexT[SubStartCorner[CellNo][LineNo]]*WallTexH;
        
        Buffer[BIndex+5 ] = SubEndX[CellNo][LineNo];
        Buffer[BIndex+6 ] = SubEndY[CellNo][LineNo];
        Buffer[BIndex+7 ] = 0.0f;
        Buffer[BIndex+8 ] = WallTexS + CornerTexS[SubEndCorner[CellNo][LineNo]]*WallTexW;
        Buffer[BIndex+9 ] = WallTexT + CornerTexT[SubEndCorner[CellNo][LineNo]]*WallTexH;
        
        Buffer[BIndex+10] = SubEndX[CellNo][LineNo];
        Buffer[BIndex+11] = SubEndY[CellNo][LineNo];
        Buffer[BIndex+12] = CornerWallHeight[SubEndCorner[CellNo][LineNo]];
        Buffer[BIndex+13] = WallTexS + CornerTe2S[SubEndCorner[CellNo][LineNo]]*WallTexW;
        Buffer[BIndex+14] = WallTexT + CornerTe2T[SubEndCorner[CellNo][LineNo]]*WallTexH;
        
        Buffer[BIndex+15] = SubEndX[CellNo][LineNo];
        Buffer[BIndex+16] = SubEndY[CellNo][LineNo];
        Buffer[BIndex+17] = CornerWallHeight[SubEndCorner[CellNo][LineNo]];
        Buffer[BIndex+18] = WallTexS + CornerTe2S[SubEndCorner[CellNo][LineNo]]*WallTexW;
        Buffer[BIndex+19] = WallTexT + CornerTe2T[SubEndCorner[CellNo][LineNo]]*WallTexH;
        
        Buffer[BIndex+20] = SubStartX[CellNo][LineNo];
        Buffer[BIndex+21] = SubStartY[CellNo][LineNo];
        Buffer[BIndex+22] = CornerWallHeight[SubStartCorner[CellNo][LineNo]];
        Buffer[BIndex+23] = WallTexS + CornerTe2S[SubStartCorner[CellNo][LineNo]]*WallTexW;
        Buffer[BIndex+24] = WallTexT + CornerTe2T[SubStartCorner[CellNo][LineNo]]*WallTexH;
        
        Buffer[BIndex+25] = SubStartX[CellNo][LineNo];
        Buffer[BIndex+26] = SubStartY[CellNo][LineNo];
        Buffer[BIndex+27] = 0.0f;
        Buffer[BIndex+28] = WallTexS + CornerTexS[SubStartCorner[CellNo][LineNo]]*WallTexW;
        Buffer[BIndex+29] = WallTexT + CornerTexT[SubStartCorner[CellNo][LineNo]]*WallTexH;
        
        BIndex += 30;
        
        TotalFloats += 30;
        
    }
        
    return TotalFloats;
    
}

int PlainMapGen::LoadCommonUnreachableWallFloats (float* Buffer, int Index, int CellNo) {
    
    int LineNo;
    int BIndex = Index;
    int TotalFloats = 0;
    int Try;
    int ExtCW, ExtCCW;
    float CWHeight, CCWHeight;
    
    for (LineNo = 0; LineNo < SubLinesForThisSite[CellNo]; LineNo++) {
        
        CWHeight = CornerWallHeight[SubEndCorner[CellNo][LineNo]];
        CCWHeight = CornerWallHeight[SubStartCorner[CellNo][LineNo]];

        if (SubAdjacentSite[CellNo][LineNo] >= 0) {
            if (ExtCellType[SubAdjacentSite[CellNo][LineNo]] == EXT_CLIFF) {
                // Get surrounding Ext signatures
                Try = LineNo - 1;
                if (Try < 0) Try += SubLinesForThisSite[CellNo];
                if (SubAdjacentSite[CellNo][Try] >= 0)
                    ExtCCW = ExtCellType[SubAdjacentSite[CellNo][Try]];
                else ExtCCW = EXT_NONE;
                Try = LineNo + 1;
		        if (Try >= SubLinesForThisSite[CellNo]) Try -= SubLinesForThisSite[CellNo];
                if (SubAdjacentSite[CellNo][Try] >= 0)
                    ExtCW = ExtCellType[SubAdjacentSite[CellNo][Try]];
                else ExtCW = EXT_NONE;
                if (ExtCCW == EXT_NONE) CCWHeight *= 3.0f;
                if (ExtCW == EXT_NONE) CWHeight *= 3.0f;
                // Generate wall
                if ((ExtCCW == EXT_FLOOR) && (ExtCW == EXT_FLOOR)) continue;
                else if (ExtCCW == EXT_FLOOR) {

                    Buffer[BIndex   ] = SubStartX[CellNo][LineNo];
                    Buffer[BIndex+1 ] = SubStartY[CellNo][LineNo];
                    Buffer[BIndex+2 ] = 0.0f;
                    Buffer[BIndex+3 ] = WallTexS + CornerTexS[SubStartCorner[CellNo][LineNo]]*WallTexW;
                    Buffer[BIndex+4 ] = WallTexT + CornerTexT[SubStartCorner[CellNo][LineNo]]*WallTexH;
                    
                    Buffer[BIndex+5 ] = SubEndX[CellNo][LineNo];
                    Buffer[BIndex+6 ] = SubEndY[CellNo][LineNo];
                    Buffer[BIndex+7 ] = 0.0f;
                    Buffer[BIndex+8 ] = WallTexS + CornerTexS[SubEndCorner[CellNo][LineNo]]*WallTexW;
                    Buffer[BIndex+9 ] = WallTexT + CornerTexT[SubEndCorner[CellNo][LineNo]]*WallTexH;
                    
                    Buffer[BIndex+10] = SubEndX[CellNo][LineNo];
                    Buffer[BIndex+11] = SubEndY[CellNo][LineNo];
                    Buffer[BIndex+12] = CWHeight;
                    Buffer[BIndex+13] = WallTexS + CornerTe2S[SubEndCorner[CellNo][LineNo]]*WallTexW;
                    Buffer[BIndex+14] = WallTexT + CornerTe2T[SubEndCorner[CellNo][LineNo]]*WallTexH;

                    BIndex += 15;
                    TotalFloats += 15;
                }
                else if (ExtCW == EXT_FLOOR) {

                    Buffer[BIndex   ] = SubStartX[CellNo][LineNo];
                    Buffer[BIndex+1 ] = SubStartY[CellNo][LineNo];
                    Buffer[BIndex+2 ] = CCWHeight;
                    Buffer[BIndex+3 ] = WallTexS + CornerTe2S[SubStartCorner[CellNo][LineNo]]*WallTexW;
                    Buffer[BIndex+4 ] = WallTexT + CornerTe2T[SubStartCorner[CellNo][LineNo]]*WallTexH;
                    
                    Buffer[BIndex+5 ] = SubStartX[CellNo][LineNo];
                    Buffer[BIndex+6 ] = SubStartY[CellNo][LineNo];
                    Buffer[BIndex+7 ] = 0.0f;
                    Buffer[BIndex+8 ] = WallTexS + CornerTexS[SubStartCorner[CellNo][LineNo]]*WallTexW;
                    Buffer[BIndex+9 ] = WallTexT + CornerTexT[SubStartCorner[CellNo][LineNo]]*WallTexH;
                    
                    Buffer[BIndex+10] = SubEndX[CellNo][LineNo];
                    Buffer[BIndex+11] = SubEndY[CellNo][LineNo];
                    Buffer[BIndex+12] = 0.0f;
                    Buffer[BIndex+13] = WallTexS + CornerTexS[SubEndCorner[CellNo][LineNo]]*WallTexW;
                    Buffer[BIndex+14] = WallTexT + CornerTexT[SubEndCorner[CellNo][LineNo]]*WallTexH;

                    BIndex += 15;
                    TotalFloats += 15;
                }
                else {
                    
                    Buffer[BIndex   ] = SubStartX[CellNo][LineNo];
                    Buffer[BIndex+1 ] = SubStartY[CellNo][LineNo];
                    Buffer[BIndex+2 ] = 0.0f;
                    Buffer[BIndex+3 ] = WallTexS + CornerTexS[SubStartCorner[CellNo][LineNo]]*WallTexW;
                    Buffer[BIndex+4 ] = WallTexT + CornerTexT[SubStartCorner[CellNo][LineNo]]*WallTexH;
                    
                    Buffer[BIndex+5 ] = SubEndX[CellNo][LineNo];
                    Buffer[BIndex+6 ] = SubEndY[CellNo][LineNo];
                    Buffer[BIndex+7 ] = 0.0f;
                    Buffer[BIndex+8 ] = WallTexS + CornerTexS[SubEndCorner[CellNo][LineNo]]*WallTexW;
                    Buffer[BIndex+9 ] = WallTexT + CornerTexT[SubEndCorner[CellNo][LineNo]]*WallTexH;
                    
                    Buffer[BIndex+10] = SubEndX[CellNo][LineNo];
                    Buffer[BIndex+11] = SubEndY[CellNo][LineNo];
                    Buffer[BIndex+12] = CWHeight;
                    Buffer[BIndex+13] = WallTexS + CornerTe2S[SubEndCorner[CellNo][LineNo]]*WallTexW;
                    Buffer[BIndex+14] = WallTexT + CornerTe2T[SubEndCorner[CellNo][LineNo]]*WallTexH;
                    
                    Buffer[BIndex+15] = SubEndX[CellNo][LineNo];
                    Buffer[BIndex+16] = SubEndY[CellNo][LineNo];
                    Buffer[BIndex+17] = CWHeight;
                    Buffer[BIndex+18] = WallTexS + CornerTe2S[SubEndCorner[CellNo][LineNo]]*WallTexW;
                    Buffer[BIndex+19] = WallTexT + CornerTe2T[SubEndCorner[CellNo][LineNo]]*WallTexH;
                    
                    Buffer[BIndex+20] = SubStartX[CellNo][LineNo];
                    Buffer[BIndex+21] = SubStartY[CellNo][LineNo];
                    Buffer[BIndex+22] = CCWHeight;
                    Buffer[BIndex+23] = WallTexS + CornerTe2S[SubStartCorner[CellNo][LineNo]]*WallTexW;
                    Buffer[BIndex+24] = WallTexT + CornerTe2T[SubStartCorner[CellNo][LineNo]]*WallTexH;
                    
                    Buffer[BIndex+25] = SubStartX[CellNo][LineNo];
                    Buffer[BIndex+26] = SubStartY[CellNo][LineNo];
                    Buffer[BIndex+27] = 0.0f;
                    Buffer[BIndex+28] = WallTexS + CornerTexS[SubStartCorner[CellNo][LineNo]]*WallTexW;
                    Buffer[BIndex+29] = WallTexT + CornerTexT[SubStartCorner[CellNo][LineNo]]*WallTexH;
                    
                    BIndex += 30;
                    TotalFloats += 30;
                    
                }
            }
        }
        else {

            // Get surrounding Ext signatures
            Try = LineNo - 1;
            if (Try < 0) Try += SubLinesForThisSite[CellNo];
            if (SubAdjacentSite[CellNo][Try] >= 0)
                if (ExtCellType[SubAdjacentSite[CellNo][Try]] == EXT_CLIFF) CCWHeight *= 3.0f;
            Try = LineNo + 1;
		    if (Try >= SubLinesForThisSite[CellNo]) Try -= SubLinesForThisSite[CellNo];
            if (SubAdjacentSite[CellNo][Try] >= 0)
                if (ExtCellType[SubAdjacentSite[CellNo][Try]] == EXT_CLIFF) CWHeight *= 3.0f;

            Buffer[BIndex   ] = SubStartX[CellNo][LineNo];
            Buffer[BIndex+1 ] = SubStartY[CellNo][LineNo];
            Buffer[BIndex+2 ] = 0.0f;
            Buffer[BIndex+3 ] = WallTexS + CornerTexS[SubStartCorner[CellNo][LineNo]]*WallTexW;
            Buffer[BIndex+4 ] = WallTexT + CornerTexT[SubStartCorner[CellNo][LineNo]]*WallTexH;
            
            Buffer[BIndex+5 ] = SubEndX[CellNo][LineNo];
            Buffer[BIndex+6 ] = SubEndY[CellNo][LineNo];
            Buffer[BIndex+7 ] = 0.0f;
            Buffer[BIndex+8 ] = WallTexS + CornerTexS[SubEndCorner[CellNo][LineNo]]*WallTexW;
            Buffer[BIndex+9 ] = WallTexT + CornerTexT[SubEndCorner[CellNo][LineNo]]*WallTexH;
            
            Buffer[BIndex+10] = SubEndX[CellNo][LineNo];
            Buffer[BIndex+11] = SubEndY[CellNo][LineNo];
            Buffer[BIndex+12] = CWHeight;
            Buffer[BIndex+13] = WallTexS + CornerTe2S[SubEndCorner[CellNo][LineNo]]*WallTexW;
            Buffer[BIndex+14] = WallTexT + CornerTe2T[SubEndCorner[CellNo][LineNo]]*WallTexH;
            
            Buffer[BIndex+15] = SubEndX[CellNo][LineNo];
            Buffer[BIndex+16] = SubEndY[CellNo][LineNo];
            Buffer[BIndex+17] = CWHeight;
            Buffer[BIndex+18] = WallTexS + CornerTe2S[SubEndCorner[CellNo][LineNo]]*WallTexW;
            Buffer[BIndex+19] = WallTexT + CornerTe2T[SubEndCorner[CellNo][LineNo]]*WallTexH;
            
            Buffer[BIndex+20] = SubStartX[CellNo][LineNo];
            Buffer[BIndex+21] = SubStartY[CellNo][LineNo];
            Buffer[BIndex+22] = CCWHeight;
            Buffer[BIndex+23] = WallTexS + CornerTe2S[SubStartCorner[CellNo][LineNo]]*WallTexW;
            Buffer[BIndex+24] = WallTexT + CornerTe2T[SubStartCorner[CellNo][LineNo]]*WallTexH;
            
            Buffer[BIndex+25] = SubStartX[CellNo][LineNo];
            Buffer[BIndex+26] = SubStartY[CellNo][LineNo];
            Buffer[BIndex+27] = 0.0f;
            Buffer[BIndex+28] = WallTexS + CornerTexS[SubStartCorner[CellNo][LineNo]]*WallTexW;
            Buffer[BIndex+29] = WallTexT + CornerTexT[SubStartCorner[CellNo][LineNo]]*WallTexH;
            
            BIndex += 30;
            TotalFloats += 30;

        }
    }
        
    return TotalFloats;
    
}

int PlainMapGen::LoadCommonCliffFloats (float* Buffer, int Index, int CellNo) {
    
	int LineNo;
	int BIndex = Index;
	int TotalFloats = 0;
	int Try, TryCell;

	int Sig, SigCW, SigCCW;
	int Ext, ExtCW, ExtCCW;
	float HeightCW, HeightCCW;
	int CornerCW, CornerCCW;

	// Get signatures for LineNo = -1
	TryCell = SubAdjacentSite[CellNo][SubLinesForThisSite[CellNo] - 1];
	if (TryCell >= 0) {
		Sig = CellType[TryCell];
		Ext = ExtCellType[TryCell];
	}
	else {
		Sig = -1;
		Ext = EXT_NONE;
	}
    TryCell = SubAdjacentSite[CellNo][0];
	if (TryCell >= 0) {
		SigCW = CellType[TryCell];
		ExtCW = ExtCellType[TryCell];
	}
	else {
		SigCW = -1;
		ExtCW = EXT_NONE;
	}
	CornerCW = SubStartCorner[CellNo][0];

    for (LineNo = 0; LineNo < SubLinesForThisSite[CellNo]; LineNo++) {

		// Shift signature codes across to new LineNo
		CornerCCW = CornerCW;
		CornerCW = SubEndCorner[CellNo][LineNo];
		SigCCW = Sig;
		ExtCCW = Ext;
		Sig = SigCW;
		Ext = ExtCW;
		Try = LineNo + 1;
		if (Try >= SubLinesForThisSite[CellNo]) Try -= SubLinesForThisSite[CellNo];
        TryCell = SubAdjacentSite[CellNo][Try];
		if (TryCell >= 0) {
			SigCW = CellType[TryCell];
			ExtCW = ExtCellType[TryCell];
		}
		else {
			SigCW = -1;
			ExtCW = EXT_NONE;
		}

		// Get floats required depending on signatures around this line
		switch (Sig) {
		case -1:

			// Tall cliff face at the boundary of the map

			if (ExtCCW == EXT_FLOOR) HeightCCW = CornerWallHeight[CornerCCW];
			else HeightCCW = 3.0f * CornerWallHeight[CornerCCW];
			if (ExtCW == EXT_FLOOR) HeightCW = CornerWallHeight[CornerCW];
			else HeightCW = 3.0f * CornerWallHeight[CornerCW];

			Buffer[BIndex] = SubSiteX[CellNo];
			Buffer[BIndex + 1] = SubSiteY[CellNo];
			Buffer[BIndex + 2] = 2.0f;
			Buffer[BIndex + 3] = WallTexS + SiteTexS[CellNo] * WallTexW;
			Buffer[BIndex + 4] = WallTexT + SiteTexT[CellNo] * WallTexH;

			Buffer[BIndex + 5] = SubStartX[CellNo][LineNo];
			Buffer[BIndex + 6] = SubStartY[CellNo][LineNo];
			Buffer[BIndex + 7] = HeightCCW;
			Buffer[BIndex + 8] = WallTexS + CornerTe2S[CornerCCW] * WallTexW;
			Buffer[BIndex + 9] = WallTexT + CornerTe2T[CornerCCW] * WallTexH;

			Buffer[BIndex + 10] = SubEndX[CellNo][LineNo];
			Buffer[BIndex + 11] = SubEndY[CellNo][LineNo];
			Buffer[BIndex + 12] = HeightCW;
			Buffer[BIndex + 13] = WallTexS + CornerTe2S[CornerCW] * WallTexW;
			Buffer[BIndex + 14] = WallTexT + CornerTe2T[CornerCW] * WallTexH;

			TotalFloats += 15;
			BIndex += 15;

            break;
		case 1:
		case 2:
			if ((ExtCW == EXT_NONE) && (ExtCCW == EXT_NONE)) {

				// Edges meet vacancies on both sides

				Buffer[BIndex] = SubSiteX[CellNo];
				Buffer[BIndex + 1] = SubSiteY[CellNo];
				Buffer[BIndex + 2] = 2.0f;
				Buffer[BIndex + 3] = WallTexS + SiteTexS[CellNo] * WallTexW;
				Buffer[BIndex + 4] = WallTexT + SiteTexT[CellNo] * WallTexH;

				Buffer[BIndex + 5] = SubStartX[CellNo][LineNo];
				Buffer[BIndex + 6] = SubStartY[CellNo][LineNo];
				Buffer[BIndex + 7] = CornerWallHeight[CornerCCW];
				Buffer[BIndex + 8] = WallTexS + CornerTe2S[CornerCCW] * WallTexW;
				Buffer[BIndex + 9] = WallTexT + CornerTe2T[CornerCCW] * WallTexH;

				Buffer[BIndex + 10] = SubEndX[CellNo][LineNo];
				Buffer[BIndex + 11] = SubEndY[CellNo][LineNo];
				Buffer[BIndex + 12] = CornerWallHeight[CornerCW];
				Buffer[BIndex + 13] = WallTexS + CornerTe2S[CornerCW] * WallTexW;
				Buffer[BIndex + 14] = WallTexT + CornerTe2T[CornerCW] * WallTexH;

                Buffer[BIndex + 15] = SubEndX[CellNo][LineNo];
				Buffer[BIndex + 16] = SubEndY[CellNo][LineNo];
				Buffer[BIndex + 17] = CornerWallHeight[CornerCW];
				Buffer[BIndex + 18] = WallTexS + CornerTe2S[CornerCW] * WallTexW;
				Buffer[BIndex + 19] = WallTexT + CornerTe2T[CornerCW] * WallTexH;

				Buffer[BIndex + 20] = SubEndX[CellNo][LineNo];
				Buffer[BIndex + 21] = SubEndY[CellNo][LineNo];
				Buffer[BIndex + 22] = 0.0f;
				Buffer[BIndex + 23] = WallTexS + CornerTexS[CornerCW] * WallTexW;
				Buffer[BIndex + 24] = WallTexT + CornerTexT[CornerCW] * WallTexH;

				Buffer[BIndex + 25] = SubStartX[CellNo][LineNo];
				Buffer[BIndex + 26] = SubStartY[CellNo][LineNo];
				Buffer[BIndex + 27] = 0.0f;
				Buffer[BIndex + 28] = WallTexS + CornerTexS[CornerCCW] * WallTexW;
				Buffer[BIndex + 29] = WallTexT + CornerTexT[CornerCCW] * WallTexH;

				Buffer[BIndex + 30] = SubStartX[CellNo][LineNo];
				Buffer[BIndex + 31] = SubStartY[CellNo][LineNo];
				Buffer[BIndex + 32] = 0.0f;
				Buffer[BIndex + 33] = WallTexS + CornerTexS[CornerCCW] * WallTexW;
				Buffer[BIndex + 34] = WallTexT + CornerTexT[CornerCCW] * WallTexH;

				Buffer[BIndex + 35] = SubStartX[CellNo][LineNo];
				Buffer[BIndex + 36] = SubStartY[CellNo][LineNo];
				Buffer[BIndex + 37] = CornerWallHeight[CornerCCW];
				Buffer[BIndex + 38] = WallTexS + CornerTe2S[CornerCCW] * WallTexW;
				Buffer[BIndex + 39] = WallTexT + CornerTe2T[CornerCCW] * WallTexH;

				Buffer[BIndex + 40] = SubEndX[CellNo][LineNo];
				Buffer[BIndex + 41] = SubEndY[CellNo][LineNo];
				Buffer[BIndex + 42] = CornerWallHeight[CornerCW];
				Buffer[BIndex + 43] = WallTexS + CornerTe2S[CornerCW] * WallTexW;
				Buffer[BIndex + 44] = WallTexT + CornerTe2T[CornerCW] * WallTexH;

				TotalFloats += 45;
				BIndex += 45;

			}
			else if (ExtCCW == EXT_NONE) {

				// Wall height at CCW side (floor on CW side)

				Buffer[BIndex    ] = SubSiteX[CellNo];
				Buffer[BIndex + 1] = SubSiteY[CellNo];
				Buffer[BIndex + 2] = 2.0f;
				Buffer[BIndex + 3] = WallTexS + SiteTexS[CellNo] * WallTexW;
				Buffer[BIndex + 4] = WallTexT + SiteTexT[CellNo] * WallTexH;

				Buffer[BIndex + 5] = SubEndX[CellNo][LineNo];
				Buffer[BIndex + 6] = SubEndY[CellNo][LineNo];
				Buffer[BIndex + 7] = 0.0f;
				Buffer[BIndex + 8] = WallTexS + CornerTexS[CornerCW] * WallTexW;
				Buffer[BIndex + 9] = WallTexT + CornerTexT[CornerCW] * WallTexH;

				Buffer[BIndex + 10] = SubStartX[CellNo][LineNo];
				Buffer[BIndex + 11] = SubStartY[CellNo][LineNo];
				Buffer[BIndex + 12] = CornerWallHeight[CornerCCW];
				Buffer[BIndex + 13] = WallTexS + CornerTe2S[CornerCCW] * WallTexW;
				Buffer[BIndex + 14] = WallTexT + CornerTe2T[CornerCCW] * WallTexH;

				Buffer[BIndex + 15] = SubStartX[CellNo][LineNo];
				Buffer[BIndex + 16] = SubStartY[CellNo][LineNo];
				Buffer[BIndex + 17] = CornerWallHeight[CornerCCW];
				Buffer[BIndex + 18] = WallTexS + CornerTe2S[CornerCCW] * WallTexW;
				Buffer[BIndex + 19] = WallTexT + CornerTe2T[CornerCCW] * WallTexH;

				Buffer[BIndex + 20] = SubEndX[CellNo][LineNo];
				Buffer[BIndex + 21] = SubEndY[CellNo][LineNo];
				Buffer[BIndex + 22] = 0.0f;
				Buffer[BIndex + 23] = WallTexS + CornerTexS[CornerCW] * WallTexW;
				Buffer[BIndex + 24] = WallTexT + CornerTexT[CornerCW] * WallTexH;

				Buffer[BIndex + 25] = SubStartX[CellNo][LineNo];
				Buffer[BIndex + 26] = SubStartY[CellNo][LineNo];
				Buffer[BIndex + 27] = 0.0f;
				Buffer[BIndex + 28] = WallTexS + CornerTexS[CornerCCW] * WallTexW;
				Buffer[BIndex + 29] = WallTexT + CornerTexT[CornerCCW] * WallTexH;

				TotalFloats += 30;
				BIndex += 30;

			}
			else if (ExtCW == EXT_NONE) {

				// Wall height at CW side (floor on CCW side)

				Buffer[BIndex    ] = SubSiteX[CellNo];
				Buffer[BIndex + 1] = SubSiteY[CellNo];
				Buffer[BIndex + 2] = 2.0f;
				Buffer[BIndex + 3] = WallTexS + SiteTexS[CellNo] * WallTexW;
				Buffer[BIndex + 4] = WallTexT + SiteTexT[CellNo] * WallTexH;

				Buffer[BIndex + 5] = SubEndX[CellNo][LineNo];
				Buffer[BIndex + 6] = SubEndY[CellNo][LineNo];
				Buffer[BIndex + 7] = CornerWallHeight[CornerCW];
				Buffer[BIndex + 8] = WallTexS + CornerTe2S[CornerCW] * WallTexW;
				Buffer[BIndex + 9] = WallTexT + CornerTe2T[CornerCW] * WallTexH;

				Buffer[BIndex + 10] = SubStartX[CellNo][LineNo];
				Buffer[BIndex + 11] = SubStartY[CellNo][LineNo];
				Buffer[BIndex + 12] = 0.0f;
				Buffer[BIndex + 13] = WallTexS + CornerTexS[CornerCCW] * WallTexW;
				Buffer[BIndex + 14] = WallTexT + CornerTexT[CornerCCW] * WallTexH;

				Buffer[BIndex + 15] = SubEndX[CellNo][LineNo];
				Buffer[BIndex + 16] = SubEndY[CellNo][LineNo];
				Buffer[BIndex + 17] = CornerWallHeight[CornerCW];
				Buffer[BIndex + 18] = WallTexS + CornerTe2S[CornerCW] * WallTexW;
				Buffer[BIndex + 19] = WallTexT + CornerTe2T[CornerCW] * WallTexH;

				Buffer[BIndex + 20] = SubEndX[CellNo][LineNo];
				Buffer[BIndex + 21] = SubEndY[CellNo][LineNo];
				Buffer[BIndex + 22] = 0.0f;
				Buffer[BIndex + 23] = WallTexS + CornerTexS[CornerCW] * WallTexW;
				Buffer[BIndex + 24] = WallTexT + CornerTexT[CornerCW] * WallTexH;

				Buffer[BIndex + 25] = SubStartX[CellNo][LineNo];
				Buffer[BIndex + 26] = SubStartY[CellNo][LineNo];
				Buffer[BIndex + 27] = 0.0f;
				Buffer[BIndex + 28] = WallTexS + CornerTexS[CornerCCW] * WallTexW;
				Buffer[BIndex + 29] = WallTexT + CornerTexT[CornerCCW] * WallTexH;

				TotalFloats += 30;
				BIndex += 30;

			}
			else {
                
				// Grounded on both sides

				Buffer[BIndex] = SubSiteX[CellNo];
				Buffer[BIndex + 1] = SubSiteY[CellNo];
				Buffer[BIndex + 2] = 2.0f;
				Buffer[BIndex + 3] = WallTexS + SiteTexS[CellNo] * WallTexW;
				Buffer[BIndex + 4] = WallTexT + SiteTexT[CellNo] * WallTexH;

				Buffer[BIndex + 5] = SubStartX[CellNo][LineNo];
				Buffer[BIndex + 6] = SubStartY[CellNo][LineNo];
				Buffer[BIndex + 7] = 0.0f;
				Buffer[BIndex + 8] = WallTexS + CornerTexS[CornerCCW] * WallTexW;
				Buffer[BIndex + 9] = WallTexT + CornerTexT[CornerCCW] * WallTexH;

				Buffer[BIndex + 10] = SubEndX[CellNo][LineNo];
				Buffer[BIndex + 11] = SubEndY[CellNo][LineNo];
				Buffer[BIndex + 12] = 0.0f;
				Buffer[BIndex + 13] = WallTexS + CornerTexS[CornerCW] * WallTexW;
				Buffer[BIndex + 14] = WallTexT + CornerTexT[CornerCW] * WallTexH;

				TotalFloats += 15;
				BIndex += 15;

			}
            break;
		case 3:
		case 4:
			if ((ExtCW == EXT_FLOOR) && (ExtCCW == EXT_FLOOR)) {
				
				// Edges meet floors on both sides

				Buffer[BIndex] = SubSiteX[CellNo];
				Buffer[BIndex + 1] = SubSiteY[CellNo];
				Buffer[BIndex + 2] = 2.0f;
				Buffer[BIndex + 3] = WallTexS + SiteTexS[CellNo] * WallTexW;
				Buffer[BIndex + 4] = WallTexT + SiteTexT[CellNo] * WallTexH;

				Buffer[BIndex + 5] = SubStartX[CellNo][LineNo];
				Buffer[BIndex + 6] = SubStartY[CellNo][LineNo];
				Buffer[BIndex + 7] = 0.0f;
				Buffer[BIndex + 8] = WallTexS + CornerTexS[CornerCCW] * WallTexW;
				Buffer[BIndex + 9] = WallTexT + CornerTexT[CornerCCW] * WallTexH;

				Buffer[BIndex + 10] = SubEndX[CellNo][LineNo];
				Buffer[BIndex + 11] = SubEndY[CellNo][LineNo];
				Buffer[BIndex + 12] = 0.0f;
				Buffer[BIndex + 13] = WallTexS + CornerTexS[CornerCW] * WallTexW;
				Buffer[BIndex + 14] = WallTexT + CornerTexT[CornerCW] * WallTexH;

				TotalFloats += 15;
				BIndex += 15;

			}
            else if (ExtCCW == EXT_FLOOR) {

                if (ExtCW == EXT_NONE) HeightCW = 3.0f * CornerWallHeight[CornerCW];
                else HeightCW = CornerWallHeight[CornerCW];
                
				// Wall height at CW side (floor on CCW side)

				Buffer[BIndex    ] = SubSiteX[CellNo];
				Buffer[BIndex + 1] = SubSiteY[CellNo];
				Buffer[BIndex + 2] = 2.0f;
				Buffer[BIndex + 3] = WallTexS + SiteTexS[CellNo] * WallTexW;
				Buffer[BIndex + 4] = WallTexT + SiteTexT[CellNo] * WallTexH;

				Buffer[BIndex + 5] = SubEndX[CellNo][LineNo];
				Buffer[BIndex + 6] = SubEndY[CellNo][LineNo];
				Buffer[BIndex + 7] = HeightCW;
				Buffer[BIndex + 8] = WallTexS + CornerTe2S[CornerCW] * WallTexW;
				Buffer[BIndex + 9] = WallTexT + CornerTe2T[CornerCW] * WallTexH;

				Buffer[BIndex + 10] = SubStartX[CellNo][LineNo];
				Buffer[BIndex + 11] = SubStartY[CellNo][LineNo];
				Buffer[BIndex + 12] = 0.0f;
				Buffer[BIndex + 13] = WallTexS + CornerTexS[CornerCCW] * WallTexW;
				Buffer[BIndex + 14] = WallTexT + CornerTexT[CornerCCW] * WallTexH;
                
				TotalFloats += 15;
				BIndex += 15;

            }
            else if (ExtCW == EXT_FLOOR) {
                
                if (ExtCCW == EXT_NONE) HeightCCW = 3.0f * CornerWallHeight[CornerCCW];
                else HeightCCW = CornerWallHeight[CornerCCW];
                
				// Wall height at CCW side (floor on CW side)

				Buffer[BIndex    ] = SubSiteX[CellNo];
				Buffer[BIndex + 1] = SubSiteY[CellNo];
				Buffer[BIndex + 2] = 2.0f;
				Buffer[BIndex + 3] = WallTexS + SiteTexS[CellNo] * WallTexW;
				Buffer[BIndex + 4] = WallTexT + SiteTexT[CellNo] * WallTexH;

				Buffer[BIndex + 5] = SubEndX[CellNo][LineNo];
				Buffer[BIndex + 6] = SubEndY[CellNo][LineNo];
				Buffer[BIndex + 7] = 0.0f;
				Buffer[BIndex + 8] = WallTexS + CornerTexS[CornerCW] * WallTexW;
				Buffer[BIndex + 9] = WallTexT + CornerTexT[CornerCW] * WallTexH;

				Buffer[BIndex + 10] = SubStartX[CellNo][LineNo];
				Buffer[BIndex + 11] = SubStartY[CellNo][LineNo];
				Buffer[BIndex + 12] = HeightCCW;
				Buffer[BIndex + 13] = WallTexS + CornerTe2S[CornerCCW] * WallTexW;
				Buffer[BIndex + 14] = WallTexT + CornerTe2T[CornerCCW] * WallTexH;
                
				TotalFloats += 15;
				BIndex += 15;

            }
            else {
                
                if (ExtCW == EXT_NONE) HeightCW = 3.0f * CornerWallHeight[CornerCW];
                else HeightCW = CornerWallHeight[CornerCW];
                
                if (ExtCCW == EXT_NONE) HeightCCW = 3.0f * CornerWallHeight[CornerCCW];
                else HeightCCW = CornerWallHeight[CornerCCW];
                
				// Edges meet stuff on both sides

				Buffer[BIndex] = SubSiteX[CellNo];
				Buffer[BIndex + 1] = SubSiteY[CellNo];
				Buffer[BIndex + 2] = 2.0f;
				Buffer[BIndex + 3] = WallTexS + SiteTexS[CellNo] * WallTexW;
				Buffer[BIndex + 4] = WallTexT + SiteTexT[CellNo] * WallTexH;

				Buffer[BIndex + 5] = SubStartX[CellNo][LineNo];
				Buffer[BIndex + 6] = SubStartY[CellNo][LineNo];
				Buffer[BIndex + 7] = HeightCCW;
				Buffer[BIndex + 8] = WallTexS + CornerTe2S[CornerCCW] * WallTexW;
				Buffer[BIndex + 9] = WallTexT + CornerTe2T[CornerCCW] * WallTexH;

				Buffer[BIndex + 10] = SubEndX[CellNo][LineNo];
				Buffer[BIndex + 11] = SubEndY[CellNo][LineNo];
				Buffer[BIndex + 12] = HeightCW;
				Buffer[BIndex + 13] = WallTexS + CornerTe2S[CornerCW] * WallTexW;
				Buffer[BIndex + 14] = WallTexT + CornerTe2T[CornerCW] * WallTexH;

				TotalFloats += 15;
				BIndex += 15;

            }
			break;
		}
		
    }
    
    return TotalFloats;
    
}

int PlainMapGen::LoadCommonBoulderFloats (float* Buffer, int Index, int CellNo) {
    
    int LineNo;
    int BIndex;
    
    for (LineNo = 0; LineNo < SubLinesForThisSite[CellNo]; LineNo++) {
        
        BIndex = Index + 15*LineNo;
        
        // Draw floor polygon
        
        Buffer[BIndex   ] = SubSiteX[CellNo];
        Buffer[BIndex+1 ] = SubSiteY[CellNo];
        Buffer[BIndex+2 ] = 5.0f;
        Buffer[BIndex+3 ] = WallTexS + SiteTexS[CellNo]*WallTexW;
        Buffer[BIndex+4 ] = WallTexT + SiteTexT[CellNo]*WallTexH;
        
        Buffer[BIndex+5 ] = SubStartX[CellNo][LineNo];
        Buffer[BIndex+6 ] = SubStartY[CellNo][LineNo];
        Buffer[BIndex+7 ] = CornerWallHeight[SubStartCorner[CellNo][LineNo]];
        Buffer[BIndex+8 ] = WallTexS + CornerTexS[SubStartCorner[CellNo][LineNo]]*WallTexW;
        Buffer[BIndex+9 ] = WallTexT + CornerTexT[SubStartCorner[CellNo][LineNo]]*WallTexH;
        
        Buffer[BIndex+10] = SubEndX[CellNo][LineNo];
        Buffer[BIndex+11] = SubEndY[CellNo][LineNo];
        Buffer[BIndex+12] = CornerWallHeight[SubEndCorner[CellNo][LineNo]];
        Buffer[BIndex+13] = WallTexS + CornerTexS[SubEndCorner[CellNo][LineNo]]*WallTexW;
        Buffer[BIndex+14] = WallTexT + CornerTexT[SubEndCorner[CellNo][LineNo]]*WallTexH;
        
    }
    
    return 15*SubLinesForThisSite[CellNo];
    
}

int PlainMapGen::LoadCommonSunkenFloats (float* Buffer, int Index, int CellNo) {
    
    int LineNo;
    int BIndex = Index;
    int AdjSite;
    int Try, AlsoSunk;
    
    for (LineNo = 0; LineNo < SubLinesForThisSite[CellNo]; LineNo++) {
        
        AdjSite = SubAdjacentSite[CellNo][LineNo];
        if (AdjSite >= 0) {
            if (ExtCellType[AdjSite] == EXT_POND) {
                    
                // Draw pool polygon only
                    
                Buffer[BIndex   ] = SubSiteX[CellNo];
                Buffer[BIndex+1 ] = SubSiteY[CellNo];
                Buffer[BIndex+2 ] = -1.0f;
				Buffer[BIndex+3] = PoolTexS + SiteTexS[CellNo]*PoolTexW;
				Buffer[BIndex+4] = PoolTexT + SiteTexT[CellNo]*PoolTexH;
                    
                Buffer[BIndex+5 ] = SubStartX[CellNo][LineNo];
                Buffer[BIndex+6 ] = SubStartY[CellNo][LineNo];
                Buffer[BIndex+7 ] = -1.0f;
				Buffer[BIndex+8] = PoolTexS + CornerTexS[SubStartCorner[CellNo][LineNo]]*PoolTexW;
				Buffer[BIndex+9] = PoolTexT + CornerTexT[SubStartCorner[CellNo][LineNo]]*PoolTexH;
                    
                Buffer[BIndex+10] = SubEndX[CellNo][LineNo];
                Buffer[BIndex+11] = SubEndY[CellNo][LineNo];
                Buffer[BIndex+12] = -1.0f;
				Buffer[BIndex+13] = PoolTexS + CornerTexS[SubEndCorner[CellNo][LineNo]]*PoolTexW;
				Buffer[BIndex+14] = PoolTexT + CornerTexT[SubEndCorner[CellNo][LineNo]]*PoolTexH;
                    
                BIndex += 15;
                    
                // Add bank segment on either side if it isn't a junction between 3 pool cells
                    
                Try = LineNo - 1;
                if (Try < 0) Try += SubLinesForThisSite[CellNo];
                AlsoSunk = 0;
                if (SubAdjacentSite[CellNo][Try] >= 0) {
                    if (ExtCellType[SubAdjacentSite[CellNo][Try]] == EXT_POND)
                        AlsoSunk = 1;
                }
                if (AlsoSunk == 0) {
                        
                    Buffer[BIndex   ] = 0.2f*SubSiteX[CellNo] + 0.8f*SubStartX[CellNo][LineNo];
                    Buffer[BIndex+1 ] = 0.2f*SubSiteY[CellNo] + 0.8f*SubStartY[CellNo][LineNo];
                    Buffer[BIndex+2 ] = -1.0f;
                    Buffer[BIndex+3 ] = BankTexS + CornerTe2S[SubStartCorner[CellNo][LineNo]]*BankTexW;
                    Buffer[BIndex+4 ] = BankTexT + CornerTe2T[SubStartCorner[CellNo][LineNo]]*BankTexH;
                        
                    Buffer[BIndex+5 ] = SubStartX[CellNo][LineNo];
                    Buffer[BIndex+6 ] = SubStartY[CellNo][LineNo];
                    Buffer[BIndex+7 ] = 0.0f;
                    Buffer[BIndex+8 ] = BankTexS + CornerTexS[SubStartCorner[CellNo][LineNo]]*BankTexW;
                    Buffer[BIndex+9 ] = BankTexT + CornerTexT[SubStartCorner[CellNo][LineNo]]*BankTexH;
                        
                    Buffer[BIndex+10] = 0.8f*SubStartX[CellNo][LineNo] + 0.2f*SubEndX[CellNo][LineNo];
                    Buffer[BIndex+11] = 0.8f*SubStartY[CellNo][LineNo] + 0.2f*SubEndY[CellNo][LineNo];
                    Buffer[BIndex+12] = -1.0f;
                    Buffer[BIndex+13] = BankTexS + CornerTe2S[SubEndCorner[CellNo][LineNo]]*BankTexW;
                    Buffer[BIndex+14] = BankTexT + CornerTe2T[SubEndCorner[CellNo][LineNo]]*BankTexH;
                        
                    BIndex += 15;
                        
                }
                    
                Try = LineNo + 1;
                if (Try >= SubLinesForThisSite[CellNo]) Try -= SubLinesForThisSite[CellNo];
                AlsoSunk = 0;
                if (SubAdjacentSite[CellNo][Try] >= 0) {
                    if (ExtCellType[SubAdjacentSite[CellNo][Try]] == EXT_POND)
                        AlsoSunk = 1;
                }
                if (AlsoSunk == 0) {
                        
                    Buffer[BIndex   ] = 0.2f*SubStartX[CellNo][LineNo] + 0.8f*SubEndX[CellNo][LineNo];
                    Buffer[BIndex+1 ] = 0.2f*SubStartY[CellNo][LineNo] + 0.8f*SubEndY[CellNo][LineNo];
                    Buffer[BIndex+2 ] = -1.0f;
                    Buffer[BIndex+3 ] = BankTexS + CornerTe2S[SubStartCorner[CellNo][LineNo]]*BankTexW;
                    Buffer[BIndex+4 ] = BankTexT + CornerTe2T[SubStartCorner[CellNo][LineNo]]*BankTexH;
                        
                    Buffer[BIndex+5 ] = SubEndX[CellNo][LineNo];
                    Buffer[BIndex+6 ] = SubEndY[CellNo][LineNo];
                    Buffer[BIndex+7 ] = 0.0f;
                    Buffer[BIndex+8 ] = BankTexS + CornerTexS[SubEndCorner[CellNo][LineNo]]*BankTexW;
                    Buffer[BIndex+9 ] = BankTexT + CornerTexT[SubEndCorner[CellNo][LineNo]]*BankTexH;
                        
                    Buffer[BIndex+10] = 0.2f*SubSiteX[CellNo] + 0.8f*SubEndX[CellNo][LineNo];
                    Buffer[BIndex+11] = 0.2f*SubSiteY[CellNo] + 0.8f*SubEndY[CellNo][LineNo];
                    Buffer[BIndex+12] = -1.0f;
                    Buffer[BIndex+13] = BankTexS + CornerTe2S[SubEndCorner[CellNo][LineNo]]*BankTexW;
                    Buffer[BIndex+14] = BankTexT + CornerTe2T[SubEndCorner[CellNo][LineNo]]*BankTexH;
                        
                    BIndex += 15;
                        
                }
                    
                continue;
                    
            }
        }
        
        // Draw pool polygon, then a bank
        
        Buffer[BIndex   ] = SubSiteX[CellNo];
        Buffer[BIndex+1 ] = SubSiteY[CellNo];
        Buffer[BIndex+2 ] = -1.0f;
        Buffer[BIndex+3 ] = PoolTexS + SiteTexS[CellNo]*PoolTexW;
        Buffer[BIndex+4 ] = PoolTexT + SiteTexT[CellNo]*PoolTexH;
        
        Buffer[BIndex+5 ] = 0.2f*SubSiteX[CellNo] + 0.8f*SubStartX[CellNo][LineNo];
        Buffer[BIndex+6 ] = 0.2f*SubSiteY[CellNo] + 0.8f*SubStartY[CellNo][LineNo];
        Buffer[BIndex+7 ] = -1.0f;
        Buffer[BIndex+8 ] = PoolTexS + CornerTexS[SubStartCorner[CellNo][LineNo]]*PoolTexW;
        Buffer[BIndex+9 ] = PoolTexT + CornerTexT[SubStartCorner[CellNo][LineNo]]*PoolTexH;
        
        Buffer[BIndex+10] = 0.2f*SubSiteX[CellNo] + 0.8f*SubEndX[CellNo][LineNo];
        Buffer[BIndex+11] = 0.2f*SubSiteY[CellNo] + 0.8f*SubEndY[CellNo][LineNo];
        Buffer[BIndex+12] = -1.0f;
        Buffer[BIndex+13] = PoolTexS + CornerTexS[SubEndCorner[CellNo][LineNo]]*PoolTexW;
        Buffer[BIndex+14] = PoolTexT + CornerTexT[SubEndCorner[CellNo][LineNo]]*PoolTexH;
        
        Buffer[BIndex+15] = 0.2f*SubSiteX[CellNo] + 0.8f*SubStartX[CellNo][LineNo];
        Buffer[BIndex+16] = 0.2f*SubSiteY[CellNo] + 0.8f*SubStartY[CellNo][LineNo];
        Buffer[BIndex+17] = -1.0f;
        Buffer[BIndex+18] = BankTexS + CornerTe2S[SubStartCorner[CellNo][LineNo]]*BankTexW;
        Buffer[BIndex+19] = BankTexT + CornerTe2T[SubStartCorner[CellNo][LineNo]]*BankTexH;
        
        Buffer[BIndex+20] = SubStartX[CellNo][LineNo];
        Buffer[BIndex+21] = SubStartY[CellNo][LineNo];
        Buffer[BIndex+22] = 0.0f;
        Buffer[BIndex+23] = BankTexS + CornerTexS[SubStartCorner[CellNo][LineNo]]*BankTexW;
        Buffer[BIndex+24] = BankTexT + CornerTexT[SubStartCorner[CellNo][LineNo]]*BankTexH;
        
        Buffer[BIndex+25] = SubEndX[CellNo][LineNo];
        Buffer[BIndex+26] = SubEndY[CellNo][LineNo];
        Buffer[BIndex+27] = 0.0f;
        Buffer[BIndex+28] = BankTexS + CornerTexS[SubEndCorner[CellNo][LineNo]]*BankTexW;
        Buffer[BIndex+29] = BankTexT + CornerTexT[SubEndCorner[CellNo][LineNo]]*BankTexH;
        
        Buffer[BIndex+30] = SubEndX[CellNo][LineNo];
        Buffer[BIndex+31] = SubEndY[CellNo][LineNo];
        Buffer[BIndex+32] = 0.0f;
        Buffer[BIndex+33] = BankTexS + CornerTexS[SubEndCorner[CellNo][LineNo]]*BankTexW;
        Buffer[BIndex+34] = BankTexT + CornerTexT[SubEndCorner[CellNo][LineNo]]*BankTexH;
        
        Buffer[BIndex+35] = 0.2f*SubSiteX[CellNo] + 0.8f*SubEndX[CellNo][LineNo];
        Buffer[BIndex+36] = 0.2f*SubSiteY[CellNo] + 0.8f*SubEndY[CellNo][LineNo];
        Buffer[BIndex+37] = -1.0f;
        Buffer[BIndex+38] = BankTexS + CornerTe2S[SubEndCorner[CellNo][LineNo]]*BankTexW;
        Buffer[BIndex+39] = BankTexT + CornerTe2T[SubEndCorner[CellNo][LineNo]]*BankTexH;
        
        Buffer[BIndex+40] = 0.2f*SubSiteX[CellNo] + 0.8f*SubStartX[CellNo][LineNo];
        Buffer[BIndex+41] = 0.2f*SubSiteY[CellNo] + 0.8f*SubStartY[CellNo][LineNo];
        Buffer[BIndex+42] = -1.0f;
        Buffer[BIndex+43] = BankTexS + CornerTe2S[SubStartCorner[CellNo][LineNo]]*BankTexW;
        Buffer[BIndex+44] = BankTexT + CornerTe2T[SubStartCorner[CellNo][LineNo]]*BankTexH;
        
        BIndex += 45;
        
    }
    
    return BIndex - Index;
    
}

int PlainMapGen::LoadCommonRubbleFloats (float* Buffer, int Index, int CellNo) {
    
    int TotalFloats = 0;

    int n;
    float HeightCW, HeightCCW;
    float MaxHeight;
    float Pt1X, Pt1Y, Pt2X, Pt2Y, Pt3X, Pt3Y, Pt4X, Pt4Y, Pt5X, Pt5Y, Pt6X, Pt6Y;
    float Len, dX, dY;

    for (n = 0; n < SubLinesForThisSite[CellNo]; n++) {
        
        // Add a sloped segment
        if (SubAdjacentSite[CellNo][n] >= 0) {
            if ((ExtCellType[SubAdjacentSite[CellNo][n]] == EXT_FLOOR) || (ExtCellType[SubAdjacentSite[CellNo][n]] == EXT_POND)) {
                HeightCCW = 0.0f;
                HeightCW = 0.0f;
            }
            else {
                // Set CW height
                if ((AdjExtCW[CellNo][n] == EXT_FLOOR) || (AdjExtCW[CellNo][n] == EXT_POND)) HeightCW = 0.0f;
                else HeightCW = 3.0f;
                // Set CCW height
                if ((AdjExtCCW[CellNo][n] == EXT_FLOOR) || (AdjExtCCW[CellNo][n] == EXT_POND)) HeightCCW = 0.0f;
                else HeightCCW = 3.0f;
            }
        }
        else {
            // Set CW height
            if ((AdjExtCW[CellNo][n] == EXT_FLOOR) || (AdjExtCW[CellNo][n] == EXT_POND)) HeightCW = 0.0f;
            else HeightCW = 3.0f;
            // Set CCW height
            if ((AdjExtCCW[CellNo][n] == EXT_FLOOR) || (AdjExtCCW[CellNo][n] == EXT_POND)) HeightCCW = 0.0f;
            else HeightCCW = 3.0f;
        }

        Buffer[Index   ] = SubSiteX[CellNo];
        Buffer[Index+1 ] = SubSiteY[CellNo];
        Buffer[Index+2 ] = 2.0f;
        Buffer[Index+3 ] = Spc1TexS + Spc1TexW;
        Buffer[Index+4 ] = Spc1TexT + 0.5f*Spc1TexH;
            
        Buffer[Index+5 ] = SubEndX[CellNo][n];
        Buffer[Index+6 ] = SubEndY[CellNo][n];
        Buffer[Index+7 ] = HeightCW;
        Buffer[Index+8 ] = Spc1TexS;
        Buffer[Index+9 ] = Spc1TexT;
            
        Buffer[Index+10] = SubStartX[CellNo][n];
        Buffer[Index+11] = SubStartY[CellNo][n];
        Buffer[Index+12] = HeightCCW;
        Buffer[Index+13] = Spc1TexS;
        Buffer[Index+14] = Spc1TexT + Spc1TexH;
        
        Index += 15;
        TotalFloats += 15;
        
        if (SubAdjacentSite[CellNo][n] < 0) continue;
        if (ExtCellType[SubAdjacentSite[CellNo][n]] != EXT_FLOOR) continue;

        // Add one variation of the rubble border structure
        HeightCCW = CornerWallHeight[SubStartCorner[CellNo][n]];
        HeightCW = CornerWallHeight[SubEndCorner[CellNo][n]];

        if (HeightCW < HeightCCW) MaxHeight = HeightCCW;
        else MaxHeight = HeightCW;

        dX = SubEndX[CellNo][n] - SubStartX[CellNo][n];
        dY = SubEndY[CellNo][n] - SubStartY[CellNo][n];
        Len = (float)sqrt((double)(dX*dX + dY*dY));
        
        if ((MaxHeight > 3.0f) && (Len > 8.0f)) {
            
            // Draw a quarter-way post
            
            Pt1X = SubStartX[CellNo][n] + (0.25f - 0.75f/Len)*dX;
            Pt1Y = SubStartY[CellNo][n] + (0.25f - 0.75f/Len)*dY;
            Pt2X = Pt1X + 1.125f*dX/Len + 0.65f*dY/Len;
            Pt2Y = Pt1Y + 1.125f*dY/Len - 0.65f*dX/Len;
            Pt3X = Pt2X - 1.3f*dY/Len;
            Pt3Y = Pt2Y + 1.3f*dX/Len;

            Buffer[Index   ] = Pt1X;
            Buffer[Index+1 ] = Pt1Y;
            Buffer[Index+2 ] = 0.0f;
            Buffer[Index+3 ] = Spc1TexS;
            Buffer[Index+4 ] = Spc1TexT;
            
            Buffer[Index+5 ] = Pt2X;
            Buffer[Index+6 ] = Pt2Y;
            Buffer[Index+7 ] = 0.0f;
            Buffer[Index+8 ] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+9 ] = Spc1TexT;
            
            Buffer[Index+10] = Pt2X;
            Buffer[Index+11] = Pt2Y;
            Buffer[Index+12] = MaxHeight - 0.75f;
            Buffer[Index+13] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+14] = Spc1TexT + Spc1TexH;

            Buffer[Index+15] = Pt2X;
            Buffer[Index+16] = Pt2Y;
            Buffer[Index+17] = MaxHeight - 0.75f;
            Buffer[Index+18] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+19] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+20] = Pt1X;
            Buffer[Index+21] = Pt1Y;
            Buffer[Index+22] = MaxHeight - 0.75f;
            Buffer[Index+23] = Spc1TexS;
            Buffer[Index+24] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+25] = Pt1X;
            Buffer[Index+26] = Pt1Y;
            Buffer[Index+27] = 0.0f;
            Buffer[Index+28] = Spc1TexS;
            Buffer[Index+29] = Spc1TexT;

            Buffer[Index+30] = Pt2X;
            Buffer[Index+31] = Pt2Y;
            Buffer[Index+32] = 0.0f;
            Buffer[Index+33] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+34] = Spc1TexT;
            
            Buffer[Index+35] = Pt3X;
            Buffer[Index+36] = Pt3Y;
            Buffer[Index+37] = 0.0f;
            Buffer[Index+38] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+39] = Spc1TexT;
            
            Buffer[Index+40] = Pt3X;
            Buffer[Index+41] = Pt3Y;
            Buffer[Index+42] = MaxHeight - 0.75f;
            Buffer[Index+43] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+44] = Spc1TexT + Spc1TexH;

            Buffer[Index+45] = Pt3X;
            Buffer[Index+46] = Pt3Y;
            Buffer[Index+47] = MaxHeight - 0.75f;
            Buffer[Index+48] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+49] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+50] = Pt2X;
            Buffer[Index+51] = Pt2Y;
            Buffer[Index+52] = MaxHeight - 0.75f;
            Buffer[Index+53] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+54] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+55] = Pt2X;
            Buffer[Index+56] = Pt2Y;
            Buffer[Index+57] = 0.0f;
            Buffer[Index+58] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+59] = Spc1TexT;

            Buffer[Index+60] = Pt3X;
            Buffer[Index+61] = Pt3Y;
            Buffer[Index+62] = 0.0f;
            Buffer[Index+63] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+64] = Spc1TexT;
            
            Buffer[Index+65] = Pt1X;
            Buffer[Index+66] = Pt1Y;
            Buffer[Index+67] = 0.0f;
            Buffer[Index+68] = Spc1TexS + Spc1TexW;
            Buffer[Index+69] = Spc1TexT;
            
            Buffer[Index+70] = Pt1X;
            Buffer[Index+71] = Pt1Y;
            Buffer[Index+72] = MaxHeight - 0.75f;
            Buffer[Index+73] = Spc1TexS + Spc1TexW;
            Buffer[Index+74] = Spc1TexT + Spc1TexH;

            Buffer[Index+75] = Pt1X;
            Buffer[Index+76] = Pt1Y;
            Buffer[Index+77] = MaxHeight - 0.75f;
            Buffer[Index+78] = Spc1TexS + Spc1TexW;
            Buffer[Index+79] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+80] = Pt3X;
            Buffer[Index+81] = Pt3Y;
            Buffer[Index+82] = MaxHeight - 0.75f;
            Buffer[Index+83] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+84] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+85] = Pt3X;
            Buffer[Index+86] = Pt3Y;
            Buffer[Index+87] = 0.0f;
            Buffer[Index+88] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+89] = Spc1TexT;
            
            Index += 90;
            TotalFloats += 90;

            // The horizontal bar

            Pt1X = SubStartX[CellNo][n] + 0.65f*dY/Len;
            Pt1Y = SubStartY[CellNo][n] - 0.65f*dX/Len;
            Pt2X = SubStartX[CellNo][n];
            Pt2Y = SubStartY[CellNo][n];
            Pt3X = SubStartX[CellNo][n] - 0.65f*dY/Len;
            Pt3Y = SubStartY[CellNo][n] + 0.65f*dX/Len;
            Pt5X = SubStartX[CellNo][n] + (0.5f + 0.375f/Len)*dX;
            Pt5Y = SubStartY[CellNo][n] + (0.5f + 0.375f/Len)*dY;
            Pt4X = Pt5X + 0.65f*dY/Len;
            Pt4Y = Pt5Y - 0.65f*dX/Len;
            Pt6X = Pt5X - 0.65f*dY/Len;
            Pt6Y = Pt5Y + 0.65f*dX/Len;

            Buffer[Index   ] = Pt2X;
            Buffer[Index+1 ] = Pt2Y;
            Buffer[Index+2 ] = MaxHeight + 0.375f;
            Buffer[Index+3 ] = Spc1TexS;
            Buffer[Index+4 ] = Spc1TexT;
            
            Buffer[Index+5 ] = Pt3X;
            Buffer[Index+6 ] = Pt3Y;
            Buffer[Index+7 ] = MaxHeight - 0.75f;
            Buffer[Index+8 ] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+9 ] = Spc1TexT;
            
            Buffer[Index+10] = Pt6X;
            Buffer[Index+11] = Pt6Y;
            Buffer[Index+12] = MaxHeight - 0.75f;
            Buffer[Index+13] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+14] = Spc1TexT + Spc1TexH;

            Buffer[Index+15] = Pt6X;
            Buffer[Index+16] = Pt6Y;
            Buffer[Index+17] = MaxHeight - 0.75f;
            Buffer[Index+18] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+19] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+20] = Pt5X;
            Buffer[Index+21] = Pt5Y;
            Buffer[Index+22] = MaxHeight + 0.375f;
            Buffer[Index+23] = Spc1TexS;
            Buffer[Index+24] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+25] = Pt2X;
            Buffer[Index+26] = Pt2Y;
            Buffer[Index+27] = MaxHeight + 0.375f;
            Buffer[Index+28] = Spc1TexS;
            Buffer[Index+29] = Spc1TexT;

            Buffer[Index+30] = Pt3X;
            Buffer[Index+31] = Pt3Y;
            Buffer[Index+32] = MaxHeight - 0.75f;
            Buffer[Index+33] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+34] = Spc1TexT;
            
            Buffer[Index+35] = Pt1X;
            Buffer[Index+36] = Pt1Y;
            Buffer[Index+37] = MaxHeight - 0.75f;
            Buffer[Index+38] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+39] = Spc1TexT;
            
            Buffer[Index+40] = Pt4X;
            Buffer[Index+41] = Pt4Y;
            Buffer[Index+42] = MaxHeight - 0.75f;
            Buffer[Index+43] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+44] = Spc1TexT + Spc1TexH;

            Buffer[Index+45] = Pt4X;
            Buffer[Index+46] = Pt4Y;
            Buffer[Index+47] = MaxHeight - 0.75f;
            Buffer[Index+48] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+49] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+50] = Pt6X;
            Buffer[Index+51] = Pt6Y;
            Buffer[Index+52] = MaxHeight - 0.75f;
            Buffer[Index+53] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+54] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+55] = Pt3X;
            Buffer[Index+56] = Pt3Y;
            Buffer[Index+57] = MaxHeight - 0.75f;
            Buffer[Index+58] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+59] = Spc1TexT;

            Buffer[Index+60] = Pt1X;
            Buffer[Index+61] = Pt1Y;
            Buffer[Index+62] = MaxHeight - 0.75f;
            Buffer[Index+63] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+64] = Spc1TexT;
            
            Buffer[Index+65] = Pt2X;
            Buffer[Index+66] = Pt2Y;
            Buffer[Index+67] = MaxHeight + 0.375f;
            Buffer[Index+68] = Spc1TexS + Spc1TexW;
            Buffer[Index+69] = Spc1TexT;
            
            Buffer[Index+70] = Pt5X;
            Buffer[Index+71] = Pt5Y;
            Buffer[Index+72] = MaxHeight + 0.375f;
            Buffer[Index+73] = Spc1TexS + Spc1TexW;
            Buffer[Index+74] = Spc1TexT + Spc1TexH;

            Buffer[Index+75] = Pt5X;
            Buffer[Index+76] = Pt5Y;
            Buffer[Index+77] = MaxHeight + 0.375f;
            Buffer[Index+78] = Spc1TexS + Spc1TexW;
            Buffer[Index+79] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+80] = Pt4X;
            Buffer[Index+81] = Pt4Y;
            Buffer[Index+82] = MaxHeight - 0.75f;
            Buffer[Index+83] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+84] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+85] = Pt1X;
            Buffer[Index+86] = Pt1Y;
            Buffer[Index+87] = MaxHeight - 0.75f;
            Buffer[Index+88] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+89] = Spc1TexT;
            
            Index += 90;
            TotalFloats += 90;

            // The ends of the bar

            Buffer[Index   ] = Pt1X;
            Buffer[Index+1 ] = Pt1Y;
            Buffer[Index+2 ] = MaxHeight - 0.75f;
            Buffer[Index+3 ] = Spc1TexS;
            Buffer[Index+4 ] = Spc1TexT;
            
            Buffer[Index+5 ] = Pt3X;
            Buffer[Index+6 ] = Pt3Y;
            Buffer[Index+7 ] = MaxHeight - 0.75f;
            Buffer[Index+8 ] = Spc1TexS;
            Buffer[Index+9 ] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+10] = Pt2X;
            Buffer[Index+11] = Pt2Y;
            Buffer[Index+12] = MaxHeight + 0.375f;
            Buffer[Index+13] = Spc1TexS + Spc1TexW;
            Buffer[Index+14] = Spc1TexT + 0.5f*Spc1TexH;

            Buffer[Index+15] = Pt6X;
            Buffer[Index+16] = Pt6Y;
            Buffer[Index+17] = MaxHeight - 0.75f;
            Buffer[Index+18] = Spc1TexS;
            Buffer[Index+19] = Spc1TexT;
            
            Buffer[Index+20] = Pt4X;
            Buffer[Index+21] = Pt4Y;
            Buffer[Index+22] = MaxHeight - 0.75f;
            Buffer[Index+23] = Spc1TexS;
            Buffer[Index+24] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+25] = Pt5X;
            Buffer[Index+26] = Pt5Y;
            Buffer[Index+27] = MaxHeight + 0.375f;
            Buffer[Index+28] = Spc1TexS + Spc1TexW;
            Buffer[Index+29] = Spc1TexT + 0.5f*Spc1TexH;

            Index += 30;
            TotalFloats += 30;
            
            // Draw a three-quarter-way post
            
            Pt1X = SubStartX[CellNo][n] + (0.75f + 0.75f/Len)*dX;
            Pt1Y = SubStartY[CellNo][n] + (0.75f + 0.75f/Len)*dY;
            Pt3X = Pt1X - 1.125f*dX/Len + 0.65f*dY/Len;
            Pt3Y = Pt1Y - 1.125f*dY/Len - 0.65f*dX/Len;
            Pt2X = Pt3X - 1.3f*dY/Len;
            Pt2Y = Pt3Y + 1.3f*dX/Len;

            Buffer[Index   ] = Pt1X;
            Buffer[Index+1 ] = Pt1Y;
            Buffer[Index+2 ] = 0.0f;
            Buffer[Index+3 ] = Spc1TexS;
            Buffer[Index+4 ] = Spc1TexT;
            
            Buffer[Index+5 ] = Pt2X;
            Buffer[Index+6 ] = Pt2Y;
            Buffer[Index+7 ] = 0.0f;
            Buffer[Index+8 ] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+9 ] = Spc1TexT;
            
            Buffer[Index+10] = Pt2X;
            Buffer[Index+11] = Pt2Y;
            Buffer[Index+12] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+13] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+14] = Spc1TexT + Spc1TexH;

            Buffer[Index+15] = Pt2X;
            Buffer[Index+16] = Pt2Y;
            Buffer[Index+17] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+18] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+19] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+20] = Pt1X;
            Buffer[Index+21] = Pt1Y;
            Buffer[Index+22] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+23] = Spc1TexS;
            Buffer[Index+24] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+25] = Pt1X;
            Buffer[Index+26] = Pt1Y;
            Buffer[Index+27] = 0.0f;
            Buffer[Index+28] = Spc1TexS;
            Buffer[Index+29] = Spc1TexT;

            Buffer[Index+30] = Pt2X;
            Buffer[Index+31] = Pt2Y;
            Buffer[Index+32] = 0.0f;
            Buffer[Index+33] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+34] = Spc1TexT;
            
            Buffer[Index+35] = Pt3X;
            Buffer[Index+36] = Pt3Y;
            Buffer[Index+37] = 0.0f;
            Buffer[Index+38] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+39] = Spc1TexT;
            
            Buffer[Index+40] = Pt3X;
            Buffer[Index+41] = Pt3Y;
            Buffer[Index+42] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+43] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+44] = Spc1TexT + Spc1TexH;

            Buffer[Index+45] = Pt3X;
            Buffer[Index+46] = Pt3Y;
            Buffer[Index+47] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+48] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+49] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+50] = Pt2X;
            Buffer[Index+51] = Pt2Y;
            Buffer[Index+52] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+53] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+54] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+55] = Pt2X;
            Buffer[Index+56] = Pt2Y;
            Buffer[Index+57] = 0.0f;
            Buffer[Index+58] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+59] = Spc1TexT;

            Buffer[Index+60] = Pt3X;
            Buffer[Index+61] = Pt3Y;
            Buffer[Index+62] = 0.0f;
            Buffer[Index+63] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+64] = Spc1TexT;
            
            Buffer[Index+65] = Pt1X;
            Buffer[Index+66] = Pt1Y;
            Buffer[Index+67] = 0.0f;
            Buffer[Index+68] = Spc1TexS + Spc1TexW;
            Buffer[Index+69] = Spc1TexT;
            
            Buffer[Index+70] = Pt1X;
            Buffer[Index+71] = Pt1Y;
            Buffer[Index+72] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+73] = Spc1TexS + Spc1TexW;
            Buffer[Index+74] = Spc1TexT + Spc1TexH;

            Buffer[Index+75] = Pt1X;
            Buffer[Index+76] = Pt1Y;
            Buffer[Index+77] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+78] = Spc1TexS + Spc1TexW;
            Buffer[Index+79] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+80] = Pt3X;
            Buffer[Index+81] = Pt3Y;
            Buffer[Index+82] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+83] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+84] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+85] = Pt3X;
            Buffer[Index+86] = Pt3Y;
            Buffer[Index+87] = 0.0f;
            Buffer[Index+88] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+89] = Spc1TexT;
            
            Index += 90;
            TotalFloats += 90;

            // The horizontal bar

            Pt2X = SubStartX[CellNo][n] + (0.75f - 0.375f/Len)*dX;
            Pt2Y = SubStartY[CellNo][n] + (0.75f - 0.375f/Len)*dY;
            Pt1X = Pt2X + 0.65f*dY/Len;
            Pt1Y = Pt2Y - 0.65f*dX/Len;
            Pt3X = Pt2X - 0.65f*dY/Len;
            Pt3Y = Pt2Y + 0.65f*dX/Len;
            Pt4X = SubEndX[CellNo][n] + 0.65f*dY/Len;
            Pt4Y = SubEndY[CellNo][n] - 0.65f*dX/Len;
            Pt5X = SubEndX[CellNo][n];
            Pt5Y = SubEndY[CellNo][n];
            Pt6X = SubEndX[CellNo][n] - 0.65f*dY/Len;
            Pt6Y = SubEndY[CellNo][n] + 0.65f*dX/Len;

            Buffer[Index   ] = Pt2X;
            Buffer[Index+1 ] = Pt2Y;
            Buffer[Index+2 ] = 0.6f*MaxHeight + 0.375f;
            Buffer[Index+3 ] = Spc1TexS;
            Buffer[Index+4 ] = Spc1TexT;
            
            Buffer[Index+5 ] = Pt3X;
            Buffer[Index+6 ] = Pt3Y;
            Buffer[Index+7 ] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+8 ] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+9 ] = Spc1TexT;
            
            Buffer[Index+10] = Pt6X;
            Buffer[Index+11] = Pt6Y;
            Buffer[Index+12] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+13] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+14] = Spc1TexT + Spc1TexH;

            Buffer[Index+15] = Pt6X;
            Buffer[Index+16] = Pt6Y;
            Buffer[Index+17] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+18] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+19] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+20] = Pt5X;
            Buffer[Index+21] = Pt5Y;
            Buffer[Index+22] = 0.6f*MaxHeight + 0.375f;
            Buffer[Index+23] = Spc1TexS;
            Buffer[Index+24] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+25] = Pt2X;
            Buffer[Index+26] = Pt2Y;
            Buffer[Index+27] = 0.6f*MaxHeight + 0.375f;
            Buffer[Index+28] = Spc1TexS;
            Buffer[Index+29] = Spc1TexT;

            Buffer[Index+30] = Pt3X;
            Buffer[Index+31] = Pt3Y;
            Buffer[Index+32] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+33] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+34] = Spc1TexT;
            
            Buffer[Index+35] = Pt1X;
            Buffer[Index+36] = Pt1Y;
            Buffer[Index+37] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+38] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+39] = Spc1TexT;
            
            Buffer[Index+40] = Pt4X;
            Buffer[Index+41] = Pt4Y;
            Buffer[Index+42] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+43] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+44] = Spc1TexT + Spc1TexH;

            Buffer[Index+45] = Pt4X;
            Buffer[Index+46] = Pt4Y;
            Buffer[Index+47] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+48] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+49] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+50] = Pt6X;
            Buffer[Index+51] = Pt6Y;
            Buffer[Index+52] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+53] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+54] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+55] = Pt3X;
            Buffer[Index+56] = Pt3Y;
            Buffer[Index+57] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+58] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+59] = Spc1TexT;

            Buffer[Index+60] = Pt1X;
            Buffer[Index+61] = Pt1Y;
            Buffer[Index+62] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+63] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+64] = Spc1TexT;
            
            Buffer[Index+65] = Pt2X;
            Buffer[Index+66] = Pt2Y;
            Buffer[Index+67] = 0.6f*MaxHeight + 0.375f;
            Buffer[Index+68] = Spc1TexS + Spc1TexW;
            Buffer[Index+69] = Spc1TexT;
            
            Buffer[Index+70] = Pt5X;
            Buffer[Index+71] = Pt5Y;
            Buffer[Index+72] = 0.6f*MaxHeight + 0.375f;
            Buffer[Index+73] = Spc1TexS + Spc1TexW;
            Buffer[Index+74] = Spc1TexT + Spc1TexH;

            Buffer[Index+75] = Pt5X;
            Buffer[Index+76] = Pt5Y;
            Buffer[Index+77] = 0.6f*MaxHeight + 0.375f;
            Buffer[Index+78] = Spc1TexS + Spc1TexW;
            Buffer[Index+79] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+80] = Pt4X;
            Buffer[Index+81] = Pt4Y;
            Buffer[Index+82] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+83] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+84] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+85] = Pt1X;
            Buffer[Index+86] = Pt1Y;
            Buffer[Index+87] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+88] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+89] = Spc1TexT;
            
            Index += 90;
            TotalFloats += 90;

            // The ends of the bar

            Buffer[Index   ] = Pt1X;
            Buffer[Index+1 ] = Pt1Y;
            Buffer[Index+2 ] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+3 ] = Spc1TexS;
            Buffer[Index+4 ] = Spc1TexT;
            
            Buffer[Index+5 ] = Pt3X;
            Buffer[Index+6 ] = Pt3Y;
            Buffer[Index+7 ] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+8 ] = Spc1TexS;
            Buffer[Index+9 ] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+10] = Pt2X;
            Buffer[Index+11] = Pt2Y;
            Buffer[Index+12] = 0.6f*MaxHeight + 0.375f;
            Buffer[Index+13] = Spc1TexS + Spc1TexW;
            Buffer[Index+14] = Spc1TexT + 0.5f*Spc1TexH;

            Buffer[Index+15] = Pt6X;
            Buffer[Index+16] = Pt6Y;
            Buffer[Index+17] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+18] = Spc1TexS;
            Buffer[Index+19] = Spc1TexT;
            
            Buffer[Index+20] = Pt4X;
            Buffer[Index+21] = Pt4Y;
            Buffer[Index+22] = 0.6f*MaxHeight - 0.75f;
            Buffer[Index+23] = Spc1TexS;
            Buffer[Index+24] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+25] = Pt5X;
            Buffer[Index+26] = Pt5Y;
            Buffer[Index+27] = 0.6f*MaxHeight + 0.375f;
            Buffer[Index+28] = Spc1TexS + Spc1TexW;
            Buffer[Index+29] = Spc1TexT + 0.5f*Spc1TexH;

            Index += 30;
            TotalFloats += 30;
            
            // The middle bar
            
            Pt2X = SubStartX[CellNo][n] + (0.75f - 0.375f/Len)*dX;
            Pt2Y = SubStartY[CellNo][n] + (0.75f - 0.375f/Len)*dY;
            Pt1X = Pt2X + 0.65f*dY/Len;
            Pt1Y = Pt2Y - 0.65f*dX/Len;
            Pt3X = Pt2X - 0.65f*dY/Len;
            Pt3Y = Pt2Y + 0.65f*dX/Len;
            Pt5X = SubStartX[CellNo][n] + (0.25f + 0.375f/Len)*dX;
            Pt5Y = SubStartY[CellNo][n] + (0.25f + 0.375f/Len)*dY;
            Pt4X = Pt5X + 0.65f*dY/Len;
            Pt4Y = Pt5Y - 0.65f*dX/Len;
            Pt6X = Pt5X - 0.65f*dY/Len;
            Pt6Y = Pt5Y + 0.65f*dX/Len;

            Buffer[Index   ] = Pt2X;
            Buffer[Index+1 ] = Pt2Y;
            Buffer[Index+2 ] = 0.3f*MaxHeight + 0.375f;
            Buffer[Index+3 ] = Spc1TexS;
            Buffer[Index+4 ] = Spc1TexT;
            
            Buffer[Index+5 ] = Pt3X;
            Buffer[Index+6 ] = Pt3Y;
            Buffer[Index+7 ] = 0.3f*MaxHeight - 0.75f;
            Buffer[Index+8 ] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+9 ] = Spc1TexT;
            
            Buffer[Index+10] = Pt6X;
            Buffer[Index+11] = Pt6Y;
            Buffer[Index+12] = 0.3f*MaxHeight - 0.75f;
            Buffer[Index+13] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+14] = Spc1TexT + Spc1TexH;

            Buffer[Index+15] = Pt6X;
            Buffer[Index+16] = Pt6Y;
            Buffer[Index+17] = 0.3f*MaxHeight - 0.75f;
            Buffer[Index+18] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+19] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+20] = Pt5X;
            Buffer[Index+21] = Pt5Y;
            Buffer[Index+22] = 0.3f*MaxHeight + 0.375f;
            Buffer[Index+23] = Spc1TexS;
            Buffer[Index+24] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+25] = Pt2X;
            Buffer[Index+26] = Pt2Y;
            Buffer[Index+27] = 0.3f*MaxHeight + 0.375f;
            Buffer[Index+28] = Spc1TexS;
            Buffer[Index+29] = Spc1TexT;

            Buffer[Index+30] = Pt3X;
            Buffer[Index+31] = Pt3Y;
            Buffer[Index+32] = 0.3f*MaxHeight - 0.75f;
            Buffer[Index+33] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+34] = Spc1TexT;
            
            Buffer[Index+35] = Pt1X;
            Buffer[Index+36] = Pt1Y;
            Buffer[Index+37] = 0.3f*MaxHeight - 0.75f;
            Buffer[Index+38] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+39] = Spc1TexT;
            
            Buffer[Index+40] = Pt4X;
            Buffer[Index+41] = Pt4Y;
            Buffer[Index+42] = 0.3f*MaxHeight - 0.75f;
            Buffer[Index+43] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+44] = Spc1TexT + Spc1TexH;

            Buffer[Index+45] = Pt4X;
            Buffer[Index+46] = Pt4Y;
            Buffer[Index+47] = 0.3f*MaxHeight - 0.75f;
            Buffer[Index+48] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+49] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+50] = Pt6X;
            Buffer[Index+51] = Pt6Y;
            Buffer[Index+52] = 0.3f*MaxHeight - 0.75f;
            Buffer[Index+53] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+54] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+55] = Pt3X;
            Buffer[Index+56] = Pt3Y;
            Buffer[Index+57] = 0.3f*MaxHeight - 0.75f;
            Buffer[Index+58] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+59] = Spc1TexT;

            Buffer[Index+60] = Pt1X;
            Buffer[Index+61] = Pt1Y;
            Buffer[Index+62] = 0.3f*MaxHeight - 0.75f;
            Buffer[Index+63] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+64] = Spc1TexT;
            
            Buffer[Index+65] = Pt2X;
            Buffer[Index+66] = Pt2Y;
            Buffer[Index+67] = 0.3f*MaxHeight + 0.375f;
            Buffer[Index+68] = Spc1TexS + Spc1TexW;
            Buffer[Index+69] = Spc1TexT;
            
            Buffer[Index+70] = Pt5X;
            Buffer[Index+71] = Pt5Y;
            Buffer[Index+72] = 0.3f*MaxHeight + 0.375f;
            Buffer[Index+73] = Spc1TexS + Spc1TexW;
            Buffer[Index+74] = Spc1TexT + Spc1TexH;

            Buffer[Index+75] = Pt5X;
            Buffer[Index+76] = Pt5Y;
            Buffer[Index+77] = 0.3f*MaxHeight + 0.375f;
            Buffer[Index+78] = Spc1TexS + Spc1TexW;
            Buffer[Index+79] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+80] = Pt4X;
            Buffer[Index+81] = Pt4Y;
            Buffer[Index+82] = 0.3f*MaxHeight - 0.75f;
            Buffer[Index+83] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+84] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+85] = Pt1X;
            Buffer[Index+86] = Pt1Y;
            Buffer[Index+87] = 0.3f*MaxHeight - 0.75f;
            Buffer[Index+88] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+89] = Spc1TexT;
            
            Index += 90;
            TotalFloats += 90;

        }
        else if ((MaxHeight > 5.5f) && (Len > 8.0f)) {
            
            // Draw a thing at the end
            
            Pt1X = SubEndX[CellNo][n];
            Pt1Y = SubEndY[CellNo][n];
            Pt3X = Pt1X - 1.125f*dX/Len + 0.65f*dY/Len;
            Pt3Y = Pt1Y - 1.125f*dY/Len - 0.65f*dX/Len;
            Pt2X = Pt3X - 1.3f*dY/Len;
            Pt2Y = Pt3Y + 1.3f*dX/Len;

            Buffer[Index   ] = Pt1X;
            Buffer[Index+1 ] = Pt1Y;
            Buffer[Index+2 ] = 0.0f;
            Buffer[Index+3 ] = Spc1TexS;
            Buffer[Index+4 ] = Spc1TexT;
            
            Buffer[Index+5 ] = Pt2X;
            Buffer[Index+6 ] = Pt2Y;
            Buffer[Index+7 ] = 0.0f;
            Buffer[Index+8 ] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+9 ] = Spc1TexT;
            
            Buffer[Index+10] = Pt2X;
            Buffer[Index+11] = Pt2Y;
            Buffer[Index+12] = 0.6f*MaxHeight;
            Buffer[Index+13] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+14] = Spc1TexT + Spc1TexH;

            Buffer[Index+15] = Pt2X;
            Buffer[Index+16] = Pt2Y;
            Buffer[Index+17] = 0.6f*MaxHeight;
            Buffer[Index+18] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+19] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+20] = Pt1X;
            Buffer[Index+21] = Pt1Y;
            Buffer[Index+22] = 0.6f*MaxHeight;
            Buffer[Index+23] = Spc1TexS;
            Buffer[Index+24] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+25] = Pt1X;
            Buffer[Index+26] = Pt1Y;
            Buffer[Index+27] = 0.0f;
            Buffer[Index+28] = Spc1TexS;
            Buffer[Index+29] = Spc1TexT;

            Buffer[Index+30] = Pt2X;
            Buffer[Index+31] = Pt2Y;
            Buffer[Index+32] = 0.0f;
            Buffer[Index+33] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+34] = Spc1TexT;
            
            Buffer[Index+35] = Pt3X;
            Buffer[Index+36] = Pt3Y;
            Buffer[Index+37] = 0.0f;
            Buffer[Index+38] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+39] = Spc1TexT;
            
            Buffer[Index+40] = Pt3X;
            Buffer[Index+41] = Pt3Y;
            Buffer[Index+42] = 0.6f*MaxHeight;
            Buffer[Index+43] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+44] = Spc1TexT + Spc1TexH;

            Buffer[Index+45] = Pt3X;
            Buffer[Index+46] = Pt3Y;
            Buffer[Index+47] = 0.6f*MaxHeight;
            Buffer[Index+48] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+49] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+50] = Pt2X;
            Buffer[Index+51] = Pt2Y;
            Buffer[Index+52] = 0.6f*MaxHeight;
            Buffer[Index+53] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+54] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+55] = Pt2X;
            Buffer[Index+56] = Pt2Y;
            Buffer[Index+57] = 0.0f;
            Buffer[Index+58] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+59] = Spc1TexT;

            Buffer[Index+60] = Pt3X;
            Buffer[Index+61] = Pt3Y;
            Buffer[Index+62] = 0.0f;
            Buffer[Index+63] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+64] = Spc1TexT;
            
            Buffer[Index+65] = Pt1X;
            Buffer[Index+66] = Pt1Y;
            Buffer[Index+67] = 0.0f;
            Buffer[Index+68] = Spc1TexS + Spc1TexW;
            Buffer[Index+69] = Spc1TexT;
            
            Buffer[Index+70] = Pt1X;
            Buffer[Index+71] = Pt1Y;
            Buffer[Index+72] = 0.6f*MaxHeight;
            Buffer[Index+73] = Spc1TexS + Spc1TexW;
            Buffer[Index+74] = Spc1TexT + Spc1TexH;

            Buffer[Index+75] = Pt1X;
            Buffer[Index+76] = Pt1Y;
            Buffer[Index+77] = 0.6f*MaxHeight;
            Buffer[Index+78] = Spc1TexS + Spc1TexW;
            Buffer[Index+79] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+80] = Pt3X;
            Buffer[Index+81] = Pt3Y;
            Buffer[Index+82] = 0.6f*MaxHeight;
            Buffer[Index+83] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+84] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+85] = Pt3X;
            Buffer[Index+86] = Pt3Y;
            Buffer[Index+87] = 0.0f;
            Buffer[Index+88] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+89] = Spc1TexT;
            
            Index += 90;
            TotalFloats += 90;
            
            // The end of the thing

            Buffer[Index   ] = Pt1X;
            Buffer[Index+1 ] = Pt1Y;
            Buffer[Index+2 ] = 0.6f*MaxHeight;
            Buffer[Index+3 ] = Spc1TexS;
            Buffer[Index+4 ] = Spc1TexT;
            
            Buffer[Index+5 ] = Pt2X;
            Buffer[Index+6 ] = Pt2Y;
            Buffer[Index+7 ] = 0.6f*MaxHeight;
            Buffer[Index+8 ] = Spc1TexS;
            Buffer[Index+9 ] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+10] = Pt3X;
            Buffer[Index+11] = Pt3Y;
            Buffer[Index+12] = 0.6f*MaxHeight;
            Buffer[Index+13] = Spc1TexS + Spc1TexW;
            Buffer[Index+14] = Spc1TexT + 0.5f*Spc1TexH;
            
            Index += 15;
            TotalFloats += 15;
            
            // The bar leaning against the thing
            
            Pt1X = SubStartX[CellNo][n] + 0.65f*dY/Len;
            Pt1Y = SubStartY[CellNo][n] - 0.65f*dX/Len;
            Pt2X = SubStartX[CellNo][n];
            Pt2Y = SubStartY[CellNo][n];
            Pt3X = SubStartX[CellNo][n] - 0.65f*dY/Len;
            Pt3Y = SubStartY[CellNo][n] + 0.65f*dX/Len;
            Pt4X = SubEndX[CellNo][n] + 0.65f*dY/Len;
            Pt4Y = SubEndY[CellNo][n] - 0.65f*dX/Len;
            Pt5X = SubEndX[CellNo][n];
            Pt5Y = SubEndY[CellNo][n];
            Pt6X = SubEndX[CellNo][n] - 0.65f*dY/Len;
            Pt6Y = SubEndY[CellNo][n] + 0.65f*dX/Len;

            Buffer[Index   ] = Pt2X;
            Buffer[Index+1 ] = Pt2Y;
            Buffer[Index+2 ] = MaxHeight + 0.375f;
            Buffer[Index+3 ] = Spc1TexS;
            Buffer[Index+4 ] = Spc1TexT;
            
            Buffer[Index+5 ] = Pt3X;
            Buffer[Index+6 ] = Pt3Y;
            Buffer[Index+7 ] = MaxHeight - 0.75f;
            Buffer[Index+8 ] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+9 ] = Spc1TexT;
            
            Buffer[Index+10] = Pt6X;
            Buffer[Index+11] = Pt6Y;
            Buffer[Index+12] = 0.0f;
            Buffer[Index+13] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+14] = Spc1TexT + Spc1TexH;

            Buffer[Index+15] = Pt6X;
            Buffer[Index+16] = Pt6Y;
            Buffer[Index+17] = 0.0f;
            Buffer[Index+18] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+19] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+20] = Pt5X;
            Buffer[Index+21] = Pt5Y;
            Buffer[Index+22] = 1.5f;
            Buffer[Index+23] = Spc1TexS;
            Buffer[Index+24] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+25] = Pt2X;
            Buffer[Index+26] = Pt2Y;
            Buffer[Index+27] = MaxHeight + 0.375f;
            Buffer[Index+28] = Spc1TexS;
            Buffer[Index+29] = Spc1TexT;

            Buffer[Index+30] = Pt3X;
            Buffer[Index+31] = Pt3Y;
            Buffer[Index+32] = MaxHeight - 0.75f;
            Buffer[Index+33] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+34] = Spc1TexT;
            
            Buffer[Index+35] = Pt1X;
            Buffer[Index+36] = Pt1Y;
            Buffer[Index+37] = MaxHeight - 0.75f;
            Buffer[Index+38] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+39] = Spc1TexT;
            
            Buffer[Index+40] = Pt4X;
            Buffer[Index+41] = Pt4Y;
            Buffer[Index+42] = 0.0f;
            Buffer[Index+43] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+44] = Spc1TexT + Spc1TexH;

            Buffer[Index+45] = Pt4X;
            Buffer[Index+46] = Pt4Y;
            Buffer[Index+47] = 0.0f;
            Buffer[Index+48] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+49] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+50] = Pt6X;
            Buffer[Index+51] = Pt6Y;
            Buffer[Index+52] = 0.0f;
            Buffer[Index+53] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+54] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+55] = Pt3X;
            Buffer[Index+56] = Pt3Y;
            Buffer[Index+57] = MaxHeight - 0.75f;
            Buffer[Index+58] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+59] = Spc1TexT;

            Buffer[Index+60] = Pt1X;
            Buffer[Index+61] = Pt1Y;
            Buffer[Index+62] = MaxHeight - 0.75f;
            Buffer[Index+63] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+64] = Spc1TexT;
            
            Buffer[Index+65] = Pt2X;
            Buffer[Index+66] = Pt2Y;
            Buffer[Index+67] = MaxHeight + 0.375f;
            Buffer[Index+68] = Spc1TexS + Spc1TexW;
            Buffer[Index+69] = Spc1TexT;
            
            Buffer[Index+70] = Pt5X;
            Buffer[Index+71] = Pt5Y;
            Buffer[Index+72] = 1.5f;
            Buffer[Index+73] = Spc1TexS + Spc1TexW;
            Buffer[Index+74] = Spc1TexT + Spc1TexH;

            Buffer[Index+75] = Pt5X;
            Buffer[Index+76] = Pt5Y;
            Buffer[Index+77] = 1.5f;
            Buffer[Index+78] = Spc1TexS + Spc1TexW;
            Buffer[Index+79] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+80] = Pt4X;
            Buffer[Index+81] = Pt4Y;
            Buffer[Index+82] = 0.0f;
            Buffer[Index+83] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+84] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+85] = Pt1X;
            Buffer[Index+86] = Pt1Y;
            Buffer[Index+87] = MaxHeight - 0.75f;
            Buffer[Index+88] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+89] = Spc1TexT;
            
            Index += 90;
            TotalFloats += 90;

            // The end of the bar

            Buffer[Index   ] = Pt1X;
            Buffer[Index+1 ] = Pt1Y;
            Buffer[Index+2 ] = MaxHeight - 0.75f;
            Buffer[Index+3 ] = Spc1TexS;
            Buffer[Index+4 ] = Spc1TexT;
            
            Buffer[Index+5 ] = Pt3X;
            Buffer[Index+6 ] = Pt3Y;
            Buffer[Index+7 ] = MaxHeight - 0.75f;
            Buffer[Index+8 ] = Spc1TexS;
            Buffer[Index+9 ] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+10] = Pt2X;
            Buffer[Index+11] = Pt2Y;
            Buffer[Index+12] = MaxHeight + 0.375f;
            Buffer[Index+13] = Spc1TexS + Spc1TexW;
            Buffer[Index+14] = Spc1TexT + 0.5f*Spc1TexH;

            Index += 15;
            TotalFloats += 15;

        }
        else {
        
            // Draw a centre post
            
            Pt1X = SubStartX[CellNo][n] + (0.5f - 0.75f/Len)*dX;
            Pt1Y = SubStartY[CellNo][n] + (0.5f - 0.75f/Len)*dY;
            Pt2X = Pt1X + 1.125f*dX/Len + 0.65f*dY/Len;
            Pt2Y = Pt1Y + 1.125f*dY/Len - 0.65f*dX/Len;
            Pt3X = Pt2X - 1.3f*dY/Len;
            Pt3Y = Pt2Y + 1.3f*dX/Len;

            Buffer[Index   ] = Pt1X;
            Buffer[Index+1 ] = Pt1Y;
            Buffer[Index+2 ] = 0.0f;
            Buffer[Index+3 ] = Spc1TexS;
            Buffer[Index+4 ] = Spc1TexT;
            
            Buffer[Index+5 ] = Pt2X;
            Buffer[Index+6 ] = Pt2Y;
            Buffer[Index+7 ] = 0.0f;
            Buffer[Index+8 ] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+9 ] = Spc1TexT;
            
            Buffer[Index+10] = Pt2X;
            Buffer[Index+11] = Pt2Y;
            Buffer[Index+12] = MaxHeight - 0.75f;
            Buffer[Index+13] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+14] = Spc1TexT + Spc1TexH;

            Buffer[Index+15] = Pt2X;
            Buffer[Index+16] = Pt2Y;
            Buffer[Index+17] = MaxHeight - 0.75f;
            Buffer[Index+18] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+19] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+20] = Pt1X;
            Buffer[Index+21] = Pt1Y;
            Buffer[Index+22] = MaxHeight - 0.75f;
            Buffer[Index+23] = Spc1TexS;
            Buffer[Index+24] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+25] = Pt1X;
            Buffer[Index+26] = Pt1Y;
            Buffer[Index+27] = 0.0f;
            Buffer[Index+28] = Spc1TexS;
            Buffer[Index+29] = Spc1TexT;

            Buffer[Index+30] = Pt2X;
            Buffer[Index+31] = Pt2Y;
            Buffer[Index+32] = 0.0f;
            Buffer[Index+33] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+34] = Spc1TexT;
            
            Buffer[Index+35] = Pt3X;
            Buffer[Index+36] = Pt3Y;
            Buffer[Index+37] = 0.0f;
            Buffer[Index+38] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+39] = Spc1TexT;
            
            Buffer[Index+40] = Pt3X;
            Buffer[Index+41] = Pt3Y;
            Buffer[Index+42] = MaxHeight - 0.75f;
            Buffer[Index+43] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+44] = Spc1TexT + Spc1TexH;

            Buffer[Index+45] = Pt3X;
            Buffer[Index+46] = Pt3Y;
            Buffer[Index+47] = MaxHeight - 0.75f;
            Buffer[Index+48] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+49] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+50] = Pt2X;
            Buffer[Index+51] = Pt2Y;
            Buffer[Index+52] = MaxHeight - 0.75f;
            Buffer[Index+53] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+54] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+55] = Pt2X;
            Buffer[Index+56] = Pt2Y;
            Buffer[Index+57] = 0.0f;
            Buffer[Index+58] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+59] = Spc1TexT;

            Buffer[Index+60] = Pt3X;
            Buffer[Index+61] = Pt3Y;
            Buffer[Index+62] = 0.0f;
            Buffer[Index+63] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+64] = Spc1TexT;
            
            Buffer[Index+65] = Pt1X;
            Buffer[Index+66] = Pt1Y;
            Buffer[Index+67] = 0.0f;
            Buffer[Index+68] = Spc1TexS + Spc1TexW;
            Buffer[Index+69] = Spc1TexT;
            
            Buffer[Index+70] = Pt1X;
            Buffer[Index+71] = Pt1Y;
            Buffer[Index+72] = MaxHeight - 0.75f;
            Buffer[Index+73] = Spc1TexS + Spc1TexW;
            Buffer[Index+74] = Spc1TexT + Spc1TexH;

            Buffer[Index+75] = Pt1X;
            Buffer[Index+76] = Pt1Y;
            Buffer[Index+77] = MaxHeight - 0.75f;
            Buffer[Index+78] = Spc1TexS + Spc1TexW;
            Buffer[Index+79] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+80] = Pt3X;
            Buffer[Index+81] = Pt3Y;
            Buffer[Index+82] = MaxHeight - 0.75f;
            Buffer[Index+83] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+84] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+85] = Pt3X;
            Buffer[Index+86] = Pt3Y;
            Buffer[Index+87] = 0.0f;
            Buffer[Index+88] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+89] = Spc1TexT;
            
            Index += 90;
            TotalFloats += 90;

            // The horizontal bar
            
            Pt1X = SubStartX[CellNo][n] + 0.65f*dY/Len;
            Pt1Y = SubStartY[CellNo][n] - 0.65f*dX/Len;
            Pt2X = SubStartX[CellNo][n];
            Pt2Y = SubStartY[CellNo][n];
            Pt3X = SubStartX[CellNo][n] - 0.65f*dY/Len;
            Pt3Y = SubStartY[CellNo][n] + 0.65f*dX/Len;
            Pt4X = SubEndX[CellNo][n] + 0.65f*dY/Len;
            Pt4Y = SubEndY[CellNo][n] - 0.65f*dX/Len;
            Pt5X = SubEndX[CellNo][n];
            Pt5Y = SubEndY[CellNo][n];
            Pt6X = SubEndX[CellNo][n] - 0.65f*dY/Len;
            Pt6Y = SubEndY[CellNo][n] + 0.65f*dX/Len;

            Buffer[Index   ] = Pt2X;
            Buffer[Index+1 ] = Pt2Y;
            Buffer[Index+2 ] = MaxHeight + 0.375f;
            Buffer[Index+3 ] = Spc1TexS;
            Buffer[Index+4 ] = Spc1TexT;
            
            Buffer[Index+5 ] = Pt3X;
            Buffer[Index+6 ] = Pt3Y;
            Buffer[Index+7 ] = MaxHeight - 0.75f;
            Buffer[Index+8 ] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+9 ] = Spc1TexT;
            
            Buffer[Index+10] = Pt6X;
            Buffer[Index+11] = Pt6Y;
            Buffer[Index+12] = MaxHeight - 0.75f;
            Buffer[Index+13] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+14] = Spc1TexT + Spc1TexH;

            Buffer[Index+15] = Pt6X;
            Buffer[Index+16] = Pt6Y;
            Buffer[Index+17] = MaxHeight - 0.75f;
            Buffer[Index+18] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+19] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+20] = Pt5X;
            Buffer[Index+21] = Pt5Y;
            Buffer[Index+22] = MaxHeight + 0.375f;
            Buffer[Index+23] = Spc1TexS;
            Buffer[Index+24] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+25] = Pt2X;
            Buffer[Index+26] = Pt2Y;
            Buffer[Index+27] = MaxHeight + 0.375f;
            Buffer[Index+28] = Spc1TexS;
            Buffer[Index+29] = Spc1TexT;

            Buffer[Index+30] = Pt3X;
            Buffer[Index+31] = Pt3Y;
            Buffer[Index+32] = MaxHeight - 0.75f;
            Buffer[Index+33] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+34] = Spc1TexT;
            
            Buffer[Index+35] = Pt1X;
            Buffer[Index+36] = Pt1Y;
            Buffer[Index+37] = MaxHeight - 0.75f;
            Buffer[Index+38] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+39] = Spc1TexT;
            
            Buffer[Index+40] = Pt4X;
            Buffer[Index+41] = Pt4Y;
            Buffer[Index+42] = MaxHeight - 0.75f;
            Buffer[Index+43] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+44] = Spc1TexT + Spc1TexH;

            Buffer[Index+45] = Pt4X;
            Buffer[Index+46] = Pt4Y;
            Buffer[Index+47] = MaxHeight - 0.75f;
            Buffer[Index+48] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+49] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+50] = Pt6X;
            Buffer[Index+51] = Pt6Y;
            Buffer[Index+52] = MaxHeight - 0.75f;
            Buffer[Index+53] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+54] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+55] = Pt3X;
            Buffer[Index+56] = Pt3Y;
            Buffer[Index+57] = MaxHeight - 0.75f;
            Buffer[Index+58] = Spc1TexS + 0.33f*Spc1TexW;
            Buffer[Index+59] = Spc1TexT;

            Buffer[Index+60] = Pt1X;
            Buffer[Index+61] = Pt1Y;
            Buffer[Index+62] = MaxHeight - 0.75f;
            Buffer[Index+63] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+64] = Spc1TexT;
            
            Buffer[Index+65] = Pt2X;
            Buffer[Index+66] = Pt2Y;
            Buffer[Index+67] = MaxHeight + 0.375f;
            Buffer[Index+68] = Spc1TexS + Spc1TexW;
            Buffer[Index+69] = Spc1TexT;
            
            Buffer[Index+70] = Pt5X;
            Buffer[Index+71] = Pt5Y;
            Buffer[Index+72] = MaxHeight + 0.375f;
            Buffer[Index+73] = Spc1TexS + Spc1TexW;
            Buffer[Index+74] = Spc1TexT + Spc1TexH;

            Buffer[Index+75] = Pt5X;
            Buffer[Index+76] = Pt5Y;
            Buffer[Index+77] = MaxHeight + 0.375f;
            Buffer[Index+78] = Spc1TexS + Spc1TexW;
            Buffer[Index+79] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+80] = Pt4X;
            Buffer[Index+81] = Pt4Y;
            Buffer[Index+82] = MaxHeight - 0.75f;
            Buffer[Index+83] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+84] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+85] = Pt1X;
            Buffer[Index+86] = Pt1Y;
            Buffer[Index+87] = MaxHeight - 0.75f;
            Buffer[Index+88] = Spc1TexS + 0.67f*Spc1TexW;
            Buffer[Index+89] = Spc1TexT;
            
            Index += 90;
            TotalFloats += 90;

            // The ends of the bar

            Buffer[Index   ] = Pt1X;
            Buffer[Index+1 ] = Pt1Y;
            Buffer[Index+2 ] = MaxHeight - 0.75f;
            Buffer[Index+3 ] = Spc1TexS;
            Buffer[Index+4 ] = Spc1TexT;
            
            Buffer[Index+5 ] = Pt3X;
            Buffer[Index+6 ] = Pt3Y;
            Buffer[Index+7 ] = MaxHeight - 0.75f;
            Buffer[Index+8 ] = Spc1TexS;
            Buffer[Index+9 ] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+10] = Pt2X;
            Buffer[Index+11] = Pt2Y;
            Buffer[Index+12] = MaxHeight + 0.375f;
            Buffer[Index+13] = Spc1TexS + Spc1TexW;
            Buffer[Index+14] = Spc1TexT + 0.5f*Spc1TexH;

            Buffer[Index+15] = Pt6X;
            Buffer[Index+16] = Pt6Y;
            Buffer[Index+17] = MaxHeight - 0.75f;
            Buffer[Index+18] = Spc1TexS;
            Buffer[Index+19] = Spc1TexT;
            
            Buffer[Index+20] = Pt4X;
            Buffer[Index+21] = Pt4Y;
            Buffer[Index+22] = MaxHeight - 0.75f;
            Buffer[Index+23] = Spc1TexS;
            Buffer[Index+24] = Spc1TexT + Spc1TexH;
            
            Buffer[Index+25] = Pt5X;
            Buffer[Index+26] = Pt5Y;
            Buffer[Index+27] = MaxHeight + 0.375f;
            Buffer[Index+28] = Spc1TexS + Spc1TexW;
            Buffer[Index+29] = Spc1TexT + 0.5f*Spc1TexH;

            Index += 30;
            TotalFloats += 30;

        }

    }

    return TotalFloats;
    
}

int PlainMapGen::LoadCommonDecoFloats (float* Buffer, int Index, int CellNo, int NoOfDecos, int* DecoTypes, float* DecoData) {
    
    int TotalFloats = 0;
    int n;
    
    float PX, PY;
    float CX, CY;
    float C, S;
    
    for (n = 0; n < NoOfDecos; n++) {
        CX = DecoData[n*3];
        CY = DecoData[n*3 + 1];
        C = (float)cos ((double)DecoData[n*3 + 2] * M_PI / 180.0);
        S = (float)sin ((double)DecoData[n*3 + 2] * M_PI / 180.0);
        switch (DecoTypes[n]) {
            case 0:
				// Rock 1
                PX = -0.2f;
                PY = -0.2f;
                Buffer[Index   ] = CX + C*PX - S*PY;
                Buffer[Index+1 ] = CY + S*PX + C*PY;
                Buffer[Index+2 ] = 0.0f;
                Buffer[Index+3 ] = Spc1TexS;
                Buffer[Index+4 ] = Spc1TexT;
                PX = 0.2f;
                PY = -0.2f;
                Buffer[Index+5 ] = CX + C*PX - S*PY;
                Buffer[Index+6 ] = CY + S*PX + C*PY;
                Buffer[Index+7 ] = 0.0f;
				Buffer[Index+8 ] = Spc1TexS + Spc1TexW;
				Buffer[Index+9 ] = Spc1TexT;
                PX = 0.0f;
                PY = 0.1f;
                Buffer[Index+10] = CX + C*PX - S*PY;
                Buffer[Index+11] = CY + S*PX + C*PY;
                Buffer[Index+12] = 0.1f;
				Buffer[Index+13] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+14] = Spc1TexT + 0.5f*Spc1TexH;
				PX = 0.2f;
				PY = -0.2f;
				Buffer[Index+15] = CX + C*PX - S*PY;
				Buffer[Index+16] = CY + S*PX + C*PY;
				Buffer[Index+17] = 0.0f;
				Buffer[Index+18] = Spc1TexS + Spc1TexW;
				Buffer[Index+19] = Spc1TexT;
				PX = 0.0f;
				PY = 0.3f;
				Buffer[Index+20] = CX + C*PX - S*PY;
				Buffer[Index+21] = CY + S*PX + C*PY;
				Buffer[Index+22] = 0.0f;
				Buffer[Index+23] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+24] = Spc1TexT + Spc1TexH;
				PX = 0.0f;
				PY = 0.1f;
				Buffer[Index+25] = CX + C*PX - S*PY;
				Buffer[Index+26] = CY + S*PX + C*PY;
				Buffer[Index+27] = 0.1f;
				Buffer[Index+28] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+29] = Spc1TexT + 0.5f*Spc1TexH;
				PX = 0.0f;
				PY = 0.3f;
				Buffer[Index+30] = CX + C*PX - S*PY;
				Buffer[Index+31] = CY + S*PX + C*PY;
				Buffer[Index+32] = 0.0f;
				Buffer[Index+33] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+34] = Spc1TexT + Spc1TexH;
				PX = -0.2f;
				PY = -0.2f;
				Buffer[Index+35] = CX + C*PX - S*PY;
				Buffer[Index+36] = CY + S*PX + C*PY;
				Buffer[Index+37] = 0.0f;
				Buffer[Index+38] = Spc1TexS;
				Buffer[Index+39] = Spc1TexT;
				PX = 0.0f;
				PY = 0.1f;
				Buffer[Index+40] = CX + C*PX - S*PY;
				Buffer[Index+41] = CY + S*PX + C*PY;
				Buffer[Index+42] = 0.1f;
				Buffer[Index+43] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+44] = Spc1TexT + 0.5f*Spc1TexH;
                TotalFloats += 45;
                Index += 45;
                break;
            case 1:
				// Rock 2 (pair of rocks)
                PX = -0.4f;
                PY = -0.2f;
                Buffer[Index   ] = CX + C*PX - S*PY;
                Buffer[Index+1 ] = CY + S*PX + C*PY;
                Buffer[Index+2 ] = 0.0f;
                Buffer[Index+3 ] = Spc1TexS;
                Buffer[Index+4 ] = Spc1TexT;
                PX = -0.1f;
                PY = -0.2f;
                Buffer[Index+5 ] = CX + C*PX - S*PY;
                Buffer[Index+6 ] = CY + S*PX + C*PY;
                Buffer[Index+7 ] = 0.0f;
				Buffer[Index+8 ] = Spc1TexS + Spc1TexW;
				Buffer[Index+9 ] = Spc1TexT;
                PX = -0.25f;
                PY = -0.1f;
                Buffer[Index+10] = CX + C*PX - S*PY;
                Buffer[Index+11] = CY + S*PX + C*PY;
                Buffer[Index+12] = 0.1f;
				Buffer[Index+13] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+14] = Spc1TexT + 0.5f*Spc1TexH;
				PX = -0.1f;
				PY = -0.2f;
				Buffer[Index+15] = CX + C*PX - S*PY;
				Buffer[Index+16] = CY + S*PX + C*PY;
				Buffer[Index+17] = 0.0f;
				Buffer[Index+18] = Spc1TexS + Spc1TexW;
				Buffer[Index+19] = Spc1TexT;
				PX = -0.25f;
				PY = 0.0f;
				Buffer[Index+20] = CX + C*PX - S*PY;
				Buffer[Index+21] = CY + S*PX + C*PY;
				Buffer[Index+22] = 0.0f;
				Buffer[Index+23] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+24] = Spc1TexT + Spc1TexH;
				PX = -0.25f;
				PY = -0.1f;
				Buffer[Index+25] = CX + C*PX - S*PY;
				Buffer[Index+26] = CY + S*PX + C*PY;
				Buffer[Index+27] = 0.1f;
				Buffer[Index+28] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+29] = Spc1TexT + 0.5f*Spc1TexH;
				PX = -0.25f;
				PY = 0.0f;
				Buffer[Index+30] = CX + C*PX - S*PY;
				Buffer[Index+31] = CY + S*PX + C*PY;
				Buffer[Index+32] = 0.0f;
				Buffer[Index+33] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+34] = Spc1TexT + Spc1TexH;
				PX = -0.4f;
				PY = -0.2f;
				Buffer[Index+35] = CX + C*PX - S*PY;
				Buffer[Index+36] = CY + S*PX + C*PY;
				Buffer[Index+37] = 0.0f;
				Buffer[Index+38] = Spc1TexS;
				Buffer[Index+39] = Spc1TexT;
				PX = -0.25f;
				PY = -0.1f;
				Buffer[Index+40] = CX + C*PX - S*PY;
				Buffer[Index+41] = CY + S*PX + C*PY;
				Buffer[Index+42] = 0.1f;
				Buffer[Index+43] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+44] = Spc1TexT + 0.5f*Spc1TexH;
				PX = 0.1f;
				PY = 0.0f;
				Buffer[Index+45] = CX + C*PX - S*PY;
				Buffer[Index+46] = CY + S*PX + C*PY;
				Buffer[Index+47] = 0.1f;
				Buffer[Index+48] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+49] = Spc1TexT + 0.5f*Spc1TexH;
				PX = 0.3f;
				PY = 0.1f;
				Buffer[Index+50] = CX + C*PX - S*PY;
				Buffer[Index+51] = CY + S*PX + C*PY;
				Buffer[Index+52] = 0.0f;
				Buffer[Index+53] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+54] = Spc1TexT + Spc1TexH;
				PX = 0.2f;
				PY = 0.1f;
				Buffer[Index+55] = CX + C*PX - S*PY;
				Buffer[Index+56] = CY + S*PX + C*PY;
				Buffer[Index+57] = 0.0f;
				Buffer[Index+58] = Spc1TexS;
				Buffer[Index+59] = Spc1TexT;
				PX = 0.3f;
				PY = 0.1f;
				Buffer[Index+60] = CX + C*PX - S*PY;
				Buffer[Index+61] = CY + S*PX + C*PY;
				Buffer[Index+62] = 0.1f;
				Buffer[Index+63] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+64] = Spc1TexT + 0.5f*Spc1TexH;
				PX = 0.2f;
				PY = 0.2f;
				Buffer[Index+65] = CX + C*PX - S*PY;
				Buffer[Index+66] = CY + S*PX + C*PY;
				Buffer[Index+67] = 0.1f;
				Buffer[Index+68] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+69] = Spc1TexT + 0.5f*Spc1TexH;
				PX = 0.2f;
				PY = 0.1f;
				Buffer[Index+70] = CX + C*PX - S*PY;
				Buffer[Index+71] = CY + S*PX + C*PY;
				Buffer[Index+72] = 0.0f;
				Buffer[Index+73] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+74] = Spc1TexT + Spc1TexH;
				PX = 0.2f;
				PY = 0.2f;
				Buffer[Index+75] = CX + C*PX - S*PY;
				Buffer[Index+76] = CY + S*PX + C*PY;
				Buffer[Index+77] = 0.0f;
				Buffer[Index+78] = Spc1TexS;
				Buffer[Index+79] = Spc1TexT;
				PX = 0.1f;
				PY = 0.0f;
				Buffer[Index+80] = CX + C*PX - S*PY;
				Buffer[Index+81] = CY + S*PX + C*PY;
				Buffer[Index+82] = 0.1f;
				Buffer[Index+83] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+84] = Spc1TexT + 0.5f*Spc1TexH;
				PX = 0.2f;
				PY = 0.1f;
				Buffer[Index+85] = CX + C*PX - S*PY;
				Buffer[Index+86] = CY + S*PX + C*PY;
				Buffer[Index+87] = 0.1f;
				Buffer[Index+88] = Spc1TexS + 0.5f*Spc1TexW;
				Buffer[Index+89] = Spc1TexT + 0.5f*Spc1TexH;
                TotalFloats += 90;
                Index += 90;
                break;
        }
    }
    
    return TotalFloats;
    
}
