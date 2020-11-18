//
// Created by rahul.kaplesh on 11/14/2019.
//

#include "FPProcess.h"

FPProcess::FPProcess(QObject *parent) : QObject(parent)
{
   FP = new QProcess(parent);
   connect(FP, &QProcess::readyReadStandardOutput,this,&FPProcess::printStandardOutput);
   connect(FP,&QProcess::readyReadStandardOutput,this,&FPProcess::printStandardError);
}

void FPProcess::printStandardOutput()
{
   qDebug() << FP->readAllStandardOutput() ;
}

FPProcess::~FPProcess()
{
   FP->kill();
   FP->deleteLater();
}

bool FPProcess::beginServer() {
   try
   {
      FP->start(mProcessName,QStringList()<<mServerLocation);
      return true;
   }
   catch(std::exception ex)
   {
      return false;
   }
}

void FPProcess::printStandardError()
{
   QString Error(FP->readAllStandardError());
   if (!Error.isEmpty())
   {
      qDebug() << "Error in FrontEnd Process : " << Error ;
      qFatal("Frontend unable to begin aborting");
   }
}
