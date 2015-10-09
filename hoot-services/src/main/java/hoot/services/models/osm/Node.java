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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.models.osm;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.NamedNodeMap;

import com.mysema.query.sql.RelationalPathBase;
import com.mysema.query.sql.SQLExpressions;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.types.path.BooleanPath;
import com.mysema.query.types.path.NumberPath;
import com.mysema.query.types.path.SimplePath;

import hoot.services.db.DbUtils;
import hoot.services.db.DbUtils.EntityChangeType;
import hoot.services.db2.CurrentNodes;
import hoot.services.db2.QCurrentWayNodes;

import hoot.services.geo.BoundingBox;
import hoot.services.geo.GeoUtils;
import hoot.services.geo.QuadTileCalculator;

/**
 * Represents the model for an OSM node
 */
public class Node extends Element
{
  private static final Logger log = LoggerFactory.getLogger(Node.class);

  public Node(final Long mapId, Connection dbConnection)
  {
    super(dbConnection);
    elementType = ElementType.Node;

    try
    {
    	setMapId(mapId);
    }
    catch(Exception ex)
    {

    }
    this.record = new CurrentNodes();
  }

  public Node(final Long mapId, Connection dbConnection, final CurrentNodes record)
  {
    super(dbConnection);

    elementType = ElementType.Node;

    CurrentNodes nodeRecord = new CurrentNodes();
    nodeRecord.setChangesetId(record.getChangesetId());
    nodeRecord.setId(record.getId());
    nodeRecord.setLatitude(record.getLatitude());
    nodeRecord.setLongitude(record.getLongitude());
    nodeRecord.setTile(record.getTile());
    nodeRecord.setTimestamp(record.getTimestamp());
    nodeRecord.setVersion(record.getVersion());
    nodeRecord.setVisible(record.getVisible());
    nodeRecord.setTags(record.getTags());
    this.record = nodeRecord;

    try
    {
    	setMapId(mapId);
    }
    catch(Exception ex)
    {

    }
  }

  /**
   * Returns the bounds of this element
   *
   * Any change to a node, including deletion, adds the node's new location to
   * the bbox.
   *
   * @return a bounding box
   * @throws Exception
   *           if the element has invalid coordinates
   */
  public BoundingBox getBounds() throws Exception
  {
    CurrentNodes nodeRecord = null;
    if (record != null)
    {
      nodeRecord = (CurrentNodes) record;
    }
    else
    {
      nodeRecord = new SQLQuery(conn, DbUtils.getConfiguration(getMapId())).from(currentNodes)
          .where(currentNodes.id.eq(getId()))
          .singleResult(currentNodes);

    }
    return new BoundingBox(nodeRecord.getLongitude(), nodeRecord.getLatitude(), 
      nodeRecord.getLongitude(), nodeRecord.getLatitude());
  }

  /**
   * Returns the nodes specified in the collection of nodes IDs
   *
   * @param mapId
   *          ID of the map the nodes belong to
   * @param nodeIds
   *          a collection of node IDs
   * @param dbConn
   *          JDBC Connection
   * @return a collection of node records
   */
  public static List<CurrentNodes> getNodes(final long mapId, final Set<Long> nodeIds,
      Connection dbConn)
  {
  	//This seems redundant when compared to Element::getElementRecords
  	
    if (nodeIds.size() > 0)
    {
      return new SQLQuery(dbConn, DbUtils.getConfiguration(mapId)).from(currentNodes)
          .where(currentNodes.id.in(nodeIds)).list(currentNodes);
    }
    else
    {
      return new ArrayList<CurrentNodes>();
    }
  }

  /**
   * Returns the IDs of all ways which own this node
   *
   * The ordering of returned records by ID and the use of TreeSet to keep them
   * sorted is only for error reporting readability purposes only.
   *
   * @return a sorted list of way IDs
   * @throws DataAccessException
   * @throws Exception
   */
  private Set<Long> getOwningWayIds() throws Exception
  {
    QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;

    return 
    	new TreeSet<Long>(new SQLQuery(conn, DbUtils.getConfiguration(getMapId()))
    	  .from(currentWayNodes)
        .where(currentWayNodes.nodeId.eq(getId()))
        .orderBy(currentWayNodes.nodeId.asc()).list(currentWayNodes.nodeId));
  }

