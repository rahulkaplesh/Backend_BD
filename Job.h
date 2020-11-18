//
// Created by rahul.kaplesh on 11/21/2019.
//

#ifndef POLYGONDIVIDE_BD_JOB_H
#define POLYGONDIVIDE_BD_JOB_H


#include <QtCore/qrunnable.h>
#include <QTCore/QObject.h>
#include <QTCore/QUuid.h>
#include <QTCore/QString.h>
#include <QtCore/QDebug.h>
#include <QtCore/QJsonObject.h>


class Job : public QObject ,public QRunnable {
   Q_OBJECT
   QUuid      mJobUuid;
   QString    mType;
public:
   explicit Job(QObject *parent = nullptr);

   virtual bool setUuid(QUuid aUuid);
   QUuid getUuid();
   bool setType(QString aType);
   virtual bool instantiateJob(QUuid id,QJsonObject Query,int numDiv);
   QString getType();
   virtual void run();
   virtual QJsonObject getSolution();
signals:
   void complete();
public slots:
};


#endif //POLYGONDIVIDE_BD_JOB_H
