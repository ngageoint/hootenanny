
#ifndef JOSHUA_TRANSLATOR_H
#define JOSHUA_TRANSLATOR_H

// hoot
#include <hoot/core/language/ToEnglishTranslator.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QTcpSocket>
#include <QByteArray>

namespace hoot
{

/**
 *
 *
 * @article{post2015joshua,
    Author = {Post, Matt and Cao, Yuan and Kumar, Gaurav},
    Journal = {The Prague Bulletin of Mathematical Linguistics},
    Title = {Joshua 6: A phrase-based and hierarchical statistical machine translation system},
    Year = {2015}
  }
 */
class JoshuaTranslator : public QObject, public ToEnglishTranslator, public Configurable
{
  Q_OBJECT

public:

  typedef struct ServiceMapping
  {
    public:

      QString langCode;
      QString langPackDir;
      QString host;
      int port;

      QString toString() const
      { return langCode + ";" + langPackDir + ";" + host + ";" + QString::number(port); }
  } ServiceMapping;

  JoshuaTranslator(QObject* parent = 0);

  virtual void setSourceLanguage(const QString langCode);
  virtual void translate(const QString textToTranslate);
  virtual QString getTranslatedText() const { return _translatedText; }

  virtual void setConfiguration(const Settings& conf);

signals:

  void translationComplete();

private slots:

  void _readyRead();
  void _error(QAbstractSocket::SocketError error);

private:

  boost::shared_ptr<QTcpSocket> _client;
  QByteArray _returnedData;
  QString _translatedText;
  QMap<QString, ServiceMapping> _serviceMappings;

  void _readServiceMappings(const QString serviceMappingsFile);
};

}

#endif // JOSHUA_TRANSLATOR_H
