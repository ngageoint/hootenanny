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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef TAGMERGERFACTORY_H
#define TAGMERGERFACTORY_H

// Standard
#include <memory>

// Qt
#include <QString>
#include <QHash>

namespace hoot
{

class TagMerger;
class Tags;
class ElementType;

/**
 * Factory for creating tag mergers based on the configured tag merging strategy (Singleton)
 */
class TagMergerFactory
{
public:

  static TagMergerFactory& getInstance();

  /**
   * Returns the default TagMerger.
   */
  const TagMerger& getDefault() { return *getDefaultPtr(); }

  std::shared_ptr<TagMerger> getDefaultPtr();

  const TagMerger& getMerger(const QString& name) { return *getMergerPtr(name); }

  std::shared_ptr<TagMerger> getMergerPtr(const QString& name);

  /**
   * A convenience function for merging tags using the default mechanism. Equivalent to:
   * TagMergerFactory::getInstance().getDefault().mergeTags(t1, t2, et);
   */
  static Tags mergeTags(const Tags& t1, const Tags& t2, ElementType et);

  /**
   * Reset all cached information. This is necessary if the default tag config information changes.
   */
  void reset();

  /**
   * Return the list of mergers as a string
   */
  QString toString() const;

private:

  QHash<QString, std::shared_ptr<TagMerger>> _mergers;
  std::shared_ptr<TagMerger> _default;

  /** Default constructor/destructor */
  TagMergerFactory() = default;
  ~TagMergerFactory() = default;
  /** Delete copy constructor and assignment operator */
  TagMergerFactory(const TagMergerFactory&) = delete;
  TagMergerFactory& operator=(const TagMergerFactory&) = delete;
};

}

#endif // TAGMERGERFACTORY_H
