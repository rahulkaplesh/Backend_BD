//
// Created by rahul.kaplesh on 11/21/2019.
//

#include "Polygon.h"
#include "Utils.h"
#include <cmath>

// Utility Functions Not beyond the scope of this File
// Utility End

Polygon::Polygon(QObject *parent) : QObject(parent) {

}

bool Polygon::addPoint(Point_2 aPtToAdd) 
{
   try
   {
      mPoly.push_back(aPtToAdd);
      return true;
   }
   catch (std::exception ex)
   {
      qDebug() << "Exception is : " << ex.what() ;
      return false;
   }
}

bool Polygon::CreatePolygon(QJsonArray PolyRequest) 
{
    try {
        bool ok;
        //qDebug() << "Creating Polygons!" ;
        //qDebug() << PolyRequest ;
        // Need to create polygon and check whether it is a simple polygon !!
        foreach(const QJsonValue & Point, PolyRequest) {
            //qInfo() << Point;
            QString pAlt = Point["Altitude"].toString();
            double  dAlt = pAlt.toDouble();                         // Not Using Altitude Just disregarding altitude beyond this point
            QString pLat = Point["Latitude"].toString();
            double  dLat = pLat.toDouble();
            QString pLon = Point["Longitude"].toString();
            double  dLon = pLon.toDouble();
            //qInfo() << pLat << ":" << pLon << ":" << pAlt;
            //qInfo() << dAlt << ":" << dLat << ":" << dLon;
            ok = addPoint(Point_2(dLat, dLon));
        }
        CGAL::set_pretty_mode(std::cout);
        std::cout << "created the polygon :" << std::endl;
        std::cout << mPoly << std::endl;
        std::cout << std::endl;
        if (CGAL::is_simple_2(mPoly.begin(), mPoly.end()) && 
           (CGAL::is_ccw_strongly_convex_2(mPoly.begin(), mPoly.end()) || 
           CGAL::is_cw_strongly_convex_2(mPoly.begin(), mPoly.end()))) {
            qDebug() << "The Polygon constructed out of the query is simple!";
            ok = true;
        }
        else {
            ok = false;
        }
        return ok;
    }
    catch (std::exception ex) {
        qDebug() << "Exception is : " << ex.what();
        return false;
    }
}

bool Polygon::launchOperation()
{
    try 
    {
        int runNumber = 1;
        bool ok = true;
        while (runNumber < mNumOfPart)
        {
            double ratio = runNumber / (double)mNumOfPart;
            ok &= dividePolygon(ratio);
            runNumber++;
        }
        return ok;
    }
    catch (std::exception ex)
    {
        return false;
    }
}

bool Polygon::dividePolygon(double ratio)
{
   bool ok ;
   ok = findLongestSideBisector();
   if (ok)
   {
      std::cout << "Longest Side Bisector :" << std::endl;
      std::cout << mLongestBisector << std::endl;
      ok = FindMinimumCutLines();
      ok = DivideSubPolyGons(ratio);
      return ok;
   }
   return false;
}

/*
 * Finds the longest side bisector
 * @param lenBisectorMap Map of the lengths with the side bisectors
 * @param sideBisector Varaible to hold the side bisector
 * @return true or false based on whether it set the value of mLengthLongBisect and mLongestBisector
 */
bool Polygon::findLongestSideBisector() 
{
   try
   {
      std::multimap<double,Segment_2> lenBisectorMap;
      Segment_2 sideBisector;
      for( EdgeIterator k = mPoly.edges_begin() ; k != mPoly.edges_end() ; ++k )
      {
         for( EdgeIterator l = mPoly.edges_begin() ; l != mPoly.edges_end() ; ++l )
         {
            if ( *l != *k)
            {
               // Find the code to find longest edge bisector
               sideBisector = aFindBisector(*l,*k);
               double length = std::sqrt(sideBisector.squared_length());
               lenBisectorMap.insert(std::pair<double, Segment_2>(length,sideBisector));
            }
         }
      }

      auto bisectorLenPair = get_max(lenBisectorMap);
      mLengthLongBisect = bisectorLenPair.first;
      mLongestBisector = bisectorLenPair.second;
      return true;
   }
   catch (std::exception ex)
   {
      std::cout << ex.what();
      return false;
   }
}

