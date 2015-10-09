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

import java.lang.reflect.InvocationTargetException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

import hoot.services.db.DbUtils;
import hoot.services.db.DbUtils.EntityChangeType;
import hoot.services.db.postgres.PostgresUtils;
import hoot.services.db2.QChangesets;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.db2.QCurrentRelations;
import hoot.services.db2.QCurrentWays;
import hoot.services.db2.QElementIdMappings;
import hoot.services.db2.QUsers;
import hoot.services.geo.BoundingBox;

import org.apache.commons.lang3.StringUtils;
import org.apache.commons.lang3.reflect.MethodUtils;
import org.apache.xpath.XPathAPI;
import org.joda.time.format.DateTimeFormat;
import org.joda.time.format.DateTimeFormatter;
import org.postgresql.util.PGobject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.NodeList;

import com.mysema.query.Tuple;
import com.mysema.query.sql.RelationalPathBase;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLDeleteClause;
import com.mysema.query.types.path.NumberPath;

/**
 * Data common across all OSM elements.  Since all elements are to be serializable to XML and the
 * services database, we implement as much of those related interfaces as possible here.
 */
public abstract class Element implements XmlSerializable, DbSerializable
{
  private static final Logger log = LoggerFactory.getLogger(Element.class);
  
  protected static final QCurrentWays currentWays = QCurrentWays.currentWays;
  protected static final QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
  protected static final QCurrentRelations currentRelations = QCurrentRelations.currentRelations;
  private static final QElementIdMappings elementIdMappings = QElementIdMappings.elementIdMappings;

  //order in the enum here is important, since the request diff writer methods use this to determine
  //the order for creating/updating/deleting elements; i.e. create nodes before referencing them
  //in a way, etc.
  public enum ElementType
  {
    Node,
    Way,
    Relation,
    //Technically, changeset doesn't inherit from Element and thus doesn't implement
    //XmlSerializable or DbSerializable, so giving it an element type is a little bit confusing.
    //It helps the code clean up a little bit in places, so leaving as is for now.
    Changeset
  }

  /**
   * OSM element type
   */
  protected ElementType elementType;
  public ElementType getElementType() { return elementType; }

  /**
   * The corresponding changeset ID parsed for this element from the changeset upload request
   */
  private long requestChangesetId = -1;
  public long getRequestChangesetId() { return requestChangesetId; }
  public void setRequestChangesetId(long id) { this.requestChangesetId = id; }

  /**
   * a JDBC Connection
   */
  protected Connection conn;
  public Connection getDbConnection() { return conn; }
  public void setDbConnection(Connection connection) { conn = connection; }
  private static DateTimeFormatter timeFormatter;
  public static DateTimeFormatter getTimeFormatter() { return timeFormatter; }

  /**
   * The element's ID before it is updated by a changeset diff
   */
  protected long oldId;
  public long getOldId() { return oldId; }
  public void setOldId(long id) { oldId = id; }

  /*
   * see ChangesetDiffDbWriter::parsedElementIdsToElementsByType
   *
   * This cache is ignored by elements which don't have related element (e.g. Node).
  */
  protected Map<ElementType, Map<Long, Element>> parsedElementIdsToElementsByType;
  public void setElementCache(Map<ElementType, Map<Long, Element>> parsedElementIdsToElementsByType)
  {
    this.parsedElementIdsToElementsByType = parsedElementIdsToElementsByType;
  }

  /**
   * The associated services database record
   */
  protected Object record;
  public Object getRecord() { return record; }
  public void setRecord(Object record) throws Exception {
  	if(record instanceof Tuple)
  	{
  		// This was forced since we are using reflection which need to be refactored to something more solid.

  		Tuple tRec = (Tuple)record;
  		Object[] tRecs = tRec.toArray();
  		if(tRecs.length > 0)
  		{
  			// assume first record is good.
  			this.record = tRecs[0];
  		}
  		else
  		{
  			throw new Exception("Bad Record type. Tuple is empty. Please make sure the first object is tuple is DTO that supports setVersion.");
  		}
  	}
  	else
  	{
  		this.record = record;
  	}
  }

  /**
   * Records associated with the contained services database record
   */
  protected Collection<Object> relatedRecords;
  public Collection<Object> getRelatedRecords() { return relatedRecords; }

  /**
   * IDs of records associated with the contained services database record
   */
  protected Collection<Long> relatedRecordIds;

