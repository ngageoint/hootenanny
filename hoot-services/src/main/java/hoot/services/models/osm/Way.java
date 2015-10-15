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
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.xpath.XPathAPI;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.NodeList;

import com.mysema.query.Tuple;
import com.mysema.query.sql.RelationalPathBase;
import com.mysema.query.sql.SQLExpressions;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.types.path.BooleanPath;
import com.mysema.query.types.path.NumberPath;
import com.mysema.query.types.path.SimplePath;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.db.DbUtils.EntityChangeType;
import hoot.services.db.DbUtils.RecordBatchType;
import hoot.services.db2.CurrentNodes;
import hoot.services.db2.CurrentWayNodes;
import hoot.services.db2.CurrentWays;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.geo.BoundingBox;
import hoot.services.geo.Coordinates;

/**
 * Represents the model for an OSM way
 */
public class Way extends Element
{
  private static final Logger log = LoggerFactory.getLogger(Way.class);
  private int maxRecordBatchSize = -1;
  private List<Long> wayNodeIdsCache = new ArrayList<Long>();

  public List<Long> getWayNodeIdsCache()
  {
    return wayNodeIdsCache;
  }

  private static final QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;

  // temp collection of way node coordinates used to calculate the way's bounds
  private Map<Long, Coordinates> nodeCoordsCollection;

  public Way(final long mapId, Connection dbConn)
  {
    super(dbConn);

    elementType = ElementType.Way;

    this.record = new CurrentWays();
    try
    {
    	setMapId(mapId);
      maxRecordBatchSize = Integer.parseInt(HootProperties.getInstance().getProperty(
          "maxRecordBatchSize", HootProperties.getDefault("maxRecordBatchSize")));
    }
    catch (Exception ex)
    {

    }
  }

  public Way(final long mapId, Connection dbConn, final CurrentWays record)
  {
    super(dbConn);
    elementType = ElementType.Way;

    CurrentWays wayRecord = new CurrentWays();
    wayRecord.setChangesetId(record.getChangesetId());
    wayRecord.setId(record.getId());
    wayRecord.setTimestamp(record.getTimestamp());
    wayRecord.setVersion(record.getVersion());
    wayRecord.setVisible(record.getVisible());
    wayRecord.setTags(record.getTags());
    this.record = wayRecord;
    try
    {
    	setMapId(mapId);
      maxRecordBatchSize = Integer.parseInt(HootProperties.getInstance().getProperty(
          "maxRecordBatchSize", HootProperties.getDefault("maxRecordBatchSize")));
    }
    catch (Exception ex)
    {

    }
  }

  /**
   * Returns all node records for the specified ways from the services database
   *
   * @param mapId
   *          ID of the map owning the ways
   * @param wayIds
   *          a collection of way IDs for which to retrieve node records
   * @param dbConn
   *          JDBC Connection
   * @return a list of node records
   */
  public static List<CurrentNodes> getNodesForWays(final long mapId, final Set<Long> wayIds,
      Connection dbConn)
  {
    if (wayIds.size() > 0)
    {
      return new SQLQuery(dbConn, DbUtils.getConfiguration(mapId)).from(currentWayNodes)
          .join(currentNodes)
          .on(currentWayNodes.nodeId.eq(currentNodes.id))
          .where(currentWayNodes.wayId.in(wayIds))
          .list(currentNodes);
    }
    else
    {
      return new ArrayList<CurrentNodes>();
    }
  }

  /*
   * Returns the nodes associated with this way
   */
  private List<CurrentNodes> getNodes() throws Exception
  {
    return new SQLQuery(conn, DbUtils.getConfiguration(getMapId())).from(currentWayNodes).join(currentNodes)
        .on(currentWayNodes.nodeId.eq(currentNodes.id))
        .where(currentWayNodes.wayId.eq(getId()))
        .orderBy(currentWayNodes.sequenceId.asc()).list(currentNodes);
  }

