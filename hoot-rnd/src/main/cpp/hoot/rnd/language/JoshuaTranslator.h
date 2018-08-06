
#ifndef JOSHUA_TRANSLATOR_H
#define JOSHUA_TRANSLATOR_H

// hoot
#include <hoot/core/language/ToEnglishTranslator.h>

// Qt
#include <QTcpSocket>

namespace hoot
{

/**
 *
 *
 * //TODO: move this to separate Joshua translator later
 * @article{post2015joshua,
    Author = {Post, Matt and Cao, Yuan and Kumar, Gaurav},
    Journal = {The Prague Bulletin of Mathematical Linguistics},
    Title = {Joshua 6: A phrase-based and hierarchical statistical machine translation system},
    Year = {2015}
  }
 */
class JoshuaTranslator : public ToEnglishTranslator
{

public:

  JoshuaTranslator();
  ~JoshuaTranslator();

  virtual QString translate(const QString toTranslate) const;
  virtual void setSourceLanguage(const QString langCode);

private:

  QString _sourceLang;
};

}

#endif // JOSHUA_TRANSLATOR_H