/*
 * Finding the side bisector
 * @param a segment of one side of polygon
 * @param b segment of another side of polygon
 * @return segment connecting the midpoint of the two sides
 */
Segment_2 Polygon::aFindBisector(Segment_2 a,
                                 Segment_2 b)
{
   Point_2 mid_Point1 = CGAL::midpoint(a.start(),a.end());
   Point_2 mid_Point2 = CGAL::midpoint(b.start(),b.end());
   return Segment_2(mid_Point1,mid_Point2);
}

bool Polygon::SetNumPart(int numDiv)
{
    mNumOfPart = numDiv;
    return true;
}

bool Polygon::FindMinimumCutLines()
{
    try
    {
        Line_2  longestBisectorLine = Line_2(mLongestBisector);
        auto vertices = mPoly.vertices_begin();
        Point_2 endPoint;
        Point_2 startPoint;
        while (vertices != mPoly.vertices_end())
        {
            Point_2 projectedPoint = longestBisectorLine.projection(*vertices);
            Line_2  connectingLine = Line_2(*vertices, projectedPoint);
            Segment_2 cutLineCheck(*vertices, projectedPoint);
            //mMinimumCutLines.push_back(cutLineCheck);
            if (CGAL::angle(projectedPoint, mLongestBisector.source(), mLongestBisector.target()) == CGAL::OBTUSE)
            {
                startPoint = projectedPoint;
            }
            if (CGAL::angle(mLongestBisector.source(), mLongestBisector.target(), projectedPoint) == CGAL::OBTUSE)
            {
                endPoint = projectedPoint;
            }
            // TODO : Need to check only the perpendicular to longestBisectorLine , Points are being plotted perpendicular to the sides as well !!
            //Segment_2 linetosee = Segment_2(*vertices, projectedPoint); Not using
            // mMinimumCutLines.push_back(linetosee);
            // TODO : Find minimum cut lines and put them in an array
            auto polyedges = mPoly.edges_begin();
            while (polyedges != mPoly.edges_end())
            {
                if (!polyedges->has_on(*vertices)) {
                    CGAL::cpp11::result_of<Intersect_2(Segment_2, Line_2)>::type result = intersection(*polyedges, connectingLine);
                    if (result) {
                        if (const Point_2* p = boost::get<Point_2>(&*result)) {
                            if (*p != *vertices) {
                                Segment_2 cutLine(Point_2(*p), *vertices);
                                mMinimumCutLines.push_back(cutLine);
                            }
                        }
                    }
                }                
                polyedges++;
            }
            vertices++;
        }
        mLongerBisectorCheck = Segment_2(startPoint, endPoint);
        return true;
    }
    catch (std::exception ex)
    {
        return false;
    }
}

