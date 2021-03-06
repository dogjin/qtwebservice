/****************************************************************************
**
** Copyright (C) 2011 Tomasz Siekierda
** All rights reserved.
** Contact: Tomasz Siekierda (sierdzio@gmail.com)
**
** This file is part of the QWebService library.
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.txt included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/

#include "../headers/qwebservicemethod_p.h"

/*!
    \class QWebServiceMethod
    \brief Extends QWebMethod with some generic constructors and synchronous message sending.

    Subclass of QWebMethod, contains some generic methods for sending messages
    and various additional convenience constructors. It can save some lines of
    code, when compared with QWebMethod, at the expense of producing
    less readable code.

    Can be used both synchronously (through static invokeMethod() method),
    or asynchronously (indicates, when reply is ready by emitting
    a replyReady() signal).
  */

/* !
    Constructs web method object with \a parent.
    Requires specifying other parameters later.
    If you use that constructor, you can probably go on
    and use the base QWebMethod class instead of this one.
  */
//QWebServiceMethod::QWebServiceMethod(QObject *parent) :
//    QWebMethod(*new QWebServiceMethodPrivate, Soap12, Post, parent)
//{
//}

/*!
    A constructor that takes in \a protocol information,
    \a httpMethod to use, and \a parent
    to satisfy QObject requirements. All other data has to
    be set later using setter methods.

    \sa setParameters(), invokeMethod()
  */
QWebServiceMethod::QWebServiceMethod(Protocol protocol, HttpMethod httpMethod,
                                     QObject *parent) :
    QWebMethod(*new QWebServiceMethodPrivate, protocol, httpMethod, parent)
{
}

/*!
    Constructs the message using \a hostUrl (QUrl), \a methodName, \a parent,
    \a protocol (which defaults to soap12),
    and \a method (which defaults to POST).
    Requires params to be specified later.

    \sa setParameters(), invokeMethod()
  */
QWebServiceMethod::QWebServiceMethod(const QUrl &hostUrl, const QString &methodName,
                                     Protocol protocol, HttpMethod method,
                                     QObject *parent) :
    QWebMethod(*new QWebServiceMethodPrivate, protocol, method, parent)
{
    Q_D(QWebServiceMethod);
    d->m_hostUrl = hostUrl;
    d->m_methodName = methodName;
    setProtocol(protocol);
    setHttpMethod(method);
}

/*!
    Constructs the message using \a host (QString), \a methodName, \a parent,
    \a protocol (which defaults to soap12),
    and \a httpMethod (which defaults to POST).
    Requires params to be specified later.

    \sa setParameters(), invokeMethod()
  */
QWebServiceMethod::QWebServiceMethod(const QString &host, const QString &methodName,
                                     Protocol protocol, HttpMethod httpMethod,
                                     QObject *parent) :
    QWebMethod(*new QWebServiceMethodPrivate, protocol, httpMethod, parent)
{
    Q_D(QWebServiceMethod);
    d->m_methodName = methodName;
    setProtocol(protocol);
    setHttpMethod(httpMethod);
    d->m_hostUrl.setUrl(host);
}

/*!
    Constructs the message using \a host, \a methodName, \a targetNamespace,
    \a parent, \a protocol (which defaults to soap12),
    and \a httpMethod (which defaults to POST).
    This constructor also takes message parameters (\a params).
    Does not require specifying any more information, but you still need to
    manually send the message using sendMessage() (without any arguments,
    or else - if you want to change ones specified here).

    \sa invokeMethod()
  */
QWebServiceMethod::QWebServiceMethod(const QString &host, const QString &methodName,
                                     const QString &targetNamespace,
                                     const QMap<QString, QVariant> &params,
                                     Protocol protocol, HttpMethod httpMethod,
                                     QObject *parent) :
    QWebMethod(*new QWebServiceMethodPrivate, protocol, httpMethod, parent)
{
    Q_D(QWebServiceMethod);
    d->m_methodName = methodName;
    d->m_targetNamespace = targetNamespace;
    d->parameters = params;
    setProtocol(protocol);
    setHttpMethod(httpMethod);
    d->m_hostUrl.setUrl(host);
}

/*!
    \internal

    Constructor needed for private headers implementation.
  */
QWebServiceMethod::QWebServiceMethod(QWebServiceMethodPrivate &d,
                                     Protocol protocol, HttpMethod httpMethod,
                                     QObject *parent) :
    QWebMethod(d, protocol, httpMethod, parent)
{
}

/*!
    \overload invokeMethod()

    Convenience method - sends the method asynchronously using parameters
    specified in \a params. With QWebMethod, you would have to specify parameters
    by setParameters() first, this method can save one line of code.
    This method returns immediately. When reply is ready, replyReady() signal is
    being emitted. Presence of the reply can also be checked by isReplyReady()
    method.

    Returns true on success.
  */
bool QWebServiceMethod::invokeMethod(const QMap<QString, QVariant> &params)
{
    setParameters(params);    
    return invokeMethod();
}

/*!
    \overload invokeMethod()

    Static synchronous method. Sends the message synchronously, using \a url, \a msgName,
    \a tNamespace, \a params and \a parent.
    Protocol can optionally be specified by \a protocol (default is SOAP 1.2),
    as well as HTTP \a method (default is POST).

    Returns with web service reply, once it is received. This is a blocking method.
  */
QByteArray QWebServiceMethod::invokeMethod(const QUrl &url,
                                          const QString &methodName,
                                          const QString &targetNamespace,
                                          const QMap<QString, QVariant> &params,
                                          Protocol protocol, HttpMethod httpMethod,
                                          QObject *parent)
{
    QWebServiceMethod qsm(url.toString(), methodName, targetNamespace, params,
                          protocol, httpMethod, parent);

    qsm.invokeMethod();
    // TODO: ADD ERROR HANDLING!
    forever {
        if (qsm.isReplyReady()) {
            return qsm.d_func()->reply;
        } else {
            qApp->processEvents();
        }
    }
}
