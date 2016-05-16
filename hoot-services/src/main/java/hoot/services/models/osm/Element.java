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
package hoot.services.models.osm;

import java.lang.reflect.InvocationTargetException;
import java.sql.Connection;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

import org.apache.commons.lang3.StringUtils;
import org.apache.commons.lang3.reflect.MethodUtils;
import org.joda.time.format.DateTimeFormat;
import org.joda.time.format.DateTimeFormatter;
import org.postgresql.util.PGobject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.NodeList;

import com.mysema.query.sql.RelationalPathBase;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLDeleteClause;
import com.mysema.query.types.path.NumberPath;

import hoot.services.db.DbUtils;
import hoot.services.db.DbUtils.EntityChangeType;
import hoot.services.db.postgres.PostgresUtils;
import hoot.services.db2.CurrentNodes;
import hoot.services.db2.QChangesets;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.db2.QCurrentRelations;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.db2.QCurrentWays;
import hoot.services.db2.QUsers;
import hoot.services.geo.BoundingBox;


/**
 * Data common across all OSM elements. Since all elements are to be
 * serializable to XML and the services database, we implement as much of those
 * related interfaces as possible here.
 */
public abstract class Element implements XmlSerializable, DbSerializable {
    private static final Logger log = LoggerFactory.getLogger(Element.class);

    protected static final QCurrentWays currentWays = QCurrentWays.currentWays;
    protected static final QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
    protected static final QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;
    protected static final QCurrentRelations currentRelations = QCurrentRelations.currentRelations;
    protected static final QCurrentRelationMembers currentRelationMembers = QCurrentRelationMembers.currentRelationMembers;

    protected Map<Long, CurrentNodes> dbNodeCache;

    public void setDbNodeCache(Map<Long, CurrentNodes> cache) {
        dbNodeCache = cache;
    }

    // order in the enum here is important, since the request diff writer
    // methods use this to determine
    // the order for creating/updating/deleting elements; i.e. create nodes
    // before referencing them
    // in a way, etc.
    public enum ElementType {
        Node, Way, Relation,
        // Technically, changeset doesn't inherit from Element and thus doesn't
        // implement
        // XmlSerializable or DbSerializable, so giving it an element type is a
        // little bit confusing.
        // It helps the code clean up a little bit in places, so leaving as is
        // for now.
        Changeset
    }

    /**
     * OSM element type
     */
    protected ElementType elementType;

    public ElementType getElementType() {
        return elementType;
    }

    /**
     * The corresponding changeset ID parsed for this element from the changeset
     * upload request
     */
    private long requestChangesetId = -1;

    public long getRequestChangesetId() {
        return requestChangesetId;
    }

    @Override
    public void setRequestChangesetId(long id) {
        this.requestChangesetId = id;
    }

    /**
     * a JDBC Connection
     */
    protected Connection conn;

    public Connection getDbConnection() {
        return conn;
    }

    public void setDbConnection(Connection connection) {
        conn = connection;
    }

    private static DateTimeFormatter timeFormatter;

    public static DateTimeFormatter getTimeFormatter() {
        return timeFormatter;
    }

    /**
     * The element's ID before it is updated by a changeset diff
     */
    protected long oldId;

    @Override
    public long getOldId() {
        return oldId;
    }

    @Override
    public void setOldId(long id) {
        oldId = id;
    }

    /*
     * see ChangesetDiffDbWriter::parsedElementIdsToElementsByType
     *
     * This cache is ignored by elements which don't have related element (e.g.
     * Node).
     */
    protected Map<ElementType, Map<Long, Element>> parsedElementIdsToElementsByType;

    @Override
    public void setElementCache(Map<ElementType, Map<Long, Element>> parsedElementIdsToElementsByType) {
        this.parsedElementIdsToElementsByType = parsedElementIdsToElementsByType;
    }

    /**
     * The associated services database record
     */
    protected Object record;

    @Override
    public Object getRecord() {
        return record;
    }

    @Override
    public void setRecord(Object record) {
        this.record = record;
    }

    /**
     * Records associated with the contained services database record
     */
    protected Collection<Object> relatedRecords;

    @Override
    public Collection<Object> getRelatedRecords() {
        return relatedRecords;
    }

    /**
     * IDs of records associated with the contained services database record
     */
    protected Collection<Long> relatedRecordIds;

    /**
     * Changeset diff type being applied to the node: create, modify, or delete
     */
    protected EntityChangeType entityChangeType = EntityChangeType.CREATE;

