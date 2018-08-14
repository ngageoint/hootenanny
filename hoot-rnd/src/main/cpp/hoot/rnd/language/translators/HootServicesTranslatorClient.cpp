
#include "HootServicesTranslatorClient.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
//#include <QTextCodec>

namespace hoot
{

HootServicesTranslatorClient::HootServicesTranslatorClient(QObject* parent) :
QObject(parent),
_detectedLangOverrides(false),
_performExhaustiveSearch(false)
{
  connect(this, SIGNAL(translationComplete()), parent, SLOT(translationComplete()));
}

HootServicesTranslatorClient::~HootServicesTranslatorClient()
{
  //TODO: close client connections?
}

void HootServicesTranslatorClient::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _detectedLangOverrides =
    opts.getLanguageTranslationDetectedLanguageOverridesSpecifiedSourceLanguages();
  _performExhaustiveSearch = opts.getLanguageTranslationPerformExhaustiveSearchWithNoDetection();
  _translator = opts.getLanguageTranslationHootServicesTranslator();
  _detectors = opts.getLanguageTranslationHootServicesDetectors();
  _url = opts.getLanguageTranslationHootServicesEndpoint();
}

void HootServicesTranslatorClient::setSourceLanguages(const QStringList langCodes)
{    
  if (langCodes.contains("detect", Qt::CaseInsensitive))
  {
    if (langCodes.size() != 1)
    {
      throw HootException(
        "When specifying 'detect' in source languages, no other languages may be specified.");
    }
  }
  //TODO: get support langs from web service and check here

  _sourceLangs = langCodes;
}

//void HootServicesTranslator::translate(const QString sourceLangCode, const QString textToTranslate)
//{
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
//}

void HootServicesTranslatorClient::translate(const QString textToTranslate)
{
  if (_sourceLangs.size() > 1)
  {
    throw HootException("Cannot determine source language.");
  }

  LOG_DEBUG(
    "Translating to English with specified source languages: " <<
     QStringList::join(",", _sourceLangs) << "; text: " << textToTranslate);

  _detectionMade = false;
  //TODO: make translation call
  //translate(_sourceLangs.at(0), textToTranslate);
}

void HootServicesTranslatorClient::_readyRead()
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

  //TODO: process translation response data
}

void HootServicesTranslatorClient::_error(QAbstractSocket::SocketError error)
{
  LOG_ERROR("error: " << error);
}

}
