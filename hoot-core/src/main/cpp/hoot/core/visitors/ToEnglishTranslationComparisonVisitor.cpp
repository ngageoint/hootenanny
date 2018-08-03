
#include "ToEnglishTranslationComparisonVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QProcess>
#include <QTextCodec>
#include <QLocale>
#include <QDataStream>

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, ToEnglishTranslationComparisonVisitor)

ToEnglishTranslationComparisonVisitor::ToEnglishTranslationComparisonVisitor()
{
  _translationClient.reset(new QTcpSocket());
  _translationClient->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
  //TODO: configure
  _translationClient->connectToHost("localhost", 5674);
  const bool connected = _translationClient->waitForConnected();
  LOG_VARD(connected);
}

ToEnglishTranslationComparisonVisitor::~ToEnglishTranslationComparisonVisitor()
{
  //keeps complaining about already being disconnected...
//  _translationClient->disconnectFromHost();
//  const bool disconnected = _translationClient->waitForDisconnected();
//  LOG_VARD(disconnected);
}

void ToEnglishTranslationComparisonVisitor::setConfiguration(const Settings& /*conf*/)
{
}

void ToEnglishTranslationComparisonVisitor::visit(const boost::shared_ptr<Element>& e)
{
  //TODO: be more lenient on what a name is
  _translateName(e->getTags(), "name:en", "name");
  _translateName(e->getTags(), "alt_name:en", "alt_name");
}

void ToEnglishTranslationComparisonVisitor::_translateName(Tags& tags,
                                                           const QString preTranslatedNameKey,
                                                           const QString toTranslateNameKey)
{
  if ((tags.contains(toTranslateNameKey) && tags.contains(preTranslatedNameKey)))
  {
    QString name = tags.get(toTranslateNameKey).trimmed();
    LOG_VARD(name);
    const QString preTranslatedName = tags.get(preTranslatedNameKey).trimmed();;
    LOG_VARD(preTranslatedName);
    //TODO: add english common word lookup
    if (!name.isEmpty() && name != preTranslatedName)
    {
      QByteArray returnData;
      QString translatedName;
      //TODO: make this non-blocking
      if (_translationClient->state() == QAbstractSocket::ConnectedState)
      {
        //joshua expects a newline at the end
        _translationClient->write(QString(name + "\n").toUtf8());
        const bool bytesWereWritten = _translationClient->waitForBytesWritten();
        LOG_VART(bytesWereWritten);
        LOG_VART(_translationClient->bytesAvailable());
        const bool readyRead = _translationClient->waitForReadyRead();
        LOG_VART(readyRead);
        while (_translationClient->bytesAvailable())
        {
          returnData.append(_translationClient->readAll());
        }
        //106 = UTF-8
        translatedName = QTextCodec::codecForMib(106)->toUnicode(returnData).trimmed();
      }
      else
      {
        throw HootException("Language ranslation client has no active connection to server.");
      }
      LOG_VARD(translatedName);

      //not sure if locale is needed yet
      //QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));
      //QLocale::setDefault(QLocale(QLocale::German));
      //const int nameComparison = translatedName.localeAwareCompare(name);
      const int nameComparison = translatedName.compare(name, Qt::CaseInsensitive);
      LOG_VARD(nameComparison);
      if (nameComparison != 0)
      {
        tags.appendValue("hoot:translated:" + toTranslateNameKey + ":en", translatedName);
        const double similarityScore = _levDist.score(preTranslatedName, translatedName);
        LOG_VARD(similarityScore);
        tags.appendValue(
          "hoot:translated:similarity:score:" + toTranslateNameKey + ":en",
          QString::number(similarityScore));
      }
    }
  }
}

}