  /**
   * Changeset diff type being applied to the node: create, modify, or delete
   */
  protected EntityChangeType entityChangeType = EntityChangeType.CREATE;
  public EntityChangeType getEntityChangeType() { return entityChangeType; }
  public void setEntityChangeType(EntityChangeType changeType)
  {
    entityChangeType = changeType;
  }

  public Element(Connection conn)
  {
    this.conn = conn;
    timeFormatter = DateTimeFormat.forPattern(DbUtils.TIMESTAMP_DATE_FORMAT);
  }

  /**
   * Returns the ID of the element associated services database record
   */
  public long getId() throws Exception
  {
    //this is a little risky, but I'm assuming the field probably won't ever change in name
    //in the OSM tables
    return (Long)MethodUtils.invokeMethod(record, "getId", new Object[]{});
  }

  /**
   * Sets the ID of the element associated services database record
   */
  public void setId(long id) throws Exception
  {
    //this is a little risky, but I'm assuming the field probably won't ever change in name
    //in the OSM tables
    MethodUtils.invokeMethod(record, "setId", new Object[]{ id });
  }

  //We will keep track of map id internally since we do not have map id column in table any longer
  private long _mapId = -1;

  /**
   * Returns the map ID of the element's associated services database record
   */
  public long getMapId() throws Exception
  {
  	return _mapId;
  }


  /**
   * Sets the map ID of the element associated map
   */
  public void setMapId(long id) throws Exception
  {
  	_mapId = id;
  }

  /**
   *  Returns the tags of the element associated services database record
   *
   * @return a string map with tag key/value pairs
   * @throws Exception
   */
  @SuppressWarnings("unchecked")
  public Map<String, String> getTags() throws Exception
  {
    //this is a little risky, but I'm assuming the field probably won't ever change in name
    //in the OSM tables

    Object oTags = MethodUtils.invokeMethod(record, "getTags", new Object[]{});
    if(oTags instanceof PGobject)
    {
    	return
          PostgresUtils.postgresObjToHStore(
            (PGobject)MethodUtils.invokeMethod(record, "getTags", new Object[]{}));
    }

    return (Map<String, String>)oTags;
  }

  /**
   * Sets tags on an element
   *
   * @param tags string map with tag key/value pairs
   * @throws NoSuchMethodException
   * @throws IllegalAccessException
   * @throws InvocationTargetException
   */
  public void setTags(final Map<String, String> tags) throws NoSuchMethodException,
    IllegalAccessException, InvocationTargetException
  {
    MethodUtils.invokeMethod(
      record,
      "setTags",
      tags);
  }

  /**
   * Determines whether an element possesses tags beginning with a particular prefix
   *
   * @param startsWithText text to search tags for
   * @return true if the element has one or more tags beginning with startsWithText
   * @throws NoSuchMethodException
   * @throws IllegalAccessException
   * @throws InvocationTargetException
   * @throws Exception
   */
  public boolean hasTagsStartingWithText(final String startsWithText)
    throws NoSuchMethodException, IllegalAccessException, InvocationTargetException, Exception
  {
    final Map<String, String> tags = getTags();
    for (Map.Entry<String, String> tagEntry : tags.entrySet())
    {
      if (tagEntry.getKey().startsWith(startsWithText))
      {
        return true;
      }
    }
    return false;
  }

  /**
   * Returns the visibility of the element associated services database record
   */
  public boolean getVisible() throws Exception
  {
    //this is a little risky, but I'm assuming the field probably won't ever change in name
    //in the OSM tables
    return (Boolean)MethodUtils.invokeMethod(record, "getVisible", new Object[]{});
  }

  /**
   * Returns the version of the element associated services database record
   */
  public long getVersion() throws Exception
  {
    //this is a little risky, but I'm assuming the field probably won't ever change in name
    //in the OSM tables
    return (Long)MethodUtils.invokeMethod(record, "getVersion", new Object[]{});
  }

  public void setVersion(long version) throws Exception
  {
    //this is a little risky, but I'm assuming the field probably won't ever change in name
    //in the OSM tables
    MethodUtils.invokeMethod(record, "setVersion", version);
  }

  /**
   * The geospatial bounds for this element
   */
  public abstract BoundingBox getBounds() throws Exception;

