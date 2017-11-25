
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

// Prevent erros which inhibit use of deprecated functions (eg. strcpy)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <math.h>
#include <stdio.h>
#include <string.h>

#ifndef _STDLIB_H_
#include <stdlib.h>
#endif

// Key parameters
const float RandMax = static_cast<float>(RAND_MAX);
const float BoundXMin = -50.0f;
const float BoundXMax = 50.0f;
const float BoundYMin = 0.0f;
const float BoundYMax = 200.0f;
const float EntryWidth = 10.0f;
const int NoOfSites = 200;
const int MaxAdjacentCells = 15;

template <class T> class GrimQueue;
class GrimLineData;
class GrimParabola;
class GrimEvent;

// Voronoi Diagram class
class Voronoi {
    
    public:
        
        Voronoi (int);
        
        static float* GenerateBoundaryBuffer ();
        
        float* GenerateSiteBuffer (int);
        float* GenerateDiagramReturnLines (int*, int);
        float* GenerateDiagramVertices (int*);
        float* GenerateSubVertices (int*);
        
        // Buffers for subset map data
        int NoOfSubsetSites;
        int CellType[NoOfSites];
        float SubSiteX[NoOfSites];
        float SubSiteY[NoOfSites];
        int SubLinesForThisSite[NoOfSites];
        int SubAdjacentSite[NoOfSites][MaxAdjacentCells];
        float SubAdjacentDistance[NoOfSites][MaxAdjacentCells];
        float SubBorderWidth[NoOfSites][MaxAdjacentCells];
        int SubStartCorner[NoOfSites][MaxAdjacentCells];
        int SubEndCorner[NoOfSites][MaxAdjacentCells];
        float SubStartX[NoOfSites][MaxAdjacentCells];
        float SubStartY[NoOfSites][MaxAdjacentCells];
        float SubEndX[NoOfSites][MaxAdjacentCells];
        float SubEndY[NoOfSites][MaxAdjacentCells];
        
    private:
        
        // Internal data
        
        int NextCornerNo;
        float Sites[2*NoOfSites];
        
        // Buffers to fill while sorting raw line data
        int* LinesForThisSite;
        int (*AdjacentSite)[MaxAdjacentCells];
        float (*AdjacentDistance)[MaxAdjacentCells];
        float (*BorderWidth)[MaxAdjacentCells];
        int (*StartCorner)[MaxAdjacentCells];
        int (*EndCorner)[MaxAdjacentCells];
        float (*StartX)[MaxAdjacentCells];
        float (*StartY)[MaxAdjacentCells];
        float (*EndX)[MaxAdjacentCells];
        float (*EndY)[MaxAdjacentCells];
        
        // Functions for generating output
        void InsertNew (GrimParabola**, float, float, int, float, GrimQueue<GrimEvent>*);
        int DeleteParabola (GrimParabola**, GrimParabola*, float, float, float, float,
                float, float, float, GrimQueue<GrimEvent>*, GrimQueue<GrimLineData>*);
        
        float* DecodeLineData (GrimQueue<GrimLineData>*, int*);
        void AddOutputLine (GrimQueue<GrimLineData>*, float, float, float, float, int, int, int, int);
        void RunAStar (int[], int, int);
        void BuildSubset (int);

};

// Map generator class
class PlainMapGen {
    
    protected:
        
        // Texture coordinates set in specific constructors
        
        const float GrndTexS = 0.0f;
		const float GrndTexT = 0.875f;
		const float GrndTexW = 0.25f;
		const float GrndTexH = 0.125f;
        
		const float WallTexS = 0.5f;
		const float WallTexT = 0.75f;
		const float WallTexW = 0.125f;
		const float WallTexH = 0.25f;
    
		const float PoolTexS = 0.25f;
		const float PoolTexT = 0.875f;
		const float PoolTexW = 0.125f;
		const float PoolTexH = 0.125f;
        
		const float BankTexS = 0.375f;
		const float BankTexT = 0.875f;
		const float BankTexW = 0.125f;
		const float BankTexH = 0.125f;

		const float Spc1TexS = 0.0f;
		const float Spc1TexT = 0.625f;
		const float Spc1TexW = 0.125f;
		const float Spc1TexH = 0.125f;

		const float Spc2TexS = 0.125f;
		const float Spc2TexT = 0.625f;
		const float Spc2TexW = 0.125f;
		const float Spc2TexH = 0.125f;

		const float Spc3TexS = 0.25f;
		const float Spc3TexT = 0.625f;
		const float Spc3TexW = 0.125f;
		const float Spc3TexH = 0.125f;

        // Random parameters used for map generation
        
