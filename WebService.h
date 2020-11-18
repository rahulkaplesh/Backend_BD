//
// Created by rahul.kaplesh on 11/20/2019.
//

#ifndef POLYGONDIVIDE_BD_WEBSERVICE_H
#define POLYGONDIVIDE_BD_WEBSERVICE_H

#include <QtCore/QObject>
#include <QtCore/QThreadPool>

//Tufao Includes
#include <Tufao/httpserver.h>
#include <Tufao/HttpServerRequest>
#include <Tufao/Headers>
#include <Tufao/HttpServerRequestRouter>
#include <Tufao/WebSocket>
#include <QtCore/QUrl>
#include <QtCore/QJsonArray>


#include "RestApiHandler.h"
#include "JobTypes.h"
#include "DivideJob.h"

//Signal handlers
#ifdef _WIN32
#include <Windows.h>
#else
#include <csignal.h>
#endif

class WebService : public QObject {
   Q_OBJECT
   int                                mPort;
   Tufao::HttpServer*                 mServer;
   Tufao::HttpServerRequestRouter*    mRouter;
   QThreadPool*                       m_jobPool;
   RestApiHandler*                    m_Handler;
   JobTypes*                          m_JobType;
   Tufao::WebSocket*                  m_wSocket;
public:
   explicit WebService(QObject *parent = nullptr, int aPort = 10129);

   bool start();

   void RequestHandler(QJsonObject aRequestObject);

   void onJobComplete();

private:
   bool setupRouter();
   void WsErrorPrinter();
   void WsConnectedPrinter();
   void MessagePrinter(QByteArray msg);
signals:

public slots:
};


#endif //POLYGONDIVIDE_BD_WEBSERVICE_H
