
#include "TikaLanguageDetector.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QProcess>
#include <QFileInfo>

namespace hoot
{

HOOT_FACTORY_REGISTER(LanguageDetector, TikaLanguageDetector)

TikaLanguageDetector::TikaLanguageDetector()
{
}

void TikaLanguageDetector::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  //TODO: most of this checking can be eliminated with a real build process for this feature
  //integration

  const QString rootDir = "conf/core/language-translation/tika";

  const QString appClassPath = rootDir + "/TikaLanguageDetector.class";
  QFileInfo appInfo(appClassPath);
  if (!appInfo.exists())
  {
    throw HootException("Expected Tika language detector class at: " + appClassPath);
  }

  const QString jarPath = rootDir + "/" + opts.getLanguageTranslationTikaLibrary();
  QFileInfo jarInfo(jarPath);
  if (!jarInfo.exists())
  {
    throw HootException("Expected Tika library at: " + jarPath);
  }

  _classPath = rootDir + ":" + jarPath;
}

QString TikaLanguageDetector::detect(const QString text)
{
  QProcess process;
  const QString cmd = QString("java -cp ") + _classPath + " TikaLanguageDetector \"" + text + "\"";
  LOG_VART(cmd);
  process.start(cmd);
  process.waitForFinished(5000);

  const QString error = process.readAllStandardError();
  if (!error.trimmed().isEmpty())
  {
    LOG_WARN("Unable to detect language for: " << text << "; error: " << error);
    return "";
  }

  return process.readAllStandardOutput().trimmed();
}

}
