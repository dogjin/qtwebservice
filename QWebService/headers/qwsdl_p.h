/****************************************************************************
**
** Copyright (C) 2011 Tomasz Siekierda
** All rights reserved.
** Contact: Tomasz Siekierda (sierdzio@gmail.com)
**
** This file is part of the QWebService library, QtNetwork Module.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWSDL_P_H
#define QWSDL_P_H

#include <QtCore/QXmlStreamReader>
#include <QtCore/qfile.h>
#include <QtCore/qmap.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qdatetime.h>
#include "qwebservicemethod.h"
#include "qwsdl.h"

class QWsdlPrivate
{
    Q_DECLARE_PUBLIC(QWsdl)

public:
    QWsdlPrivate() {}
    QWsdlPrivate(QWsdl *q) : q_ptr(q) {}
    QWsdl *q_ptr;

    void init();
    bool parse();
    void prepareFile();
    void prepareMethods();
    void readDefinitions();
    void readTypes();
    void readTypeSchemaElement();
    void readPorts();
    void readMessages();
    void readBindings();
    void readService();
    void readDocumentation();
    QString convertReplyToUtf(const QString &textToConvert);
    bool enterErrorState(const QString &errMessage = QString());

    bool errorState;
    bool replyReceived;
    QUrl m_hostUrl;
    QString errorMessage;
    QString wsdlFilePath;
    QString m_webServiceName;
    QString m_targetNamespace;
    QXmlStreamReader xmlReader;

    QStringList *workMethodList;
    // Param if one, QList if many.
    QMap<int, QMap<QString, QVariant> > *workMethodParameters;
    QMap<QString, QWebServiceMethod *> *methodsMap;
};

#endif // QWSDL_P_H