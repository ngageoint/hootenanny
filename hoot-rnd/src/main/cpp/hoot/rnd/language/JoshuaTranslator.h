
#ifndef JOSHUA_TRANSLATOR_H
#define JOSHUA_TRANSLATOR_H

// hoot
#include <hoot/core/language/ToEnglishTranslator.h>

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
class JoshuaTranslator : public QObject, public ToEnglishTranslator
{
  Q_OBJECT

public:

  JoshuaTranslator(const QString host, const int port, QObject* parent = 0);

  virtual void setSourceLanguage(const QString langCode);
  virtual void translate(const QString textToTranslate);
  virtual QString getTranslatedText() const { return _translatedText; }

signals:

  void translationComplete();

private slots:

  void _readyRead();
  void _error(QAbstractSocket::SocketError error);

private:

  boost::shared_ptr<QTcpSocket> _client;
  QByteArray _returnedData;
  QString _translatedText;
};

}

#endif // JOSHUA_TRANSLATOR_H