    @Override
    public EntityChangeType getEntityChangeType() {
        return entityChangeType;
    }

    @Override
    public void setEntityChangeType(EntityChangeType changeType) {
        entityChangeType = changeType;
    }

    public Element(Connection conn) {
        this.conn = conn;
        timeFormatter = DateTimeFormat.forPattern(DbUtils.TIMESTAMP_DATE_FORMAT);
    }

    /**
     * Returns the ID of the element associated services database record
     */
    @Override
    public long getId() throws Exception {
        return (Long) MethodUtils.invokeMethod(record, "getId", new Object[] {});
    }

    /**
     * Sets the ID of the element associated services database record
     */
    @Override
    public void setId(long id) throws Exception {
        MethodUtils.invokeMethod(record, "setId", new Object[] { id });
    }

    // We will keep track of map id internally since we do not have map id
    // column in table any longer
    private long _mapId = -1;

    /**
     * Returns the map ID of the element's associated services database record
     */
    public long getMapId() {
        return _mapId;
    }

    /**
     * Sets the map ID of the element associated map
     */
    public void setMapId(long id) {
        _mapId = id;
    }

    /**
     * Returns the tags of the element associated services database record
     *
     * @return a string map with tag key/value pairs
     * @throws Exception
     */
    @SuppressWarnings("unchecked")
    public Map<String, String> getTags() throws Exception {
        Object oTags = MethodUtils.invokeMethod(record, "getTags", new Object[] {});

        if (oTags instanceof PGobject) {
            return PostgresUtils
                    .postgresObjToHStore((PGobject) MethodUtils.invokeMethod(record, "getTags", new Object[] {}));
        }

        return (Map<String, String>) oTags;
    }

    /**
     * Sets tags on an element
     *
     * @param tags
     *            string map with tag key/value pairs
     * @throws NoSuchMethodException
     * @throws IllegalAccessException
     * @throws InvocationTargetException
     */
    public void setTags(Map<String, String> tags)
            throws NoSuchMethodException, IllegalAccessException, InvocationTargetException {
        MethodUtils.invokeMethod(record, "setTags", tags);
    }

    /**
     * Returns the visibility of the element associated services database record
     */
    public boolean getVisible() throws Exception {
        return (Boolean) MethodUtils.invokeMethod(record, "getVisible", new Object[] {});
    }

    /**
     * The geospatial bounds for this element
     */
    public abstract BoundingBox getBounds() throws Exception;

    @Override
    public String toString() {
        String[] classParts = getClass().getName().split("\\.");
        return getClass().getName().split("\\.")[classParts.length - 1].toLowerCase();
    }

    /**
     * Returns an XML representation of the element
     *
     * @param parentXml
     *            XML node this element should be attached under
     * @param modifyingUserId
     *            ID of the user which created this element
     * @param modifyingUserDisplayName
     *            user display name of the user which created this element
     * @param multiLayerUniqueElementIds
     *            if true, IDs are prepended with <map id>_<first letter of the
     *            element type>_; this setting activated is not compatible with
     *            standard OSM clients (specific to Hootenanny iD)
     * @param addChildren
     *            if true, element children are added to the element xml (way
     *            nodes for ways relation member for relations); parameter is
     *            ignored by the Node element
     * @return an XML element
     */
    @Override
    public org.w3c.dom.Element toXml(org.w3c.dom.Element parentXml, long modifyingUserId,
            String modifyingUserDisplayName, boolean multiLayerUniqueElementIds, boolean addChildren) throws Exception {
        Document doc = parentXml.getOwnerDocument();
        org.w3c.dom.Element element = doc.createElement(toString());
        String id = String.valueOf(getId());
        if (multiLayerUniqueElementIds) {
            // hoot custom id unique across map layers
            id = getMapId() + "_" + elementType.toString().toLowerCase().charAt(0) + "_" + id;
        }
        element.setAttribute("id", id);
        element.setAttribute("visible",
                String.valueOf(MethodUtils.invokeMethod(record, "getVisible", new Object[] {})));
        element.setAttribute("version",
                String.valueOf(MethodUtils.invokeMethod(record, "getVersion", new Object[] {})));
        element.setAttribute("changeset",
                String.valueOf(MethodUtils.invokeMethod(record, "getChangesetId", new Object[] {})));
        element.setAttribute("timestamp",
                String.valueOf(MethodUtils.invokeMethod(record, "getTimestamp", new Object[] {})));
        element.setAttribute("user", modifyingUserDisplayName);
        element.setAttribute("uid", String.valueOf(modifyingUserId));
        return element;
    }