        float CornerWallHeight[3*NoOfSites];
        float CornerTexS[3*NoOfSites];
        float CornerTexT[3*NoOfSites];
        float CornerTe2S[3*NoOfSites];
        float CornerTe2T[3*NoOfSites];
        float CornerTe3S[3*NoOfSites];
        float CornerTe3T[3*NoOfSites];
        float SiteTexS[NoOfSites];
        float SiteTexT[NoOfSites];
        
        // Get floats required to build standard components
        
        int GetCommonFloorNoOfFloats (int);
        int GetCommonReachableWallNoOfFloats (int);
        int GetCommonUnreachableWallNoOfFloats (int);
        int GetCommonCliffNoOfFloats (int);
        int GetCommonBoulderNoOfFloats (int);
        int GetCommonSunkenNoOfFloats (int);
        int GetCommonRubbleNoOfFloats (int);
        int GetCommonDecoNoOfFloats (int, int, int*);
        
        // Build common components into a buffer
        
        int LoadCommonFloorFloats (float*, int, int);
        int LoadCommonReachableWallFloats (float*, int, int);
        int LoadCommonUnreachableWallFloats (float*, int, int);
        int LoadCommonCliffFloats (float*, int, int);
        int LoadCommonBoulderFloats (float*, int, int);
        int LoadCommonSunkenFloats (float*, int, int);
        int LoadCommonRubbleFloats (float*, int, int);
        int LoadCommonDecoFloats (float*, int, int, int, int*, float*);
        
        // Get floats required to build this component
        
        virtual int GetFloorCellNoOfFloats (int) = 0;
        virtual int GetWalledFloorCellNoOfFloats (int) = 0;
        virtual int GetBlockedCellNoOfFloats (int) = 0;
        virtual int GetEnclosedCellNoOfFloats (int) = 0;
        
        // Build the components into a buffer passed as an argument
        
        virtual int LoadFloorCellFloats (float*, int, int) = 0;
        virtual int LoadWalledFloorCellFloats (float*, int, int) = 0;
        virtual int LoadBlockedCellFloats (float*, int, int) = 0;
        virtual int LoadEnclosedCellFloats (float*, int, int) = 0;
        
        // Pointers to cell data from an active voronoi diagram object
        
        int NoOfSubsetSites;
        int* CellType;
        float* SubSiteX;
        float* SubSiteY;
        int* SubLinesForThisSite;
        int (*SubAdjacentSite)[MaxAdjacentCells];
        int (*SubStartCorner)[MaxAdjacentCells];
        int (*SubEndCorner)[MaxAdjacentCells];
        float (*SubStartX)[MaxAdjacentCells];
        float (*SubStartY)[MaxAdjacentCells];
        float (*SubEndX)[MaxAdjacentCells];
        float (*SubEndY)[MaxAdjacentCells];
        int AdjExtCW[NoOfSites][MaxAdjacentCells];
        int AdjExtCCW[NoOfSites][MaxAdjacentCells];
        
        // Optional extra cell data
        
        int* ExtCellType;
		int* ExtDecoCount;
		int **ExtDecoType;
		float **ExtDecoParams;
        int **ExtIntVector;
        float **ExtFloatVector;
        
    public:
        
        // Interface functions
        
        PlainMapGen (Voronoi*);
        virtual ~PlainMapGen () = 0;
        static float* GenerateMapData (int*, int, Voronoi*, char*);
        void FinishObject();
        
    private:
        
        // Internal functions
        
        int GetTotalFloats ();
        float* GetBuffer (int);
        
};

class ForestMapGen : public PlainMapGen {
    
    public:
        
        // Constructor, sets default texture parameters
        
        ForestMapGen (Voronoi*);
        virtual ~ForestMapGen ();
        
    protected:

        // Get floats required to build this component
        
        int GetFloorCellNoOfFloats (int);
        int GetWalledFloorCellNoOfFloats (int);
        int GetBlockedCellNoOfFloats (int);
        int GetEnclosedCellNoOfFloats (int);
        
        // Build the components into a buffer passed as an argument
        
        int LoadFloorCellFloats (float*, int, int);
        int LoadWalledFloorCellFloats (float*, int, int);
        int LoadBlockedCellFloats (float*, int, int);
        int LoadEnclosedCellFloats (float*, int, int);
        
};

class LavaCaveMapGen : public PlainMapGen {
    
    public:
        
        // Constructor, sets default texture parameters
        
        LavaCaveMapGen (Voronoi*);
        virtual ~LavaCaveMapGen ();
        
    protected:

        // Get floats required to build this component
        
        int GetFloorCellNoOfFloats (int);
        int GetWalledFloorCellNoOfFloats (int);
        int GetBlockedCellNoOfFloats (int);
        int GetEnclosedCellNoOfFloats (int);
        
        // Build the components into a buffer passed as an argument
        