  @Override
  public String toString()
  {
    final String[] classParts = getClass().getName().split("\\.");
    return getClass().getName().split("\\.")[classParts.length - 1].toLowerCase();
  }

  /**
   * Returns an XML representation of the element
   *
   * @param parentXml XML node this element should be attached under
   * @param modifyingUserId ID of the user which created this element
   * @param modifyingUserDisplayName user display name of the user which created this element
   * @param multiLayerUniqueElementIds if true, IDs are prepended with
   * <map id>_<first letter of the element type>_; this setting activated is not compatible with
   * standard OSM clients (specific to Hootenanny iD)
   * @param addChildren if true, element children are added to the element xml (way nodes for ways
   * relation member for relations); parameter is ignored by the Node element
   * @return an XML element
   */
  public org.w3c.dom.Element toXml(final org.w3c.dom.Element parentXml, final long modifyingUserId,
    final String modifyingUserDisplayName, final boolean multiLayerUniqueElementIds,
    final boolean addChildren) throws Exception
  {
    Document doc = parentXml.getOwnerDocument();
    org.w3c.dom.Element element = doc.createElement(toString());
    String id = String.valueOf(getId());
    if (multiLayerUniqueElementIds)
    {
      //hoot custom id unique across map layers
      id = getMapId() + "_" + elementType.toString().toLowerCase().charAt(0) + "_" + id;
    }
    element.setAttribute("id", id);
    element.setAttribute(
      "visible", String.valueOf(MethodUtils.invokeMethod(record, "getVisible", new Object[]{})));
    element.setAttribute(
      "version", String.valueOf(MethodUtils.invokeMethod(record, "getVersion", new Object[]{})));
    element.setAttribute(
      "changeset", String.valueOf(MethodUtils.invokeMethod(record, "getChangesetId", new Object[]{})));
    element.setAttribute(
      "timestamp", String.valueOf(MethodUtils.invokeMethod(record, "getTimestamp", new Object[]{})));
    element.setAttribute("user", modifyingUserDisplayName);
    element.setAttribute("uid", String.valueOf(modifyingUserId));
    return element;
  }

  /*
   * This ensures that the changeset ID specified in the element XML is the same as
   * what was specified in the changeset request
   */
  protected long parseChangesetId(final NamedNodeMap xmlAttributes) throws Exception
  {
    long elementChangesetId = -1;
    try
    {
      elementChangesetId = Long.parseLong(xmlAttributes.getNamedItem("changeset").getNodeValue());
    }
    catch (NumberFormatException e)
    {
    }
    if (elementChangesetId != requestChangesetId)
    {
      throw new Exception("Invalid changeset ID: " + elementChangesetId +
        " for " + toString() + ".  Expected changeset ID: " + requestChangesetId);
    }
    return elementChangesetId;
  }

  /*
   * If a new element is being created, it always gets a newly assigned version = 1.  Otherwise, the
   * version passed in the changeset request must match the existing version the server to ensure
   * data integrity.
   */
  protected long parseVersion(final NamedNodeMap xmlAttributes) throws Exception
  {
    long version = 1;
    //version passed in the request can be ignored if it is a create request
    if (!entityChangeType.equals(EntityChangeType.CREATE))
    {
      //if it is ever determined that doing this fetch when this method is called in a loop hinders
      //performance (#2951), replace this query with a query outside the loop that checks for the
      //existence of all nodes and validates their versions in a batch query.  The downside to
      //this, however, would be parsing the XML node data more than once.

    	Object existingRecord =
    	  new SQLQuery(conn, DbUtils.getConfiguration(getMapId()))
    	    .from(getElementTable())
    	    .where(getElementIdField().eq(new Long(oldId)))
    			.singleResult(getElementTable());
      if (existingRecord == null)
      {
        throw new Exception(toString() + " to be updated does not exist with ID: " + oldId);
      }
      version = Long.parseLong(xmlAttributes.getNamedItem("version").getNodeValue());
      //the specified version must be validated with what's already in the database on a modify
      final long existingVersion =
        (Long)MethodUtils.invokeMethod(existingRecord, "getVersion", new Object[]{});
      if (version != existingVersion)
      {
        throw new Exception("Invalid version: " + version + " specified for " + toString() + 
        	" with ID: " + getId() + " and expected version " + existingVersion + " in changeset " +
          " with ID: " + MethodUtils.invokeMethod(record, "getChangesetId", new Object[]{}));
      }
      version++;
    }
    else
    {
      //I'm not sure how important it is that this check is strictly enforced here, but I'm
      //doing it for now anyway.
      final long parsedVersion =
        Long.parseLong(xmlAttributes.getNamedItem("version").getNodeValue());
      if (parsedVersion != 0)
      {
        throw new Exception("Invalid version: " + parsedVersion + " specified for " + toString() + 
          " with ID: " + getId() + " and expected version 0 in changeset " +
          " with ID: " + MethodUtils.invokeMethod(record, "getChangesetId", new Object[]{}));
      }
    }
    return version;
  }