bool Polygon::FindCutLine(Point_2 startPoint, Point_2 endPoint, int decimalPrecision,double ratio)
/*
 Passed in the correct form so the divison can happen !!
*/
{
    try
    {
        bool ok = false;
        double x = (startPoint.x() + endPoint.x()) / 2;
        double y = (startPoint.y() + endPoint.y()) / 2;
        Point_2 midPoint = Point_2(x, y);
        std::vector<Point_2> intersectionPoints = std::vector<Point_2>();
        Polygon_2 subPolygon;
        Line_2 pLine = Line_2(mLongestBisector).perpendicular(midPoint);
        auto polyedges = mPoly.edges_begin();
        while (polyedges != mPoly.edges_end())
        {
            CGAL::cpp11::result_of<Intersect_2(Segment_2, Line_2)>::type result = intersection(*polyedges, pLine); // TODO : Intersection finding !!
            if (result)
            {
                if (const Point_2* p = boost::get<Point_2>(&*result)) {
                    intersectionPoints.push_back(Point_2(*p));
                }
            }
            polyedges++;
        }
        if (intersectionPoints.size() > 2)
        {
            std::cout << "Something is wrong!" << std::endl;
        }
        subPolygon.push_back(intersectionPoints[0]);
        subPolygon.push_back(intersectionPoints[1]);
        for (auto vert = mPoly.vertices_begin(); vert != mPoly.vertices_end(); ++vert)
        {
            if (CGAL::orientation(intersectionPoints[0], intersectionPoints[1], startPoint)
                == CGAL::orientation(intersectionPoints[0], intersectionPoints[1], *vert))
            {
                if (subPolygon.size() > 2)
                {
                    // TODO : Need to insert on the basis of angle ( Check this !! )
                    auto subpolyVert = std::next(subPolygon.vertices_begin(), 2);
                    bool inserted{ false };
                    while (subpolyVert != subPolygon.vertices_end())
                    {
                        Vector_2 vec1 = Vector_2(intersectionPoints[0], intersectionPoints[1]);
                        Vector_2 vec2 = Vector_2((*subpolyVert), intersectionPoints[1]);
                        double currentVertexangle = (std::acos((vec1 * vec2) / (std::sqrt(vec1.squared_length()) * std::sqrt(vec2.squared_length()))) * 57.2958);
                        std::cout << "Current Vertex angle " << currentVertexangle << "degrees" << std::endl;
                        Vector_2 vec3 = Vector_2((*vert), intersectionPoints[1]);
                        double Vertexangle = (std::acos((vec1 * vec3) / (std::sqrt(vec1.squared_length()) * std::sqrt(vec3.squared_length()))) * 57.2958);
                        std::cout << "Vertex angle " << ":" << Vertexangle << "degrees" << std::endl;
                        if (currentVertexangle < Vertexangle)
                        {
                            subPolygon.insert(subpolyVert, (*vert));
                            inserted = true;
                            break;
                        }
                        subpolyVert++;
                    }
                    if (inserted == false)
                    {
                        subPolygon.push_back(*vert);
                    }
                }
                else
                {
                    subPolygon.push_back(*vert);
                }
            }
        }
        /*if (CGAL::collinear(mLongestBisector.source(),mLongestBisector.target(),midPoint))
        {
            Line_2 pLine = Line_2(mLongestBisector).perpendicular(midPoint);
            auto polyedges = mPoly.edges_begin();
            while (polyedges != mPoly.edges_end())
            {
                CGAL::cpp11::result_of<Intersect_2(Segment_2, Line_2)>::type result = intersection(*polyedges, pLine); // TODO : Intersection finding !!
                if (result)
                {
                    if (const Point_2* p = boost::get<Point_2>(&*result)) {
                        intersectionPoints.push_back(Point_2(*p));
                    }
                }
                polyedges++;
            }
            if (intersectionPoints.size() > 2)
            {
                std::cout << "Something is wrong!" << std::endl;
            }
            subPolygon.push_back(intersectionPoints[0]);
            subPolygon.push_back(intersectionPoints[1]);
            for (auto vert = mPoly.vertices_begin(); vert != mPoly.vertices_end(); ++vert)
            {
                if (CGAL::orientation(intersectionPoints[0], intersectionPoints[1], startPoint)
                    == CGAL::orientation(intersectionPoints[0], intersectionPoints[1], *vert))
                {
                    if (subPolygon.size() > 2)
                    {
                        // TODO : Need to insert on the basis of angle ( Check this !! )
                        auto subpolyVert = std::next(subPolygon.vertices_begin(), 2);
                        bool inserted{ false };
                        while (subpolyVert != subPolygon.vertices_end())
                        {
                            Vector_2 vec1 = Vector_2(intersectionPoints[0], intersectionPoints[1]);
                            Vector_2 vec2 = Vector_2((*subpolyVert), intersectionPoints[1]);
                            double currentVertexangle = (std::acos((vec1 * vec2) / (std::sqrt(vec1.squared_length()) * std::sqrt(vec2.squared_length()))) * 57.2958);
                            std::cout << "Current Vertex angle " << currentVertexangle << "degrees" << std::endl;
                            Vector_2 vec3 = Vector_2((*vert), intersectionPoints[1]);
                            double Vertexangle = (std::acos((vec1 * vec3) / (std::sqrt(vec1.squared_length()) * std::sqrt(vec3.squared_length()))) * 57.2958);
                            std::cout << "Vertex angle " << ":" << Vertexangle << "degrees" << std::endl;
                            if (currentVertexangle < Vertexangle)
                            {
                                subPolygon.insert(subpolyVert, (*vert));
                                inserted = true;
                                break;
                            }
                            subpolyVert++;
                        }
                        if (inserted == false)
                        {
                            subPolygon.push_back(*vert);
                        }
                    }
                    else
                    {
                        subPolygon.push_back(*vert);
                    }
                }
            }
        }
        else
        {
            std::cout << "This is the Culprit Right Here !! " << std::endl;
        }*/
        if (subPolygon.size() < 2)
        {
            std::cout << "Something is wrong!" << std::endl;
        }
        else if (roundDecimalTo(fabs(subPolygon.area()), decimalPrecision) == roundDecimalTo((mPoly.area()*ratio), decimalPrecision))
        {
            mDivisonLine = Segment_2(intersectionPoints[0],intersectionPoints[1]);
            mDivisonLines.push_back(mDivisonLine);
        }
        else if (roundDecimalTo(fabs(subPolygon.area()), decimalPrecision) < roundDecimalTo((mPoly.area()*ratio), decimalPrecision))
        {
            std::cout << "SubPolygon Area: " << roundDecimalTo(fabs(subPolygon.area()), decimalPrecision) << std::endl;
            std::cout << "Area To be Found: " << roundDecimalTo((mPoly.area() * ratio), decimalPrecision) << std::endl;
            ok = FindCutLine(midPoint, endPoint, decimalPrecision,ratio);
        }
        else
        {
            std::cout << "SubPolygon Area: " << roundDecimalTo(fabs(subPolygon.area()), decimalPrecision) << std::endl;
            std::cout << "Area To be Found: " << roundDecimalTo((mPoly.area() * ratio), decimalPrecision) << std::endl;
            ok = FindCutLine(startPoint, midPoint, decimalPrecision,ratio);
        }
        return true;
    }
    catch (std::exception ex)
    {
        std::cout << "Error : " << ex.what() << std::endl;
        return false;
    }
}

