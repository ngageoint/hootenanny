#include <vector>

#include <boost/shared_ptr.hpp>

#include <geos/geom/Geometry.h>

#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/io/ScriptToOgrTranslator.h>
#include <hoot/core/io/ScriptTranslator.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>


#include "TranslationVisitor.h"

namespace hoot
{

TranslationVisitor::TranslationVisitor(ScriptTranslator& t, bool toOgr, OsmMap* map) : _t(t),
  _map(map), _toOgr(toOgr)
{
  _circularErrorKey = "error:circular";
  _accuracyKey = "accuracy";

  if (toOgr)
  {
    _togr = dynamic_cast<ScriptToOgrTranslator*>(&t);
    if (_togr == 0)
    {
      throw HootException("Translating to OGR requires a script that supports to OGR "
        "translations.");
    }
  }
}

void TranslationVisitor::visit(const ConstElementPtr& ce)
{
  // this is a hack to get around the visitor interface. The visitor interface should probably be
  // redesigned into ConstElementVisitor and ElementVisitor.
  ElementPtr e = _map->getElement(ce->getElementId());

  if (e->getTags().getNonDebugCount() > 0)
  {
    if (_toOgr)
    {
      GeometryTypeId gtype = ElementConverter::getGeometryType(e, false);

      vector<Tags> allTags = _togr->translateToOgrTags(e->getTags(), e->getElementType(), gtype);

      if (allTags.size() > 0)
      {
        if (allTags.size() > 1)
        {
          LOG_WARN("More than one feature was returned, only keeping the first feature.");
        }

        e->setTags(allTags[0]);
      }
    }
    else
    {
      _t.translateToOsm(e->getTags(), "");

      if (e->getTags().contains(_circularErrorKey))
      {
        e->setCircularError(e->getTags().getDouble(_circularErrorKey));
        e->getTags().remove(_circularErrorKey);
        e->getTags().remove(_accuracyKey);
      }
      else if (e->getTags().contains(_accuracyKey))
      {
        e->setCircularError(e->getTags().getDouble(_accuracyKey));
        e->getTags().remove(_circularErrorKey);
        e->getTags().remove(_accuracyKey);
      }
    }
  }
}

}
