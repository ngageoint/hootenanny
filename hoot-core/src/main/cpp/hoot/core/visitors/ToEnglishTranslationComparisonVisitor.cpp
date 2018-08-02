
#include "ToEnglishTranslationComparisonVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QProcess>

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
    //TODO: fix encoding
    QString name = tags.get(toTranslateNameKey)/*.toUtf8()*/.trimmed();
    name = name.replace("&amp;", "and");
    name = name.replace("&", "and");
    name = name.replace("'", "");
    name = name.replace("(", "");
    name = name.replace(")", "");
    QString preTranslatedName = tags.get(preTranslatedNameKey)/*.toUtf8()*/.trimmed();
    preTranslatedName = preTranslatedName.replace("&amp;", "and");
    preTranslatedName = preTranslatedName.replace("&", "and");
    preTranslatedName = preTranslatedName.replace("'", "");
    preTranslatedName = preTranslatedName.replace("(", "");
    preTranslatedName = preTranslatedName.replace(")", "");
    //TODO: add english common word lookup
    if (!name.isEmpty() && name != preTranslatedName)
    {
      //TODO: replace with net call
      QProcess process;
      process.start("bash -c \"echo " + name + " | nc localhost 5674");
      process.waitForFinished(-1); // will wait forever until finished

      const QString error = process.readAllStandardError();
      if (!error.trimmed().isEmpty())
      {
        throw HootException("Unable to translate name: " + name + "; error: " + error);
      }
      QString translatedName = process.readAllStandardOutput();
      //translatedName = translatedName.toUtf8();
      if (translatedName.compare(name, Qt::CaseInsensitive) != 0)
      //if (translatedName != name)
      {
        tags.appendValue("joshua:translated:" + toTranslateNameKey, translatedName);
        //LOG_ERROR(translatedName);
        tags.appendValue(
          "joshua:similarityscore:" + toTranslateNameKey,
          QString::number(_levDist.score(preTranslatedName, translatedName)));
      }
    }
  }
}

}