bool Polygon::addToPolygon(Polygon_2& poly, Point_2& point)
{
    try
    {
        auto vert = poly.vertices_begin();
        Point_2 vert1 = *vert;
        vert++;
        Point_2 vert2 = *vert;
        vert++;
        if (poly.size() > 2)
        {
            bool inserted{ false };
            while (vert != poly.vertices_end())
            {
                Vector_2 vec1 = Vector_2(vert1, vert2);
                Vector_2 vec2 = Vector_2((*vert), vert2);
                double currentVertexangle = (std::acos((vec1 * vec2) / (std::sqrt(vec1.squared_length()) * std::sqrt(vec2.squared_length()))) * 57.2958);
                std::cout << "Current Vertex angle " << currentVertexangle << "degrees" << std::endl;
                Vector_2 vec3 = Vector_2(point, vert2);
                double Vertexangle = (std::acos((vec1 * vec3) / (std::sqrt(vec1.squared_length()) * std::sqrt(vec3.squared_length()))) * 57.2958);
                std::cout << "Vertex angle " << ":" << Vertexangle << "degrees" << std::endl;
                if (currentVertexangle < Vertexangle)
                {
                    poly.insert(vert, point);
                    inserted = true;
                    break;
                }
                vert++;
            }
            if (inserted == false)
            {
                poly.push_back(point);
            }
        }
        else 
        {
            poly.push_back(point);
        }
        return true;
    }
    catch (std::exception ex)
    {
        std::cout << "Error : " << ex.what() << std::endl;
        return false;
    }
}