  /*
   * Returns the IDs of the nodes associated with this way
   *
   * This is a List, rather than a Set, since the same node ID can be used for
   * the first and last node ID in the way nodes sequence for closed polygons.
   */
  private List<Long> getNodeIds() throws Exception
  {
    return new SQLQuery(conn, DbUtils.getConfiguration(getMapId()))
      .from(currentWayNodes)
      .where(currentWayNodes.wayId.eq(getId()))
      .orderBy(currentWayNodes.sequenceId.asc()).list(currentWayNodes.nodeId);
  }

  /*
   * Returns the number of nodes associated with this way
   */
  private long getNodeCount() throws Exception
  {
    return new SQLQuery(conn, DbUtils.getConfiguration(getMapId())).from(currentWayNodes)
        .where(currentWayNodes.wayId.eq(getId())).count();
  }

  /*
   * Adds node refs to the way nodes services database table
   *
   * @param nodeIds a list of node ref IDs; This is a List, rather than a Set,
   * since the same node ID can be used for the first and last node ID in the
   * way nodes sequence for closed polygons.
   *
   * @throws Exception if the number of node refs is larger than the max allowed
   * number of way nodes, if any of the referenced nodes do not exist in the
   * services db, or if any of the referenced nodes are not set to be visible in
   * the services db
   */
  private void addWayNodes(final long mapId, final List<Long> nodeIds) throws Exception
  {
    CurrentWays wayRecord = (CurrentWays) record;
    if (nodeIds == null || nodeIds.size() < 2)
    {
      throw new Exception("Too few nodes specified for way with ID: " + wayRecord.getId());
    }
    final long numExistingNodes = getNodeCount();
    final long maximumWayNodes = Long.parseLong(HootProperties.getInstance().getProperty(
        "maximumWayNodes", HootProperties.getDefault("maximumWayNodes")));
    if ((nodeIds.size() + numExistingNodes) > maximumWayNodes)
    {
      throw new Exception("Too many nodes specified for way with ID: " + wayRecord.getId());
    }
    if (!Element.allElementsExist(getMapId(), ElementType.Node, new HashSet<Long>(nodeIds), conn))
    {
      throw new Exception("Not all nodes exist specified for way with ID: " + wayRecord.getId());
    }
    if (!Element.allElementsVisible(getMapId(), ElementType.Node, new HashSet<Long>(nodeIds), conn))
    {
      throw new Exception("Not all nodes are visible for way with ID: " + wayRecord.getId());
    }

    List<CurrentWayNodes> wayNodeRecords = new ArrayList<CurrentWayNodes>();
    long sequenceCtr = 1;
    for (long nodeId : nodeIds)
    {
      CurrentWayNodes wayNodeRecord = new CurrentWayNodes();
      wayNodeRecord.setNodeId(nodeId);
      wayNodeRecord.setSequenceId(sequenceCtr);
      wayNodeRecord.setWayId(wayRecord.getId());
      wayNodeRecords.add(wayNodeRecord);
      sequenceCtr++;
    }

    DbUtils.batchRecords(mapId, wayNodeRecords, QCurrentWayNodes.currentWayNodes, null,
        RecordBatchType.INSERT, conn, maxRecordBatchSize);
  }
  
