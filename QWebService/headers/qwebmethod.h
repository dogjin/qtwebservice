#ifndef QWebMethod_H
#define QWebMethod_H

#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "QWebService_global.h"

class QWEBSERVICESHARED_EXPORT QWebMethod : public QObject
{
    Q_OBJECT
    Q_ENUMS(Protocol)
    Q_FLAGS(Protocols)

public:
    enum Protocol
    {
        http    = 0x01,
        soap10  = 0x02,
        soap12  = 0x04,
        soap    = 0x06,
        json    = 0x08
    };
    Q_DECLARE_FLAGS(Protocols, Protocol)

    explicit QWebMethod(QObject *parent = 0);
    QWebMethod(QUrl hostUrl, QString messageName, QObject *parent = 0);
    QWebMethod(QString host, QString messageName, QObject *parent = 0);
    QWebMethod(QString host, QString messageName, QMap<QString, QVariant> params,
                 QObject *parent = 0);
    ~QWebMethod();

    void setParams(QMap<QString, QVariant> params);
    void setReturnValue(QMap<QString, QVariant> returnValue);
    void setTargetNamespace(QString tNamespace);
    void setProtocol(Protocol protocol);
    bool sendMessage();
    bool sendMessage(QMap<QString, QVariant> params);
    QVariant static sendMessage(QObject *parent, QUrl url, QString _messageName,
                                QMap<QString, QVariant> params);
    QVariant replyRead();
    QString getMessageName();
    QStringList getParameterNames() const;
    QStringList getReturnValueName() const;
    QMap<QString, QVariant> getParameterNamesTypes() const;
    QMap<QString, QVariant> getReturnValueNameType() const;
    QString getTargetNamespace();

signals:
    void replyReady(QVariant rply);

public slots:
    void replyFinished(QNetworkReply *reply);

private:
    void init();
    void prepareRequestData();
    QString convertReplyToUtf(QString textToConvert);

    bool replyReceived;
    Protocol protocol;
    QUrl hostUrl;
    QString host;
    QString messageName;
    QString targetNamespace;
    QVariant reply;
    QMap<QString, QVariant> parameters;
    QMap<QString, QVariant> returnValue;
    QNetworkAccessManager *manager;
    QNetworkReply *networkReply;
    QByteArray data;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QWebMethod::Protocols)

#endif // QWebMethod_H