  //is this timestamp even actually honored from the xml in the rails port?...don't think so;
  //if not, remove this
  protected Timestamp parseTimestamp(final NamedNodeMap xmlAttributes)
  {
    Timestamp timestamp = null;
    org.w3c.dom.Node timestampXml = xmlAttributes.getNamedItem("timestamp");
    final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
    if (timestampXml != null)
    {
      try
      {
        timestamp =
          new Timestamp(
            Element.getTimeFormatter().parseDateTime(timestampXml.getNodeValue()).getMillis());
      }
      catch (IllegalArgumentException e)
      {
      }
    }
    if (timestamp == null)
    {
      timestamp = now;
    }
    return timestamp;
  }

  /**
   * Returns an XML representation of the element's data suitable for a changeset upload response
   *
   * It is a little risky to use reflection, but I'm assuming the ID/version fields probably won't
   * ever change in name in the OSM tables.
   *
   * @param parent XML node this element should be attached under
   * @throws Exception
   */
  public org.w3c.dom.Element toChangesetResponseXml(final org.w3c.dom.Element parent)
    throws Exception
  {
    Document doc = parent.getOwnerDocument();
    org.w3c.dom.Element entityElement = doc.createElement(toString());
    entityElement.setAttribute("old_id", String.valueOf(getOldId()));
    if (!getEntityChangeType().equals(EntityChangeType.DELETE))
    {
      entityElement.setAttribute(
        "new_id",
        String.valueOf(MethodUtils.invokeMethod(record, "getId", new Object[]{})));
      entityElement.setAttribute(
        "new_version",
        String.valueOf(MethodUtils.invokeMethod(record, "getVersion", new Object[]{})));
    }
    return entityElement;
  }

  /**
   * Returns a set of elements from the services database
   *
   * @param mapId ID of the map owning the records
   * @param elementType type of elements to be returned
   * @param elementIds IDs of the elements to be returned
   * @param dbConn JDBC Connection
   * @return a set of element records
   * @throws InvocationTargetException
   * @throws NoSuchMethodException
   * @throws ClassNotFoundException
   * @throws IllegalAccessException
   * @throws InstantiationException
   */
  @SuppressWarnings("rawtypes")
  public static List<?> getElementRecords(final long mapId, final ElementType elementType,
    final Set<Long> elementIds, Connection dbConn) throws InstantiationException,
    IllegalAccessException, ClassNotFoundException, NoSuchMethodException,
    InvocationTargetException
  {
    final Element prototype = ElementFactory.getInstance().create(mapId, elementType, dbConn);
  	if(elementIds.size() > 0)
  	{
  	return
  			new SQLQuery(dbConn, DbUtils.getConfiguration(mapId)).from(prototype.getElementTable()).
  				where(prototype.getElementIdField().in(elementIds)).orderBy(prototype.getElementIdField().asc()).
  			list(prototype.getElementTable());
  	}

  	return new ArrayList();
  }

  /**
   * Returns a set of elements from the services database with user information joined to it
   *
   * @param mapId ID of the map owning the records
   * @param elementType type of elements to be returned
   * @param elementIds IDs of the elements to be returned
   * @param dbConn JDBC Connection
   * @return a set of  element records
   * @throws InvocationTargetException
   * @throws NoSuchMethodException
   * @throws ClassNotFoundException
   * @throws IllegalAccessException
   * @throws InstantiationException
   */
  @SuppressWarnings("rawtypes")
  public static List<?> getElementRecordsWithUserInfo(final long mapId,
    final ElementType elementType, final Set<Long> elementIds, Connection dbConn)
    throws InstantiationException, IllegalAccessException, ClassNotFoundException,
    NoSuchMethodException, InvocationTargetException
  {
    final Element prototype = ElementFactory.getInstance().create(mapId, elementType, dbConn);

    QChangesets changesets = QChangesets.changesets;
    QUsers users = QUsers.users;

    if (elementIds.size() > 0)
    {
  	  return
  	    new SQLQuery(dbConn, DbUtils.getConfiguration("" + mapId))
  	      .from(prototype.getElementTable())
  	      .join(QChangesets.changesets)
  	      .on(prototype.getChangesetIdField().eq(changesets.id))
  			  .join(users)
  			  .on(changesets.userId.eq(users.id))
  			  .where(prototype.getElementIdField().in(elementIds))
  			  .orderBy(prototype.getElementIdField().asc())
  			  .list(prototype.getElementTable(), users, changesets);
    }

    return new ArrayList();
  }

