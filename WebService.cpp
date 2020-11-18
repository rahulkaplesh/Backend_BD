//
// Created by rahul.kaplesh on 11/20/2019.
//

#include "WebService.h"
#include "RestApiHandler.h"

WebService::WebService(QObject *parent, int aPort) : QObject(parent), mPort(aPort)
{
   mServer = new Tufao::HttpServer(parent);
   mRouter = new Tufao::HttpServerRequestRouter(parent);
   m_jobPool = new QThreadPool(parent);
   m_Handler = RestApiHandler::createInstance(parent);
   QObject::connect(m_Handler,&RestApiHandler::ProcessRequestType,this,&WebService::RequestHandler);
   m_JobType = JobTypes::GetInstance(parent);
   m_JobType->AddJobType("Divide_Area",DivideJob::CreateDivideJob);
   m_wSocket = new Tufao::WebSocket(parent);
   QObject::connect(m_wSocket,&Tufao::WebSocket::disconnected,this,&WebService::WsErrorPrinter);
   QObject::connect(m_wSocket,&Tufao::WebSocket::connected,this,&WebService::WsConnectedPrinter);
   QObject::connect(m_wSocket,&Tufao::WebSocket::newMessage,this,&WebService::MessagePrinter);
   m_wSocket->connectToHost(QHostAddress::LocalHost,9010,"/result-val");
}

bool WebService::start()
{
   try
   {
      bool ok = true ;
      ok &= setupRouter();
      QObject::connect(mServer,&Tufao::HttpServer::requestReady,mRouter,&Tufao::HttpServerRequestRouter::handleRequest);
      ok &= mServer->listen(QHostAddress::Any, mPort);
      if (ok)
      {
         qInfo() << "Server started on port number: " << mPort ;
      }
      return ok;
   }
   catch (std::exception ex)
   {
      qCritical() << "Big Issue !! Just occurred !! ";
      return false;
   }

}

bool WebService::setupRouter() {
   try
   {
      mRouter->map({
         {QRegularExpression("^/divide_area"),"POST",RestApiHandler::DivideArea},
      });
      return true;
   }
   catch(std::exception ex)
   {
      qCritical() << "Big Issue !! Just occurred !! ";
      return false;
   }
}

void WebService::RequestHandler(QJsonObject aRequestObject)
{
   qDebug() << "Request Recieved From RestApiHandler : " << aRequestObject;

   QUuid id = QUuid::fromString(aRequestObject["UUID"].toString());
   // This has to be modified as the parent object needs to be unset when going multithreaded

   QJsonArray Ploygons = aRequestObject["PolyGon-Points"].toArray();
   QString numofDivisons = aRequestObject["NumOfPart"].toString();
   int numDiv = numofDivisons.toInt();
   foreach(const QJsonValue& Polygon , Ploygons)
   {
      Job* aJob = JobTypes::CreateJob(aRequestObject["Request_Type"].toString(),this->parent());
      bool ok = aJob->instantiateJob(id,Polygon.toObject(),numDiv);
      if (ok){
         QObject::connect(aJob,&Job::complete,this,&WebService::onJobComplete);
         aJob->run();
      }
   }
}

void WebService::onJobComplete() {
   Job* jobCompleted = qobject_cast<Job*>(sender());
   if (jobCompleted)
   {
      /*
       * TODO : NEED TO BUILD THE LOGIC FOR RETURNING BACK TO FRONTEND AND UPDATING THE VIEWS
       */
      std::cout << "Object Completed :" << jobCompleted << std::endl;
      /*
       * TODO : Return the solution object through webesocket to the backend server
       */
      QJsonDocument aSolution = QJsonDocument(jobCompleted->getSolution());
      qInfo() << aSolution;
      m_wSocket->sendMessage(aSolution.toJson());
   }
}

void WebService::WsErrorPrinter()
{
   std::cout << "Unable to connect !! " << std::endl ;
}

void WebService::WsConnectedPrinter()
{
   std::cout << "Connected !! " << std::endl ;
}

void WebService::MessagePrinter(QByteArray msg) {
   qInfo() << "Printed at the service:" << msg ;
}
