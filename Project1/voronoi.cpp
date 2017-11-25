
#include "classes.hpp"

// Queue class
template <class T>
class GrimQueue {
    
    private:
        int Capacity;
        T* Data;
        int QueuedElements;
        int Position;
    
    public:
        
        GrimQueue (int SetSize) {
            
            Capacity = SetSize;
            Data = new T[SetSize];
            QueuedElements = 0;
            Position = 0;
            
        }
        
        ~GrimQueue () {
            
            delete[] Data;
            
        }
        
        int GetQueuedElements () {
            
            return QueuedElements;
            
        }
        
        int Add (T NewElement) {
            
            // Return if the queue is full
            if (QueuedElements >= Capacity)
                return -1;
            
            // Get the position of the new element
            int GetPos = Position + QueuedElements;
            while (GetPos >= Capacity) GetPos -= Capacity;
            
            // Place the data in the queue
            Data[GetPos] = NewElement;
            QueuedElements++;
            
            return 0;
            
        }
        
        T Remove () {
            
            if (QueuedElements <= 0) return T ();
            
            T Get = Data[Position];
            Position++;
            if (Position >= Capacity) Position = 0;
            QueuedElements--;
            
            return Get;
            
        }
        
        void SortAscending () {
            
            // Get the end position of elements to sort (start position is "Position")
            int EndPos = Position + QueuedElements - 1;
            while (EndPos >= Capacity) EndPos -= Capacity;
            
            // Sort them
            int n;
            int CheckNext;
            int ElementsSorted;
            T Temp;
            do {
                ElementsSorted = 0;
                n = Position;
                while (n != EndPos) {
                    CheckNext = n + 1;
                    if (CheckNext >= Capacity) CheckNext -= Capacity;
                    if (Data[n] > Data[CheckNext]) {
                        Temp = Data[CheckNext];
                        Data[CheckNext] = Data[n];
                        Data[n] = Temp;
                        ElementsSorted++;
                    }
                    n = CheckNext;
                }
            }
            while (ElementsSorted > 0);
            
        }
        
        int InsertSorted (T NewElement) {
            
            // Return if the queue is full
            if (QueuedElements >= Capacity) return -1;
            
            // Scan until the suitable spot is found
            int InsertPoint;
            int n = Position;
            int Checks = 0;
            while ((NewElement > Data[n]) && (Checks < QueuedElements)) {
                if (++n >= Capacity) n = 0;
                Checks++;
            }
            InsertPoint = n;
            
            // Move everything forward to open the space
            n = Position + QueuedElements;
            if (n >= Capacity) n -= Capacity;
            int GetNext;
            while (n != InsertPoint) {
                // Find previous point
                GetNext = n - 1;
                if (GetNext < 0) GetNext += Capacity;
                // Copy its contents across
                Data[n] = Data[GetNext];
                n = GetNext;
            }
            Data[InsertPoint] = NewElement;
            QueuedElements++;
            
            return 0;
            
        }
        
        int SearchDelete (T Remove) {
            
            int n = Position;
            int Final = Position + QueuedElements;
            if (Final >= Capacity) Final -= Capacity;
            int RemovePoint = -1;
            
            do {
                if (Data[n] == Remove) {
                    RemovePoint = n;
                    break;
                }
                n++;
                if (n >= Capacity) n -= Capacity;
            }
            while (n != Final);
            
            if (RemovePoint < 0) return 0;
            
            n = RemovePoint;
            do {
                if (n + 1 < Capacity) {Data[n] = Data[n+1]; n++;}
                else {Data[n] = Data[0]; n = 0;}
            }
            while (n != Final);
            QueuedElements--;
            
            return 1;
            
        }
        
};

// Class for output lines
class GrimLineData {
    public:
        float Pt1X, Pt1Y, Pt2X, Pt2Y;
        int Site1, Site2;
        int Corner1, Corner2;
        GrimLineData () {
            Pt1X = 0.0f;
            Pt1Y = 0.0f;
            Pt2X = 0.0f;
            Pt2Y = 0.0f;
            Site1 = 0;
            Site2 = 0;
            Corner1 = 0;
            Corner2 = 0;
        }
        GrimLineData (float X1, float Y1, float X2, float Y2, int S1, int S2, int C1, int C2) {
            Pt1X = X1;
            Pt1Y = Y1;
            Pt2X = X2;
            Pt2Y = Y2;
            Site1 = S1;
            Site2 = S2;
            Corner1 = C1;
            Corner2 = C2;
        }
};

// Class for edges between parabolas
class GrimEdgeTracer {
    
    public:
        float FirstX, FirstY;
        int ParabolaPointsAttached;
        int StartCornerNo;
    
        GrimEdgeTracer () {
            
            ParabolaPointsAttached = 4;
            
        }
        
        GrimEdgeTracer (float StartX, float StartY, int StartCorner) {
            
            FirstX = StartX;
            FirstY = StartY;
            ParabolaPointsAttached = 2;
            StartCornerNo = StartCorner;
            
        }
        
        int SetEndpoint (float X, float Y, int SetCornerNo) {
            
            ParabolaPointsAttached -= 2;
            
            if (ParabolaPointsAttached > 0) {
                FirstX = X;
                FirstY = Y;
                StartCornerNo = SetCornerNo;
                return 1;
            }
            
            return 0;
            
        }
    
};

// Class for test points ('events')
class GrimEvent {
    
    friend class Voronoi;
    friend class GrimParabola;
    
    private:
        
        float PtX, PtY;
        float CircleY;
        int EventIsSite;
        int SiteNo;
        GrimParabola *CircleClosesParabola;
        float OtherFocus1X, OtherFocus1Y, OtherFocus2X, OtherFocus2Y;
        
        GrimEvent (float, float, int);
        GrimEvent (float, float, float, GrimParabola*, float, float, float, float);
    
    public:
        
        GrimEvent ();
        int operator> (const GrimEvent&);
        int operator== (const GrimEvent&);
    
};

// Classes for arcs and arc intersections
class GrimParabola {
    
    friend class Voronoi;
    
    private:
        
        float   FocusX,
                FocusY;
        
    public:
        
        GrimEdgeTracer *TraceEdgeLeft, *TraceEdgeRight;
        GrimParabola *Prev, *Next;
        int SiteNo;
        
        // Constructors for a parabola
        GrimParabola (
                GrimParabola *SetPrevious, GrimParabola *SetNext,
                float PtX, float PtY,
                int SetSiteNo,
                GrimEdgeTracer *SetLeftEdge = 0, GrimEdgeTracer *SetRightEdge = 0
        ) {
            
            Prev = SetPrevious;
            Next = SetNext;
            FocusX = PtX;
            FocusY = PtY;
            SiteNo = SetSiteNo;
            
            TraceEdgeLeft = SetLeftEdge;
            TraceEdgeRight = SetRightEdge;
            
        }
        
        // Get private focus point
        void GetFocus (float* FX, float* FY) {
            
            *FX = FocusX;
            *FY = FocusY;
            
        }
        
        // Get Y coordinate of a parabola
        float GetY (float X, float SweepY) {
            if (SweepY == FocusY) return -1.0e20f;
            return 0.5f * (SweepY + FocusY - (X - FocusX)*(X - FocusX) / (SweepY - FocusY));
        }
        
        // Given a sweep line coordinate, find the y value and arc on the beachline for a given x value.
        static GrimParabola* GetPeakY (float *ReturnPeakY, GrimParabola *Base, float X, float SweepY) {
            
            // Variables to track the highest point
            float PeakY = -1.0e20f;
            float CheckY;
            GrimParabola *ClosestParabola = 0;
            
            // Quadratic formula variables
            float A, B, C;
            float SR, D1, D2;
            float R1, R2, Rm;
            float p1, p2;
            
            while (Base != 0) {
                
                if (Base->Next != 0) {
                    // Check intersection points with this parabola and the next.
                    // Intersections with the previous one may not be relevant.
                    if (Base->FocusY == SweepY) {Base = Base->Next; continue;}
                    else if (Base->Next->FocusY == SweepY) {
                        if (Base->Next->FocusX < X) {Base = Base->Next; continue;}
                    }
                    if (Base->FocusY == Base->Next->FocusY) {
                        R1 = 0.5f*(Base->FocusX + Base->Next->FocusX);
                        R2 = R1;
                    }
                    else {
                        D1 = SweepY - Base->FocusY;
                        D2 = SweepY - Base->Next->FocusY;
                        A = Base->FocusY - Base->Next->FocusY;
                        B = 2*Base->Next->FocusX*D1 - 2*Base->FocusX*D2;
                        C = Base->FocusX*Base->FocusX*D2 - Base->Next->FocusX*Base->Next->FocusX*D1 + (Base->Next->FocusY - Base->FocusY)*D1*D2;
                        SR = sqrt (B*B - 4*A*C);
                        R1 = (-B - SR)/(2*A);
                        R2 = (-B + SR)/(2*A);
                        if (R2 < R1) {SR = R2; R2 = R1; R1 = SR;}
                    }
                    // Check which parabola is higher between the two intersections
                    Rm = 0.5f*(R1 + R2);
                    p1 = Base->GetY (Rm, SweepY);
                    p2 = Base->Next->GetY (Rm, SweepY);
                    // If this one is higher, consider points only between the intersection points
                    if (p1 > p2) {
                        if (X > R2) {Base = Base->Next; continue;}
                    }
                    // Else, consider points only before the first intersection
                    else {
                        if (X > R1) {Base = Base->Next; continue;}
                    }
                }
                
                CheckY = Base->GetY (X, SweepY);
                if (CheckY > PeakY) {
                    PeakY = CheckY;
                    ClosestParabola = Base;
                }
                
                
                if (Base->Next != 0) {
                    // Check whether the next arc is past the test point
                    if (p1 > p2) {
                        if (X < R2) break;
                    }
                    else {
                        if (X < R1) break;
                    }
                }
                
                Base = Base->Next;
                
            }
            
            *ReturnPeakY = PeakY;
            return ClosestParabola;
            
        }
        
        // Find the intersection point between this parabola and the next
        void GetNextIntersection (float SweepY, float* ReturnX, float* ReturnY) {
            
            // Quadratic formula variables
            float A, B, C;
            float SR, D1, D2;
            float R1, R2, Rm;
            float p1, p2;
            
            // Find X coordinates of this parabola with the next. Order them such that R1 < R2.
            D1 = SweepY - FocusY;
            D2 = SweepY - Next->FocusY;
            A = FocusY - Next->FocusY;
            B = 2*Next->FocusX*D1 - 2*FocusX*D2;
            C = FocusX*FocusX*D2 - Next->FocusX*Next->FocusX*D1 + (Next->FocusY - FocusY)*D1*D2;
            SR = sqrt (B*B - 4*A*C);
            R1 = (-B - SR)/(2*A);
            R2 = (-B + SR)/(2*A);
            if (R2 < R1) {SR = R2; R2 = R1; R1 = SR;}
            
            // Check which parabola is higher between the two intersections
            Rm = 0.5f*(R1 + R2);
            p1 = GetY (Rm, SweepY);
            p2 = Next->GetY (Rm, SweepY);
            
            // Return the point where this parabola arc intersects the next
            if (p1 > p2) {
                *ReturnX = R2;
                *ReturnY = GetY (R2, SweepY);
            }
            else {
                *ReturnX = R1;
                *ReturnY = GetY (R1, SweepY);
            }
            
        }
        
