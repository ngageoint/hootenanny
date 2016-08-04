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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef APIDBREADER_H
#define APIDBREADER_H

// hoot
#include <hoot/core/util/Configurable.h>

#include <boost/shared_ptr.hpp>

// tgs
#include <tgs/BigContainers/BigMap.h>

namespace hoot
{

class ApiDbReader
{
public:

  static void addTagsToElement(shared_ptr<Element> element)
  {
    bool ok;
    Tags& tags = element->getTags();

    if (tags.contains("hoot:status"))
    {
      QString statusStr = tags.get("hoot:status");
      bool ok;
      const int statusInt = statusStr.toInt(&ok);
      Status status = static_cast<Status::Type>(statusInt);
      if (ok && status.getEnum() >= Status::Invalid && status.getEnum() <= Status::Conflated)
      {
        element->setStatus(status);
      }
      else
      {
        LOG_WARN("Invalid status: " + statusStr + " for element with ID: " +
                 QString::number(element->getId()));
      }
      tags.remove("hoot:status");
    }

    if (tags.contains("type"))
    {
      Relation* r = dynamic_cast<Relation*>(element.get());
      if (r)
      {
        r->setType(tags["type"]);
        tags.remove("type");
      }
    }

    if (tags.contains("error:circular"))
    {
      element->setCircularError(tags.get("error:circular").toDouble(&ok));
      if (!ok)
      {
        try
        {
          double tv = tags.getLength("error:circular").value();
          element->setCircularError(tv);
          ok = true;
          /*LOG_DEBUG(
            "Set circular error from error:circular tag to " << tv << " for element with ID: " <<
            element->getId());*/
        }
        catch (const HootException& e)
        {
          ok = false;
        }

        if (!ok)
        {
          LOG_WARN("Error parsing error:circular.");
        }
      }
      tags.remove("error:circular");
    }
    else if (tags.contains("accuracy"))
    {
      element->setCircularError(tags.get("accuracy").toDouble(&ok));

      if (!ok)
      {
        try
        {
          double tv = tags.getLength("accuracy").value();
          element->setCircularError(tv);
          ok = true;
          /*LOG_DEBUG(
            "Set circular error from accuracy tag to " << tv << " for element with ID: " <<
            element->getId());*/
        }
        catch (const HootException& e)
        {
          ok = false;
        }

        if (!ok)
        {
          LOG_WARN("Error parsing accuracy.");
        }
      }
      tags.remove("accuracy");
    }
  }
};

}

#endif // APIDBREADER_H