bool Polygon::DivideSubPolyGons(double ratio)
{
    try
    {
        /*
        *To Be Used Later
        */
        Point_2 evalPoint = mLongestBisector.source();
        std::map<double, std::pair<Point_2,Segment_2>> minCutLinesDistance = std::map<double, std::pair<Point_2, Segment_2>>();
        for (auto& cutLine : mMinimumCutLines) {
            CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type result = intersection(mLongestBisector, cutLine);
            if (result) {
                if (const Point_2* p = boost::get<Point_2>(&*result)) {
                    double distance = std::sqrt(CGAL::squared_distance(*p, evalPoint));
                    minCutLinesDistance.insert(std::pair<double, std::pair<Point_2, Segment_2>>(distance, std::pair<Point_2, Segment_2>(*p, cutLine)));
                }
            }
            /*double distance = std::sqrt(CGAL::squared_distance(cutLine.target(), evalPoint));
            minCutLinesDistance.insert(std::pair<double, std::pair<Point_2, Segment_2>>(distance, std::pair<Point_2, Segment_2>(cutLine.target(), cutLine)));*/
        }
        auto mCutlinesDistanceiter = minCutLinesDistance.begin();
        int i = 1;
        bool partitionAvail = false;
        while (mCutlinesDistanceiter != minCutLinesDistance.end()) {
            // TODO : Need to form polygons and check where the partition line lies and get the partition line            
            Segment_2 cutLineBeingEvaluated = mCutlinesDistanceiter->second.second;
            // Loop to check all vertices of the main polygon and form sub polygon
            Polygon_2 subPolygon;
            subPolygon.push_back(cutLineBeingEvaluated.source());
            subPolygon.push_back(cutLineBeingEvaluated.target());            
            for (auto vert = mPoly.vertices_begin(); vert != mPoly.vertices_end(); ++vert)
            {
                if (CGAL::orientation(cutLineBeingEvaluated.source(), cutLineBeingEvaluated.target(), evalPoint)
                    == CGAL::orientation(cutLineBeingEvaluated.source(), cutLineBeingEvaluated.target(), *vert))
                {
                    if (subPolygon.size() > 2)
                    {
                        // TODO : Need to insert on the basis of angle ( Check this !! )
                        auto subpolyVert = std::next(subPolygon.vertices_begin(),2);
                        bool inserted{ false };
                        while (subpolyVert != subPolygon.vertices_end())
                        {
                            Vector_2 vec1 = Vector_2(cutLineBeingEvaluated.source(), cutLineBeingEvaluated.target());
                            Vector_2 vec2 = Vector_2((*subpolyVert), cutLineBeingEvaluated.target());
                            double currentVertexangle = (std::acos((vec1 * vec2) / ( std::sqrt(vec1.squared_length()) * std::sqrt(vec2.squared_length()))) * 57.2958);
                            std::cout << "Current Vertex angle " << currentVertexangle << "degrees" << std::endl;
                            Vector_2 vec3 = Vector_2((*vert), cutLineBeingEvaluated.target());
                            double Vertexangle = (std::acos((vec1 * vec3) / (std::sqrt(vec1.squared_length()) * std::sqrt(vec3.squared_length()))) * 57.2958);
                            std::cout << "Vertex angle " << ":" << Vertexangle << "degrees" << std::endl;
                            if (currentVertexangle < Vertexangle)
                            {
                                subPolygon.insert(subpolyVert, (*vert));
                                inserted = true;
                                break;
                            }
                            subpolyVert++;
                        }
                        if (inserted == false) 
                        {
                            subPolygon.push_back(*vert);
                        }
                    }
                    else
                    {
                        subPolygon.push_back(*vert);
                    }
                }
            }
            std::cout << "Main polygon area : " << mPoly.area() << ":" << roundDecimalTo(mPoly.area(),2) << std::endl;
            std::cout << " Sub polygon area : " << fabs(subPolygon.area()) << std::endl;
            if (fabs(subPolygon.area()) > (mPoly.area() * ratio)) {
                // TODO : To Insert a Break Statement and check if the cut line is populated !!
                // TODO : Problem will come when the cut is before the cutline and after the last cutline !!
                partitionAvail = true;
                bool ok;
                if (mCutlinesDistanceiter == minCutLinesDistance.begin())
                {
                    //TODO : If line lies before the first line 
                    std::cout << "Divide Line lies before the first min cut line !!" << std::endl;
                    ok = FindCutLine(mLongerBisectorCheck.source(), mCutlinesDistanceiter->second.first, 2, ratio);
                }
                else {
                    // TODO : In all other conditions do this !!
                    ok = FindCutLine(std::prev(mCutlinesDistanceiter)->second.first, mCutlinesDistanceiter->second.first, 2, ratio);
                    std::cout << "Minimum cut Between : " << i - 1 << " & " << i << std::endl;
                }                
                if (ok == true) {
                    // TODO : Have the mDivisonLine Need to check for area and validation
                    mSubPolygons.push_back(Polygon_2());
                    mSubPolygons.push_back(Polygon_2());
                    mSubPolygons[0].push_back(mDivisonLine.source());
                    mSubPolygons[1].push_back(mDivisonLine.source());
                    mSubPolygons[0].push_back(mDivisonLine.target());
                    mSubPolygons[1].push_back(mDivisonLine.target());
                    for (auto vert = mPoly.vertices_begin(); vert != mPoly.vertices_end(); ++vert)
                    {
                        bool ok = false;
                        if (CGAL::left_turn(mDivisonLine.source(), mDivisonLine.target(), *vert))
                        {
                            ok = addToPolygon(mSubPolygons[0], *vert);
                        }
                        else 
                        {
                            ok = addToPolygon(mSubPolygons[1], *vert);
                        }
                        if (ok == false)
                        {
                            std::cout << "Something really bad happened!!" << std::endl;
                        }
                    }
                    std::cout << "Area of main polygon:" << roundDecimalTo(mPoly.area(), 2) << std::endl;
                    std::cout << "Area of sub polygon1:" << roundDecimalTo(fabs(mSubPolygons[0].area()), 5) << std::endl;
                    std::cout << "Area of sub polygon2:" << roundDecimalTo(fabs(mSubPolygons[1].area()), 5) << std::endl;
                    break; 
                }
            }
            if (fabs(subPolygon.area()) == (mPoly.area() * ratio))
            {
                partitionAvail = true;
                std::cout << "Min Cut line is the divide line !!" << std::endl;
                mDivisonLine = mCutlinesDistanceiter->second.second;
                mDivisonLines.push_back(mDivisonLine);
                mSubPolygons.push_back(Polygon_2());
                mSubPolygons.push_back(Polygon_2());
                mSubPolygons[0].push_back(mDivisonLine.source());
                mSubPolygons[1].push_back(mDivisonLine.source());
                mSubPolygons[0].push_back(mDivisonLine.target());
                mSubPolygons[1].push_back(mDivisonLine.target());
                for (auto vert = mPoly.vertices_begin(); vert != mPoly.vertices_end(); ++vert)
                {
                    bool ok = false;
                    if (CGAL::left_turn(mDivisonLine.source(), mDivisonLine.target(), *vert))
                    {
                        ok = addToPolygon(mSubPolygons[0], *vert);
                    }
                    else
                    {
                        ok = addToPolygon(mSubPolygons[1], *vert);
                    }
                    if (ok == false)
                    {
                        std::cout << "Something really bad happened!!" << std::endl;
                    }
                }
                std::cout << "Area of main polygon:" << roundDecimalTo(mPoly.area(), 2) << std::endl;
                std::cout << "Area of sub polygon1:" << roundDecimalTo(fabs(mSubPolygons[0].area()), 5) << std::endl;
                std::cout << "Area of sub polygon2:" << roundDecimalTo(fabs(mSubPolygons[1].area()), 5) << std::endl;
                break;
            }
            i++;
            mCutlinesDistanceiter++;
        }
        if (partitionAvail == false)
        {
            std::cout << "Divide Line lies beyond the last min cut line !!" << std::endl;
            bool ok = FindCutLine(std::prev(minCutLinesDistance.end())->second.first, mLongerBisectorCheck.target(), 2, ratio);
            if (ok == true) {
                // TODO : Have the mDivisonLine Need to check for area and validation
                mSubPolygons.push_back(Polygon_2());
                mSubPolygons.push_back(Polygon_2());
                mSubPolygons[0].push_back(mDivisonLine.source());
                mSubPolygons[1].push_back(mDivisonLine.source());
                mSubPolygons[0].push_back(mDivisonLine.target());
                mSubPolygons[1].push_back(mDivisonLine.target());
                for (auto vert = mPoly.vertices_begin(); vert != mPoly.vertices_end(); ++vert)
                {
                    bool ok = false;
                    if (CGAL::left_turn(mDivisonLine.source(), mDivisonLine.target(), *vert))
                    {
                        ok = addToPolygon(mSubPolygons[0], *vert);
                    }
                    else
                    {
                        ok = addToPolygon(mSubPolygons[1], *vert);
                    }
                    if (ok == false)
                    {
                        std::cout << "Something really bad happened!!" << std::endl;
                    }
                }
                std::cout << "Area of main polygon:" << roundDecimalTo(mPoly.area(), 2) << std::endl;
                std::cout << "Area of sub polygon1:" << roundDecimalTo(fabs(mSubPolygons[0].area()), 5) << std::endl;
                std::cout << "Area of sub polygon2:" << roundDecimalTo(fabs(mSubPolygons[1].area()), 5) << std::endl;
            }
        }
        /*std::map<double, Point_2> cutlinesNearestPoints;
        for (auto& cutline : mMinimumCutLines)
        {
            cutlinesNearestPoints = std::map<double, Point_2>();
            auto vert = mPoly.vertices_begin();
            while (vert != mPoly.vertices_end())
            {
                if (!cutline.has_on(*vert)) {
                    double distance = std::sqrt(CGAL::squared_distance(*vert, cutline));
                    cutlinesNearestPoints.insert(std::pair<double, Point_2>(distance, *vert));
                }
                vert++;
            }
            Polygon_2 firstPolygon, secondPolygon;
            firstPolygon.push_back(cutline.source());
            firstPolygon.push_back(cutline.target());
            secondPolygon.push_back(cutline.source());
            secondPolygon.push_back(cutline.target());            
            Segment_2 cutLineContain;
            bool passesCutLines{ false };
            
            // Trying to add to the First Polygon
            auto cutlinesNearestPointsIter = cutlinesNearestPoints.begin();
            Point_2 nearestPoint = cutlinesNearestPointsIter->second;
            for (auto& cutLineCheck : mMinimumCutLines) {
                if (cutLineCheck.has_on(nearestPoint)) {
                    passesCutLines = true;
                    cutLineContain = cutLineCheck;
                }
            }
            if (passesCutLines == true) 
            {
                if (cutline.direction() == cutLineContain.direction())
                {
                    firstPolygon.push_back(cutLineContain.target());
                    firstPolygon.push_back(cutLineContain.source());
                }
                else
                {
                    firstPolygon.push_back(cutLineContain.source());
                    firstPolygon.push_back(cutLineContain.target());
                }
            }
            else 
            {
                firstPolygon.push_back(nearestPoint);
            }
            // Trying to add to the second Polygon

        }*/
        return true;
    }
    catch (std::exception ex)
    {
        return false;
    }
}

