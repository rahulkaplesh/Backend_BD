//
// Created by rahul.kaplesh on 11/21/2019.
//

#include "Job.h"

Job::Job(QObject *parent) : QObject(parent)
{
   mJobUuid = QUuid::createUuid();
}

QUuid Job::getUuid()
{
   return mJobUuid;
}

bool Job::setType(QString aType)
{
   try
   {
      mType = aType;
      return true;
   }
   catch (std::exception ex)
   {
      qDebug() << ex.what() ;
      return false;
   }
}

QString Job::getType() {
   return mType;
}

bool Job::setUuid(QUuid aUuid) {
   mJobUuid = aUuid ;
   return true ;
}

void Job::run() {
   qDebug() << "Please Check the request details !!" ;
}

bool Job::instantiateJob(QUuid id,QJsonObject Query,int numDiv) {
   return false;
}

QJsonObject Job::getSolution() {
   return QJsonObject();
}