        int LoadFloorCellFloats (float*, int, int);
        int LoadWalledFloorCellFloats (float*, int, int);
        int LoadBlockedCellFloats (float*, int, int);
        int LoadEnclosedCellFloats (float*, int, int);
        
};

class LakeMapGen : public PlainMapGen {
    
    public:
        
        // Constructor, sets default texture parameters
        
        LakeMapGen (Voronoi*);
        virtual ~LakeMapGen ();
        
    protected:

        // Get floats required to build this component
        
        int GetFloorCellNoOfFloats (int);
        int GetWalledFloorCellNoOfFloats (int);
        int GetBlockedCellNoOfFloats (int);
        int GetEnclosedCellNoOfFloats (int);
        
        // Build the components into a buffer passed as an argument
        
        int LoadFloorCellFloats (float*, int, int);
        int LoadWalledFloorCellFloats (float*, int, int);
        int LoadBlockedCellFloats (float*, int, int);
        int LoadEnclosedCellFloats (float*, int, int);
        
};

class AridMapGen : public PlainMapGen {
    
    public:
        
        // Constructor, sets default texture parameters
        
        AridMapGen (Voronoi*);
        virtual ~AridMapGen ();
        
    protected:

        // Get floats required to build this component
        
        int GetFloorCellNoOfFloats (int);
        int GetWalledFloorCellNoOfFloats (int);
        int GetBlockedCellNoOfFloats (int);
        int GetEnclosedCellNoOfFloats (int);
        
        // Build the components into a buffer passed as an argument
        
        int LoadFloorCellFloats (float*, int, int);
        int LoadWalledFloorCellFloats (float*, int, int);
        int LoadBlockedCellFloats (float*, int, int);
        int LoadEnclosedCellFloats (float*, int, int);
        
};

class CaveMapGen : public PlainMapGen {
    
    public:
        
        // Constructor, sets default texture parameters
        
        CaveMapGen (Voronoi*);
        virtual ~CaveMapGen ();
        
    protected:

        // Get floats required to build this component
        
        int GetFloorCellNoOfFloats (int);
        int GetWalledFloorCellNoOfFloats (int);
        int GetBlockedCellNoOfFloats (int);
        int GetEnclosedCellNoOfFloats (int);
        
        // Build the components into a buffer passed as an argument
        
        int LoadFloorCellFloats (float*, int, int);
        int LoadWalledFloorCellFloats (float*, int, int);
        int LoadBlockedCellFloats (float*, int, int);
        int LoadEnclosedCellFloats (float*, int, int);
        
};

class SnowMapGen : public PlainMapGen {
    
    public:
        
        // Constructor, sets default texture parameters
        
        SnowMapGen (Voronoi*);
        virtual ~SnowMapGen ();
        
    protected:

        // Get floats required to build this component
        
        int GetFloorCellNoOfFloats (int);
        int GetWalledFloorCellNoOfFloats (int);
        int GetBlockedCellNoOfFloats (int);
        int GetEnclosedCellNoOfFloats (int);
        
        // Build the components into a buffer passed as an argument
        
        int LoadFloorCellFloats (float*, int, int);
        int LoadWalledFloorCellFloats (float*, int, int);
        int LoadBlockedCellFloats (float*, int, int);
        int LoadEnclosedCellFloats (float*, int, int);
        
};

class VolcanoMapGen : public PlainMapGen {
    
    public:
        
        // Constructor, sets default texture parameters
        
        VolcanoMapGen (Voronoi*);
        virtual ~VolcanoMapGen ();
        
    protected:

        // Get floats required to build this component
        
        int GetFloorCellNoOfFloats (int);
        int GetWalledFloorCellNoOfFloats (int);
        int GetBlockedCellNoOfFloats (int);
        int GetEnclosedCellNoOfFloats (int);
        
        // Build the components into a buffer passed as an argument
        
        int LoadFloorCellFloats (float*, int, int);
        int LoadWalledFloorCellFloats (float*, int, int);
        int LoadBlockedCellFloats (float*, int, int);
        int LoadEnclosedCellFloats (float*, int, int);
        
};

class FlowersMapGen : public PlainMapGen {
    
    public:
        
        // Constructor, sets default texture parameters
        
        FlowersMapGen (Voronoi*);
        virtual ~FlowersMapGen ();
        
    protected:

        // Get floats required to build this component
        
        int GetFloorCellNoOfFloats (int);
        int GetWalledFloorCellNoOfFloats (int);
        int GetBlockedCellNoOfFloats (int);
        int GetEnclosedCellNoOfFloats (int);
        
        // Build the components into a buffer passed as an argument
        
        int LoadFloorCellFloats (float*, int, int);
        int LoadWalledFloorCellFloats (float*, int, int);
        int LoadBlockedCellFloats (float*, int, int);
        int LoadEnclosedCellFloats (float*, int, int);
        
};