        // Find the y coordinate of a cicle event for three points
        static int GetCircleEvent (GrimParabola *Para2, float *GetX, float *GetY, float *GetRadius) {
            
            // Check input parabolas
            if (Para2 == 0) return 0;
            GrimParabola *Para1 = Para2->Prev;
            GrimParabola *Para3 = Para2->Next;
            if (Para1 == 0 || Para3 == 0) return 0;
            
            // Sort the points so the same set of parabolas always yileds the same results
            GrimParabola* Temp;
            GrimEvent GetObj1 = GrimEvent (Para1->FocusX, Para1->FocusY, -1);
            GrimEvent GetObj2 = GrimEvent (Para2->FocusX, Para2->FocusY, -1);
            GrimEvent GetObj3 = GrimEvent (Para3->FocusX, Para3->FocusY, -1);
            
            if (GetObj1 > GetObj2) {
                Temp = Para1;
                Para1 = Para2;
                Para2 = Temp;
                GetObj1 = GrimEvent (Para1->FocusX, Para1->FocusY, -1);
                GetObj2 = GrimEvent (Para2->FocusX, Para2->FocusY, -1);
            }
            if (GetObj2 > GetObj3) {
                Temp = Para2;
                Para2 = Para3;
                Para3 = Temp;
                GetObj2 = GrimEvent (Para2->FocusX, Para2->FocusY, -1);
                GetObj3 = GrimEvent (Para3->FocusX, Para3->FocusY, -1);
            }
            if (GetObj1 > GetObj2) {
                Temp = Para1;
                Para1 = Para2;
                Para2 = Temp;
                GetObj1 = GrimEvent (Para1->FocusX, Para1->FocusY, -1);
                GetObj2 = GrimEvent (Para2->FocusX, Para2->FocusY, -1);
            }
            
            // Check for identical points
            double Pt1X = (double)Para1->FocusX;
            double Pt1Y = (double)Para1->FocusY;
            double Pt2X = (double)Para2->FocusX;
            double Pt2Y = (double)Para2->FocusY;
            double Pt3X = (double)Para3->FocusX;
            double Pt3Y = (double)Para3->FocusY;
            if (Pt1X == Pt2X && Pt1Y == Pt2Y) return 0;
            if (Pt1X == Pt3X && Pt1Y == Pt3Y) return 0;
            if (Pt2X == Pt3X && Pt2Y == Pt3Y) return 0;
            
            // Get the denominator
            double Den = 2.0*(Pt1X*(Pt2Y - Pt3Y) - Pt2X*(Pt1Y - Pt3Y) + Pt3X*(Pt1Y - Pt2Y));
            if (Den == 0.0) return 0;
            
            // Get the sums of squares
            double x1s = Pt1X*Pt1X + Pt1Y*Pt1Y;
            double x2s = Pt2X*Pt2X + Pt2Y*Pt2Y;
            double x3s = Pt3X*Pt3X + Pt3Y*Pt3Y;
            
            // Get the circle centre point
            double H = (x1s*(Pt2Y - Pt3Y) - x2s*(Pt1Y - Pt3Y) + x3s*(Pt1Y - Pt2Y)) / Den;
            double K = (Pt1X*(x2s - x3s) - Pt2X*(x1s - x3s) + Pt3X*(x1s - x2s)) / Den;
            
            // Get the radius
            double R = sqrt ((Pt1X - H)*(Pt1X - H) + (Pt1Y - K)*(Pt1Y - K));
            
            // Return the topmost coordinate
            *GetX = (float)H;
            *GetY = (float)(K + R);
            *GetRadius = (float)R;
            return 1;
            
        }
        
};

// Given a pointer to the left-most parabola, insert a new parabola somewhere along the beach line
void Voronoi::InsertNew (GrimParabola **Base, float PtX, float PtY, int SiteNo, float SweepY, GrimQueue<GrimEvent> *EventQueue) {
    
    // Start a new beach line if it doesn't yet exist
    if (*Base == 0) {
        *Base = new GrimParabola (0, 0, PtX, PtY, SiteNo);
        return;
    }
    
    // Find the parabola below this point
    float PeakY;
    GrimParabola *ClosestParabola = GrimParabola::GetPeakY (&PeakY, *Base, PtX, SweepY);
    
    // If none found, add this one to the end of the list
    GrimParabola *TryParabola;
    if (ClosestParabola == 0) {
        TryParabola = *Base;
        while (TryParabola->Next != 0) TryParabola = TryParabola->Next;
        TryParabola->TraceEdgeRight = new GrimEdgeTracer (0.5f*(TryParabola->FocusX + PtX), BoundYMin - 1.0f, NextCornerNo++);
        TryParabola->Next = new GrimParabola (TryParabola, 0, PtX, PtY, SiteNo, TryParabola->TraceEdgeRight, 0);
        ClosestParabola = TryParabola;
    }
    
    // Else split the nearest parabola and insert the new one in between
    else {
        // Split this nearest parabola
        TryParabola = ClosestParabola->Next;
        ClosestParabola->Next = new GrimParabola (
            ClosestParabola, ClosestParabola->Next,
            ClosestParabola->FocusX, ClosestParabola->FocusY,
            ClosestParabola->SiteNo,
            new GrimEdgeTracer (), ClosestParabola->TraceEdgeRight
        );
        if (TryParabola != 0) TryParabola->Prev = ClosestParabola->Next;
        ClosestParabola->TraceEdgeRight = ClosestParabola->Next->TraceEdgeLeft;
        // Insert the new
        ClosestParabola->Next->Prev = new GrimParabola (
                ClosestParabola, ClosestParabola->Next, PtX, PtY, SiteNo,
                ClosestParabola->TraceEdgeRight, ClosestParabola->TraceEdgeRight
        );
        ClosestParabola->Next = ClosestParabola->Next->Prev;
    }
    
    // Check for circle events
    // Circle event sweep line (GetY) is allowed to be equal to the new site's Y coordinate
    // since this can legitimately result from limitations in accuracy of using single
    // precision floats
    float GetX, GetY, GetRadius;
    if (GrimParabola::GetCircleEvent (ClosestParabola, &GetX, &GetY, &GetRadius) && GetY >= SweepY)
        if (GetX < PtX)
        (*EventQueue).InsertSorted (
                GrimEvent (
                        GetX, GetY, GetY - GetRadius, ClosestParabola,
                        ClosestParabola->Prev->FocusX, ClosestParabola->Prev->FocusY, ClosestParabola->Next->FocusX, ClosestParabola->Next->FocusY
                )
        );
    ClosestParabola = ClosestParabola->Next->Next;
    if (GrimParabola::GetCircleEvent (ClosestParabola, &GetX, &GetY, &GetRadius) && GetY >= SweepY)
        if (GetX > PtX)
        (*EventQueue).InsertSorted (
                GrimEvent (
                        GetX, GetY, GetY - GetRadius, ClosestParabola,
                        ClosestParabola->Prev->FocusX, ClosestParabola->Prev->FocusY, ClosestParabola->Next->FocusX, ClosestParabola->Next->FocusY
                )
        );
    
}

int Voronoi::DeleteParabola (
        GrimParabola **BaseParabola, GrimParabola *Delete, float Other1X, float Other1Y, float Other2X, float Other2Y,
        float SweepY, float FinishX, float FinishY, GrimQueue<GrimEvent> *EventQueue, GrimQueue<GrimLineData> *Output
) {
    
    float PeakY;

    // Ignore this circle event if its intersection point lies underneath the beachline
    GrimParabola *BeachArc = GrimParabola::GetPeakY (&PeakY, *BaseParabola, FinishX, SweepY);
    if (BeachArc != Delete && (BeachArc->FocusX != Other1X || BeachArc->FocusY != Other1Y) && (BeachArc->FocusX != Other2X || BeachArc->FocusY != Other2Y))
        return 0;
    
    // Output new lines.
    if (Delete->Prev != 0) {
        if (!Delete->TraceEdgeLeft->SetEndpoint (FinishX, FinishY, NextCornerNo)) {
            // This edge already has an endpoint marked. Finish it and delete it.
            AddOutputLine (Output, Delete->TraceEdgeLeft->FirstX, Delete->TraceEdgeLeft->FirstY, FinishX, FinishY, Delete->Prev->SiteNo, Delete->SiteNo, Delete->TraceEdgeLeft->StartCornerNo, NextCornerNo);
            delete Delete->TraceEdgeLeft;
        }
    }
    if (Delete->Next != 0) {
        if (!Delete->TraceEdgeRight->SetEndpoint (FinishX, FinishY, NextCornerNo)) {
            // This edge already has an endpoint marked. Finish it and delete it.
            AddOutputLine (Output, Delete->TraceEdgeRight->FirstX, Delete->TraceEdgeRight->FirstY, FinishX, FinishY, Delete->SiteNo, Delete->Next->SiteNo, Delete->TraceEdgeRight->StartCornerNo, NextCornerNo);
            delete Delete->TraceEdgeRight;
        }
    }
    
    // Create a new edge where the two outer parabolas now meet
    if (Delete->Prev && Delete->Next) {
        Delete->Prev->TraceEdgeRight = new GrimEdgeTracer (FinishX, FinishY, NextCornerNo);
        Delete->Next->TraceEdgeLeft = Delete->Prev->TraceEdgeRight;
    }
    
    // Increment corner number tracker
    NextCornerNo++;
    
    // Link the two outer parabolas
    Delete->Prev->Next = Delete->Next;
    Delete->Next->Prev = Delete->Prev;
    
    // Check for new circle events
    float GetX, GetY, GetRadius;
    if (Delete->Prev != 0) {
        if (GrimParabola::GetCircleEvent (Delete->Prev, &GetX, &GetY, &GetRadius) && GetY > SweepY)
            //if (GetX < FinishX)
            (*EventQueue).InsertSorted (
                    GrimEvent (
                            GetX, GetY, GetY - GetRadius, Delete->Prev,
                            Delete->Prev->Prev->FocusX, Delete->Prev->Prev->FocusY, Delete->Prev->Next->FocusX, Delete->Prev->Next->FocusY
                    )
            );
    }
    if (Delete->Next != 0) {
        if (GrimParabola::GetCircleEvent (Delete->Next, &GetX, &GetY, &GetRadius) && GetY > SweepY)
            //if (GetX > FinishX)
            (*EventQueue).InsertSorted (
                    GrimEvent (
                            GetX, GetY, GetY - GetRadius, Delete->Next,
                            Delete->Next->Prev->FocusX, Delete->Next->Prev->FocusY, Delete->Next->Next->FocusX, Delete->Next->Next->FocusY
                    )
            );
    }
    
    // Delete this parabola (free memory)
    delete Delete;
    
    // Return being successful
    return 1;
    
}

// Define functions of event class
GrimEvent::GrimEvent () {
    PtX = 0.0f;
    PtY = 0.0f;
    EventIsSite = 1;
    CircleY = 0.0f;
    CircleClosesParabola = 0;
}

GrimEvent::GrimEvent (float X, float Y, int SetSiteNo) {
    PtX = X;
    PtY = Y;
    EventIsSite = 1;
    SiteNo = SetSiteNo;
    CircleY = 0.0f;
    CircleClosesParabola = 0;
}

GrimEvent::GrimEvent (
        float X, float Y,
        float SetCircleY, GrimParabola *ClosesParabola,
        float SetOtherFocus1X, float SetOtherFocus1Y, float SetOtherFocus2X, float SetOtherFocus2Y
) {
    PtX = X;
    PtY = Y;
    EventIsSite = 0;
    SiteNo = -1;
    CircleY = SetCircleY;
    CircleClosesParabola = ClosesParabola;
    OtherFocus1X = SetOtherFocus1X;
    OtherFocus1Y = SetOtherFocus1Y;
    OtherFocus2X = SetOtherFocus2X;
    OtherFocus2Y = SetOtherFocus2Y;
    if (PtX < 1.0e-35f && PtY < 1.0e-35f) {
        PtX = 1.0f;
        PtY = 1.0f;
    }
}

int GrimEvent::operator> (const GrimEvent &comp) {
    if (PtY == comp.PtY) return PtX > comp.PtX;
    else return PtY > comp.PtY;
}

int GrimEvent::operator== (const GrimEvent &comp) {
    return CircleClosesParabola == comp.CircleClosesParabola;
}

