
#ifndef TIKA_LANGUAGE_DETECTOR_H
#define TIKA_LANGUAGE_DETECTOR_H

// hoot
#include <hoot/rnd/language/LanguageDetector.h>

namespace hoot
{

/**
 *
 */
class TikaLanguageDetector : public LanguageDetector
{

public:

  static std::string className() { return "hoot::TikaLanguageDetector"; }

  TikaLanguageDetector();

  virtual QString detect(const QString text);

  virtual void setConfiguration(const Settings& conf);

private:

  //This will eventually go away.
  QString _classPath;
};

}

#endif // TIKA_LANGUAGE_DETECTOR_H
