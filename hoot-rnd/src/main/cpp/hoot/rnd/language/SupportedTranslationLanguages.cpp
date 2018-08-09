
#include "SupportedTranslationLanguages.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QFile>

namespace hoot
{

SupportedTranslationLanguages::SupportedTranslationLanguages(const QString configPath)
{
  _readConfig(configPath);
}

SupportedTranslationLanguages::~SupportedTranslationLanguages()
{
  if (_configFile.isOpen())
  {
    _configFile.close();
  }
}

void SupportedTranslationLanguages::_readConfig(const QString configPath)
{
  LOG_VART(configPath);
  _configFile.setFileName(configPath);
  if (!_configFile.open(QIODevice::ReadOnly))
  {
    throw HootException(QObject::tr("Error opening %1 for writing.").arg(_configFile.fileName()));
  }
  while (!_configFile.atEnd())
  {
    const QString line = QString::fromUtf8(_configFile.readLine().constData()).trimmed();
    if (!line.trimmed().isEmpty() && !line.startsWith("#"))
    {
      const QStringList lineParts = line.split("\t");
      if (!lineParts.size() == 4)
      {
        throw HootException("Invalid supported languages config entry: " + line);
      }

      _iso6392To1[lineParts[1]] = lineParts[0];
      _iso6391ToCountry[lineParts[0]] = lineParts[2];
      _iso6391ToDetectable[lineParts[0]] = lineParts[2].toLower() == "yes" ? true : false;
    }
  }
  _configFile.close();
}

bool SupportedTranslationLanguages::isSupportedLanguage(const QString iso6391Code) const
{
  return _iso6391ToCountry.contains(iso6391Code);
}

QString SupportedTranslationLanguages::getIso6391Code(const QString iso639Code) const
{
  if (_iso6391ToCountry.contains(iso639Code))
  {
    return iso639Code;
  }
  //TODO: don't think we need this check
  if (_iso6392To1.contains(iso639Code))
  {
    return _iso6392To1[iso639Code];
  }
  return "";
}

QString SupportedTranslationLanguages::getCountryName(const QString iso6391Code) const
{
  return _iso6391ToCountry[iso6391Code];
}

bool SupportedTranslationLanguages::isDetectableLanguage(const QString iso6391Code) const
{
  return _iso6391ToDetectable[iso6391Code];
}

}