Voronoi::Voronoi (int GenMode) {
    
    NextCornerNo = 0;
    
    int n = 0;
    
    float MinX = BoundXMin < BoundXMax ? BoundXMin : BoundXMax;
    float MaxX = BoundXMin >= BoundXMax ? BoundXMin : BoundXMax;
    float MinY = BoundYMin < BoundYMax ? BoundYMin : BoundYMax;
    float MaxY = BoundYMin >= BoundYMax ? BoundYMin : BoundYMax;
    float DX = MaxX - MinX;
    float DY = MaxY - MinY;
    
    // PERFECT
    //float TestX[10] = {16, 23, 27, 35, 50, 60, 65, 85, 85, 87};
    //float TestY[10] = {113, 25, 62, 85, 137, 35, 80, 65, 117, 13};
    
    // PERFECT
    //float TestX[15] = {12, 30, 14, 9, 1, 98, 58, 70, 0, 53, 60, 82, 19, 17, 34};
    //float TestY[15] = {2, 4, 34, 73, 76, 90, 96, 104, 113, 115, 122, 150, 162, 172, 180};
    
    // PERFECT
    //float TestX[10] = {30, 54, 14, 86, 46, 52, 45,  9, 58, 70};
    //float TestY[10] = { 4, 29, 34, 42, 48, 61, 71, 73, 96, 103};
    
    // PERFECT
    //float TestX[11] = {12, 30, 83, 67, 49, 37, 74, 54, 14, 48, 86};
    //float TestY[11] = {1, 3, 4, 11, 12, 18, 21, 28, 33, 40, 41};
    
    // PERFECT
    //float TestX[11] = {66, 22, 49, 51, 60, 86, 68, 91, 10, 0, 49};
    //float TestY[11] = {12, 30, 38, 39, 40, 50, 54, 56, 63, 71, 80};
    
    // PERFECT
    //float TestX[11] = {36, 45, 37,  0, 95, 65, 89, 82, 73, 86, 90};
    //float TestY[11] = {13, 14, 16, 41, 57, 61, 78, 88, 89, 92, 99};
    
    // PERFECT (WHEN DEBUGGING ONLY, OTHERWISE SHOWS ERRONEOUS RESULTS)
    //float TestX[11] = {83, 19, 12, 73, 72, 81, 15, 28, 82, 68, 15 };
    //float TestY[11] = { 5, 22, 45, 49, 58, 58, 61, 69, 70, 91, 105};
    
    // PERFECT
    //float TestX[11] = {42, 78, 19, 77, 17, 34,  1, 95, 45, 51, 99};
    //float TestY[11] = { 4,  4,  5, 10, 13, 18, 21, 23, 26, 32, 33};
    

	// Set boundaries
	const float HeightFactor = 0.1f;
	float GenYMin = 1.1f*HeightFactor * BoundYMax;

	// Parameters for setting up the first few sites
	double Theta = (double)M_PI / 180.0 * (30.0 + 30.0*static_cast<float>(rand()) / RandMax);
	const float Cos = (float)cos(Theta);
	const float Sin = (float)sin(Theta);
	const float InterSiteDistance = 2.0f*HeightFactor*BoundYMax*Sin + EntryWidth*Cos;
	const float DivX = Cos*InterSiteDistance;
	const float DivY = Sin*InterSiteDistance;

	// Allocate space for site data and place initial sites to yield suitably-sized entrance
	Sites[0] = 0.0f;
	Sites[1] = HeightFactor*BoundYMax;
	Sites[2] = -0.5f*EntryWidth - DivX;
	Sites[3] = HeightFactor*BoundYMax - DivY;
	Sites[4] = 0.5f*EntryWidth + DivX;
	Sites[5] = HeightFactor*BoundYMax - DivY;


    float Aspect, SitesPerCol, SitesPerRow;
    int iCol, iRow;
    
    if (GenMode == 1) {
        Aspect = (BoundXMax - BoundXMin) / (BoundYMax - BoundYMin);
        SitesPerCol = (float)sqrt(((double)NoOfSites / (double)Aspect));
        SitesPerRow = SitesPerCol * Aspect;
        iCol = (int)SitesPerCol;
        iRow = (int)SitesPerRow;
        if (iCol * iRow > NoOfSites) {
            iCol--;
            iRow--;
        }
        DX /= (float)iRow;
        DY /= (float)iCol;
    }
    else if (GenMode == 2) {
        DY = (BoundYMax - BoundYMin) / ((float)NoOfSites + 1);
    }
    
	n = 6;
    while (n < 2*NoOfSites) {
        
        switch (GenMode) {
            
            case 0:
                //Buffer[n++] = TestX[n/5];
                //Buffer[n++] = TestY[n/5];
                Sites[n++] = MinX + DX * static_cast<float>(rand()) / RandMax;
                Sites[n++] = MinY + DY * static_cast<float>(rand()) / RandMax;
                break;
            
            case 1:
                Sites[n++] = MinX + DX * ((float)((n/2)%iRow) + 0.1f + 0.8f*(static_cast<float>(rand()) / RandMax));
                Sites[n++] = MinY + DY * ((float)((n/2)/iCol) + 0.1f + 0.8f*(static_cast<float>(rand()) / RandMax));
                break;
            
            case 2:
                Sites[n++] = MinX + DX * static_cast<float>(rand()) / RandMax;
                Sites[n++] = MinY + DY * ((float)(n/2) + 1.0f);
                break;
            
        }
        
    }
    
}

float* Voronoi::GenerateBoundaryBuffer () {
    
    float* Buffer = new float[220];
    
    Buffer[0] = BoundXMin;
    Buffer[1] = BoundYMin;
    Buffer[2] = 0.0f;
    Buffer[3] = 0.0f;
    Buffer[4] = 1.0f;
    
    Buffer[5] = BoundXMax;
    Buffer[6] = BoundYMin;
    Buffer[7] = 0.0f;
    Buffer[8] = 0.0f;
    Buffer[9] = 1.0f;
    
    Buffer[10] = BoundXMin;
    Buffer[11] = BoundYMax;
    Buffer[12] = 0.0f;
    Buffer[13] = 0.0f;
    Buffer[14] = 1.0f;
    
    Buffer[15] = BoundXMax;
    Buffer[16] = BoundYMax;
    Buffer[17] = 0.0f;
    Buffer[18] = 0.0f;
    Buffer[19] = 1.0f;
    
    Buffer[20] = BoundXMin;
    Buffer[21] = BoundYMin;
    Buffer[22] = 0.0f;
    Buffer[23] = 0.0f;
    Buffer[24] = 1.0f;
    
    Buffer[25] = BoundXMin;
    Buffer[26] = BoundYMax;
    Buffer[27] = 0.0f;
    Buffer[28] = 0.0f;
    Buffer[29] = 1.0f;
    
    Buffer[30] = BoundXMax;
    Buffer[31] = BoundYMin;
    Buffer[32] = 0.0f;
    Buffer[33] = 0.0f;
    Buffer[34] = 1.0f;
    
    Buffer[35] = BoundXMax;
    Buffer[36] = BoundYMax;
    Buffer[37] = 0.0f;
    Buffer[38] = 0.0f;
    Buffer[39] = 1.0f;
    
	Buffer[40] = -300.0f;
	Buffer[41] = -300.0f;
	Buffer[42] = -300.0f;
	Buffer[43] = 0.0f;
	Buffer[44] = 0.25f;

	Buffer[45] = -300.0f;
	Buffer[46] = 300.0f;
	Buffer[47] = -300.0f;
	Buffer[48] = 0.25f;
	Buffer[49] = 0.25f;

	Buffer[50] = -300.0f;
	Buffer[51] = 300.0f;
	Buffer[52] = 300.0f;
	Buffer[53] = 0.25f;
	Buffer[54] = 0.5f;

	Buffer[55] = -300.0f;
	Buffer[56] = 300.0f;
	Buffer[57] = 300.0f;
	Buffer[58] = 0.25f;
	Buffer[59] = 0.5f;

	Buffer[60] = -300.0f;
	Buffer[61] = -300.0f;
	Buffer[62] = 300.0f;
	Buffer[63] = 0.0f;
	Buffer[64] = 0.5f;

	Buffer[65] = -300.0f;
	Buffer[66] = -300.0f;
	Buffer[67] = -300.0f;
	Buffer[68] = 0.0f;
	Buffer[69] = 0.25f;

	Buffer[70] = -300.0f;
	Buffer[71] = 300.0f;
	Buffer[72] = -300.0f;
	Buffer[73] = 0.25f;
	Buffer[74] = 0.25f;

	Buffer[75] = 300.0f;
	Buffer[76] = 300.0f;
	Buffer[77] = -300.0f;
	Buffer[78] = 0.5f;
	Buffer[79] = 0.25f;

	Buffer[80] = 300.0f;
	Buffer[81] = 300.0f;
	Buffer[82] = 300.0f;
	Buffer[83] = 0.5f;
	Buffer[84] = 0.5f;

	Buffer[85] = 300.0f;
	Buffer[86] = 300.0f;
	Buffer[87] = 300.0f;
	Buffer[88] = 0.5f;
	Buffer[89] = 0.5f;

	Buffer[90] = -300.0f;
	Buffer[91] = 300.0f;
	Buffer[92] = 300.0f;
	Buffer[93] = 0.25f;
	Buffer[94] = 0.5f;

	Buffer[95] = -300.0f;
	Buffer[96] = 300.0f;
	Buffer[97] = -300.0f;
	Buffer[98] = 0.25f;
	Buffer[99] = 0.25f;

	Buffer[100] = 300.0f;
	Buffer[101] = 300.0f;
	Buffer[102] = -300.0f;
	Buffer[103] = 0.5f;
	Buffer[104] = 0.25f;

	Buffer[105] = 300.0f;
	Buffer[106] = -300.0f;
	Buffer[107] = -300.0f;
	Buffer[108] = 0.75f;
	Buffer[109] = 0.25f;

	Buffer[110] = 300.0f;
	Buffer[111] = -300.0f;
	Buffer[112] = 300.0f;
	Buffer[113] = 0.75f;
	Buffer[114] = 0.5f;

	Buffer[115] = 300.0f;
	Buffer[116] = -300.0f;
	Buffer[117] = 300.0f;
	Buffer[118] = 0.75f;
	Buffer[119] = 0.5f;

	Buffer[120] = 300.0f;
	Buffer[121] = 300.0f;
	Buffer[122] = 300.0f;
	Buffer[123] = 0.5f;
	Buffer[124] = 0.5f;

	Buffer[125] = 300.0f;
	Buffer[126] = 300.0f;
	Buffer[127] = -300.0f;
	Buffer[128] = 0.5f;
	Buffer[129] = 0.25f;

	Buffer[130] = 300.0f;
	Buffer[131] = -300.0f;
	Buffer[132] = -300.0f;
	Buffer[133] = 0.75f;
	Buffer[134] = 0.25f;

	Buffer[135] = -300.0f;
	Buffer[136] = -300.0f;
	Buffer[137] = -300.0f;
	Buffer[138] = 1.0f;
	Buffer[139] = 0.25f;

	Buffer[140] = -300.0f;
	Buffer[141] = -300.0f;
	Buffer[142] = 300.0f;
	Buffer[143] = 1.0f;
	Buffer[144] = 0.5f;

	Buffer[145] = -300.0f;
	Buffer[146] = -300.0f;
	Buffer[147] = 300.0f;
	Buffer[148] = 1.0f;
	Buffer[149] = 0.5f;

	Buffer[150] = 300.0f;
	Buffer[151] = -300.0f;
	Buffer[152] = 300.0f;
	Buffer[153] = 0.75f;
	Buffer[154] = 0.5f;

	Buffer[155] = 300.0f;
	Buffer[156] = -300.0f;
	Buffer[157] = -300.0f;
	Buffer[158] = 0.75f;
	Buffer[159] = 0.25f;

	Buffer[160] = -300.0f;
	Buffer[161] = -300.0f;
	Buffer[162] = -300.0f;
	Buffer[163] = 0.25f;
	Buffer[164] = 0.0f;

	Buffer[165] = 300.0f;
	Buffer[166] = -300.0f;
	Buffer[167] = -300.0f;
	Buffer[168] = 0.5f;
	Buffer[169] = 0.0f;

	Buffer[170] = 300.0f;
	Buffer[171] = 300.0f;
	Buffer[172] = -300.0f;
	Buffer[173] = 0.5f;
	Buffer[174] = 0.25f;

	Buffer[175] = 300.0f;
	Buffer[176] = 300.0f;
	Buffer[177] = -300.0f;
	Buffer[178] = 0.5f;
	Buffer[179] = 0.25f;

	Buffer[180] = -300.0f;
	Buffer[181] = 300.0f;
	Buffer[182] = -300.0f;
	Buffer[183] = 0.25f;
	Buffer[184] = 0.25f;

	Buffer[185] = -300.0f;
	Buffer[186] = -300.0f;
	Buffer[187] = -300.0f;
	Buffer[188] = 0.25f;
	Buffer[189] = 0.0f;

	Buffer[190] = 300.0f;
	Buffer[191] = -300.0f;
	Buffer[192] = 300.0f;
	Buffer[193] = 0.25f;
	Buffer[194] = 0.0f;

	Buffer[195] = -300.0f;
	Buffer[196] = -300.0f;
	Buffer[197] = 300.0f;
	Buffer[198] = 0.0f;
	Buffer[199] = 0.0f;

	Buffer[200] = -300.0f;
	Buffer[201] = 300.0f;
	Buffer[202] = 300.0f;
	Buffer[203] = 0.0f;
	Buffer[204] = 0.25f;

	Buffer[205] = -300.0f;
	Buffer[206] = 300.0f;
	Buffer[207] = 300.0f;
	Buffer[208] = 0.0f;
	Buffer[209] = 0.25f;

	Buffer[210] = 300.0f;
	Buffer[211] = 300.0f;
	Buffer[212] = 300.0f;
	Buffer[213] = 0.25f;
	Buffer[214] = 0.25f;

	Buffer[215] = 300.0f;
	Buffer[216] = -300.0f;
	Buffer[217] = 300.0f;
	Buffer[218] = 0.25f;
	Buffer[219] = 0.0f;

    return Buffer;
    
}

float* Voronoi::GenerateSiteBuffer (int GenMode) {
    
    float *Buffer = new float[5*NoOfSites];
    int n;
    
    for (n = 0; n < NoOfSites; n++) {
        
        Buffer[n*5  ] = Sites[n*2];
        Buffer[n*5+1] = Sites[n*2+1];
        Buffer[n*5+2] = 1.0f;
        Buffer[n*5+3] = 1.0f;
        Buffer[n*5+4] = 1.0f;
        
    }
    
    return Buffer;
    
}

