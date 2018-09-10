/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef HOOT_SERVICES_TRANSLATOR_CLIENT_H
#define HOOT_SERVICES_TRANSLATOR_CLIENT_H

// hoot
#include <hoot/rnd/language/translators/ToEnglishTranslator.h>
#include <hoot/rnd/language/translators/TranslationInfoProvider.h>

// Qt
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace hoot
{

/**
 * Translates text from a single source language to English using translation/detection
 * technologies integrated into the Hootenanny web services.
 *
 * TODO: integrate with HootNetworkRequest (#2606)
 */
class HootServicesTranslatorClient : public QObject, public ToEnglishTranslator
{
  Q_OBJECT

public:

  static std::string className() { return "hoot::HootServicesTranslatorClient"; }

  HootServicesTranslatorClient();

  virtual QStringList getSourceLanguages() const { return _sourceLangs; }
  virtual void setSourceLanguages(const QStringList langCodes);
  virtual void translate(const QString textToTranslate);
  virtual QString getTranslatedText() const { return _translatedText; }
  virtual bool detectionMade() const { return _detectionMade; }

  virtual void setConfiguration(const Settings& conf);

signals:

  /**
   * Fired when the translation service completes a translation
   */
  void translationComplete();

  /**
   * Fired when an error is received from the translation service
   *
   * @param textSent text sent for translation
   * @param message error message
   */
  void translationError(QString textSent, QString message);

protected:

  //the source languages to tell the translator which languages to translate to English from; at
  //least one must be populated, or use "detect" to force the service to try to detect the language
  //(can be expensive)
  QStringList _sourceLangs;

  QString _translatedText;
  bool _detectionMade;
  QString _detectedLang;
  QString _detectorUsed;
  bool _detectedLangAvailableForTranslation;

  boost::shared_ptr<TranslationInfoProvider> _infoClient;

private:

  friend class HootServicesTranslatorClientTest;

  //translator implementation to use server side
  QString _translator;
  //detector implementations to use server side; if left empty, the server will try to use as many
  //of them as necessary to detect the language
  QStringList _detectors;
  QString _translationUrl;
  //if true and a detected lang doesn't match a specified lang, then the detected lang takes
  //priority
  bool _detectedLangOverrides;
  //if true and no lang can be detected, an exhaustive search for the correct translation is made
  //against all available translatable languages (expensive)
  bool _performExhaustiveSearch;

  boost::shared_ptr<QNetworkAccessManager> _client;

  /**
   * Verifies that every language specified for this translator is supported by the server
   *
   * @param type type of check; either "detectable" or "translatable"; a failure to verify a
   * detectable language results in a warning, while a failure to verify a translatable language
   * results in an error
   */
  void _checkLangsAvailable(const QString type);

  void _validateAvailableLangs(boost::shared_ptr<boost::property_tree::ptree> replyObj,
                               const QString type);

  boost::shared_ptr<QNetworkRequest> _getTranslateRequest(const QString text,
                                                          std::stringstream& requestStrStrm);
  void _parseTranslateResponse(boost::shared_ptr<boost::property_tree::ptree> replyObj);
};

}

#endif // HOOT_SERVICES_TRANSLATOR_CLIENT_H