  /*
   * First calculates the bounds for all nodes belonging to this way that were
   * referenced explicitly in the changeset upload request. Then calculates the
   * bounds for all the way's nodes not mentioned in the request after
   * retrieving them from the services database. The bounds returned is a sum of
   * the two calculated bounds.
   */
  private BoundingBox getBoundsFromRequestDataAndRemainderFromDatabase() throws Exception
  {
    double minLon = BoundingBox.LON_LIMIT + 1;
    double minLat = BoundingBox.LAT_LIMIT + 1;
    double maxLon = (-1 * BoundingBox.LON_LIMIT) - 1;
    double maxLat = (-1 * BoundingBox.LAT_LIMIT) - 1;

    // assert(relatedRecordIds != null && relatedRecordIds.size() >= 2);

    // nodes that were parsed in the same request referencing this way; either
    // as a create or modify
    Set<Long> modifiedRecordIds = new HashSet<Long>(relatedRecordIds);
    for (long wayNodeId : relatedRecordIds)
    {
      assert (nodeCoordsCollection.containsKey(wayNodeId));
      final double lat = nodeCoordsCollection.get(wayNodeId).lat;
      final double lon = nodeCoordsCollection.get(wayNodeId).lon;
      if (lat < minLat)
      {
        minLat = lat;
      }
      if (lat > maxLat)
      {
        maxLat = lat;
      }
      if (lon < minLon)
      {
        minLon = lon;
      }
      if (lon > maxLon)
      {
        maxLon = lon;
      }

      modifiedRecordIds.remove(wayNodeId);
    }

    // any way nodes not mentioned in the created/modified in the changeset XML
    // represented by
    // the remainder of the IDs in relatedRecordIds, request must now be
    // retrieved from the
    // database
    final List<?> nodeRecords = Element.getElementRecords(getMapId(), ElementType.Node,
        modifiedRecordIds, conn);
    for (Object record : nodeRecords)
    {
      final CurrentNodes nodeRecord = (CurrentNodes) record;
      final double lat = nodeRecord.getLatitude();
      final double lon = nodeRecord.getLongitude();
      if (lat < minLat)
      {
        minLat = lat;
      }
      if (lat > maxLat)
      {
        maxLat = lat;
      }
      if (lon < minLon)
      {
        minLon = lon;
      }
      if (lon > maxLon)
      {
        maxLon = lon;
      }

      assert (modifiedRecordIds.contains(nodeRecord.getId()));
      modifiedRecordIds.remove(nodeRecord.getId());
    }
    assert (modifiedRecordIds.size() == 0);

    return new BoundingBox(minLon, minLat, maxLon, maxLat);
  }

  /**
   * Returns the bounds of this element
   *
   * Any change to a way, including deletion, adds all of the way's nodes to the
   * bbox.
   *
   * @return a bounding box
   * @throws Exception
   */
  public BoundingBox getBounds() throws Exception
  {
    // this is a little kludgy, but...first see if the related record data (way
    // node data) is left
    // over from the XML parsing (clearTempData clears it out). If it is still
    // here, use it
    // because the way nodes will not have been written to the database yet, so
    // use the cached
    // way node IDs and node coordinate info to construct the bounds
    if (relatedRecordIds != null && relatedRecordIds.size() > 0)
    {
      return getBoundsFromRequestDataAndRemainderFromDatabase();
    }
    // If no temp related record data is present (hasn't been cleared out), the
    // way nodes data
    // for this way must be in the services database and up to date, so get it
    // from there.
    else
    {
      return new BoundingBox(new ArrayList<CurrentNodes>(getNodes()));
    }
  }

  /**
   * Populates this element model object based on osm diff data
   *
   * @param xml
   *          xml data to construct the element from
   * @throws Exception
   */
  public void fromXml(final org.w3c.dom.Node xml) throws Exception
  {
    log.debug("Parsing way...");

    NamedNodeMap xmlAttributes = xml.getAttributes();

    assert (record != null);
    CurrentWays wayRecord = (CurrentWays) record;
    wayRecord.setChangesetId(parseChangesetId(xmlAttributes));
    wayRecord.setVersion(parseVersion(xmlAttributes));

    final Set<Long> owningRelationIds = getOwningRelationIds();
    if (entityChangeType.equals(EntityChangeType.DELETE) && owningRelationIds.size() > 0)
    {
      throw new Exception("Way to be deleted with ID " + getId() + " is still used by "
          + "relation(s): " + Arrays.toString(owningRelationIds.toArray()));
    }

    wayRecord.setTimestamp(parseTimestamp(xmlAttributes));
    wayRecord.setVisible(true);

    if (!entityChangeType.equals(EntityChangeType.DELETE))
    {
      final java.util.Map<String, String> tags = parseTags(xml);
      wayRecord.setTags(tags);
    }

    setRecord(wayRecord);

    // if we're deleting the way, all the way nodes will get deleted
    // automatically..and no new
    // ones need to be parsed
    if (!entityChangeType.equals(EntityChangeType.DELETE))
    {
      parseWayNodesXml(xml);
    }
  }