float* Voronoi::GenerateDiagramReturnLines (int* GetNoOfLines, int PathMode) {
    
    // Initialise variables
    int n = 0;
    GrimQueue<GrimEvent> EventQueue (5*NoOfSites);
    GrimParabola *BeachLine = 0;
    
    // Queue to hold output points
    GrimQueue<GrimLineData> OutputLines (NoOfSites*10);
    GrimQueue<GrimEvent> OutputPoints (NoOfSites);
    
    // Generate sites, and queue them in ascending order of y coordinates
    for (n = 0; n < NoOfSites; n++)
        EventQueue.Add (GrimEvent (Sites[n*2], Sites[n*2+1], n));
    EventQueue.SortAscending ();
    
    // Start sweeping
    float SweepLine;
    GrimEvent GetEvent;
    NextCornerNo = 4;
    
    GrimParabola* Debuggy[30];
    GrimParabola* Debuggier;
    
    while (EventQueue.GetQueuedElements () > 0) {
        
        // Pop next event
        GetEvent = EventQueue.Remove ();
        SweepLine = GetEvent.PtY;
        
        for (n = 0; n < 30; n++) Debuggy[n] = 0;
        n = 0;
        Debuggier = BeachLine;
        while (Debuggier != 0 && n < 30) {Debuggy[n] = Debuggier; Debuggier = Debuggier->Next; n++;}
        
        // Add point to display queue
        if (GetEvent.EventIsSite)
            OutputPoints.Add (GetEvent);
        
        // Add or remove parabola (for site or circle event)
        if (GetEvent.EventIsSite) {
            // Add a new parabola segment to the beach line
            InsertNew (&BeachLine, GetEvent.PtX, GetEvent.PtY, GetEvent.SiteNo, SweepLine, &EventQueue);
        }
        else {
            // Remove this parabola and add a voronoi segment
            if (DeleteParabola (&BeachLine, GetEvent.CircleClosesParabola, GetEvent.OtherFocus1X, GetEvent.OtherFocus1Y, GetEvent.OtherFocus2X, GetEvent.OtherFocus2Y, SweepLine, GetEvent.PtX, GetEvent.CircleY, &EventQueue, &OutputLines))
                // Remove any other circle eventsa from the queue which may delete the same parabola arc
                while (EventQueue.SearchDelete (GetEvent));
        }
        
    }
    
    // Push the sweep line a little further so all tracers have moved
    SweepLine += 5.0f;
    
    // Go through beach line and finish tracers.
    float StX, StY, FinX, FinY;
    float FX1, FY1, FX2, FY2;
    float tX, tY, SetX, SetY;
    while (BeachLine->Next != 0) {
        
        // If one endpoint is set, finish it.
        if (BeachLine->TraceEdgeRight->ParabolaPointsAttached == 2) {
            
            StX = BeachLine->TraceEdgeRight->FirstX;
            StY = BeachLine->TraceEdgeRight->FirstY;
            
            // Make sure the start point is within the boundaries
            if ((StX > BoundXMin) && (StX < BoundXMax) && (StY > BoundYMin) && (StY < BoundYMax)) {
                
                // Get another point along the traced edge. Will adjust its length.
                BeachLine->GetNextIntersection (SweepLine, &FinX, &FinY);
                
                // Get parameters to adjust its length to one of the four boundaries.
                if (StX < FinX) {
                    tX = (BoundXMax - StX)/(FinX - StX);
                    SetX = BoundXMax;
                }
                else if (StX > FinX) {
                    tX = (BoundXMin- StX)/(FinX - StX);
                    SetX = BoundXMin;
                }
                else tX = -1.0f;
                if (StY < FinY) {
                    tY = (BoundYMax - StY)/(FinY - StY);
                    SetY = BoundYMax;
                }
                else if (StY > FinY) {
                    tY = (BoundYMin- StY)/(FinY - StY);
                    SetY = BoundYMin;
                }
                else tY = -1.0f;
                
                // Make sure vertical and horizontal lines are accounted for.
                if (tX < 0.0f) tX = tY + 1.0f;
                if (tY < 0.0f) tY = tX + 1.0f;
                
                // Adjust length to meet the boundary.
                if (tX < tY) {
                    FinX = SetX;
                    FinY = StY + tX*(FinY - StY);
                }
                else {
                    FinX = StX + tY*(FinX - StX);
                    FinY = SetY;
                }
                
                // Output the result
                AddOutputLine (&OutputLines, StX, StY, FinX, FinY, BeachLine->SiteNo, BeachLine->Next->SiteNo, BeachLine->TraceEdgeRight->StartCornerNo, NextCornerNo++);
                
            }
        }
        
        // No endpoint yet set; likely a horizontal-ish line
        else if (BeachLine->TraceEdgeRight->ParabolaPointsAttached == 4) {
            
            BeachLine->GetFocus (&FX1, &FY1);
            BeachLine->Next->GetFocus (&FX2, &FY2);
            
            // Vertical lines will generate an error here
            if (FY1 != FY2) {
                
                StX = BoundXMin;
                StY = 0.5f*(FY1 + FY2) + (BoundXMin - 0.5f*(FX1 + FX2))*(-FX1 + FX2)/(FY1 - FY2);
                
                FinX = BoundXMax;
                FinY = 0.5f*(FY1 + FY2) + (BoundXMax - 0.5f*(FX1 + FX2))*(-FX1 + FX2)/(FY1 - FY2);
                
                if (StY < BoundYMin) {
                    StX = StX + (FinX - StX) * (BoundYMin - StY)/(FinY - StY);
                    StY = BoundYMin;
                }
                else if (StY > BoundYMax) {
                    StX = StX + (FinX - StX) * (BoundYMax - StY)/(FinY - StY);
                    StY = BoundYMax;
                }
                if (FinY < BoundYMin) {
                    FinX = FinX + (StX - FinX) * (BoundYMin - FinY)/(StY - FinY);
                    FinY = BoundYMin;
                }
                else if (FinY > BoundYMax) {
                    FinX = FinX + (StX - FinX) * (BoundYMax - FinY)/(StY - FinY);
                    FinY = BoundYMax;
                }
                
                // Output the result
                AddOutputLine (&OutputLines, StX, StY, FinX, FinY, BeachLine->SiteNo, BeachLine->Next->SiteNo, BeachLine->TraceEdgeRight->StartCornerNo, NextCornerNo++);
                
            }
        }
        
        // Move along and delete this
        BeachLine = BeachLine->Next;
        delete BeachLine->Prev;
        
    }
    delete BeachLine;
    
    LinesForThisSite = new int[NoOfSites];
    AdjacentSite = new int[NoOfSites][MaxAdjacentCells];
    AdjacentDistance = new float[NoOfSites][MaxAdjacentCells];
    BorderWidth = new float[NoOfSites][MaxAdjacentCells];
    StartCorner = new int[NoOfSites][MaxAdjacentCells];
    EndCorner = new int[NoOfSites][MaxAdjacentCells];
    StartX = new float[NoOfSites][MaxAdjacentCells];
    StartY = new float[NoOfSites][MaxAdjacentCells];
    EndX = new float[NoOfSites][MaxAdjacentCells];
    EndY = new float[NoOfSites][MaxAdjacentCells];
    
    float* GetLineData = DecodeLineData (&OutputLines, GetNoOfLines);
    BuildSubset (PathMode);
    
    return GetLineData;
    
}

