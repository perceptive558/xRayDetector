/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>


/*
    This file defines a helper function to open a connection to an
    in-memory SQLITE database and to create a test table.

    If you want to use another database, simply modify the code
    below. All the examples in this directory use this function to
    connect to a database.
*/
//! [0]
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("memory.db");
    if (!db.open()) {
        return false;
    }

    QSqlQuery query;


    query.exec("create table detectItems ( name varchar(20) primary key,"
                                           "productNo int,"
                                           "threshold1 int,"
                                           "threshold2 int,"
                                           "erodeSize int)");
    query.exec("insert into detectItems values('雪花啤酒', 6, 50, 100, 11)");
    query.exec("insert into detectItems values('青岛啤酒', 6, 60, 110, 13)");
    query.exec("insert into detectItems values('青岛纯生', 6, 70, 120, 15)");
    query.exec("insert into detectItems values('金士百', 6, 80, 130, 17)");


    query.exec("create table detectResults (id int primary key,"
                                           "Date varchar(20),"
                                           "startTime varchar(20),"
                                           "endTime varchar(20),"
                                           "ProductNo int,"
                                           "NGProductNo int,"
                                           "productName varchar(20))");
    query.exec("insert into detectResults "
               "values(1, "
               "'20160831',"
               "'8:20', "
               "'10:20',"
               "1000,"
               "10,"
               "'雪花啤酒') ");
    query.exec("insert into detectResults "
               "values(2, "
               "'20160731',"
               "'8:20', "
               "'10:20',"
               "1000,"
               "10,"
               "'雪花啤酒') ");
    query.exec("insert into detectResults "
               "values(3, "
               "'20160810',"
               "'8:20', "
               "'10:20',"
               "1000,"
               "10,"
               "'雪花啤酒') ");
    query.exec("insert into detectResults "
               "values(4, "
               "'20160902',"
               "'8:20', "
               "'10:20',"
               "1000,"
               "10,"
               "'雪花啤酒' )");


    return true;
}
//! [0]

#endif
