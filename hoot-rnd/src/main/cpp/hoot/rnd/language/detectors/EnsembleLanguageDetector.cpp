
#include "EnsembleLanguageDetector.h"

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

HOOT_FACTORY_REGISTER(LanguageDetector, EnsembleLanguageDetector)

EnsembleLanguageDetector::EnsembleLanguageDetector() :
_numDetections(0)
{
}

void EnsembleLanguageDetector::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  _detectors = opts.getLanguageTranslationEnsembleDetectors();
  _url = opts.getLanguageTranslationEnsembleServiceEndpoint();
}

QString EnsembleLanguageDetector::detect(const QString text)
{
  LOG_DEBUG("Detecting language for: " << text << "...");

  /*QProcess process;
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

  _numDetections++;
  return process.readAllStandardOutput().trimmed();*/

  /*
   * curl -X POST -H "Content-Type: application/json"
   * -d '{ "apps": ["tika", "opennlp"], "text": "test" }'
   * localhost:8080/hoot-services/language/detect
   */



  return "";
}

}