  /**
   * Returns an XML representation of the element returned in a query; does not
   * add tags; assumes way nodes have already been written to the services db
   *
   * @param parent
   *          XML node this element should be attached under
   * @param modifyingUserId
   *          ID of the user which created this element
   * @param modifyingUserDisplayName
   *          user display name of the user which created this element
   * @param multiLayerUniqueElementIds
   *          if true, IDs are prepended with <map id>_<first letter of the
   *          element type>_; this setting activated is not compatible with
   *          standard OSM clients (specific to Hootenanny iD)
   * @param addChildren
   *          if true, element children are added to the element xml
   * @return an XML element
   * @throws Exception
   */
  public org.w3c.dom.Element toXml(final org.w3c.dom.Element parentXml, final long modifyingUserId,
      final String modifyingUserDisplayName, final boolean multiLayerUniqueElementIds,
      final boolean addChildren) throws Exception
  {
    org.w3c.dom.Element element = super.toXml(parentXml, modifyingUserId, modifyingUserDisplayName,
        multiLayerUniqueElementIds, addChildren);
    Document doc = parentXml.getOwnerDocument();

    if (addChildren)
    {
      final List<Long> nodeIds = getNodeIds();
      Set<Long> elementIds = new HashSet<Long>();
      // way nodes are output in sequence order; list should already be sorted
      // by the query
      for (long nodeId : nodeIds)
      {
        org.w3c.dom.Element nodeElement = doc.createElement("nd");
        nodeElement.setAttribute("ref", String.valueOf(nodeId));
        element.appendChild(nodeElement);
        elementIds.add(nodeId);
      }
   
      @SuppressWarnings("unchecked")
      final List<Tuple> elementRecords = 
          (List<Tuple>) Element.getElementRecordsWithUserInfo(getMapId(), ElementType.Node, elementIds, conn);
      for(int i=0; i<elementRecords.size(); i++)
      {
      	final Element nodeFullElement = 
            ElementFactory.getInstance().create(ElementType.Node, elementRecords.get(i), conn, getMapId());
        org.w3c.dom.Element nodeXml = 
        		nodeFullElement.toXml(
          	parentXml,
          	modifyingUserId, 
          	modifyingUserDisplayName,
            false,
            false);
        parentXml.appendChild(nodeXml);
      }
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

  private void validateWayNodesSize(final NodeList wayNodesXml) throws Exception
  {
    if (!entityChangeType.equals(EntityChangeType.DELETE))
    {
      CurrentWays wayRecord = (CurrentWays) record;
      final long maximumWayNodes = Long.parseLong(HootProperties.getInstance().getProperty(
          "maximumWayNodes", HootProperties.getDefault("maximumWayNodes")));
      final long numWayNodes = wayNodesXml.getLength();
      if (numWayNodes < 2)
      {
        throw new Exception("Too few nodes specified for way with ID: " + wayRecord.getId());
      }
      else if (numWayNodes > maximumWayNodes)
      {
        throw new Exception("Too many nodes specified for way with ID: " + wayRecord.getId());
      }
    }
  }

  private long parseWayNode(final org.w3c.dom.Node nodeXml) throws Exception
  {
    final NamedNodeMap nodeXmlAttributes = nodeXml.getAttributes();

    final long parsedNodeId = Long.parseLong(nodeXmlAttributes.getNamedItem("ref").getNodeValue());
    wayNodeIdsCache.add(parsedNodeId);
    long actualNodeId;
    Coordinates nodeCoords = new Coordinates();
    final Map<Long, Element> parsedNodes = parsedElementIdsToElementsByType.get(ElementType.Node);
    // if this is a node created within the same request that is referencing this way, it won't
    // exist in the database, but it will be in the element cache created when parsing the node
    // from the request
    if (parsedNodeId < 0)
    {
      // assert(parsedNodes.containsKey(parsedNodeId));
      if (!parsedNodes.containsKey(parsedNodeId))
      {
        // TODO: add test for this
        throw new Exception("Created way references new node not found in create request with ID: "
            + parsedNodeId);
      }
    }
    // The node is referenced somewhere else in this request, so get its info
    // from the request, not
    // the database b/c the database either won't have it or will have outdated
    // info. Only
    // get info from the request if the node is being created/modified, as if it
    // is being deleted,
    // we can just get the info from the database since its coords won't be
    // changing and might not
    // be in the request (not required).
    if (parsedNodes.containsKey(parsedNodeId)
        && !parsedNodes.get(parsedNodeId).getEntityChangeType().equals(EntityChangeType.DELETE))
    {
      Node node = (Node) parsedElementIdsToElementsByType.get(ElementType.Node).get(parsedNodeId);
      CurrentNodes nodeRecord = (CurrentNodes) node.getRecord();
      actualNodeId = nodeRecord.getId();
      nodeCoords.lat = nodeRecord.getLatitude();
      nodeCoords.lon = nodeRecord.getLongitude();
    }
    // element not referenced in this request, so should already exist in the db
    // and its info up
    // to date
    else
    {
      actualNodeId = parsedNodeId;
      // I don't like having to do this in a loop; see Element::parseVersion for more info
      final CurrentNodes existingNodeRecord = new SQLQuery(conn,
          DbUtils.getConfiguration(getMapId())).from(currentNodes)
          .where(currentNodes.id.eq(new Long(actualNodeId)))
          .singleResult(currentNodes);

      if (existingNodeRecord == null)
      {
        throw new Exception("Node with ID: " + actualNodeId + " does not exist for way.");
      }
      else if (!existingNodeRecord.getVisible())
      {
        throw new Exception("Node with ID: " + actualNodeId + " is not visible for way.");
      }
      nodeCoords.lat = existingNodeRecord.getLatitude();
      nodeCoords.lon = existingNodeRecord.getLongitude();
    }
    assert (actualNodeId > 0);
    nodeCoordsCollection.put(actualNodeId, nodeCoords);

    return actualNodeId;
  }

  private CurrentWayNodes createWayNodeRecord(final long actualNodeId, final long sequenceId)
      throws Exception
  {
    CurrentWayNodes wayNodeRecord = new CurrentWayNodes();
    wayNodeRecord.setNodeId(actualNodeId);
    wayNodeRecord.setSequenceId(sequenceId);
    wayNodeRecord.setWayId(getId());
    return wayNodeRecord;
  }

  /*
   * Parse the way nodes XML. Keep a cache of the parsed records and node geo
   * info.
   */
  private void parseWayNodesXml(final org.w3c.dom.Node xml) throws Exception
  {
    assert (parsedElementIdsToElementsByType != null);
    final NodeList wayNodesXml = XPathAPI.selectNodeList(xml, "nd");

    validateWayNodesSize(wayNodesXml);

    relatedRecords = new ArrayList<Object>();
    relatedRecordIds = new HashSet<Long>();
    nodeCoordsCollection = new HashMap<Long, Coordinates>();
    for (int i = 0; i < wayNodesXml.getLength(); i++)
    {
      final org.w3c.dom.Node nodeXml = wayNodesXml.item(i);

      final long actualNodeId = parseWayNode(nodeXml);
      relatedRecordIds.add(actualNodeId);
      relatedRecords.add(createWayNodeRecord(actualNodeId, new Long(i + 1)));
    }
  }

  /**
   * Returns the generated table identifier for this element
   *
   * @return a table
   */
  public RelationalPathBase<?> getElementTable()
  {
    return currentWays;
  }

  /**
   * Returns the generated ID field for this element
   *
   * @return a table field
   */
  public NumberPath<Long> getElementIdField()
  {
    return currentWays.id;
  }


  /**
   * Returns the generated visibility field for this element
   *
   * @return a table field
   */
  public BooleanPath getElementVisibilityField()
  {
    return currentWays.visible;
  }

  /**
   * Returns the generated version field for this element
   *
   * @return a table field
   */
  public NumberPath<Long> getElementVersionField()
  {
    return currentWays.version;
  }

  /**
   * Returns the generated changeset ID field for this element
   *
   * @return a table field
   */
  public NumberPath<Long> getChangesetIdField()
  {
    return currentWays.changesetId;
  }

  /**
   * Returns the generated table identifier for records related to this element
   *
   * @return a table
   */
  public RelationalPathBase<?> getRelatedRecordTable()
  {
    return currentWayNodes;
  }

  /**
   * Returns the table field in the related record table that can be joined with
   * the parent element record table
   *
   * @return a table field
   */
  public NumberPath<Long> getRelatedRecordJoinField()
  {
    return currentWayNodes.wayId;
  }



  /**
   * Returns the generated tag field for this element
   *
   * @return a table field
   */
  public SimplePath<Object> getTagField()
  {
    return currentWays.tags;
  }

  /**
   * OSM related element type (e.g. way nodes for ways, relation members for
   * relations)
   *
   * @return a list of element types
   */
  public List<ElementType> getRelatedElementTypes()
  {
    return Arrays.asList(new ElementType[] { ElementType.Node });
  }

  /**
   * Inserts a new way into the services database
   *
   * @param changesetId
   *          corresponding changeset ID for the way to be inserted
   * @param mapId
   *          corresponding map ID for the way to be inserted
   * @param nodeIds
   *          IDs for the collection of nodes to be associated with this way
   * @param tags
   *          element tags
   * @param dbConn
   *          JDBC Connection
   * @return ID of the newly created way
   * @throws Exception
   */
  public static long insertNew(final long changesetId, final long mapId, final List<Long> nodeIds,
      final Map<String, String> tags, Connection dbConn) throws Exception
  {
    long nextWayId = new SQLQuery(dbConn, DbUtils.getConfiguration(mapId)).uniqueResult(SQLExpressions
        .nextval(Long.class, "current_ways_id_seq"));
    insertNew(nextWayId, changesetId, mapId, nodeIds, tags, dbConn);

    return nextWayId;
  }

  /**
   * Inserts a new way into the services database with the specified ID; useful
   * for testing
   *
   * @param wayId
   *          ID to assign to the new way
   * @param changesetId
   *          corresponding changeset ID for the way to be inserted
   * @param mapId
   *          corresponding map ID for the way to be inserted
   * @param nodeIds
   *          collection of nodes to be associated with this way
   * @param tags
   *          element tags
   * @param dbConn
   *          JDBC Connection
   * @throws Exception
   *           see addNodeRefs
   */
  public static void insertNew(final long wayId, final long changesetId, final long mapId,
      final List<Long> nodeIds, final Map<String, String> tags, Connection dbConn) throws Exception
  {
    CurrentWays wayRecord = new CurrentWays();
    wayRecord.setChangesetId(changesetId);
    wayRecord.setId(wayId);

    final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
    wayRecord.setTimestamp(now);
    wayRecord.setVersion(new Long(1));
    wayRecord.setVisible(true);
    if (tags != null && tags.size() > 0)
    {
      wayRecord.setTags(tags);
    }

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

      stmt = dbConn.createStatement();

      String sql = "INSERT INTO current_ways_" + mapId+ "("
          + "            id, changeset_id, \"timestamp\", visible, version, tags)"
          + " VALUES(" + wayId + "," + changesetId + "," + "CURRENT_TIMESTAMP" + ","
          + "true" + "," + "1" + "," + strTags +

          ")";
      stmt.executeUpdate(sql);
      new Way(mapId, dbConn, wayRecord).addWayNodes(mapId, nodeIds);

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