    /*
     * This ensures that the changeset ID specified in the element XML is the
     * same as what was specified in the changeset request
     */
    long parseChangesetId(NamedNodeMap xmlAttributes) throws Exception {
        long elementChangesetId = -1;

        try {
            elementChangesetId = Long.parseLong(xmlAttributes.getNamedItem("changeset").getNodeValue());
        }
        catch (NumberFormatException e) {
            //
        }

        if (elementChangesetId != requestChangesetId) {
            throw new Exception("Invalid changeset ID: " + elementChangesetId + " for " + toString()
                    + ".  Expected changeset ID: " + requestChangesetId);
        }

        return elementChangesetId;
    }

    /*
     * If a new element is being created, it always gets a newly assigned
     * version = 1. Otherwise, the version passed in the changeset request must
     * match the existing version the server to ensure data integrity.
     */
    long parseVersion() throws Exception {
        long version = 1;

        // version passed in the request can be ignored if it is a create
        // request, since we've already
        // done version error checking at this point
        if (entityChangeType != EntityChangeType.CREATE) {
            version++;
        }

        return version;
    }

    // is this timestamp even actually honored from the xml in the rails
    // port?...don't think so;
    // if not, remove this
    Timestamp parseTimestamp(NamedNodeMap xmlAttributes) {
        Timestamp timestamp = null;
        org.w3c.dom.Node timestampXml = xmlAttributes.getNamedItem("timestamp");
        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());

        if (timestampXml != null) {
            try {
                timestamp = new Timestamp(
                        Element.getTimeFormatter().parseDateTime(timestampXml.getNodeValue()).getMillis());
            }
            catch (IllegalArgumentException e) {
                //
            }
        }

        if (timestamp == null) {
            timestamp = now;
        }

