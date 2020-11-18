//
// Created by rahul.kaplesh on 11/20/2019.
//

#ifndef POLYGONDIVIDE_BD_RESTAPIHANDLER_H
#define POLYGONDIVIDE_BD_RESTAPIHANDLER_H


#include <QtCore/QObject>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QUuid>

// Tufao include
#include <Tufao/HttpServerRequest>
#include <Tufao/Headers>

#include "Job.h"

class RestApiHandler : public QObject {
   Q_OBJECT
   explicit RestApiHandler(QObject *parent);
public:
   static RestApiHandler* createInstance(QObject *parent = nullptr);
   static bool DivideArea(Tufao::HttpServerRequest& aRequest,
                          Tufao::HttpServerResponse& aResponse);

signals:
   void ProcessRequestType(QJsonObject aRequest);

public slots:

private:
   static RestApiHandler *minstance ;
};


#endif //POLYGONDIVIDE_BD_RESTAPIHANDLER_H