float* Voronoi::GenerateDiagramVertices (int* ReturnNoOfVertices) {
    
    // Create random colours
    int n;
    float Rc[NoOfSites];
    float Gc[NoOfSites];
    float Bc[NoOfSites];
    for (n = 0; n < NoOfSites; n++) {
        Rc[n] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        Gc[n] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        Bc[n] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
    
    // Get required number of polygons within the entire diagram
    int PolygonsRequired = 0;
    for (n = 0; n < NoOfSites; n++)
        PolygonsRequired += LinesForThisSite[n];
    *ReturnNoOfVertices = PolygonsRequired*3;
    
    // Search processed line data to build polygon data and internal cell information
    float* Buffer = new float[15*PolygonsRequired];
    int DoSiteNo;
    int LinesFound;
    int BufferIndex = 0;
    int LinesReadForThisSite[NoOfSites];
    for (n = 0; n < NoOfSites; n++)
       LinesReadForThisSite[n] = 0;
    
    // Build display buffer for entire diagram, and calculate adjacent distances
    for (DoSiteNo = 0; DoSiteNo < NoOfSites; DoSiteNo++) {
        
        // Search buffers for lines around this site
        LinesFound = 0;
        n = 0;
        
        while (LinesFound < LinesForThisSite[DoSiteNo] && n < MaxAdjacentCells) {
            
            // Add vertices to total bufer
            LinesFound++;
            Buffer[BufferIndex   ] = Sites[DoSiteNo*2];
            Buffer[BufferIndex+1 ] = Sites[DoSiteNo*2+1];
            Buffer[BufferIndex+2 ] = Rc[DoSiteNo];
            Buffer[BufferIndex+3 ] = Gc[DoSiteNo];
            Buffer[BufferIndex+4 ] = Bc[DoSiteNo];
            Buffer[BufferIndex+5 ] = StartX[DoSiteNo][n];
            Buffer[BufferIndex+6 ] = StartY[DoSiteNo][n];
            Buffer[BufferIndex+7 ] = Rc[DoSiteNo];
            Buffer[BufferIndex+8 ] = Gc[DoSiteNo];
            Buffer[BufferIndex+9 ] = Bc[DoSiteNo];
            Buffer[BufferIndex+10] = EndX[DoSiteNo][n];
            Buffer[BufferIndex+11] = EndY[DoSiteNo][n];
            Buffer[BufferIndex+12] = Rc[DoSiteNo];
            Buffer[BufferIndex+13] = Gc[DoSiteNo];
            Buffer[BufferIndex+14] = Bc[DoSiteNo];
            BufferIndex += 15;
            
            n++;
        }
    }
    
    delete[] LinesForThisSite;
    delete[] AdjacentSite;
    delete[] AdjacentDistance;
    delete[] BorderWidth;
    delete[] StartCorner;
    delete[] EndCorner;
    delete[] StartX;
    delete[] StartY;
    delete[] EndX;
    delete[] EndY;
    
    return Buffer;

}

float* Voronoi::GenerateSubVertices (int* ReturnNoOfVertices) {
    
    // Find polygons required to display this subset
    int PolygonsRequired = 0;
    int n;
    for (n = 0; n < NoOfSubsetSites; n++) {
        if (CellType[n] == 1 || CellType[n] == 2) PolygonsRequired += SubLinesForThisSite[n];
    }
    *ReturnNoOfVertices = PolygonsRequired*3;
    
    // Create random colours
    float *Rc = new float[NoOfSubsetSites];
    float *Gc = new float[NoOfSubsetSites];
    float *Bc = new float[NoOfSubsetSites];
    for (n = 0; n < NoOfSubsetSites; n++) {
        Rc[n] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        Gc[n] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        Bc[n] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
    
    // Build display buffer for subset of cells
    float* Buffer = new float[15*PolygonsRequired];
    int BufferIndex = 0;
    int LinesFound;
    int DoSiteNo;
    for (DoSiteNo = 0; DoSiteNo < NoOfSubsetSites; DoSiteNo++) {
        
        if (CellType[DoSiteNo] == 0) continue;
        if (CellType[DoSiteNo] > 2) continue;
        
        // Search buffers for lines around this site
        LinesFound = 0;
        n = 0;
        
        while (LinesFound < SubLinesForThisSite[DoSiteNo] && n < MaxAdjacentCells) {
            
            // Add vertices to total bufer
            LinesFound++;
            Buffer[BufferIndex   ] = SubSiteX[DoSiteNo];
            Buffer[BufferIndex+1 ] = SubSiteY[DoSiteNo];
            Buffer[BufferIndex+2 ] = Rc[DoSiteNo];
            Buffer[BufferIndex+3 ] = Gc[DoSiteNo];
            Buffer[BufferIndex+4 ] = Bc[DoSiteNo];
            Buffer[BufferIndex+5 ] = SubStartX[DoSiteNo][n];
            Buffer[BufferIndex+6 ] = SubStartY[DoSiteNo][n];
            Buffer[BufferIndex+7 ] = Rc[DoSiteNo];
            Buffer[BufferIndex+8 ] = Gc[DoSiteNo];
            Buffer[BufferIndex+9 ] = Bc[DoSiteNo];
            Buffer[BufferIndex+10] = SubEndX[DoSiteNo][n];
            Buffer[BufferIndex+11] = SubEndY[DoSiteNo][n];
            Buffer[BufferIndex+12] = Rc[DoSiteNo];
            Buffer[BufferIndex+13] = Gc[DoSiteNo];
            Buffer[BufferIndex+14] = Bc[DoSiteNo];
            BufferIndex += 15;
            
            n++;
        }
    }
    
	delete[] Rc;
	delete[] Gc;
	delete[] Bc;

    return  Buffer;
    
}

float* Voronoi::DecodeLineData (GrimQueue<GrimLineData> *Output, int* LinesInBuffer) {
    
    GrimLineData GetData;
    int DataElements = Output->GetQueuedElements ();
    int n;
    
    // Reset line count per site
    for (n = 0; n < NoOfSites; n++) LinesForThisSite[n] = 0;
    
    // Flags and coordinates for building edge lines
    int HasEdgeLeft[NoOfSites];
    int HasEdgeRight[NoOfSites];
    int HasEdgeTop[NoOfSites];
    int HasEdgeBottom[NoOfSites];
    float EdgeLeftY[NoOfSites];
    float EdgeRightY[NoOfSites];
    float EdgeTopX[NoOfSites];
    float EdgeBottomX[NoOfSites];
    int EdgeLeftCorner[NoOfSites];
    int EdgeRightCorner[NoOfSites];
    int EdgeTopCorner[NoOfSites];
    int EdgeBottomCorner[NoOfSites];
    for (n = 0; n < NoOfSites; n++) {
        HasEdgeLeft[n] = 0;
        HasEdgeRight[n] = 0;
        HasEdgeTop[n] = 0;
        HasEdgeBottom[n] = 0;
    }
    
    // For filling line buffer
    int NoOfLines = Output->GetQueuedElements ();
    *LinesInBuffer = NoOfLines;
    float *LineData = new float[10*NoOfLines];
    int LineBuffIndex = 0;
    int SiteNo1, SiteNo2, CellIndex;
    
    // Unpack GrimQueue object and load new buffers. Calculate number of lines required for each cell.
    while (Output->GetQueuedElements() > 0) {
        
        // Pop next element
        GetData = Output->Remove();
        
        // Fill line data buffer
        LineData[LineBuffIndex  ] = GetData.Pt1X;
        LineData[LineBuffIndex+1] = GetData.Pt1Y;
        LineData[LineBuffIndex+2] = 0.0f;
        LineData[LineBuffIndex+3] = 1.0f;
        LineData[LineBuffIndex+4] = 0.0f;
        LineData[LineBuffIndex+5] = GetData.Pt2X;
        LineData[LineBuffIndex+6] = GetData.Pt2Y;
        LineData[LineBuffIndex+7] = 0.0f;
        LineData[LineBuffIndex+8] = 1.0f;
        LineData[LineBuffIndex+9] = 0.0f;
        if (GetData.Corner1 == -17) {
            LineData[LineBuffIndex+3] = 0.4f;
            LineData[LineBuffIndex+8] = 0.4f;
        }
        LineBuffIndex += 10;
        
        // Add data for one site
        SiteNo1 = GetData.Site1;
        CellIndex = LinesForThisSite[SiteNo1];
        AdjacentSite[SiteNo1][CellIndex] = GetData.Site2;
        StartCorner[SiteNo1][CellIndex] = GetData.Corner1;
        EndCorner[SiteNo1][CellIndex] = GetData.Corner2;
        StartX[SiteNo1][CellIndex] = GetData.Pt1X;
        StartY[SiteNo1][CellIndex] = GetData.Pt1Y;
        EndX[SiteNo1][CellIndex] = GetData.Pt2X;
        EndY[SiteNo1][CellIndex] = GetData.Pt2Y;
        LinesForThisSite[SiteNo1]++;
        
        // Add data for the other site
        SiteNo2 = GetData.Site2;
        CellIndex = LinesForThisSite[SiteNo2];
        AdjacentSite[SiteNo2][CellIndex] = GetData.Site1;
        StartCorner[SiteNo2][CellIndex] = GetData.Corner1;
        EndCorner[SiteNo2][CellIndex] = GetData.Corner2;
        StartX[SiteNo2][CellIndex] = GetData.Pt1X;
        StartY[SiteNo2][CellIndex] = GetData.Pt1Y;
        EndX[SiteNo2][CellIndex] = GetData.Pt2X;
        EndY[SiteNo2][CellIndex] = GetData.Pt2Y;
        LinesForThisSite[SiteNo2]++;
        
        // Test for Pt1 on left boundary
        if (GetData.Pt1X == BoundXMin) {
            // Deal with site 1
            if (HasEdgeLeft[SiteNo1] == 0) {
                HasEdgeLeft[SiteNo1] = 1;
                EdgeLeftY[SiteNo1] = GetData.Pt1Y;
                EdgeLeftCorner[SiteNo1] = GetData.Corner1;
            }
            else if (HasEdgeLeft[SiteNo1] == 1) {
                CellIndex = LinesForThisSite[SiteNo1];
                HasEdgeLeft[SiteNo1] = 2;
                AdjacentSite[SiteNo1][CellIndex] = -1;
                StartCorner[SiteNo1][CellIndex] = EdgeLeftCorner[SiteNo1];
                EndCorner[SiteNo1][CellIndex] = GetData.Corner1;
                StartX[SiteNo1][CellIndex] = BoundXMin;
                StartY[SiteNo1][CellIndex] = EdgeLeftY[GetData.Site1];
                EndX[SiteNo1][CellIndex] = BoundXMin;
                EndY[SiteNo1][CellIndex] = GetData.Pt1Y;
                LinesForThisSite[SiteNo1]++;
            }
            // Deal with site 2
            if (HasEdgeLeft[SiteNo2] == 0) {
                HasEdgeLeft[SiteNo2] = 1;
                EdgeLeftY[SiteNo2] = GetData.Pt1Y;
                EdgeLeftCorner[SiteNo2] = GetData.Corner1;
            }
            else if (HasEdgeLeft[SiteNo2] == 1) {
                CellIndex = LinesForThisSite[SiteNo2];
                HasEdgeLeft[SiteNo2] = 2;
                AdjacentSite[SiteNo2][CellIndex] = -1;
                StartCorner[SiteNo2][CellIndex] = EdgeLeftCorner[SiteNo2];
                EndCorner[SiteNo2][CellIndex] = GetData.Corner1;
                StartX[SiteNo2][CellIndex] = BoundXMin;
                StartY[SiteNo2][CellIndex] = EdgeLeftY[GetData.Site2];
                EndX[SiteNo2][CellIndex] = BoundXMin;
                EndY[SiteNo2][CellIndex] = GetData.Pt1Y;
                LinesForThisSite[SiteNo2]++;
            }
        }
        // Test for Pt2 on left boundary
        if (GetData.Pt2X == BoundXMin) {
            // Deal with site 1
            if (HasEdgeLeft[SiteNo1] == 0) {
                HasEdgeLeft[SiteNo1] = 1;
                EdgeLeftY[SiteNo1] = GetData.Pt2Y;
                EdgeLeftCorner[SiteNo1] = GetData.Corner2;
            }
            else if (HasEdgeLeft[SiteNo1] == 1) {
                CellIndex = LinesForThisSite[SiteNo1];
                HasEdgeLeft[SiteNo1] = 2;
                AdjacentSite[SiteNo1][CellIndex] = -1;
                StartCorner[SiteNo1][CellIndex] = EdgeLeftCorner[SiteNo1];
                EndCorner[SiteNo1][CellIndex] = GetData.Corner2;
                StartX[SiteNo1][CellIndex] = BoundXMin;
                StartY[SiteNo1][CellIndex] = EdgeLeftY[GetData.Site1];
                EndX[SiteNo1][CellIndex] = BoundXMin;
                EndY[SiteNo1][CellIndex] = GetData.Pt2Y;
                LinesForThisSite[SiteNo1]++;
            }
            // Deal with site 2
            if (HasEdgeLeft[SiteNo2] == 0) {
                HasEdgeLeft[SiteNo2] = 1;
                EdgeLeftY[SiteNo2] = GetData.Pt2Y;
                EdgeLeftCorner[SiteNo2] = GetData.Corner2;
            }
            else if (HasEdgeLeft[SiteNo2] == 1) {
                CellIndex = LinesForThisSite[SiteNo2];
                HasEdgeLeft[SiteNo2] = 2;
                AdjacentSite[SiteNo2][CellIndex] = -1;
                StartCorner[SiteNo2][CellIndex] = EdgeLeftCorner[SiteNo2];
                EndCorner[SiteNo2][CellIndex] = GetData.Corner2;
                StartX[SiteNo2][CellIndex] = BoundXMin;
                StartY[SiteNo2][CellIndex] = EdgeLeftY[GetData.Site2];
                EndX[SiteNo2][CellIndex] = BoundXMin;
                EndY[SiteNo2][CellIndex] = GetData.Pt2Y;
                LinesForThisSite[SiteNo2]++;
            }
        }
        
        // Test for Pt1 on right boundary
        if (GetData.Pt1X == BoundXMax) {
            // Deal with site 1
            if (HasEdgeRight[SiteNo1] == 0) {
                HasEdgeRight[SiteNo1] = 1;
                EdgeRightY[SiteNo1] = GetData.Pt1Y;
                EdgeRightCorner[SiteNo1] = GetData.Corner1;
            }
            else if (HasEdgeRight[SiteNo1] == 1) {
                CellIndex = LinesForThisSite[SiteNo1];
                HasEdgeLeft[SiteNo1] = 2;
                AdjacentSite[SiteNo1][CellIndex] = -1;
                StartCorner[SiteNo1][CellIndex] = EdgeRightCorner[SiteNo1];
                EndCorner[SiteNo1][CellIndex] = GetData.Corner1;
                StartX[SiteNo1][CellIndex] = BoundXMax;
                StartY[SiteNo1][CellIndex] = EdgeRightY[GetData.Site1];
                EndX[SiteNo1][CellIndex] = BoundXMax;
                EndY[SiteNo1][CellIndex] = GetData.Pt1Y;
                LinesForThisSite[SiteNo1]++;
            }
            // Deal with site 2
            if (HasEdgeRight[SiteNo2] == 0) {
                HasEdgeRight[SiteNo2] = 1;
                EdgeRightY[SiteNo2] = GetData.Pt1Y;
                EdgeRightCorner[SiteNo2] = GetData.Corner1;
            }
            else if (HasEdgeRight[SiteNo2] == 1) {
                CellIndex = LinesForThisSite[SiteNo2];
                HasEdgeLeft[SiteNo2] = 2;
                AdjacentSite[SiteNo2][CellIndex] = -1;
                StartCorner[SiteNo2][CellIndex] = EdgeRightCorner[SiteNo2];
                EndCorner[SiteNo2][CellIndex] = GetData.Corner1;
                StartX[SiteNo2][CellIndex] = BoundXMax;
                StartY[SiteNo2][CellIndex] = EdgeRightY[GetData.Site2];
                EndX[SiteNo2][CellIndex] = BoundXMax;
                EndY[SiteNo2][CellIndex] = GetData.Pt1Y;
                LinesForThisSite[SiteNo2]++;
            }
        }
        // Test for Pt2 on right boundary
        if (GetData.Pt2X == BoundXMax) {
            // Deal with site 1
            if (HasEdgeRight[SiteNo1] == 0) {
                HasEdgeRight[SiteNo1] = 1;
                EdgeRightY[SiteNo1] = GetData.Pt2Y;
                EdgeRightCorner[SiteNo1] = GetData.Corner2;
            }
            else if (HasEdgeRight[SiteNo1] == 1) {
                CellIndex = LinesForThisSite[SiteNo1];
                HasEdgeLeft[SiteNo1] = 2;
                AdjacentSite[SiteNo1][CellIndex] = -1;
                StartCorner[SiteNo1][CellIndex] = EdgeRightCorner[SiteNo1];
                EndCorner[SiteNo1][CellIndex] = GetData.Corner2;
                StartX[SiteNo1][CellIndex] = BoundXMax;
                StartY[SiteNo1][CellIndex] = EdgeRightY[GetData.Site1];
                EndX[SiteNo1][CellIndex] = BoundXMax;
                EndY[SiteNo1][CellIndex] = GetData.Pt2Y;
                LinesForThisSite[SiteNo1]++;
            }
            // Deal with site 2
            if (HasEdgeRight[SiteNo2] == 0) {
                HasEdgeRight[SiteNo2] = 1;
                EdgeRightY[SiteNo2] = GetData.Pt2Y;
                EdgeRightCorner[SiteNo2] = GetData.Corner2;
            }
            else if (HasEdgeRight[SiteNo2] == 1) {
                CellIndex = LinesForThisSite[SiteNo2];
                HasEdgeLeft[SiteNo2] = 2;
                AdjacentSite[SiteNo2][CellIndex] = -1;
                StartCorner[SiteNo2][CellIndex] = EdgeRightCorner[SiteNo2];
                EndCorner[SiteNo2][CellIndex] = GetData.Corner2;
                StartX[SiteNo2][CellIndex] = BoundXMax;
                StartY[SiteNo2][CellIndex] = EdgeRightY[GetData.Site2];
                EndX[SiteNo2][CellIndex] = BoundXMax;
                EndY[SiteNo2][CellIndex] = GetData.Pt2Y;
                LinesForThisSite[SiteNo2]++;
            }
        }
        
        // Test for Pt1 on top boundary
        if (GetData.Pt1Y == BoundYMax) {
            // Deal with site 1
            if (HasEdgeTop[SiteNo1] == 0) {
                HasEdgeTop[SiteNo1] = 1;
                EdgeTopX[SiteNo1] = GetData.Pt1X;
                EdgeTopCorner[SiteNo1] = GetData.Corner1;
            }
            else if (HasEdgeTop[SiteNo1] == 1) {
                CellIndex = LinesForThisSite[SiteNo1];
                HasEdgeLeft[SiteNo1] = 2;
                AdjacentSite[SiteNo1][CellIndex] = -1;
                StartCorner[SiteNo1][CellIndex] = EdgeTopCorner[SiteNo1];
                EndCorner[SiteNo1][CellIndex] = GetData.Corner1;
                StartX[SiteNo1][CellIndex] = EdgeTopX[GetData.Site1];
                StartY[SiteNo1][CellIndex] = BoundYMax;
                EndX[SiteNo1][CellIndex] = GetData.Pt1X;
                EndY[SiteNo1][CellIndex] = BoundYMax;
                LinesForThisSite[SiteNo1]++;
            }
            // Deal with site 2
            if (HasEdgeTop[SiteNo2] == 0) {
                HasEdgeTop[SiteNo2] = 1;
                EdgeTopX[SiteNo2] = GetData.Pt1X;
                EdgeTopCorner[SiteNo2] = GetData.Corner1;
            }
            else if (HasEdgeTop[SiteNo2] == 1) {
                CellIndex = LinesForThisSite[SiteNo2];
                HasEdgeLeft[SiteNo2] = 2;
                AdjacentSite[SiteNo2][CellIndex] = -1;
                StartCorner[SiteNo2][CellIndex] = EdgeTopCorner[SiteNo2];
                EndCorner[SiteNo2][CellIndex] = GetData.Corner1;
                StartX[SiteNo2][CellIndex] = EdgeTopX[GetData.Site2];
                StartY[SiteNo2][CellIndex] = BoundYMax;
                EndX[SiteNo2][CellIndex] = GetData.Pt1X;
                EndY[SiteNo2][CellIndex] = BoundYMax;
                LinesForThisSite[SiteNo2]++;
            }
        }
        // Test for Pt2 on top boundary
        if (GetData.Pt2Y == BoundYMax) {
            // Deal with site 1
            if (HasEdgeTop[SiteNo1] == 0) {
                HasEdgeTop[SiteNo1] = 1;
                EdgeTopX[SiteNo1] = GetData.Pt2X;
                EdgeTopCorner[SiteNo1] = GetData.Corner2;
            }
            else if (HasEdgeTop[SiteNo1] == 1) {
                CellIndex = LinesForThisSite[SiteNo1];
                HasEdgeLeft[SiteNo1] = 2;
                AdjacentSite[SiteNo1][CellIndex] = -1;
                StartCorner[SiteNo1][CellIndex] = EdgeTopCorner[SiteNo1];
                EndCorner[SiteNo1][CellIndex] = GetData.Corner2;
                StartX[SiteNo1][CellIndex] = EdgeTopX[GetData.Site1];
                StartY[SiteNo1][CellIndex] = BoundYMax;
                EndX[SiteNo1][CellIndex] = GetData.Pt2X;
                EndY[SiteNo1][CellIndex] = BoundYMax;
                LinesForThisSite[SiteNo1]++;
            }
            // Deal with site 2
            if (HasEdgeTop[SiteNo2] == 0) {
                HasEdgeTop[SiteNo2] = 1;
                EdgeTopX[SiteNo2] = GetData.Pt2X;
                EdgeTopCorner[SiteNo2] = GetData.Corner2;
            }
            else if (HasEdgeTop[SiteNo2] == 1) {
                CellIndex = LinesForThisSite[SiteNo2];
                HasEdgeLeft[SiteNo2] = 2;
                AdjacentSite[SiteNo2][CellIndex] = -1;
                StartCorner[SiteNo2][CellIndex] = EdgeTopCorner[SiteNo2];
                EndCorner[SiteNo2][CellIndex] = GetData.Corner2;
                StartX[SiteNo2][CellIndex] = EdgeTopX[GetData.Site2];
                StartY[SiteNo2][CellIndex] = BoundYMax;
                EndX[SiteNo2][CellIndex] = GetData.Pt2X;
                EndY[SiteNo2][CellIndex] = BoundYMax;
                LinesForThisSite[SiteNo2]++;
            }
        }
        
        // Test for Pt1 on bottom boundary
        if (GetData.Pt1Y == BoundYMin) {
            // Deal with site 1
            if (HasEdgeBottom[SiteNo1] == 0) {
                HasEdgeBottom[SiteNo1] = 1;
                EdgeBottomX[SiteNo1] = GetData.Pt1X;
                EdgeBottomCorner[SiteNo1] = GetData.Corner1;
            }
            else if (HasEdgeBottom[SiteNo1] == 1) {
                CellIndex = LinesForThisSite[SiteNo1];
                HasEdgeLeft[SiteNo1] = 2;
                AdjacentSite[SiteNo1][CellIndex] = -1;
                StartCorner[SiteNo1][CellIndex] = EdgeBottomCorner[SiteNo1];
                EndCorner[SiteNo1][CellIndex] = GetData.Corner1;
                StartX[SiteNo1][CellIndex] = EdgeBottomX[GetData.Site1];
                StartY[SiteNo1][CellIndex] = BoundYMin;
                EndX[SiteNo1][CellIndex] = GetData.Pt1X;
                EndY[SiteNo1][CellIndex] = BoundYMin;
                LinesForThisSite[SiteNo1]++;
            }
            // Deal with site 2
            if (HasEdgeBottom[SiteNo2] == 0) {
                HasEdgeBottom[SiteNo2] = 1;
                EdgeBottomX[SiteNo2] = GetData.Pt1X;
                EdgeBottomCorner[SiteNo2] = GetData.Corner1;
            }
            else if (HasEdgeBottom[SiteNo2] == 1) {
                CellIndex = LinesForThisSite[SiteNo2];
                HasEdgeLeft[SiteNo2] = 2;
                AdjacentSite[SiteNo2][CellIndex] = -1;
                StartCorner[SiteNo2][CellIndex] = EdgeBottomCorner[SiteNo2];
                EndCorner[SiteNo2][CellIndex] = GetData.Corner1;
                StartX[SiteNo2][CellIndex] = EdgeBottomX[GetData.Site2];
                StartY[SiteNo2][CellIndex] = BoundYMin;
                EndX[SiteNo2][CellIndex] = GetData.Pt1X;
                EndY[SiteNo2][CellIndex] = BoundYMin;
                LinesForThisSite[SiteNo2]++;
            }
        }
        // Test for Pt2 on bottom boundary
        if (GetData.Pt2Y == BoundYMin) {
            // Deal with site 1
            if (HasEdgeBottom[SiteNo1] == 0) {
                HasEdgeBottom[SiteNo1] = 1;
                EdgeBottomX[SiteNo1] = GetData.Pt2X;
                EdgeBottomCorner[SiteNo1] = GetData.Corner2;
            }
            else if (HasEdgeBottom[SiteNo1] == 1) {
                CellIndex = LinesForThisSite[SiteNo1];
                HasEdgeLeft[SiteNo1] = 2;
                AdjacentSite[SiteNo1][CellIndex] = -1;
                StartCorner[SiteNo1][CellIndex] = EdgeBottomCorner[SiteNo1];
                EndCorner[SiteNo1][CellIndex] = GetData.Corner2;
                StartX[SiteNo1][CellIndex] = EdgeBottomX[GetData.Site1];
                StartY[SiteNo1][CellIndex] = BoundYMin;
                EndX[SiteNo1][CellIndex] = GetData.Pt2X;
                EndY[SiteNo1][CellIndex] = BoundYMin;
                LinesForThisSite[SiteNo1]++;
            }
            // Deal with site 2
            if (HasEdgeBottom[SiteNo2] == 0) {
                HasEdgeBottom[SiteNo2] = 1;
                EdgeBottomX[SiteNo2] = GetData.Pt2X;
                EdgeBottomCorner[SiteNo2] = GetData.Corner2;
            }
            else if (HasEdgeBottom[SiteNo2] == 1) {
                CellIndex = LinesForThisSite[SiteNo2];
                HasEdgeLeft[SiteNo2] = 2;
                AdjacentSite[SiteNo2][CellIndex] = -1;
                StartCorner[SiteNo2][CellIndex] = EdgeBottomCorner[SiteNo2];
                EndCorner[SiteNo2][CellIndex] = GetData.Corner2;
                StartX[SiteNo2][CellIndex] = EdgeBottomX[GetData.Site2];
                StartY[SiteNo2][CellIndex] = BoundYMin;
                EndX[SiteNo2][CellIndex] = GetData.Pt2X;
                EndY[SiteNo2][CellIndex] = BoundYMin;
                LinesForThisSite[SiteNo2]++;
            }
        }
    }
    
    // Complete corners
    for (n = 0; n < NoOfSites; n++) {
        if (HasEdgeLeft[n] == 1) {
            if (HasEdgeTop[n] == 1) {
                // Top left corner
                CellIndex = LinesForThisSite[n];
                LinesForThisSite[n]++;
                AdjacentSite[n][CellIndex] = -1;
                StartCorner[n][CellIndex] = EdgeLeftCorner[n];
                EndCorner[n][CellIndex] = NextCornerNo;
                StartX[n][CellIndex] = BoundXMin;
                StartY[n][CellIndex] = EdgeLeftY[n];
                EndX[n][CellIndex] = BoundXMin;
                EndY[n][CellIndex] = BoundYMax;
                CellIndex = LinesForThisSite[n];
                LinesForThisSite[n]++;
                AdjacentSite[n][CellIndex] = -1;
                StartCorner[n][CellIndex] = NextCornerNo++;
                EndCorner[n][CellIndex] = EdgeTopCorner[n];
                StartX[n][CellIndex] = BoundXMin;
                StartY[n][CellIndex] = BoundYMax;
                EndX[n][CellIndex] = EdgeTopX[n];
                EndY[n][CellIndex] = BoundYMax;
            }
            else if (HasEdgeBottom[n] == 1) {
                // Bottom left corner
                CellIndex = LinesForThisSite[n];
                LinesForThisSite[n]++;
                AdjacentSite[n][CellIndex] = -1;
                StartCorner[n][CellIndex] = EdgeLeftCorner[n];
                EndCorner[n][CellIndex] = NextCornerNo;
                StartX[n][CellIndex] = BoundXMin;
                StartY[n][CellIndex] = EdgeLeftY[n];
                EndX[n][CellIndex] = BoundXMin;
                EndY[n][CellIndex] = BoundYMin;
                CellIndex = LinesForThisSite[n];
                LinesForThisSite[n]++;
                AdjacentSite[n][CellIndex] = -1;
                StartCorner[n][CellIndex] = NextCornerNo++;
                EndCorner[n][CellIndex] = EdgeBottomCorner[n];
                StartX[n][CellIndex] = BoundXMin;
                StartY[n][CellIndex] = BoundYMin;
                EndX[n][CellIndex] = EdgeBottomX[n];
                EndY[n][CellIndex] = BoundYMin;
            }
            else if (HasEdgeRight[n] == 1) {
                // Corner on both left and right, across entire top or bottom edge
                if (Sites[n*2+1] > 0.5f*(BoundYMin + BoundYMax)) {
                    CellIndex = LinesForThisSite[n];
                    LinesForThisSite[n]++;
                    AdjacentSite[n][CellIndex] = -1;
                    StartCorner[n][CellIndex] = EdgeLeftCorner[n];
                    EndCorner[n][CellIndex] = NextCornerNo;
                    StartX[n][CellIndex] = BoundXMin;
                    StartY[n][CellIndex] = EdgeLeftY[n];
                    EndX[n][CellIndex] = BoundXMin;
                    EndY[n][CellIndex] = BoundYMax;
                    CellIndex = LinesForThisSite[n];
                    LinesForThisSite[n]++;
                    AdjacentSite[n][CellIndex] = -1;
                    StartCorner[n][CellIndex] = NextCornerNo++;
                    EndCorner[n][CellIndex] = NextCornerNo;
                    StartX[n][CellIndex] = BoundXMin;
                    StartY[n][CellIndex] = BoundYMax;
                    EndX[n][CellIndex] = BoundXMax;
                    EndY[n][CellIndex] = BoundYMax;
                    CellIndex = LinesForThisSite[n];
                    LinesForThisSite[n]++;
                    AdjacentSite[n][CellIndex] = -1;
                    StartCorner[n][CellIndex] = NextCornerNo++;
                    EndCorner[n][CellIndex] = EdgeRightCorner[n];
                    StartX[n][CellIndex] = BoundXMax;
                    StartY[n][CellIndex] = BoundYMax;
                    EndX[n][CellIndex] = BoundXMax;
                    EndY[n][CellIndex] = EdgeRightY[n];
                }
                else {
                    CellIndex = LinesForThisSite[n];
                    LinesForThisSite[n]++;
                    AdjacentSite[n][CellIndex] = -1;
                    StartCorner[n][CellIndex] = EdgeLeftCorner[n];
                    EndCorner[n][CellIndex] = NextCornerNo;
                    StartX[n][CellIndex] = BoundXMin;
                    StartY[n][CellIndex] = EdgeLeftY[n];
                    EndX[n][CellIndex] = BoundXMin;
                    EndY[n][CellIndex] = BoundYMin;
                    CellIndex = LinesForThisSite[n];
                    LinesForThisSite[n]++;
                    AdjacentSite[n][CellIndex] = -1;
                    StartCorner[n][CellIndex] = NextCornerNo++;
                    EndCorner[n][CellIndex] = NextCornerNo;
                    StartX[n][CellIndex] = BoundXMin;
                    StartY[n][CellIndex] = BoundYMin;
                    EndX[n][CellIndex] = BoundXMax;
                    EndY[n][CellIndex] = BoundYMin;
                    CellIndex = LinesForThisSite[n];
                    LinesForThisSite[n]++;
                    AdjacentSite[n][CellIndex] = -1;
                    StartCorner[n][CellIndex] = NextCornerNo++;
                    EndCorner[n][CellIndex] = EdgeRightCorner[n];
                    StartX[n][CellIndex] = BoundXMax;
                    StartY[n][CellIndex] = BoundYMin;
                    EndX[n][CellIndex] = BoundXMax;
                    EndY[n][CellIndex] = EdgeRightY[n];
                }
            }
        }
        else if (HasEdgeRight[n] == 1) {
            if (HasEdgeTop[n] == 1) {
                // Top right corner
                CellIndex = LinesForThisSite[n];
                LinesForThisSite[n]++;
                AdjacentSite[n][CellIndex] = -1;
                StartCorner[n][CellIndex] = EdgeRightCorner[n];
                EndCorner[n][CellIndex] = NextCornerNo;
                StartX[n][CellIndex] = BoundXMax;
                StartY[n][CellIndex] = EdgeRightY[n];
                EndX[n][CellIndex] = BoundXMax;
                EndY[n][CellIndex] = BoundYMax;
                CellIndex = LinesForThisSite[n];
                LinesForThisSite[n]++;
                AdjacentSite[n][CellIndex] = -1;
                StartCorner[n][CellIndex] = NextCornerNo++;
                EndCorner[n][CellIndex] = EdgeTopCorner[n];
                StartX[n][CellIndex] = BoundXMax;
                StartY[n][CellIndex] = BoundYMax;
                EndX[n][CellIndex] = EdgeTopX[n];
                EndY[n][CellIndex] = BoundYMax;
            }
            else if (HasEdgeBottom[n] == 1) {
                // Bottom right corner
                CellIndex = LinesForThisSite[n];
                LinesForThisSite[n]++;
                AdjacentSite[n][CellIndex] = -1;
                StartCorner[n][CellIndex] = EdgeRightCorner[n];
                EndCorner[n][CellIndex] = NextCornerNo;
                StartX[n][CellIndex] = BoundXMax;
                StartY[n][CellIndex] = EdgeRightY[n];
                EndX[n][CellIndex] = BoundXMax;
                EndY[n][CellIndex] = BoundYMin;
                CellIndex = LinesForThisSite[n];
                LinesForThisSite[n]++;
                AdjacentSite[n][CellIndex] = -1;
                StartCorner[n][CellIndex] = NextCornerNo++;
                EndCorner[n][CellIndex] = EdgeBottomCorner[n];
                StartX[n][CellIndex] = BoundXMax;
                StartY[n][CellIndex] = BoundYMin;
                EndX[n][CellIndex] = EdgeBottomX[n];
                EndY[n][CellIndex] = BoundYMin;
            }
        }
    }
    
    // Calculate the last of the parameters
    float dx, dz;
    int AdjSite;
    for (CellIndex = 0; CellIndex < NoOfSites; CellIndex++) {
        
        for (n = 0; n < LinesForThisSite[CellIndex]; n++) {
            
            // Calculate distance to adjacent site and the width of the boundary between
            AdjSite = AdjacentSite[CellIndex][n];
            if (AdjSite >= 0) {
                dx = Sites[CellIndex*2] - Sites[AdjSite*2];
                dz = Sites[CellIndex*2+1] - Sites[AdjSite*2+1];
                AdjacentDistance[CellIndex][n] = sqrt (dx*dx + dz*dz);
                dx = StartX[CellIndex][n] - EndX[CellIndex][n];
                dz = StartY[CellIndex][n] - EndY[CellIndex][n];
                BorderWidth[CellIndex][n] = sqrt (dx*dx + dz*dz);
            }
            
        }
    }
    
    return LineData;

}

void Voronoi::AddOutputLine (GrimQueue<GrimLineData> *Output, float X1, float Y1, float X2, float Y2, int S1, int S2, int C1, int C2) {
    
    float Debug[4];
    Debug[0] = X1;
    Debug[1] = Y1;
    Debug[2] = X2;
    Debug[3] = Y2;
    int DebugSite1 = S1;
    int DebugSite2 = S2;
    
    // Ignore if the line segment lies entirely outside of the boundary rectangle
    float VecX = X2 - X1;
    float VecY = Y2 - Y1;
    float Cross1 = VecX*(BoundYMin - Y1) - VecY*(BoundXMin - X1);
    float Cross2 = VecX*(BoundYMax - Y1) - VecY*(BoundXMin - X1);
    float Cross3 = VecX*(BoundYMin - Y1) - VecY*(BoundXMax - X1);
    float Cross4 = VecX*(BoundYMax - Y1) - VecY*(BoundXMax - X1);
    if ((Cross1 < 0) && (Cross2 < 0) && (Cross3 < 0) && (Cross4 < 0)) return;
    if ((Cross1 > 0) && (Cross2 > 0) && (Cross3 > 0) && (Cross4 > 0)) return;
    if ((X1 < BoundXMin) && (X2 < BoundXMin)) return;
    if ((X1 > BoundXMax) && (X2 > BoundXMax)) return;
    if ((Y1 < BoundYMin) && (Y2 < BoundYMin)) return;
    if ((Y1 > BoundYMax) && (Y2 > BoundYMax)) return;
    
    // Trim the line where it crosses a boundary
    if (X1 < BoundXMin) {
        Y1 = Y1 + (Y2 - Y1) * (BoundXMin - X1)/(X2 - X1);
        X1 = BoundXMin;
    }
    else if (X1 > BoundXMax) {
        Y1 = Y1 + (Y2 - Y1) * (BoundXMax - X1)/(X2 - X1);
        X1 = BoundXMax;
    }
    if (Y1 < BoundYMin) {
        X1 = X1 + (X2 - X1) * (BoundYMin - Y1)/(Y2 - Y1);
        Y1 = BoundYMin;
    }
    else if (Y1 > BoundYMax) {
        X1 = X1 + (X2 - X1) * (BoundYMax - Y1)/(Y2 - Y1);
        Y1 = BoundYMax;
    }
    if (X2 < BoundXMin) {
        Y2 = Y2 + (Y1 - Y2) * (BoundXMin - X2)/(X1 - X2);
        X2 = BoundXMin;
    }
    else if (X2 > BoundXMax) {
        Y2 = Y2 + (Y1 - Y2) * (BoundXMax - X2)/(X1 - X2);
        X2 = BoundXMax;
    }
    if (Y2 < BoundYMin) {
        X2 = X2 + (X1 - X2) * (BoundYMin - Y2)/(Y1 - Y2);
        Y2 = BoundYMin;
    }
    else if (Y2 > BoundYMax) {
        X2 = X2 + (X1 - X2) * (BoundYMax - Y2)/(Y1 - Y2);
        Y2 = BoundYMax;
    }
    
    Debug[0] = X1;
    Debug[1] = Y1;
    Debug[2] = X2;
    Debug[3] = Y2;
    
    // Add the line to the output queue
    Output->Add (GrimLineData (X1, Y1, X2, Y2, S1, S2, C1, C2));
    
}

class SearchNode {
    
    private:
        
        static float* VoronoiSites;
        
    public:
        
        int SiteNo;
        int ComesFromSite;
        
        SearchNode* Prev;
        SearchNode* Next;
        
        float PastDist;
        float PostDist;
        float TotalCost;
        
        // For the initial point in the open set
        SearchNode (int SetSiteNo, float* SetSiteBuffer) {
            
            SiteNo = SetSiteNo;
            ComesFromSite = -1;
            
            VoronoiSites = SetSiteBuffer;
            
            Prev = 0;
            Next = 0;
            
            PastDist = 0.0f;
            PostDist = GetPostDistance (SetSiteNo);
            TotalCost = PastDist + PostDist;
            
        }
        
        // For inserting a blank new node
        SearchNode (int SetSiteNo, int ComesFrom, SearchNode* SetPrev, SearchNode* SetNext) {
            
            SiteNo = SetSiteNo;
            ComesFromSite = ComesFrom;
            
            Prev = SetPrev;
            Next = SetNext;
            
            PastDist = 0.0f;
            PostDist = 0.0f;
            TotalCost = 0.0f;
            
        }
        
        SearchNode (int SetSiteNo, int ComesFrom, SearchNode* SetPrev, SearchNode* SetNext, float Past, float Total) {
            
            SiteNo = SetSiteNo;
            ComesFromSite = ComesFrom;
            
            PastDist = Past;
            PostDist = Total - Past;
            TotalCost = Total;
            
            Prev = SetPrev;
            Next = SetNext;
            
        }
        
        static float GetPostDistance (int ThisSiteNo) {
            
            float dx = VoronoiSites[ThisSiteNo*2] - VoronoiSites[NoOfSites*2 - 2];
            float dz = VoronoiSites[ThisSiteNo*2 + 1] - VoronoiSites[NoOfSites*2 - 2 + 1];
            
            return (float)sqrt ((double)(dx*dx + dz*dz));
            
        }
        
        SearchNode* CheckForSite (int Test) {
            
            SearchNode* CheckNode = this;
            while (CheckNode != 0) {
                if (CheckNode->SiteNo == Test)
                    return CheckNode;
                CheckNode = CheckNode->Next;
            }
            
            return 0;
            
        }
        
        void InsertNew (int SetSite, int ComesFrom, float PastDistance) {
            
            float NewTotalCost = PastDistance + GetPostDistance (SetSite);
            SearchNode* CheckNode = this;
            SearchNode* NewNode;
            
            while (CheckNode != 0) {
                
                if (CheckNode->Next != 0) {
                    
                    if (CheckNode->Next->TotalCost > NewTotalCost) {
                        NewNode = new SearchNode (SetSite, ComesFrom, CheckNode, CheckNode->Next, PastDistance, NewTotalCost);
                        CheckNode->Next->Prev = NewNode;
                        CheckNode->Next = NewNode;
                        return;
                    }
                    
                }
                else {
                    NewNode = new SearchNode (SetSite, ComesFrom, CheckNode, CheckNode->Next, PastDistance, NewTotalCost);
                    CheckNode->Next = NewNode;
                    return;
                }
                
                CheckNode = CheckNode->Next;
                
            }
            
        }
        
};
float* SearchNode::VoronoiSites = 0;

void Voronoi::RunAStar (int CellFlags[], int StartSite, int GoalSite) {
    
    // Start A* with an empty closed set and the open set containing only the start point
    SearchNode* OpenSet = new SearchNode (StartSite, Sites);
    SearchNode* ClosedSet = 0;
    
    // Loop through best-estimate nodes of the open set
    int FoundPath = 0;
    int Neighbour;
    float NeighbourCost;
    SearchNode* TempNode;
    while (OpenSet != 0) {
        
        // If the goal is the best-estimate node in the set, we've found the best path
        if (OpenSet->SiteNo == GoalSite) {
            FoundPath = 1;
            ClosedSet = new SearchNode (OpenSet->SiteNo, OpenSet->ComesFromSite, 0, ClosedSet);
            if (ClosedSet->Next != 0) ClosedSet->Next->Prev = ClosedSet;
            break;
        }
        
        // Add this node to the closed set
        ClosedSet = new SearchNode (OpenSet->SiteNo, OpenSet->ComesFromSite, 0, ClosedSet);
        if (ClosedSet->Next != 0) ClosedSet->Next->Prev = ClosedSet;
        
        // Cycle through neighbours
        for (Neighbour = 0; Neighbour < LinesForThisSite[OpenSet->SiteNo]; Neighbour++) {
            
            // Ignore boundaries that do not border with another cell
            if (AdjacentSite[OpenSet->SiteNo][Neighbour] < 0)
                continue;
            
            // Ignore boundaries that are too narrow to cross
            if (BorderWidth[OpenSet->SiteNo][Neighbour] < 5.0f)
                continue;
            
            // Ignore if already in the closed set
            if (ClosedSet->CheckForSite (AdjacentSite[OpenSet->SiteNo][Neighbour]) != 0)
                continue;
            
            // Get cost of neighbour
            NeighbourCost = OpenSet->PastDist + AdjacentDistance[OpenSet->SiteNo][Neighbour];
            
            // Add to open set if not yet there
            TempNode = OpenSet->CheckForSite (AdjacentSite[OpenSet->SiteNo][Neighbour]);
            if (TempNode == 0)
                OpenSet->InsertNew (AdjacentSite[OpenSet->SiteNo][Neighbour], OpenSet->SiteNo, NeighbourCost);
            
            // Otherwise update it if there's a lesser cost via this path
            else if (TempNode->PastDist > NeighbourCost) {
                TempNode->PastDist = NeighbourCost;
                TempNode->TotalCost = TempNode->PastDist + TempNode->PostDist;
                TempNode->ComesFromSite = OpenSet->SiteNo;
            }
            else
                FoundPath = 0;
            
        }
        
        TempNode = OpenSet;
        OpenSet = OpenSet->Next;
        delete TempNode;
        
    }
    
    // Find goal cell in closed set and track backwards
    TempNode = ClosedSet->CheckForSite (GoalSite);
    while (TempNode != 0) {
        // Set this cell to visible
        CellFlags[TempNode->SiteNo] = 1;
        // Break if starting site is found
        if (TempNode->SiteNo == StartSite) break;
        // Get previous node in chain
        TempNode = TempNode->CheckForSite (TempNode->ComesFromSite);
    }
    
    // Delete memory used
    while (OpenSet != 0) {
        TempNode = OpenSet;
        OpenSet = OpenSet->Next;
        delete TempNode;
    }
    while (ClosedSet != 0) {
        TempNode = ClosedSet;
        ClosedSet = ClosedSet->Next;
        delete TempNode;
    }
    
}

void Voronoi::BuildSubset (int PathMode) {
    
    // Allocate memory for the using-cell flags
    int UseCell[NoOfSites];
    int n;
    int LineNo;
    for (n = 0; n < NoOfSites; n++) UseCell[n] = 0;
    
    // Perform algorithm to select some cells
    int StartSite = (int)((float)NoOfSites * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    int Goal1 = (int)((float)NoOfSites * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    int Goal2 = (int)((float)NoOfSites * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    int Goal3 = (int)((float)NoOfSites * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    int Goal4 = (int)((float)NoOfSites * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    int Goal5 = (int)((float)NoOfSites * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    int Goal6 = (int)((float)NoOfSites * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    int Goal7 = (int)((float)NoOfSites * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    int Goal8 = (int)((float)NoOfSites * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    int Goal9 = (int)((float)NoOfSites * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    int Goal10 = (int)((float)NoOfSites * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    int Goal11 = (int)((float)NoOfSites * static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    
    switch (PathMode) {
        
        case 0:
            RunAStar (UseCell, StartSite, Goal1);
            RunAStar (UseCell, StartSite, Goal2);
            RunAStar (UseCell, StartSite, Goal3);
            RunAStar (UseCell, StartSite, Goal4);
            RunAStar (UseCell, StartSite, Goal5);
            break;
        
        case 1:
            RunAStar (UseCell, Goal1, Goal2);
            RunAStar (UseCell, Goal2, Goal3);
            RunAStar (UseCell, Goal3, Goal4);
            RunAStar (UseCell, Goal4, Goal5);
            RunAStar (UseCell, Goal5, Goal6);
            break;
        
        case 2:
            RunAStar (UseCell, StartSite, Goal1);
            RunAStar (UseCell, StartSite, Goal2);
            RunAStar (UseCell, StartSite, Goal3);
            RunAStar (UseCell, StartSite, Goal4);
            RunAStar (UseCell, StartSite, Goal5);
            RunAStar (UseCell, StartSite, Goal6);
            RunAStar (UseCell, StartSite, Goal7);
            RunAStar (UseCell, StartSite, Goal8);
            RunAStar (UseCell, StartSite, Goal9);
            RunAStar (UseCell, StartSite, Goal10);
            break;
        
        case 3:
            RunAStar (UseCell, Goal1, Goal2);
            RunAStar (UseCell, Goal2, Goal3);
            RunAStar (UseCell, Goal3, Goal4);
            RunAStar (UseCell, Goal4, Goal5);
            RunAStar (UseCell, Goal5, Goal6);
            RunAStar (UseCell, Goal6, Goal7);
            RunAStar (UseCell, Goal7, Goal8);
            RunAStar (UseCell, Goal8, Goal9);
            RunAStar (UseCell, Goal9, Goal10);
            RunAStar (UseCell, Goal10, Goal11);
            break;
        
    }
    
    // Flag cells to include which won't comprise the floor
    // -1 - Unused cell (temporary code)
    //  0 - Unused cell
    //  1 - Floor only
    //  2 - Floor with walls
    //  3 - Next to floor, untraversible
    //  4 - Bulk untraversible area (multiple cells) surrounded by floor space
    
    // Find area lying outside of the allocated area
    // Currently 0s and 1s in buffer
    // Add code -1
    int ChangedSites;
    do {
        ChangedSites = 0;
        for (n = 0; n < NoOfSites; n++) {
            if (UseCell[n] == 0) {
                for (LineNo = 0; LineNo < LinesForThisSite[n]; LineNo++) {
                    if (AdjacentSite[n][LineNo] == -1) {
                        UseCell[n] = -1;
                        ChangedSites++;
                        break;
                    }
                    else if (UseCell[AdjacentSite[n][LineNo]] == -1) {
                        UseCell[n] = -1;
                        ChangedSites++;
                        break;
                    }
                }
            }
        }
    }
    while (ChangedSites > 0);
    
    // Find unused cells neighbouring the floor. Set some (average 50% chance per neighnouring floor) to be 'blocked' cells.
    // Currently -1s, 0s and 1s in buffer
    // Add code 3
    for (n = 0; n < NoOfSites; n++) {
        if (UseCell[n] == -1) {
            for (LineNo = 0; LineNo < LinesForThisSite[n]; LineNo++) {
                if (AdjacentSite[n][LineNo] < 0) continue;
                else if (UseCell[AdjacentSite[n][LineNo]] == 1) {
                    if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 0.5f) {
                        UseCell[n] = 3;
                        break;
                    }
                }
            }
        }
    }
    
    // Find areas enclosed completely within the floor area, and reassign -1s to 0s
    // Currently -1s, 0s, 1s and 3s
    // Add code 4, remove code -1
    for (n = 0; n < NoOfSites; n++) {
        if (UseCell[n] == 0)
            UseCell[n] = 4;
        else if (UseCell[n] == -1)
            UseCell[n] = 0;
    }
    
    // Find cells at the edge the floor
    // Currently 0s, 1s, 3s and 4s in buffer
    // Add code 2
    for (n = 0; n < NoOfSites; n++) {
        if (UseCell[n] == 1) {
            for (LineNo = 0; LineNo < LinesForThisSite[n]; LineNo++) {
                if (AdjacentSite[n][LineNo] < 0) {
                    UseCell[n] = 2;
                    break;
                }
                else if (UseCell[AdjacentSite[n][LineNo]] == 3 || UseCell[AdjacentSite[n][LineNo]] == 4 || UseCell[AdjacentSite[n][LineNo]] == 0) {
                    UseCell[n] = 2;
                    break;
                }
            }
        }
    }
    
    // Find translations between full diagram cell numbers and subset cell numbers
    int TranslateToSubset[NoOfSites];
    int SubsetCounter = 0;
    for (n = 0; n < NoOfSites; n++) {
        if (UseCell[n]) {
            TranslateToSubset[n] = SubsetCounter;
            SubsetCounter++;
        }
        else
            TranslateToSubset[n] = -1;
    }
    
    // Fill buffers with subset data
    SubsetCounter = 0;
    for (n = 0; n < NoOfSites; n++) {
        if (UseCell[n] > 0) {
            CellType[SubsetCounter] = UseCell[n];
            SubSiteX[SubsetCounter] = Sites[n*2];
            SubSiteY[SubsetCounter] = Sites[n*2 + 1];
            for (LineNo = 0; LineNo < LinesForThisSite[n]; LineNo++) {
                SubLinesForThisSite[SubsetCounter] = LinesForThisSite[n];
                if (AdjacentSite[n][LineNo] < 0) SubAdjacentSite[SubsetCounter][LineNo] = -1;
                else SubAdjacentSite[SubsetCounter][LineNo] = TranslateToSubset[AdjacentSite[n][LineNo]];
                SubAdjacentDistance[SubsetCounter][LineNo] = AdjacentDistance[n][LineNo];
                SubBorderWidth[SubsetCounter][LineNo] = BorderWidth[n][LineNo];
                SubStartCorner[SubsetCounter][LineNo] = StartCorner[n][LineNo];
                SubEndCorner[SubsetCounter][LineNo] = EndCorner[n][LineNo];
                SubStartX[SubsetCounter][LineNo] = StartX[n][LineNo];
                SubStartY[SubsetCounter][LineNo] = StartY[n][LineNo];
                SubEndX[SubsetCounter][LineNo] = EndX[n][LineNo];
                SubEndY[SubsetCounter][LineNo] = EndY[n][LineNo];
            }
            SubsetCounter++;
        }
    }
    NoOfSubsetSites = SubsetCounter;
    
    // Organise the lines around each site in clockwise order
    double AngleStart, AngleEnd, AngleDiff;
    float AngleOfStart[MaxAdjacentCells];
    int iTemp;
    float fTemp;
    int LinesChanged;
    for (n = 0; n < NoOfSubsetSites; n++) {
        // Find angles of each line, at the start point, within the range [0,2*PI).
        for (LineNo = 0; LineNo < SubLinesForThisSite[n]; LineNo++) {
            // Find tangent of angle to both start and end point of this line
            AngleStart = atan2((double)(SubStartY[n][LineNo] - SubSiteY[n]), (double)(SubStartX[n][LineNo] - SubSiteX[n]));
            AngleEnd = atan2((double)(SubEndY[n][LineNo] - SubSiteY[n]), (double)(SubEndX[n][LineNo] - SubSiteX[n]));
            // Swap if necessary such that the end point is clockwise from the start point
            AngleDiff = AngleEnd - AngleStart;
            if (AngleDiff < M_PI) AngleDiff += 2.0*M_PI;
            if (AngleDiff >= M_PI) AngleDiff -= 2.0*M_PI;
            if (AngleDiff > 0.0) {
                // End is counterclockwise from start. Need to swap.
                AngleStart = AngleEnd;
                iTemp = SubStartCorner[n][LineNo]; SubStartCorner[n][LineNo] = SubEndCorner[n][LineNo]; SubEndCorner[n][LineNo] = iTemp;
                fTemp = SubStartX[n][LineNo]; SubStartX[n][LineNo] = SubEndX[n][LineNo]; SubEndX[n][LineNo] = fTemp;
                fTemp = SubStartY[n][LineNo]; SubStartY[n][LineNo] = SubEndY[n][LineNo]; SubEndY[n][LineNo] = fTemp;
            }
            if (AngleStart < 0.0) AngleStart += 2.0*M_PI;
            if (AngleStart >= 2.0*M_PI) AngleStart -= 2.0*M_PI;
            AngleOfStart[LineNo] = (float)AngleStart;
        }
        // Sort all the lines in order of angles
        do {
            LinesChanged = 0;
            for (LineNo = 0; LineNo < SubLinesForThisSite[n] - 1; LineNo++) {
                if (AngleOfStart[LineNo] < AngleOfStart[LineNo + 1]) {
                    LinesChanged++;
                    fTemp = AngleOfStart[LineNo]; AngleOfStart[LineNo] = AngleOfStart[LineNo+1]; AngleOfStart[LineNo+1] = fTemp;
                    iTemp = SubAdjacentSite[n][LineNo]; SubAdjacentSite[n][LineNo] = SubAdjacentSite[n][LineNo+1]; SubAdjacentSite[n][LineNo+1] = iTemp;
                    fTemp = SubAdjacentDistance[n][LineNo]; SubAdjacentDistance[n][LineNo] = SubAdjacentDistance[n][LineNo+1]; SubAdjacentDistance[n][LineNo+1] = fTemp;
                    fTemp = SubBorderWidth[n][LineNo]; SubBorderWidth[n][LineNo] = SubBorderWidth[n][LineNo+1]; SubBorderWidth[n][LineNo+1] = fTemp;
                    iTemp = SubStartCorner[n][LineNo]; SubStartCorner[n][LineNo] = SubStartCorner[n][LineNo+1]; SubStartCorner[n][LineNo+1] = iTemp;
                    iTemp = SubEndCorner[n][LineNo]; SubEndCorner[n][LineNo] = SubEndCorner[n][LineNo+1]; SubEndCorner[n][LineNo+1] = iTemp;
                    fTemp = SubStartX[n][LineNo]; SubStartX[n][LineNo] = SubStartX[n][LineNo+1]; SubStartX[n][LineNo+1] = fTemp;
                    fTemp = SubStartY[n][LineNo]; SubStartY[n][LineNo] = SubStartY[n][LineNo+1]; SubStartY[n][LineNo+1] = fTemp;
                    fTemp = SubEndX[n][LineNo]; SubEndX[n][LineNo] = SubEndX[n][LineNo+1]; SubEndX[n][LineNo+1] = fTemp;
                    fTemp = SubEndY[n][LineNo]; SubEndY[n][LineNo] = SubEndY[n][LineNo+1]; SubEndY[n][LineNo+1] = fTemp;
                }
            }
        }
        while (LinesChanged > 0);
    }
    
}
