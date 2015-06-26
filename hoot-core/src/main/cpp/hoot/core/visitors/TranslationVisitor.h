#ifndef TRANSLATIONVISITOR_H
#define TRANSLATIONVISITOR_H

// Boost
#include <boost/shared_ptr.hpp>

// Hoot
#include <hoot/core/OsmMapConsumer.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/io/ScriptToOgrTranslator.h>
#include <hoot/core/io/ScriptTranslator.h>

// Qt
#include <QString>

namespace hoot
{

class TranslationVisitor : public ElementVisitor, public OsmMapConsumer
{
public:

  TranslationVisitor(ScriptTranslator& t, bool toOgr, OsmMap* map);
  virtual ~TranslationVisitor() {}

  virtual void setOsmMap(OsmMap* map) { _map = map; }

  virtual void setOsmMap(const OsmMap*) { assert(false); }

  virtual void visit(const ConstElementPtr& ce);

private:
  ScriptTranslator& _t;
  ScriptToOgrTranslator* _togr;
  QString _circularErrorKey;
  QString _accuracyKey;
  OsmMap* _map;
  bool _toOgr;
};

}

#endif // TRANSLATIONVISITOR_H