  /**
   * Removes all records related (e.g. way nodes for ways, relation members for relations, etc.)
   * to all of the elements with the passed in IDs
   *
   * @param mapId ID of the map owning the elements
   * @param mapIdField services database table map ID field for the join table
   * @param relatedRecordTable services database table for the related record type
   * @param joinField services database table field which joins the related record to the parent
   * record
   * @param elementIds IDs of the elements for which related records are to be deleted
   * @param warnOnNothingRemoved if true, a warning will be logged if no related records were
   * removed
   * @param dbConn JDBC Connection
   */
  public static void removeRelatedRecords(final long mapId,
    final RelationalPathBase<?> relatedRecordTable, final NumberPath<Long> joinField,
    final Set<Long> elementIds, final boolean warnOnNothingRemoved, Connection dbConn)
  {
    long recordsProcessed = 0;
    if (relatedRecordTable != null && joinField != null)
    {
    	SQLDeleteClause sqldelete = new SQLDeleteClause(dbConn, DbUtils.getConfiguration(mapId), relatedRecordTable);
      recordsProcessed = 0;

      if(elementIds.size() > 0)
      {
      	recordsProcessed = sqldelete.where(joinField.in(elementIds)
              )
              .execute();
      }

    }
    if (recordsProcessed == 0)
    {
      if (warnOnNothingRemoved)
      {
        String msg = "No related records were removed";
        if (relatedRecordTable != null)
        {
          msg += " for type: " + relatedRecordTable.toString();
        }
        log.warn(msg);
      }
    }
    else
    {
      if (relatedRecordTable != null)
      {
        log.debug("Removed " + recordsProcessed + " related records for element record type " +
          relatedRecordTable.toString());
      }
    }
  }

  /**
   * Determines whether the specified elements exist in the services database
   *
   * @param mapId ID of the map owning the elements
   * @param elementType the type of element to check existence for
   * @param elementIds a collection of element IDs
   * @param dbConn JDBC Connection
   * @return true if element exist for every input element ID; false otherwise
   * @throws InvocationTargetException
   * @throws NoSuchMethodException
   * @throws ClassNotFoundException
   * @throws IllegalAccessException
   * @throws InstantiationException
   */
  public static boolean allElementsExist(final long mapId, final ElementType elementType,
    final Set<Long> elementIds, Connection dbConn) throws InstantiationException,
    IllegalAccessException, ClassNotFoundException, NoSuchMethodException,
    InvocationTargetException, Exception
  {
    final Element prototype = ElementFactory.getInstance().create(mapId, elementType, dbConn);

    if(elementIds.size() > 0)
    {
  	  return
  			new SQLQuery(dbConn, DbUtils.getConfiguration(mapId)).from(prototype.getElementTable())
  			.where(prototype.getElementIdField().in(elementIds))
  			.count() == elementIds.size();
    }
    else
    {
    	return 0  == elementIds.size();
    }

  }

  /**
   * Determines whether all elements in the input list are visible
   *
   * @param mapId ID of the map owning the elements
   * @param elementType the type of element to check visibility for
   * @param elementIds a collection of element IDs
   * @param dbConn JDBC Connection
   * @return true if every node associated with the corresponding input node ID is visible
   * @throws InvocationTargetException
   * @throws NoSuchMethodException
   * @throws ClassNotFoundException
   * @throws IllegalAccessException
   * @throws InstantiationException
   */
  public static boolean allElementsVisible(final long mapId, final ElementType elementType,
    final Set<Long> elementIds, Connection dbConn) throws InstantiationException,
    IllegalAccessException, ClassNotFoundException, NoSuchMethodException,
    InvocationTargetException
  {
    final Element prototype = ElementFactory.getInstance().create(mapId, elementType, dbConn);

    if (elementIds.size() > 0)
    {
  	  return
  			new SQLQuery(dbConn, DbUtils.getConfiguration(mapId)).from(prototype.getElementTable())
  			.where(
            prototype.getElementIdField().in(elementIds)
            .and(prototype.getElementVisibilityField().eq(true))
            )
  			.count() == elementIds.size();
    }
    else
    {
    	return 0 == elementIds.size();
    }

  }

