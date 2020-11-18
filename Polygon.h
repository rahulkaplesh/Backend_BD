//
// Created by rahul.kaplesh on 11/21/2019.
//

#ifndef POLYGONDIVIDE_BD_POLYGON_H
#define POLYGONDIVIDE_BD_POLYGON_H

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include "typedef.h"


class Polygon : public QObject {
   Q_OBJECT
   Polygon_2              mPoly;
   Segment_2              mLongestBisector;
   Segment_2              mLongerBisectorCheck;
   Segment_2              mDivisonLine;
   std::vector<Segment_2> mDivisonLines;
   double                 mLengthLongBisect;
   std::vector<Segment_2> mMinimumCutLines;
   std::vector<Polygon_2> mSubPolygons;
   int                    mNumOfPart;
public:
   Polygon(QObject *parent = nullptr);
   bool addPoint(Point_2 aPtToAdd);

   bool launchOperation();
   bool dividePolygon(double ratio);

   /*Algorithm functions*/
   bool findLongestSideBisector();
   Segment_2 aFindBisector(Segment_2 a,
                           Segment_2 b);

   bool CreatePolygon(QJsonArray PolyRequest);
   bool SetNumPart(int numDiv);

   bool FindMinimumCutLines();
   bool FindCutLine(Point_2 startPoint, Point_2 endPoint, int decimalPrecision,double ratio);
   bool addToPolygon(Polygon_2& poly, Point_2& point);

   bool DivideSubPolyGons(double ratio);

   /*
    * Get Polygon as a JsonObject
    */
   QJsonObject GetPolygon();
};


#endif //POLYGONDIVIDE_BD_POLYGON_H
