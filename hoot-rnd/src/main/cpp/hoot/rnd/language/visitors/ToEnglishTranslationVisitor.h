
#ifndef TO_ENGLISH_TRANSLATION_VISITOR_H
#define TO_ENGLISH_TRANSLATION_VISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/algorithms/StringDistance.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

// Qt
#include <QTcpSocket>

namespace hoot
{

/**
 *
 */
class ToEnglishTranslationVisitor : public ElementOsmMapVisitor, public Configurable
{

public:

  static std::string className() { return "hoot::ToEnglishTranslationVisitor"; }

  ToEnglishTranslationVisitor();
  ~ToEnglishTranslationVisitor();

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual void setConfiguration(const Settings& conf);

  virtual QString getDescription() const
  { return "TODO"; }

private:


};

}

#endif // TO_ENGLISH_TRANSLATION_VISITOR_H
