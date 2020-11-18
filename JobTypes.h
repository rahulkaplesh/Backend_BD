//
// Created by rahul.kaplesh on 11/22/2019.
//

#ifndef POLYGONDIVIDE_BD_JOBTYPES_H
#define POLYGONDIVIDE_BD_JOBTYPES_H


#include <QtCore/QObject>
#include <unordered_map>
#include "Job.h"

class JobTypes : public QObject {
   Q_OBJECT
   static std::map<QString,std::function<Job*(QObject*)>> mJobTypeLists;
   explicit JobTypes(QObject *parent = nullptr);
public:
   static JobTypes* GetInstance(QObject *parent = nullptr);

   bool AddJobType(QString arequestType, std::function<Job*(QObject*)> afunc);

   static Job* CreateJob(QString arequestType ,QObject *parent = nullptr);
signals:

public slots:

private:
   static JobTypes *mInstance ;
};


#endif //POLYGONDIVIDE_BD_JOBTYPES_H
