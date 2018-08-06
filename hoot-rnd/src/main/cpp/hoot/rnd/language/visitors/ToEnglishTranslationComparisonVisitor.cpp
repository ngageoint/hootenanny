
#include "ToEnglishTranslationComparisonVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QTextCodec>
#include <QLocale>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, ToEnglishTranslationComparisonVisitor)

ToEnglishTranslationComparisonVisitor::ToEnglishTranslationComparisonVisitor()
{
}

ToEnglishTranslationComparisonVisitor::~ToEnglishTranslationComparisonVisitor()
{
  //keeps complaining about already being disconnected already...
//  _translationClient->disconnectFromHost();
//  const bool disconnected = _translationClient->waitForDisconnected();
//  LOG_VARD(disconnected);
}

void ToEnglishTranslationComparisonVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  _translationScorer.reset(
    Factory::getInstance().constructObject<StringDistance>(
      opts.getLanguageTranslationComparisonScorer()));
  _preTranslatedTagKeys = opts.getLanguageTranslationComparisonPretranslatedTagKeys();
  _toTranslateTagKeys = opts.getLanguageTranslationComparisonToTranslateTagKeys();
  if (_preTranslatedTagKeys.size() != _toTranslateTagKeys.size())
  {
    throw HootException(
      QString("When preforming language translation comparison, the number of pre-translated ") +
      QString("tag keys must match that of the keys of the tags to be translated."));
  }

  if (opts.getLanguageTranslationSourceLanguage().toLower() != "de")
  {
    throw HootException("Unsupported source translation language.");
  }

  _translationClient.reset(new QTcpSocket());
  _translationClient->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
  _translationClient->connectToHost(
    opts.getLanguageTranslationServiceHost(), opts.getLanguageTranslationServicePort());
  if (!_translationClient->waitForConnected())
  {
    throw HootException("Unable to connect to the language translation service.");
  }
}

void ToEnglishTranslationComparisonVisitor::visit(const boost::shared_ptr<Element>& e)
{
  for (int i = 0; i < _preTranslatedTagKeys.size(); i++)
  {
    _translate(e->getTags(), _preTranslatedTagKeys.at(i), _toTranslateTagKeys.at(i));
  }
}

void ToEnglishTranslationComparisonVisitor::_translate(Tags& tags,
                                                       const QString preTranslatedNameKey,
                                                       const QString toTranslateNameKey)
{
  //only care about features that have both the pre-translated tag and the tag we want to compare
  //our translation to it with
  if ((tags.contains(toTranslateNameKey) && tags.contains(preTranslatedNameKey)))
  {
    const QString toTranslateVal = tags.get(toTranslateNameKey).trimmed();
    LOG_VARD(toTranslateVal);
    const QString preTranslatedVal = tags.get(preTranslatedNameKey).trimmed();;
    LOG_VARD(preTranslatedVal);
    //TODO: add english common word lookup
    //If the tag to be translated already matches the previously translated tag, there's no point
    //in translating it.
    //if (!toTranslateValue.isEmpty() && name != preTranslatedVal)
    //{
      QByteArray returnedData;
      QString translatedVal;
      //TODO: make this non-blocking
      if (_translationClient->state() == QAbstractSocket::ConnectedState)
      {
        //joshua expects a newline at the end of what's passed in
        _translationClient->write(QString(toTranslateVal + "\n").toUtf8());
        const bool bytesWereWritten = _translationClient->waitForBytesWritten();
        LOG_VART(bytesWereWritten);
        LOG_VART(_translationClient->bytesAvailable());
        const bool readyRead = _translationClient->waitForReadyRead();
        LOG_VART(readyRead);
        while (_translationClient->bytesAvailable())
        {
          returnedData.append(_translationClient->readAll());
        }
        //106 = UTF-8
        translatedVal = QTextCodec::codecForMib(106)->toUnicode(returnedData).trimmed();
      }
      else
      {
        throw HootException("Language translation client has no active connection to service.");
      }
      LOG_VARD(translatedVal);

      //not sure if locale is needed for the comparison yet
      //QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));
      //QLocale::setDefault(QLocale(QLocale::German));
      //const int nameComparison = translatedName.localeAwareCompare(name);
      const int strComparison = translatedVal.compare(toTranslateVal, Qt::CaseInsensitive);
      LOG_VARD(strComparison);
      //If the translator merely returned the same string we passed in, then no point in recording
      //it.
      //if (strComparison != 0)
      //{
        tags.appendValue("hoot:translated:" + toTranslateNameKey + ":en", translatedVal);
        const double similarityScore = _translationScorer->compare(preTranslatedVal, translatedVal);
        LOG_VARD(similarityScore);
        tags.appendValue(
          "hoot:translated:similarity:score:" + toTranslateNameKey + ":en",
          QString::number(similarityScore));
      //}
    //}
  }
}

}