  /**
   * Returns the database relation member type given an element type
   *
   * @param elementType the element type for which to retrive the database relation member type
   * @return a database relation member type
   */
  public static DbUtils.nwr_enum elementEnumForElementType(final ElementType elementType)
  {
    switch (elementType)
    {
      case Node:

        return DbUtils.nwr_enum.node;

      case Way:

        return DbUtils.nwr_enum.way;

      case Relation:

        return DbUtils.nwr_enum.relation;

      default:

        assert(false);
        return null;
    }
  }

  /**
   * Returns the element type given a database relation member type
   *
   * @param typeEnum a database relation member type
   * @return an element type
   */
  public static ElementType elementTypeForElementEnum(final Object typeEnum)
  {
    return elementTypeFromString(typeEnum.toString());
  }

  /**
   * Returns an element type given an element type string or element string abbreviation
   *
   * @param elementTypeStr an element type string
   * @return an element type
   */
  public static ElementType elementTypeFromString(final String elementTypeStr)
  {
    if (!StringUtils.isEmpty(elementTypeStr))
    {
      if (elementTypeStr.toLowerCase().equals("node") || elementTypeStr.toLowerCase().equals("n"))
      {
        return ElementType.Node;
      }
      else if (elementTypeStr.toLowerCase().equals("way") ||
               elementTypeStr.toLowerCase().equals("w"))
      {
        return ElementType.Way;
      }
      else if (elementTypeStr.toLowerCase().equals("relation") ||
               elementTypeStr.toLowerCase().equals("r"))
      {
        return ElementType.Relation;
      }
    }
    return null;
  }

  /**
   * Returns the IDs of all relations which own this element
   *
   * The ordering of returned records by ID and the use of TreeSet to keep them sorted is only
   * for error reporting readability purposes only.
   *
   * @return sorted list of relation IDs
   * @throws DataAccessException
   * @throws Exception
   */
  protected Set<Long> getOwningRelationIds() throws Exception
  {
    QCurrentRelationMembers currRelMem = QCurrentRelationMembers.currentRelationMembers;
  	List<Long> res =
  			new SQLQuery(conn, DbUtils.getConfiguration("" + getMapId())).from(currRelMem)
  			.where(
  					currRelMem.memberId.eq(getId())
  					.and(currRelMem.memberType.eq(Element.elementEnumForElementType(elementType)))
  					)
				.orderBy(currRelMem.relationId.asc())
				.list(currRelMem.relationId);

  	return new TreeSet<Long>(res);

  }

  /*
   * Parses tags from the element XML and returns them in a map
   */
  protected Map<String, String> parseTags(final org.w3c.dom.Node elementXml) throws Exception
  {
    Map<String, String> tags = new HashMap<String, String>();
    try
    {
      NodeList tagXmlNodes = XPathAPI.selectNodeList(elementXml, "tag");
      for (int i = 0; i < tagXmlNodes.getLength(); i++)
      {
        org.w3c.dom.Node tagXmlNode = tagXmlNodes.item(i);
        NamedNodeMap nodeTagAttributes = tagXmlNode.getAttributes();
        tags.put(
          nodeTagAttributes.getNamedItem("k").getNodeValue(),
          nodeTagAttributes.getNamedItem("v").getNodeValue());
      }
    }
    catch (Exception e)
    {
      throw new Exception("Error parsing tag.");
    }
    return tags;
  }

