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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef CONFLATE_INFO_CACHE_CONSUMER_H
#define CONFLATE_INFO_CACHE_CONSUMER_H

// Hoot
#include <hoot/core/conflate/ConflateInfoCache.h>

namespace hoot
{

/**
 * @brief The ConflateInfoCacheConsumer class is the interface for classes that make use of a
 * ConflateInfoCache
 */
class ConflateInfoCacheConsumer
{
public:

  ConflateInfoCacheConsumer() = default;
  virtual ~ConflateInfoCacheConsumer() = default;

  /**
   * @brief setConflateInfoCache sets a ConflateInfoCache instance on the consumer.
   */
  virtual void setConflateInfoCache(const std::shared_ptr<ConflateInfoCache>& cache) = 0;
};

}

#endif // CONFLATE_INFO_CACHE_CONSUMER_H
