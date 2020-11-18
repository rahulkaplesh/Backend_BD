//
// Created by rahul.kaplesh on 11/21/2019.
//

#ifndef POLYGONDIVIDE_BD_DIVIDEJOB_H
#define POLYGONDIVIDE_BD_DIVIDEJOB_H


#include "Job.h"
#include <QtCore/QObject.h>
#include <QtCore/QUuid.h>

#include "Polygon.h"

class DivideJob : public Job {
   Q_OBJECT
   Polygon         *mPoly;
   QString         mAreaName;
public:
   static Job* CreateDivideJob(QObject *parent = nullptr);
   explicit DivideJob(QObject *parent = nullptr);
   bool setUuid(QUuid uuid) override ;
   bool instantiateJob(QUuid id,QJsonObject Query,int numDiv) override ;
   void run() override;
   QJsonObject getSolution() override;
signals:
public slots:
};


#endif //POLYGONDIVIDE_BD_DIVIDEJOB_H
