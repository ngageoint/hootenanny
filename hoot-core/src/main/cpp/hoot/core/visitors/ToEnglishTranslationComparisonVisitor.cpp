
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

namespace hoot
{

HOOT_FACTORY_REGISTER(ConstElementVisitor, ToEnglishTranslationComparisonVisitor)

ToEnglishTranslationComparisonVisitor::ToEnglishTranslationComparisonVisitor()
{
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
    //TODO: these replacements are to appease the bash command; if we switch to a network call
    //with qt, shouldn't be needed (or maybe there's a better way to encode these inside the
    //bash command)
    QString name = tags.get(toTranslateNameKey).trimmed();
    name = name.replace("&amp;", "and");
    name = name.replace("&", "and");
    name = name.replace("'", "");
    name = name.replace("(", "");
    name = name.replace(")", "");
    LOG_VARD(name);
    QString preTranslatedName = tags.get(preTranslatedNameKey).trimmed();
    preTranslatedName = preTranslatedName.replace("&amp;", "and");
    preTranslatedName = preTranslatedName.replace("&", "and");
    preTranslatedName = preTranslatedName.replace("'", "");
    preTranslatedName = preTranslatedName.replace("(", "");
    preTranslatedName = preTranslatedName.replace(")", "");
    LOG_VARD(preTranslatedName);
    //TODO: add english common word lookup
    if (!name.isEmpty() && name != preTranslatedName)
    {
      //TODO: replace with net call
      QProcess process;
      const QString cmd = "bash -c \"echo " + name + " | nc localhost 5674\"";
      LOG_VARD(cmd);
      process.start(cmd);
      process.waitForFinished(-1);

      const QString error = process.readAllStandardError();
      if (!error.trimmed().isEmpty())
      {
        throw HootException("Unable to translate name: " + name + "; error: " + error);
      }

      const QString translatedName =
        QTextCodec::codecForMib(106)->toUnicode(process.readAllStandardOutput()).trimmed();
      LOG_VARD(translatedName);
      //not sure if this locale stuff is needed yet
      //QLocale::setDefault(QLocale(QLocale::German, QLocale::Germany));
      //QLocale::setDefault(QLocale(QLocale::German));
      //const int nameComparison = translatedName.localeAwareCompare(name);
      const int nameComparison = translatedName.compare(name, Qt::CaseInsensitive);
      LOG_VARD(nameComparison);
      if (nameComparison != 0)
      {
        tags.appendValue("joshua:translated:" + toTranslateNameKey, translatedName);
        const double similarityScore = _levDist.score(preTranslatedName, translatedName);
        LOG_VARD(similarityScore);
        tags.appendValue(
          "joshua:similarityscore:" + toTranslateNameKey, QString::number(similarityScore));
      }
    }
  }
}

}
