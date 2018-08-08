
#include "JoshuaTranslator.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QTextCodec>
#include <QFile>

namespace hoot
{

JoshuaTranslator::JoshuaTranslator(QObject* parent) :
QObject(parent)
{
  _client.reset(new QTcpSocket(this));
  _client->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

  connect(_client.get(), SIGNAL(readyRead()), this, SLOT(_readyRead()));
  connect(_client.get(), SIGNAL(error(QAbstractSocket::SocketError)), this,
          SLOT(_error(QAbstractSocket::SocketError)));
}

void JoshuaTranslator::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  _readServiceMappings(opts.getLanguageTranslationServiceMappingsFile());
}

void JoshuaTranslator::setSourceLanguage(const QString langCode)
{
  if (!_serviceMappings.contains(langCode.toLower()))
  {
    throw HootException("Unsupported source translation language: " + langCode);
  }

  ServiceMapping serviceMapping = _serviceMappings[langCode.toLower()];
  LOG_VART(serviceMapping);

  _client->connectToHost(serviceMapping.host, serviceMapping.port);
  if (!_client->waitForConnected(5000))
  {
    throw HootException(
      "Unable to connect to the language translation service for language: " +
      serviceMapping.langCode + " at " + serviceMapping.host + ":" +
      QString::number(serviceMapping.port));
  }
}

void JoshuaTranslator::_readServiceMappings(const QString serviceMappingsFile)
{
  LOG_VART(serviceMappingsFile);
  if (!serviceMappingsFile.trimmed().isEmpty())
  {
    QFile inputFile(serviceMappingsFile);
    if (!inputFile.open(QIODevice::ReadOnly))
    {
      throw HootException(QObject::tr("Error opening %1 for writing.").arg(inputFile.fileName()));
    }
    while (!inputFile.atEnd())
    {
      const QString line = QString::fromUtf8(inputFile.readLine().constData()).trimmed();
      if (!line.trimmed().isEmpty() && !line.startsWith("#"))
      {
        const QStringList lineParts = line.split(";");
        if (!lineParts.size() == 4)
        {
          throw HootException("Invalid language service mappings config entry: " + line);
        }
        ServiceMapping serviceMapping;
        serviceMapping.host = lineParts[2].trimmed().toLower();
        if (serviceMapping.host.isEmpty())
        {
          throw HootException(
            "Invalid language service mappings config host entry: " + lineParts[2]);
        }
        serviceMapping.langCode = lineParts[0].trimmed().toLower();
        if (serviceMapping.langCode.isEmpty())
        {
          throw HootException(
            "Invalid language service mappings config language code entry: " + lineParts[0]);
        }
        serviceMapping.langPackDir = lineParts[1].trimmed();
        //TODO: should the dir name be validated to match the Joshua lang pack dir naming
        //convention?
        if (serviceMapping.langPackDir.isEmpty())
        {
          throw HootException(
            "Invalid language service mappings config language pack directory entry: " +
            lineParts[1]);
        }
        bool ok = false;
        serviceMapping.port = lineParts[3].trimmed().toInt(&ok);
        if (!ok)
        {
          throw HootException(
            "Invalid language service mappings config port entry: " + lineParts[3]);
        }
        _serviceMappings[serviceMapping.langCode] = serviceMapping;
        LOG_VART(serviceMapping);
      }
    }
    inputFile.close();
  }
}

void JoshuaTranslator::translate(const QString textToTranslate)
{
  if (_client->state() != QAbstractSocket::ConnectedState)
  {
    throw HootException("Language translation client has no active connection to service.");
  }

  LOG_VART(textToTranslate);
  _returnedData.clear();
  //joshua expects a newline at the end of what's passed in
  _client->write(QString(textToTranslate + "\n").toUtf8());
  //this makes the call blocking
  const bool readyRead = _client->waitForReadyRead(5000);
  if (!readyRead)
  {
    throw HootException("Language translation server request timed out.");
  }
}

void JoshuaTranslator::_readyRead()
{
  if (_client->bytesAvailable())
  {
    _returnedData.append(_client->readAll());
  }
  LOG_VART(_returnedData.size());

  //server returns a single line of translated text with a newline at the end
  if (_returnedData.endsWith('\n'))
  {
    //106 = UTF-8
    _translatedText = QTextCodec::codecForMib(106)->toUnicode(_returnedData).trimmed();
    LOG_VART(_translatedText);
    emit translationComplete();
  }
}

void JoshuaTranslator::_error(QAbstractSocket::SocketError error)
{
  LOG_ERROR("error: " << error);
}

}