        return timestamp;
    }

    /**
     * Returns an XML representation of the element's data suitable for a
     * changeset upload response
     *
     * @param parent
     *            XML node this element should be attached under
     * @throws Exception
     */
    @Override
    public org.w3c.dom.Element toChangesetResponseXml(org.w3c.dom.Element parent) throws Exception {
        Document doc = parent.getOwnerDocument();
        org.w3c.dom.Element entityElement = doc.createElement(toString());
        entityElement.setAttribute("old_id", String.valueOf(getOldId()));

        if (getEntityChangeType() != EntityChangeType.DELETE) {
            entityElement.setAttribute("new_id",
                    String.valueOf(MethodUtils.invokeMethod(record, "getId", new Object[] {})));
            entityElement.setAttribute("new_version",
                    String.valueOf(MethodUtils.invokeMethod(record, "getVersion", new Object[] {})));
        }

        return entityElement;
    }

    /**
     * Returns a set of elements from the services database
     *
     * @param mapId
     *            ID of the map owning the records
     * @param elementType
     *            type of elements to be returned
     * @param elementIds
     *            IDs of the elements to be returned
     * @param dbConn
     *            JDBC Connection
     * @return a set of element records
     * @throws InvocationTargetException
     * @throws NoSuchMethodException
     * @throws ClassNotFoundException
     * @throws IllegalAccessException
     * @throws InstantiationException
     */
    @SuppressWarnings("rawtypes")
    static List<?> getElementRecords(long mapId, ElementType elementType, Set<Long> elementIds, Connection dbConn)
            throws InstantiationException, IllegalAccessException, ClassNotFoundException, NoSuchMethodException,
            InvocationTargetException {
        Element prototype = ElementFactory.getInstance().create(mapId, elementType, dbConn);

        if (!elementIds.isEmpty()) {
            return new SQLQuery(dbConn, DbUtils.getConfiguration(mapId)).from(prototype.getElementTable())
                    .where(prototype.getElementIdField().in(elementIds)).orderBy(prototype.getElementIdField().asc())
                    .list(prototype.getElementTable());
        }

        return new ArrayList();
    }

    /**
     * Returns a set of elements from the services database with user
     * information joined to it
     *
     * @param mapId
     *            ID of the map owning the records
     * @param elementType
     *            type of elements to be returned
     * @param elementIds
     *            IDs of the elements to be returned
     * @param dbConn
     *            JDBC Connection
     * @return a set of element records
     * @throws InvocationTargetException
     * @throws NoSuchMethodException
     * @throws ClassNotFoundException
     * @throws IllegalAccessException
     * @throws InstantiationException
     */
    @SuppressWarnings("rawtypes")
    public static List<?> getElementRecordsWithUserInfo(long mapId, ElementType elementType, Set<Long> elementIds,
            Connection dbConn) throws InstantiationException, IllegalAccessException, ClassNotFoundException,
            NoSuchMethodException, InvocationTargetException {
        Element prototype = ElementFactory.getInstance().create(mapId, elementType, dbConn);

        QChangesets changesets = QChangesets.changesets;
        QUsers users = QUsers.users;

        if (!elementIds.isEmpty()) {
            return new SQLQuery(dbConn, DbUtils.getConfiguration("" + mapId)).from(prototype.getElementTable())
                    .join(QChangesets.changesets).on(prototype.getChangesetIdField().eq(changesets.id)).join(users)
                    .on(changesets.userId.eq(users.id)).where(prototype.getElementIdField().in(elementIds))
                    .orderBy(prototype.getElementIdField().asc()).list(prototype.getElementTable(), users, changesets);
        }

        return new ArrayList();
    }

    /**
     * Removes all records related (e.g. way nodes for ways, relation members
     * for relations, etc.) to all of the elements with the passed in IDs
     *
     * @param mapId
     *            ID of the map owning the elements
     * @param relatedRecordTable
     *            services database table for the related record type
     * @param joinField
     *            services database table field which joins the related record
     *            to the parent record
     * @param elementIds
     *            IDs of the elements for which related records are to be
     *            deleted
     * @param warnOnNothingRemoved
     *            if true, a warning will be logged if no related records were
     *            removed
     * @param dbConn
     *            JDBC Connection
     */
    public static void removeRelatedRecords(long mapId, RelationalPathBase<?> relatedRecordTable,
            NumberPath<Long> joinField, Set<Long> elementIds, boolean warnOnNothingRemoved, Connection dbConn) {
        log.debug("Removing related records...");

        long recordsProcessed = 0;
        if ((relatedRecordTable != null) && (joinField != null)) {
            SQLDeleteClause sqldelete = new SQLDeleteClause(dbConn, DbUtils.getConfiguration(mapId),
                    relatedRecordTable);

            recordsProcessed = 0;

            if (!elementIds.isEmpty()) {
                recordsProcessed = sqldelete.where(joinField.in(elementIds)).execute();
            }
        }

        if (recordsProcessed == 0) {
            if (warnOnNothingRemoved) {
                String msg = "No related records were removed";
                if (relatedRecordTable != null) {
                    msg += " for type: " + relatedRecordTable;
                }
                log.warn(msg);
            }
        }
        else {
            if (relatedRecordTable != null) {
                log.debug("Removed " + recordsProcessed + " related records for element record type "
                        + relatedRecordTable);
            }
        }
    }

    /**
     * Determines whether the specified elements exist in the services database
     *
     * @param mapId
     *            ID of the map owning the elements
     * @param elementType
     *            the type of element to check existence for
     * @param elementIds
     *            a collection of element IDs
     * @param dbConn
     *            JDBC Connection
     * @return true if element exist for every input element ID; false otherwise
     * @throws InvocationTargetException
     * @throws NoSuchMethodException
     * @throws ClassNotFoundException
     * @throws IllegalAccessException
     * @throws InstantiationException
     */
    public static boolean allElementsExist(long mapId, ElementType elementType, Set<Long> elementIds, Connection dbConn)
            throws Exception {
        Element prototype = ElementFactory.getInstance().create(mapId, elementType, dbConn);

        if (!elementIds.isEmpty()) {
            return new SQLQuery(dbConn, DbUtils.getConfiguration(mapId)).from(prototype.getElementTable())
                    .where(prototype.getElementIdField().in(elementIds)).count() == elementIds.size();
        }

        return elementIds.isEmpty();
    }

    /**
     * Determines whether all elements in the input list are visible
     *
     * @param mapId
     *            ID of the map owning the elements
     * @param elementType
     *            the type of element to check visibility for
     * @param elementIds
     *            a collection of element IDs
     * @param dbConn
     *            JDBC Connection
     * @return true if every node associated with the corresponding input node
     *         ID is visible
     * @throws InvocationTargetException
     * @throws NoSuchMethodException
     * @throws ClassNotFoundException
     * @throws IllegalAccessException
     * @throws InstantiationException
     */
    public static boolean allElementsVisible(long mapId, ElementType elementType, Set<Long> elementIds,
            Connection dbConn) throws InstantiationException, IllegalAccessException, ClassNotFoundException,
            NoSuchMethodException, InvocationTargetException {
        Element prototype = ElementFactory.getInstance().create(mapId, elementType, dbConn);

        if (!elementIds.isEmpty()) {
            return new SQLQuery(dbConn, DbUtils.getConfiguration(mapId)).from(prototype.getElementTable()).where(
                    prototype.getElementIdField().in(elementIds).and(prototype.getElementVisibilityField().eq(true)))
                    .count() == elementIds.size();
        }

        return elementIds.isEmpty();
    }

    /**
     * Returns the database relation member type given an element string type
     *
     * @param elementTypeStr
     *            an element type string
     * @return a database relation member type
     */
    public static DbUtils.nwr_enum elementEnumFromString(final String elementTypeStr) {
        return elementEnumForElementType(elementTypeFromString(elementTypeStr));
    }

    /**
     * Returns the database relation member type given an element type
     *
     * @param elementType
     *            the element type for which to retrive the database relation
     *            member type
     * @return a database relation member type
     */
    public static DbUtils.nwr_enum elementEnumForElementType(final ElementType elementType) {
        switch (elementType) {
            case Node:

                return DbUtils.nwr_enum.node;

            case Way:

                return DbUtils.nwr_enum.way;

            case Relation:

                return DbUtils.nwr_enum.relation;

            default:

                assert (false);
                return null;
        }
    }

    /**
     * Returns the element type given a database relation member type
     *
     * @param typeEnum
     *            a database relation member type
     * @return an element type
     */
    public static ElementType elementTypeForElementEnum(final Object typeEnum) {
        return elementTypeFromString(typeEnum.toString());
    }

    /**
     * Returns an element type given an element type string or element string
     * abbreviation
     *
     * @param elementTypeStr
     *            an element type string
     * @return an element type
     */
    public static ElementType elementTypeFromString(final String elementTypeStr) {
        if (!StringUtils.isEmpty(elementTypeStr)) {
            if (elementTypeStr.toLowerCase(Locale.ENGLISH).equals("node")
                    || elementTypeStr.toLowerCase(Locale.ENGLISH).equals("n")) {
                return ElementType.Node;
            }
            else if (elementTypeStr.toLowerCase(Locale.ENGLISH).equals("way")
                    || elementTypeStr.toLowerCase(Locale.ENGLISH).equals("w")) {
                return ElementType.Way;
            }
            else if (elementTypeStr.toLowerCase(Locale.ENGLISH).equals("relation")
                    || elementTypeStr.toLowerCase(Locale.ENGLISH).equals("r")) {
                return ElementType.Relation;
            }
        }

        return null;
    }

    /*
     * Parses tags from the element XML and returns them in a map
     */
    Map<String, String> parseTags(final org.w3c.dom.Node elementXml) throws Exception {
        log.debug("Parsing element tags...");

        Map<String, String> tags = new HashMap<>();
        try {
            // using xpath api here to get the tags is *very* slow, since it
            // ends up being called many
            // times...just check each child for a tag instead
            final NodeList tagXmlNodes = elementXml.getChildNodes();
            for (int i = 0; i < tagXmlNodes.getLength(); i++) {
                org.w3c.dom.Node tagXmlNode = tagXmlNodes.item(i);
                if (tagXmlNode.getNodeName().equals("tag")) {
                    NamedNodeMap nodeTagAttributes = tagXmlNode.getAttributes();
                    tags.put(nodeTagAttributes.getNamedItem("k").getNodeValue(),
                            nodeTagAttributes.getNamedItem("v").getNodeValue());
                }
            }
        }
        catch (Exception e) {
            throw new Exception("Error parsing tag.");
        }
        return tags;
    }

    /*
     * Adds tags XML to the parent element XML
     */
    org.w3c.dom.Element addTagsXml(org.w3c.dom.Element elementXml) throws Exception {
        Document doc = elementXml.getOwnerDocument();
        Map<String, String> tags = getTags();
        if (tags.isEmpty()) {
            return null;
        }
        for (Map.Entry<String, String> tagEntry : tags.entrySet()) {
            org.w3c.dom.Element tagElement = doc.createElement("tag");
            tagElement.setAttribute("k", tagEntry.getKey());
            tagElement.setAttribute("v",
                    hoot.services.utils.StringUtils.encodeURIComponentForJavaScript(tagEntry.getValue()));
            elementXml.appendChild(tagElement);
        }
        return elementXml;
    }

    public abstract void checkAndFailIfUsedByOtherObjects() throws Exception;
}
