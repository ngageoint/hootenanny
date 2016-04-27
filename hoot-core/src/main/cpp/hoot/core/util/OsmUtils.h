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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSM_UTILS_H
#define OSM_UTILS_H

// Boost Includes
#include <boost/shared_ptr.hpp>

// GEOS

#include <geos/geom/Coordinate.h>
using namespace geos::geom;

// Qt
#include <QString>

namespace hoot
{
  using namespace boost;

  class OsmMap;
  class Node;
  class Status;

/**
  Utilities for use with the OSM data model

  Some of this functionality might already exist in other classes.  If so, it can be removed.  Over
  time, we might find better homes for some of these methods.
  */
class OsmUtils
{
  public:

    /**
      Prints a collection of nodes for debugging purposes (couldn't find a way to do this with the
      debug logger).

      @param nodeCollectionName
      @param nodes
      @todo Use a template here to make this method generic for other OSM element types.
      @todo Move to Element?
      */
    static void printNodes(const QString nodeCollectionName,
                           const QList<shared_ptr<const Node> >& nodes);

    /**
      Retrieves a collection of node ID's for a collection of nodes

      @param nodes a collection of nodes
      @return a collection of node ID's
      @todo Use a template to make this method generic for other OSM element types.
      @todo Move to Element?
      */
    static const QList<long> nodesToNodeIds(const QList<shared_ptr<const Node> >& nodes);

    /**
      Retrieves a collection of nodes given a collection of node ID's

      @param nodeIds a collection of node ID's
      @param map the map owning the nodes with the given ID's
      @return a collection of nodes
      @todo Use a template here to make this method generic for other OSM element types.
      @todo Move to Element?
      */
    static QList<shared_ptr<const Node> > nodeIdsToNodes(const QList<long>& nodeIds,
                                                         shared_ptr<const OsmMap> map);

    /**
      Converts a OSM node to a coordinate

      @param node the node to convert
      @returns a coordinate
      */
    static Coordinate nodeToCoord(shared_ptr<const Node> node);

    /**
      Converts OSM nodes to a coordinates

      @param nodes the nodes to convert
      @returns coordinates
      */
    static QList<Coordinate> nodesToCoords(const QList<shared_ptr<const Node> >& nodes);

    /**
      Converts a coordinate to an OSM node

      @param coord the coordinate to convert
      @param map the map owning the node to be created
      @returns a node
      */
    static shared_ptr<const Node> coordToNode(const Coordinate& coord, shared_ptr<const OsmMap> map);

    /**
      Converts coordinates to OSM nodes

      @param coords the coordinates to convert
      @param map the map owning the nodes to be created
      @returns nodes
      */
    static QList<shared_ptr<const Node> > coordsToNodes(const QList<Coordinate>& coords,
                                                        shared_ptr<const OsmMap> map);

    /**
      Loads an OSM map into an OsmMap object

      @param map the object to load the map into
      @param path the file path to load the map from
      @param useFileId if true, uses the element ID's in the map file; otherwise, generates new
      element ID's
      @param defaultStatus the hoot status to assign to all elements
      @todo This was copied from BaseCommand.  Point all references to BaseCommand::loadMap to this
      instead.
      */
    static void loadMap(shared_ptr<OsmMap> map, QString path, bool useFileId, Status defaultStatus);

    /**
      Saves an OSM map to an OsmMap object

      @param map the map object to save
      @param path the file path to save the map to
      @todo This was copied from BaseCommand.  Point all references to BaseCommand::saveMap to this
      instead.
      */
    static void saveMap(shared_ptr<const OsmMap> map, QString path);

    /**
      Converts an unsigned int timestamp (time from epoch) to a QString (utc zulu)

      @param timestamp unsigned int time encoding in seconds from the epoch (1970-01-01 00:00:00)
      */
    static QString toTimeString(unsigned int timestamp);

    /**
      Converts a utc zulu timestamp to time since the epoch in seconds.

      @param timestamp in utc zulu string to be convered to seconds from the epoch (1970-01-01 00:00:00)
      */
    static unsigned int fromTimeString(QString timestamp);

    /**
     * Returns a time string for the current time
     *
     * @return
     */
    static QString currentTimeAsString();
};

}

#endif // OSM_UTILS_H
