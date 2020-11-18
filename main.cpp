#include <iostream>
#include <QtCore/QCoreApplication>
#include "FPProcess.h"
#include "RestApiHandler.h"
#include "WebService.h"

using namespace Tufao;

int main(int argc, char *argv[]) {
   QCoreApplication a(argc, argv);

   //WebService Initialised
   WebService service(&a);

   //FrontEnd Process
   FPProcess FP(&a);

   if(FP.beginServer() && service.start())
   {
      return a.exec();
   }
   else
   {
      return 0;
   }
}