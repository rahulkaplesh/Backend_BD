//
// Created by rahul.kaplesh on 11/21/2019.
//

#include "DivideJob.h"

DivideJob::DivideJob(QObject *parent) : Job(parent)
{
   setType("Divide_Area");
   mPoly = new Polygon(parent);
}

bool DivideJob::setUuid(QUuid uuid) {
   bool ok = Job::setUuid(uuid);
   return ok;
}

Job *DivideJob::CreateDivideJob(QObject *parent) {
   return new DivideJob(parent);
}

void DivideJob::run() {
   qDebug() << "In Divide Job Run!!";
   mPoly->launchOperation();
   //mPoly->dividePolygon(0.5);
   emit complete();
}

bool DivideJob::instantiateJob(QUuid id,QJsonObject Query, int numDiv) {
   try
   {
      bool ok = false;
      mAreaName = Query["areaName"].toString();
      ok = mPoly->CreatePolygon(Query["points"].toArray());
      ok = mPoly->SetNumPart(numDiv);
      ok = setUuid(id);
      return ok;
   }
   catch(std::exception ex)
   {
      return false;
   }

}

QJsonObject DivideJob::getSolution() {
   QJsonObject Polygon ;
   Polygon["polygon"] = mPoly->GetPolygon();
   Polygon["area-name"] = mAreaName;
   Polygon["UUID"] = getUuid().toString();
   return Polygon;
}
