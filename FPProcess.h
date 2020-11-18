//
// Created by rahul.kaplesh on 11/14/2019.
//
/*
 * This file just creates a new node process and outputs to
 * standard output process
 */

#ifndef POLYGONDIVIDE_BD_FPPROCESS_H
#define POLYGONDIVIDE_BD_FPPROCESS_H


#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QString>
#include <QtCore/QDebug>

class FPProcess : public QObject{
   Q_OBJECT
   QProcess  *FP;
   QString   mProcessName{"node.exe"};
   QString   mServerLocation{"E:\\DividePolygon\\cesium_starter\\server.js"};
public:
   explicit FPProcess(QObject *parent = nullptr);

   void printStandardOutput();
   void printStandardError();

   bool beginServer();

   ~FPProcess();
signals:

public slots:

};


#endif //POLYGONDIVIDE_BD_FPPROCESS_H
