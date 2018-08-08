
#ifndef OPEN_NLP_LANGUAGE_DETECTOR_H
#define OPEN_NLP_LANGUAGE_DETECTOR_H

// hoot
#include <hoot/rnd/language/LanguageDetector.h>

namespace hoot
{

/**
 *
 */
class OpenNlpLanguageDetector : public LanguageDetector
{

public:

  static std::string className() { return "hoot::OpenNlpLanguageDetector"; }

  OpenNlpLanguageDetector();

  virtual QString detect(const QString text);

  virtual void setConfiguration(const Settings& conf);

private:

  QString _modelFile;
  QString _classPath;
};

}

#endif // OPEN_NLP_LANGUAGE_DETECTOR_H
