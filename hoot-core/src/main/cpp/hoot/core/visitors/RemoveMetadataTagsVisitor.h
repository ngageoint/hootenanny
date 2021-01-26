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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef REMOVE_METADATA_TAGS_VISITOR_H
#define REMOVE_METADATA_TAGS_VISITOR_H

// hoot
#include <hoot/core/visitors/RemoveTagsVisitor.h>

// Qt
#include <QRegExp>

namespace hoot
{

/**
 * Removes metadata tags from elements, as defined by the hoot schema
 */
class RemoveMetadataTagsVisitor : public RemoveTagsVisitor
{
public:

  static QString className() { return "hoot::RemoveMetadataTagsVisitor"; }

  RemoveMetadataTagsVisitor() = default;
  virtual ~RemoveMetadataTagsVisitor()  = default;

  /**
   * @see ElementVisitor
   */
  virtual void visit(const ElementPtr& e) override;

  virtual QString getDescription() const { return "Removes metadata tags"; }

  virtual QString getInitStatusMessage() const
  { return "Removing metadata tags..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return
      "Removed " + QString::number(_numTagsRemoved) + " metadata tags from " +
      QString::number(_numAffected) + " different elements";
  }

  virtual QString getName() const { return className(); }

  virtual QString getClassName() const override { return className(); }
};

}

#endif // REMOVE_METADATA_TAGS_VISITOR_H
