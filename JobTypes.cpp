//
// Created by rahul.kaplesh on 11/22/2019.
//

#include "JobTypes.h"

JobTypes* JobTypes::mInstance;
std::map<QString,std::function<Job*(QObject*)>> JobTypes::mJobTypeLists;

JobTypes::JobTypes(QObject *parent) : QObject(parent)
{
}

JobTypes* JobTypes::GetInstance(QObject *parent) {
   if (!mInstance)
   {
      mInstance = new JobTypes(parent);
      return mInstance;
   }
   else
   {
      return mInstance;
   }
}

bool JobTypes::AddJobType(QString arequestType, std::function<Job *(QObject *)> afunc) {
   try
   {
      mJobTypeLists.insert(std::pair<QString,std::function<Job *(QObject *)>>(arequestType,afunc));
      return true;
   }
   catch(std::exception ex)
   {
      return false;
   }
}

Job *JobTypes::CreateJob(QString arequestType ,QObject *parent) {
   for ( auto &JobType : mJobTypeLists )
   {
      if ( JobType.first == arequestType)
      {
         return JobType.second(parent);
      }
   }
   return new Job(parent);
}