  /**
   * Populates the element model object based on osm diff data
   *
   * @param xml
   *          XML data to construct the element from
   * @throws Exception
   */
  public void fromXml(final org.w3c.dom.Node xml) throws Exception
  {
    log.debug("Parsing node...");

    NamedNodeMap xmlAttributes = xml.getAttributes();

    assert (record != null);
    CurrentNodes nodeRecord = (CurrentNodes) record;

    // set these props at the very beginning, b/c they will be needed regardless of whether 
    // following checks fail
    nodeRecord.setChangesetId(parseChangesetId(xmlAttributes));
    nodeRecord.setVersion(parseVersion(xmlAttributes));

    final Set<Long> owningWayIds = getOwningWayIds();
    if (entityChangeType.equals(EntityChangeType.DELETE) && owningWayIds.size() > 0)
    {
      throw new Exception("Node to be deleted with ID " + getId() + " is still used by way(s): "
          + Arrays.toString(owningWayIds.toArray()));
    }
    final Set<Long> owningRelationIds = getOwningRelationIds();
    if (entityChangeType.equals(EntityChangeType.DELETE) && owningRelationIds.size() > 0)
    {
      throw new Exception("Node to be deleted with ID " + getId() + " is still used by "
          + "relation(s): " + Arrays.toString(owningRelationIds.toArray()));
    }

    double latitude = -91;
    double longitude = -181;
    if (!entityChangeType.equals(EntityChangeType.DELETE))
    {
      nodeRecord.setTimestamp(parseTimestamp(xmlAttributes));
      nodeRecord.setVisible(true);
      // Lat/lon are required here on a delete request as well, b/c it keeps
      // from having to do a round
      // trip to the db to get the node lat/long before it is deleted, so that can
      // be used to update
      // the changeset bounds (rails port does it this way).
      latitude = Double.parseDouble(xmlAttributes.getNamedItem("lat").getNodeValue());
      longitude = Double.parseDouble(xmlAttributes.getNamedItem("lon").getNodeValue());
      if (!GeoUtils.coordsInWorld(latitude, longitude))
      {
        throw new Exception("Coordinates for node with ID: " + getId()
            + " not within world boundary.");
      }
    }
    // Unlike OSM, we're not requiring lat/lon to be specified in the request for a delete...b/c 
    // it seems unnecessary to me. However, doing so would prevent the extra query made here.
    
    //maybe this query for the existing lat/lon could be done later in batch?
    else
    {
      final CurrentNodes existingRecord = (CurrentNodes) new SQLQuery(conn,
          DbUtils.getConfiguration(getMapId())).from(getElementTable())
          .where(getElementIdField().eq(getId()))
          .singleResult(getElementTable());

      // existence of record has already been checked
      
      // inefficient
      assert (existingRecord != null);
      latitude = existingRecord.getLatitude();
      longitude = existingRecord.getLongitude();
    }
    // If the node is being deleted, we still need to make sure that the coords
    // passed in match
    // what's on the server, since we'll be relying on them to compute the
    // changeset bounds.
    nodeRecord.setLatitude(latitude);
    nodeRecord.setLongitude(longitude);
    // no point in updating the tile if we're not deleting
    if (!entityChangeType.equals(EntityChangeType.DELETE))
    {
      nodeRecord.setTile(QuadTileCalculator.tileForPoint(latitude, longitude));
    }

    if (!entityChangeType.equals(EntityChangeType.DELETE))
    {
      final java.util.Map<String, String> tags = parseTags(xml);
      nodeRecord.setTags(tags);
    }

    setRecord(nodeRecord);
  }

  /**
   * Returns an XML representation of the element; does not add tags
   *
   * @param parentXml
   *          XML node this element should be attached under
   * @param modifyingUserId
   *          ID of the user which created this element
   * @param modifyingUserDisplayName
   *          user display name of the user which created this element
   ** @param multiLayerUniqueElementIds
   *          if true, IDs are prepended with <map id>_<first letter of the
   *          element type>_; this setting activated is not compatible with
   *          standard OSM clients (specific to Hootenanny iD)
   * @param addChildren
   *          ignored by Node
   * @return an XML node
   * @throws Exception
   */
  public org.w3c.dom.Element toXml(final org.w3c.dom.Element parentXml, final long modifyingUserId,
      final String modifyingUserDisplayName, final boolean multiLayerUniqueElementIds,
      final boolean addChildren) throws Exception
      {
    org.w3c.dom.Element element = super.toXml(parentXml, modifyingUserId, modifyingUserDisplayName,
        multiLayerUniqueElementIds, addChildren);
    CurrentNodes nodeRecord = (CurrentNodes) record;
    if (nodeRecord.getVisible())
    {
      element.setAttribute("lat", String.valueOf(nodeRecord.getLatitude()));
      element.setAttribute("lon", String.valueOf(nodeRecord.getLongitude()));
    }

    org.w3c.dom.Element elementWithTags = addTagsXml(element);
    if (elementWithTags == null)
    {
      return element;
    }
    else
    {
      return elementWithTags;
    }
      }

  /**
   * Returns the generated table identifier for this element
   *
   */
  public RelationalPathBase<?> getElementTable()
  {
    return currentNodes;
  }

  /**
   * Returns the generated ID field for this element
   *
   * @return a table field
   */
  public NumberPath<Long> getElementIdField()
  {
    return currentNodes.id;
  }

