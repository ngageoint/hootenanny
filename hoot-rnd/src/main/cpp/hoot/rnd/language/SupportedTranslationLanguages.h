
#ifndef SUPPORTED_TRANSLATION_LANGUAGES_H
#define SUPPORTED_TRANSLATION_LANGUAGES_H

// Qt
#include <QString>
#include <QMap>
#include <QFile>

namespace hoot
{

/**
 *
 */
class SupportedTranslationLanguages
{

public:

  SupportedTranslationLanguages(const QString configPath);
  ~SupportedTranslationLanguages();

  bool isSupportedLanguage(const QString iso6391Code) const;

  QString getIso6391Code(const QString iso639Code) const;

  QString getCountryName(const QString iso6391Code) const;

  bool isDetectableLanguage(const QString iso6391Code) const;

private:

  QMap<QString, QString> _iso6392To1;
  QMap<QString, QString> _iso6391ToCountry;
  QMap<QString, bool> _iso6391ToDetectable;
  QFile _configFile;

  void _readConfig(const QString configPath);
};

}

#endif // SUPPORTED_TRANSLATION_LANGUAGES_H
