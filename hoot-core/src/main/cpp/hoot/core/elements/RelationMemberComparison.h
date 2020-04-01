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
#ifndef RELATION_MEMBER_COMPARISON_H
#define RELATION_MEMBER_COMPARISON_H

// Hoot
#include <hoot/core/elements/ElementComparison.h>

namespace hoot
{

/**
 * This is an abstraction for dealing with relation member element comparisons inside of
 * collections.
 *
 * @see ElementComparison
 */
class RelationMemberComparison : public ElementComparison
{
public:

  RelationMemberComparison();
  RelationMemberComparison(ElementPtr element, const OsmMap& sourceMap, const QString& role,
                           const bool ignoreElementId = false);

  virtual bool operator==(const RelationMemberComparison& memberComp) const;

  virtual QString toString() const;

  QString getRole() const { return _role; }

private:

  QString _role;
};

inline uint qHash(const RelationMemberComparison& memberComp)
{
  return qHash(memberComp.getRole() + " " + memberComp.getElement()->nonIdHash());
}

}

#endif // RELATION_MEMBER_COMPARISON_H