  /**
   * Returns the generated visibility field for this element
   *
   * @return a table field
   */
  public BooleanPath getElementVisibilityField()
  {
    return currentNodes.visible;
  }

  /**
   * Returns the generated version field for this element
   *
   * @return a table field
   */
  public NumberPath<Long> getElementVersionField()
  {
    return currentNodes.version;
  }

  /**
   * Returns the generated changeset ID field for this element
   *
   * @return a table field
   */
  public NumberPath<Long> getChangesetIdField()
  {
    return currentNodes.changesetId;
  }


  /**
   * Returns the generated tag field for this element
   *
   * @return a table field
   */
  public SimplePath<Object> getTagField()
  {
    return currentNodes.tags;
  }

  /**
   * OSM related element type (e.g. way nodes for ways, relation members for
   * relations)
   *
   * @return a list of element types
   */
  public List<ElementType> getRelatedElementTypes()
  {
    return null;
  }

  /**
   * Returns the generated table identifier for records related to this element
   *
   * @return a table
   */
  public RelationalPathBase<?> getRelatedRecordTable()
  {
    return null;
  }

  /**
   * Returns the table field in the related record table that can be joined with
   * the parent element record table
   *
   * @return a table field
   */
  public NumberPath<Long> getRelatedRecordJoinField()
  {
    return null;
  }

  /**
   * Returns the table field in the related record table containing the map ID
   *
   * @return a table field
   */
  public NumberPath<Long> getRelatedRecordMapIdField()
  {
    return null;
  }

  /**
   * Inserts a new node into the services database
   *
   * @param changesetId
   *          corresponding changeset ID for the node to be inserted
   * @param mapId
   *          corresponding map ID for the node to be inserted
   * @param latitude
   *          latitude coordinate for the node to be inserted
   * @param longitude
   *          longitude coordinate for the node to be inserted
   * @param tags
   *          element tags
   * @param conn
   *          JDBC Connection
   * @return ID of the newly created node
   * @throws Exception
   */
  public static long insertNew(final long changesetId, final long mapId, final double latitude,
    final double longitude, final java.util.Map<String, String> tags, Connection conn)
    throws Exception
  {
    long nextNodeId = 
    	new SQLQuery(conn, DbUtils.getConfiguration(mapId)).uniqueResult(SQLExpressions
        .nextval(Long.class, "current_nodes_id_seq"));
    insertNew(nextNodeId, changesetId, mapId, latitude, longitude, tags, conn);
    return nextNodeId;
   }

  /**
   * Inserts a new node into the services database with the specified ID; useful
   * for testing
   *
   * @param nodeId
   *          ID to assign to the new node
   * @param changesetId
   *          corresponding changeset ID for the node to be inserted
   * @param mapId
   *          corresponding map ID for the node to be inserted
   * @param latitude
   *          latitude coordinate for the node to be inserted
   * @param longitude
   *          longitude coordinate for the node to be inserted
   * @param tags
   *          element tags
   * @param conn
   *          JDBC Connection
   * @throws Exception
   */
  public static void insertNew(final long nodeId, final long changesetId, final long mapId,
      final double latitude, final double longitude, final java.util.Map<String, String> tags,
      Connection conn) throws Exception
      {
    // querydsl does not support hstore so using jdbc

    String strKv = "";
    if (tags != null)
    {
      Iterator it = tags.entrySet().iterator();
      while (it.hasNext())
      {
        Map.Entry pairs = (Map.Entry) it.next();
        String key = "\"" + pairs.getKey() + "\"";
        String val = "\"" + pairs.getValue() + "\"";
        if (strKv.length() > 0)
        {
          strKv += ",";
        }

        strKv += key + "=>" + val;
      }
    }
    String strTags = "'";
    strTags += strKv;
    strTags += "'";

    String POSTGRESQL_DRIVER = "org.postgresql.Driver";
    Statement stmt = null;
    try
    {
      Class.forName(POSTGRESQL_DRIVER);

      stmt = conn.createStatement();

      String sql = "INSERT INTO current_nodes_" + mapId + "(\n"
          + "            id, latitude, longitude, changeset_id,  visible, \"timestamp\", tile, version, tags)\n"
          + " VALUES(" + nodeId + "," + latitude + ","
          + longitude + "," + changesetId + "," + "true" + ","
          + "CURRENT_TIMESTAMP" + "," + QuadTileCalculator.tileForPoint(latitude, longitude) + ","
          + "1" + "," + strTags +

          ")";
      stmt.executeUpdate(sql);

    }
    catch (Exception e)
    {
      throw new Exception("Error inserting node.");
    }

    finally
    {
      // finally block used to close resources
      try
      {
        if (stmt != null)
          stmt.close();
      }
      catch (SQLException se2)
      {

      }// nothing we can do

    }// end try

  }

}
