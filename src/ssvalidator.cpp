#include "ssvalidator.h"

const QStringList SSValidator::supportedMethod = QStringList() << "table" << "rc4" << "aes-128-cfb" << "aes-192-cfb" << "aes-256-cfb" << "bf-cfb" << "camellia-128-cfb" << "camellia-192-cfb" << "camellia-256-cfb" << "cast5-cfb" << "des-cfb" << "idea-cfb" << "rc2-cfb" << "seed-cfb";

SSValidator::SSValidator()
{}

bool SSValidator::validate(QString input)
{
    //must begin with ss:// to distinguish from random base64 encoded strings
    if (input.startsWith("ss://")) {
        input.remove(0, 5);
        QString decode(QByteArray::fromBase64(input.toLatin1()));
        QStringList decList = decode.split(':');
        if (decList.size() < 3) {
            return false;
        }

        //Validate Method
        QString method = decList.first();
        if (!validateMethod(method)) {
            return false;
        }

        //Validate Port
        QString port = decList.last();
        if (!validatePort(port)) {
            return false;
        }

        //Validate whether server and password exist
        QStringList pwdServer = decList.at(1).split('@');
        if (pwdServer.size() < 2) {
            return false;
        }

        //it seems acceptable now
        return true;
    }
    else {
        return false;
    }
}

bool SSValidator::validatePort(const QString &port)
{
    bool ok;
    int portNum = port.toInt(&ok);
    if (portNum > 1 && portNum < 65535 && ok) {
        return true;
    }
    else
        return false;
}

bool SSValidator::validateMethod(const QString &method)
{
    return supportedMethod.contains(method, Qt::CaseInsensitive);
}
