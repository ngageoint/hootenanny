
#include "OpenNlpLanguageDetector.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

// Qt
#include <QProcess>
#include <QFileInfo>

namespace hoot
{

HOOT_FACTORY_REGISTER(LanguageDetector, OpenNlpLanguageDetector)

OpenNlpLanguageDetector::OpenNlpLanguageDetector()
{
}

void OpenNlpLanguageDetector::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  //TODO: most of this checking can be eliminated with a real build process for this feature
  //integration

  const QString rootDir = "conf/core/language-translation/open-nlp";

  _modelFile = opts.getLanguageTranslationOpennlpModel();
  const QString modelPath = rootDir + "/" + _modelFile;
  QFileInfo modelInfo(modelPath);
  if (!modelInfo.exists())
  {
    throw HootException("Expected OpenNLP language detection model at: " + modelPath);
  }

  const QString appClassPath = rootDir + "/OpenNlpLanguageDetector.class";
  QFileInfo appInfo(appClassPath);
  if (!appInfo.exists())
  {
    throw HootException("Expected OpenNLP language detector class at: " + appClassPath);
  }

  const QString jarPath = rootDir + "/" + opts.getLanguageTranslationOpennlpLibrary();
  QFileInfo jarInfo(jarPath);
  if (!jarInfo.exists())
  {
    throw HootException("Expected OpenNLP library at: " + jarPath);
  }

  _classPath = rootDir + ":" + jarPath;
}

QString OpenNlpLanguageDetector::detect(const QString text)
{
  QProcess process;
  const QString cmd =
    QString("java -cp  ") + _modelFile + " " + _classPath + " OpenNlpLanguageDetector \"" +
    text + "\"";
  process.start(cmd);
  process.waitForFinished(5000);

  const QString error = process.readAllStandardError();
  if (!error.trimmed().isEmpty())
  {
    LOG_WARN("Unable to detect language for: " << text);
    return "";
  }

  return process.readAllStandardOutput();
}

}