QJsonObject Polygon::GetPolygon() {
    QJsonArray pointsArray ;
    for( VertexIterator k = mPoly.vertices_begin() ; k != mPoly.vertices_end() ; ++k )
    {
        QJsonObject PointCoord ;
        Point_2 vertex = Point_2(*k);
        PointCoord["Latitude"] = vertex.x();
        PointCoord["Longitude"] = vertex.y();
        pointsArray.push_back(PointCoord);
    }
    QJsonObject ObjToBeRet;
    ObjToBeRet["poly-points"] = pointsArray;
    QJsonArray bisectorPointsArray;
    QJsonObject PointCoord;
    PointCoord["Latitude"] = mLongestBisector.source().x();
    PointCoord["Longitude"] = mLongestBisector.source().y();
    bisectorPointsArray.push_back(PointCoord);
    PointCoord["Latitude"] = mLongestBisector.target().x();
    PointCoord["Longitude"] = mLongestBisector.target().y();
    bisectorPointsArray.push_back(PointCoord);
    ObjToBeRet["bisetor-points"] = bisectorPointsArray;
    QJsonArray CutLinesArray;
    for (auto& mLine : mMinimumCutLines) {
        QJsonArray CutLine;
        PointCoord["Latitude"] = mLine.source().x();
        PointCoord["Longitude"] = mLine.source().y();
        CutLine.push_back(PointCoord);
        PointCoord["Latitude"] = mLine.target().x();
        PointCoord["Longitude"] = mLine.target().y();
        CutLine.push_back(PointCoord);
        CutLinesArray.push_back(CutLine);
    }
    ObjToBeRet["Cut-Lines"] = CutLinesArray;
    // TODO : Need to write from the array of divison lines
    QJsonArray DivLinesArray;
    for (auto& divisonLine : mDivisonLines)
    {
        QJsonArray divisonPointArray;
        PointCoord["Latitude"] = divisonLine.source().x();
        PointCoord["Longitude"] = divisonLine.source().y();
        divisonPointArray.push_back(PointCoord);
        PointCoord["Latitude"] = divisonLine.target().x();
        PointCoord["Longitude"] = divisonLine.target().y();
        divisonPointArray.push_back(PointCoord);
        DivLinesArray.push_back(divisonPointArray);
    }
    ObjToBeRet["Divison-Lines"] = DivLinesArray;
    return ObjToBeRet;
}