  /*
   * Adds tags XML to the parent element XML
   */
  protected org.w3c.dom.Element addTagsXml(final org.w3c.dom.Element elementXml) throws Exception
  {
    Document doc = elementXml.getOwnerDocument();
    final Map<String, String> tags = getTags();
    if (tags.size() == 0)
    {
      return null;
    }
    for (Map.Entry<String, String> tagEntry : tags.entrySet())
    {
      org.w3c.dom.Element tagElement = doc.createElement("tag");
      tagElement.setAttribute("k", tagEntry.getKey());
      tagElement.setAttribute(
      	"v", hoot.services.utils.StringUtils.encodeURIComponentForJavaScript(tagEntry.getValue()));
      elementXml.appendChild(tagElement);
    }
    return elementXml;
  }

  /**
   * Gets a total tag count for a specified element type belonging to a specific map
   *
   * @param mapId ID of the map for which to retrieve the tag count
   * @param elementType element type for which to retrieve the tag count
   * @param dbConn JDBC Connection
   * @return a tag count
   * @throws Exception
   */
  public static long getTagCountForElementType(final long mapId, final ElementType elementType,
    Connection dbConn) throws Exception
  {
    final Element prototype = ElementFactory.getInstance().create(mapId, elementType, dbConn);
    List<?> records =
    		new SQLQuery(dbConn, DbUtils.getConfiguration(mapId)).from(prototype.getElementTable())
				.list(prototype.getElementTable());

    long tagCount = 0;
    for (Object record : records)
    {

    	PGobject tags =
	        (PGobject)MethodUtils.invokeMethod(record, "getTags", new Object[]{});

      if (tags != null)
      {
        tagCount += PostgresUtils.postgresObjToHStore(tags).size();
      }
    }
    return tagCount;
  }
  
  /**
   * Given a list of unique ID's, filters out any which do not have an element ID mapping record
   * created for them
   * 
   * @param mapId ID of the map owning the elements associated with the input uuid's
   * @param uuids unique IDs to search for
   * @param elementType type of the elements being searched for
   * @param dbConn database connection
   * @return a filtered list of unique ID's
   */
  public static Set<String> filterOutNonExistingElementMappingUniqueIds(final long mapId, 
  	final String[] uuids, final ElementType elementType, Connection dbConn)
  {
  	assert(uuids.length > 0);
  	return 
  		new HashSet<String>(
  		  new SQLQuery(dbConn, DbUtils.getConfiguration(mapId))
          .from(elementIdMappings)
          .where(
        	  elementIdMappings.mapId.eq(mapId)
        	  .and(elementIdMappings.elementId.in(uuids)))
          .list(elementIdMappings.elementId));
  }
  
  /**
   * Given a list of unique ID's, filters out any which aren't associated with an OSM element in 
   * the database
   * 
   * @param mapId ID of the map owning the elements associated with the input uuid's
   * @param uuids unique IDs to search for
   * @param elementType type of the elements being searched for
   * @param dbConn database connection
   * @return a filtered list of unique ID's
   * @throws InvocationTargetException 
   * @throws NoSuchMethodException 
   * @throws ClassNotFoundException 
   * @throws IllegalAccessException 
   * @throws InstantiationException 
   * @throws SQLException 
   */
  public static Set<String> filterOutNonExistingUuids(final long mapId, final String[] uuids, 
  	final ElementType elementType, Connection dbConn) throws InstantiationException, 
  	IllegalAccessException, ClassNotFoundException, NoSuchMethodException, InvocationTargetException, 
  	SQLException
  {
  	Set<String> filteredUuids = new HashSet<String>();
  	final Element prototype = ElementFactory.getInstance().create(mapId, elementType, dbConn);
  	String POSTGRESQL_DRIVER = "org.postgresql.Driver";
		Statement stmt = null;
		ResultSet rs = null;
		try
		{
			Class.forName(POSTGRESQL_DRIVER);
			stmt = dbConn.createStatement();
			final String sql = 
				"select tags->'uuid' from " + prototype.getElementTable() + "_" + mapId + 
				" where tags->'uuid' in ('" + StringUtils.join(uuids, "','") + "') ";
			stmt = dbConn.createStatement();
			rs = stmt.executeQuery(sql);
			while (rs.next())
			{
				filteredUuids.add(rs.getString(1));
			}
			rs.close();
		}
		finally
		{
			try
			{
				if (stmt != null) stmt.close();
				if (rs != null) rs.close();
			}
			catch (SQLException se2)
			{
			}
		}
		if (filteredUuids.size() > uuids.length)
		{
			log.warn("Filtered out " + String.valueOf(uuids.length - filteredUuids.size()) + " uuids.");
		}
  	return filteredUuids;
  }
}
