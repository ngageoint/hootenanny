
#ifndef HOOT_SERVICES_TRANSLATOR_CLIENT_H
#define HOOT_SERVICES_TRANSLATOR_CLIENT_H

// hoot
#include <hoot/rnd/language/translators/ToEnglishTranslator.h>

// Qt
//#include <QByteArray>
#include <QObject>

namespace hoot
{

/**
 * Translates text from a single source language to English using translation/detection
 * technologies integrated into the Hootenanny web services.
 */
class HootServicesTranslatorClient : public QObject, public ToEnglishTranslator
{
  Q_OBJECT

public:

  HootServicesTranslatorClient(QObject* parent = 0);
  ~HootServicesTranslatorClient();

  virtual QStringList getSourceLanguages() const { return _sourceLangs; }
  virtual void setSourceLanguages(const QStringList langCodes);
  virtual void translate(const QString textToTranslate);
  virtual QString getTranslatedText() const { return _translatedText; }
  virtual bool detectionMade() const { return _detectionMade; }

  virtual void setConfiguration(const Settings& conf);

signals:

  void translationComplete();

private slots:

  void _readyRead();
  void _error(QAbstractSocket::SocketError error);

private:

  QStringList _sourceLangs;
  QString _translator;
  QStringList _detectors;
  QString _url;
  bool _detectedLangOverrides;
  bool _performExhaustiveSearch;

  //QByteArray _returnedData;
  QString _translatedText;
  bool _detectionMade;
};

}

#endif // HOOT_SERVICES_TRANSLATOR_CLIENT_H
