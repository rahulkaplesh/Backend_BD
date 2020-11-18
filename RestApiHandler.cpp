//
// Created by rahul.kaplesh on 11/20/2019.
//

#include "RestApiHandler.h"

#include <QDebug>

RestApiHandler* RestApiHandler::minstance;

RestApiHandler::RestApiHandler(QObject *parent) : QObject(parent){
}

bool RestApiHandler::DivideArea(Tufao::HttpServerRequest &aRequest,
                                Tufao::HttpServerResponse &aResponse)
{
   QJsonParseError error;
   QJsonDocument aDocument(QJsonDocument::fromJson(aRequest.readBody(), &error));
   if (error.error != QJsonParseError::NoError)
   {
      qDebug() << "JSON Parse Error is : " << error.errorString() ;
   }
   QJsonObject RequestObject = aDocument.object();
   RequestObject[QString("Request_Type")] = QString("Divide_Area");
   RequestObject[QString("UUID")] = QString(QUuid::createUuid().toString());
   emit createInstance()->ProcessRequestType(RequestObject);
   RequestObject[QString("Status")] = QString("processing");
   QJsonDocument ResponseObject(RequestObject);
   aResponse.writeHead(Tufao::HttpResponseStatus::OK);
   aResponse.headers().replace("Content-Type", "application/json");
   aResponse.end(ResponseObject.toJson());
   return true;
}

RestApiHandler* RestApiHandler::createInstance(QObject *parent) {
   if (!minstance)
   {
      RestApiHandler::minstance = new RestApiHandler(parent);
      return RestApiHandler::minstance;
   } else{
      return RestApiHandler::minstance;
   }
}

