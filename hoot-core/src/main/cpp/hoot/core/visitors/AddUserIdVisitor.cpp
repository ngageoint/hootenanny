#include "AddUserIdVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/MetadataTags.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, AddUserIdVisitor)

AddUserIdVisitor::AddUserIdVisitor()
{
}

void AddUserIdVisitor::visit(const ElementPtr& pElement)
{
  Tags& tags = pElement->getTags();

  if (tags.getNonDebugCount() > 0)
  {
    if (!pElement->getUser().isEmpty())
    {
      tags[MetadataTags::HootUserName()] = pElement->getUser();
      tags[MetadataTags::HootUserId()] = QString::number(pElement->getUid());
    }
  }
}

}
