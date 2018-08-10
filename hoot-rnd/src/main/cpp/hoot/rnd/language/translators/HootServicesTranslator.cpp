
#include "HootServicesTranslator.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QTextCodec>
#include <QFile>

namespace hoot
{

HootServicesTranslator::HootServicesTranslator(QObject* parent) :
QObject(parent)
{
  connect(this, SIGNAL(translationComplete()), parent, SLOT(translationComplete()));
}

HootServicesTranslator::~HootServicesTranslator()
{
//  for (QMap<QString, boost::shared_ptr<QTcpSocket>>::const_iterator itr = _clients.begin();
//       itr != _clients.end(); ++itr)
//  {
//    boost::shared_ptr<QTcpSocket> client = *itr;
//    if (client->state() == QAbstractSocket::ConnectedState)
//    {
//      client->disconnect();
//    }
//    if (client->isOpen())
//    {
//      client->close();
//    }
//  }
}

void HootServicesTranslator::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _supportedLangs.reset(
    new SupportedTranslationLanguages(opts.getLanguageTranslationSupportedLanguagesFile()));
}

void HootServicesTranslator::setSourceLanguages(const QStringList langCodes)
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
      if (!_supportedLangs->isSupportedLanguage(langCode))
      {
        throw HootException("Specified unsupported source translation language: " + langCode);
      }

//      boost::shared_ptr<QTcpSocket> client(new QTcpSocket(this));
//      client->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
//      connect(client.get(), SIGNAL(readyRead()), this, SLOT(_readyRead()));
//      connect(client.get(), SIGNAL(error(QAbstractSocket::SocketError)), this,
//              SLOT(_error(QAbstractSocket::SocketError)));

//      ServiceMapping serviceMapping = _serviceMappings[langCode];
//      LOG_VART(serviceMapping);
//      client->connectToHost(serviceMapping.host, serviceMapping.port);
//      if (!client->waitForConnected(5000))
//      {
//        throw HootException(
//          "Unable to connect to the language translation service for language: " +
//          serviceMapping.langCode + " at " + serviceMapping.host + ":" +
//          QString::number(serviceMapping.port));
//      }

//      _clients[langCode] = client;
    }
  }
  _sourceLangs = langCodes;
}

bool HootServicesTranslator::_inDetectMode() const
{
  return _sourceLangs.contains("detect", Qt::CaseInsensitive);
}

void HootServicesTranslator::translate(const QString sourceLangCode, const QString textToTranslate)
{
//  if (!_clients.contains(sourceLangCode))
//  {
//    throw HootException(
//      "No language translation service is available for language: " + sourceLangCode);
//  }
//  _activeClient = _clients[sourceLangCode];
//  if (_activeClient->state() != QAbstractSocket::ConnectedState)
//  {
//    throw HootException("Language translation client has no active connection to service.");
//  }

//  LOG_DEBUG(
//    "Translating from: " << _supportedLangs->getCountryName(sourceLangCode) <<
//    " to English; text: " << textToTranslate);
//  _returnedData.clear();
//  //joshua expects a newline at the end of what's passed in
//  _activeClient->write(QString(textToTranslate + "\n").toUtf8());
//  //this makes the call blocking
//  const bool readyRead = _activeClient->waitForReadyRead(5000);
//  if (!readyRead)
//  {
//    throw HootException("Language translation server request timed out.");
//  }
}

void HootServicesTranslator::translate(const QString textToTranslate)
{
  if (_sourceLangs.size() > 1)
  {
    throw HootException("Cannot determine source language.");
  }

  translate(_sourceLangs.at(0), textToTranslate);
}

void HootServicesTranslator::_readyRead()
{
//  if (_activeClient->bytesAvailable())
//  {
//    _returnedData.append(_activeClient->readAll());
//  }
//  LOG_VART(_returnedData.size());

//  //server returns a single line of translated text with a newline at the end
//  if (_returnedData.endsWith('\n'))
//  {
//    //106 = UTF-8
//    _translatedText = QTextCodec::codecForMib(106)->toUnicode(_returnedData).trimmed();
//    LOG_VART(_translatedText);
//    emit translationComplete();
//  }
}

void HootServicesTranslator::_error(QAbstractSocket::SocketError error)
{
  LOG_ERROR("error: " << error);
}

}
