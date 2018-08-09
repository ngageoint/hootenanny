
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
  connect(this, SIGNAL(translationComplete()), parent, SLOT(translationComplete()));
}

JoshuaTranslator::~JoshuaTranslator()
{
  if (_serviceMappingsFile.isOpen())
  {
    _serviceMappingsFile.close();
  }
  //_clients
  for (QMap<QString, boost::shared_ptr<QTcpSocket>>::const_iterator itr = _clients.begin();
       itr != _clients.end(); ++itr)
  {
    boost::shared_ptr<QTcpSocket> client = *itr;
    if (client->state() == QAbstractSocket::ConnectedState)
    {
      client->disconnect();
    }
    if (client->isOpen())
    {
      client->close();
    }
  }
}

void JoshuaTranslator::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  _readServiceMappings(opts.getLanguageTranslationServiceMappingsFile());
}

void JoshuaTranslator::setSourceLanguages(const QStringList langCodes)
{
  if (langCodes.contains("detect", Qt::CaseInsensitive))
  {
    if (langCodes.size() != 1)
    {
      throw HootException(
        "When specifying 'detect' in source languages, no other languages may be specified.");
    }
  }
  else
  {
    for (int i = 0; i < langCodes.size(); i++)
    {
      const QString langCode = langCodes.at(i).toLower();
      if (!_serviceMappings.contains(langCode))
      {
        throw HootException("Unsupported source translation language: " + langCode);
      }

      boost::shared_ptr<QTcpSocket> client(new QTcpSocket(this));
      client->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
      connect(client.get(), SIGNAL(readyRead()), this, SLOT(_readyRead()));
      connect(client.get(), SIGNAL(error(QAbstractSocket::SocketError)), this,
              SLOT(_error(QAbstractSocket::SocketError)));

      ServiceMapping serviceMapping = _serviceMappings[langCode];
      LOG_VART(serviceMapping);
      client->connectToHost(serviceMapping.host, serviceMapping.port);
      if (!client->waitForConnected(5000))
      {
        throw HootException(
          "Unable to connect to the language translation service for language: " +
          serviceMapping.langCode + " at " + serviceMapping.host + ":" +
          QString::number(serviceMapping.port));
      }

      _clients[langCode] = client;
    }
  }
  _sourceLangs = langCodes;
}

bool JoshuaTranslator::_inDetectMode() const
{
  return _sourceLangs.contains("detect", Qt::CaseInsensitive);
}

void JoshuaTranslator::_readServiceMappings(const QString serviceMappingsFile)
{
  LOG_VART(serviceMappingsFile);

  _serviceMappingsFile.setFileName(serviceMappingsFile);
  if (!_serviceMappingsFile.open(QIODevice::ReadOnly))
  {
    throw HootException(
      QObject::tr("Error opening %1 for writing.").arg(_serviceMappingsFile.fileName()));
  }
  while (!_serviceMappingsFile.atEnd())
  {
    const QString line = QString::fromUtf8(_serviceMappingsFile.readLine().constData()).trimmed();
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
  _serviceMappingsFile.close();
}

void JoshuaTranslator::translate(const QString sourceLangCode, const QString textToTranslate)
{
  if (!_clients.contains(sourceLangCode))
  {
    throw HootException(
      "No language translation service is available for language: " + sourceLangCode);
  }
  _activeClient = _clients[sourceLangCode];
  if (_activeClient->state() != QAbstractSocket::ConnectedState)
  {
    throw HootException("Language translation client has no active connection to service.");
  }

  LOG_DEBUG(
    "Translating from: " << _supportedLangs->getCountryName(sourceLangCode) <<
    " to English; text: " << textToTranslate);
  _returnedData.clear();
  //joshua expects a newline at the end of what's passed in
  _activeClient->write(QString(textToTranslate + "\n").toUtf8());
  //this makes the call blocking
  const bool readyRead = _activeClient->waitForReadyRead(5000);
  if (!readyRead)
  {
    throw HootException("Language translation server request timed out.");
  }
}

void JoshuaTranslator::translate(const QString textToTranslate)
{
  if (_sourceLangs.size() > 1)
  {
    throw HootException("Cannot determine source language.");
  }

  translate(_sourceLangs.at(0), textToTranslate);
}

void JoshuaTranslator::_readyRead()
{
  if (_activeClient->bytesAvailable())
  {
    _returnedData.append(_activeClient->readAll());
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
