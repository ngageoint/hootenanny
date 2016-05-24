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

import java.sql.Connection;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

import org.apache.commons.lang3.StringUtils;
import org.apache.xpath.XPathAPI;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.NodeList;

import com.mysema.query.Tuple;
import com.mysema.query.sql.RelationalPathBase;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.types.path.BooleanPath;
import com.mysema.query.types.path.NumberPath;
import com.mysema.query.types.path.SimplePath;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.db.DbUtils.EntityChangeType;
import hoot.services.db.DbUtils.nwr_enum;
import hoot.services.db2.CurrentNodes;
import hoot.services.db2.CurrentWayNodes;
import hoot.services.db2.CurrentWays;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.exceptions.osm.OSMAPIAlreadyDeletedException;
import hoot.services.exceptions.osm.OSMAPIPreconditionException;
import hoot.services.geo.BoundingBox;
import hoot.services.geo.Coordinates;


/**
 * Represents the model for an OSM way
 */
public class Way extends Element {
    private static final Logger log = LoggerFactory.getLogger(Way.class);

    private final List<Long> wayNodeIdsCache = new ArrayList<>();

    List<Long> getWayNodeIdsCache() {
        return wayNodeIdsCache;
    }

    private static final QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;

    // temp collection of way node coordinates used to calculate the way's
    // bounds
    private Map<Long, Coordinates> nodeCoordsCollection;

    public Way(long mapId, Connection dbConn) throws Exception {
        super(dbConn);
        super.elementType = ElementType.Way;
        super.record = new CurrentWays();

        setMapId(mapId);
    }

    public Way(long mapId, Connection dbConn, CurrentWays record) throws Exception {
        super(dbConn);
        super.elementType = ElementType.Way;

        CurrentWays wayRecord = new CurrentWays();
        wayRecord.setChangesetId(record.getChangesetId());
        wayRecord.setId(record.getId());
        wayRecord.setTimestamp(record.getTimestamp());
        wayRecord.setVersion(record.getVersion());
        wayRecord.setVisible(record.getVisible());
        wayRecord.setTags(record.getTags());
        super.record = wayRecord;
        setMapId(mapId);
    }

    /**
     * Returns all node records for the specified ways from the services
     * database
     *
     * @param mapId
     *            ID of the map owning the ways
     * @param wayIds
     *            a collection of way IDs for which to retrieve node records
     * @param dbConn
     *            JDBC Connection
     * @return a list of node records
     */
    static List<CurrentNodes> getNodesForWays(long mapId, Set<Long> wayIds, Connection dbConn) {
        if (!wayIds.isEmpty()) {
            return new SQLQuery(dbConn, DbUtils.getConfiguration(mapId)).from(currentWayNodes).join(currentNodes)
                    .on(currentWayNodes.nodeId.eq(currentNodes.id)).where(currentWayNodes.wayId.in(wayIds))
                    .list(currentNodes);
        }
        return new ArrayList<>();
    }

    /*
     * Returns the nodes associated with this way
     */
    private List<CurrentNodes> getNodes() throws Exception {
        return new SQLQuery(conn, DbUtils.getConfiguration(getMapId())).from(currentWayNodes).join(currentNodes)
                .on(currentWayNodes.nodeId.eq(currentNodes.id)).where(currentWayNodes.wayId.eq(getId()))
                .orderBy(currentWayNodes.sequenceId.asc()).list(currentNodes);
    }

    /*
     * Returns the IDs of the nodes associated with this way
     *
     * This is a List, rather than a Set, since the same node ID can be used for
     * the first and last node ID in the way nodes sequence for closed polygons.
     */
    private List<Long> getNodeIds() throws Exception {
        return new SQLQuery(conn, DbUtils.getConfiguration(getMapId())).from(currentWayNodes)
                .where(currentWayNodes.wayId.eq(getId())).orderBy(currentWayNodes.sequenceId.asc())
                .list(currentWayNodes.nodeId);
    }

    /*
     * First calculates the bounds for all nodes belonging to this way that were
     * referenced explicitly in the changeset upload request. Then calculates
     * the bounds for all the way's nodes not mentioned in the request after
     * retrieving them from the services database. The bounds returned is a sum
     * of the two calculated bounds.
     */
    private BoundingBox getBoundsFromRequestDataAndRemainderFromDatabase() throws Exception {
        double minLon = BoundingBox.LON_LIMIT + 1;
        double minLat = BoundingBox.LAT_LIMIT + 1;
        double maxLon = (-1 * BoundingBox.LON_LIMIT) - 1;
        double maxLat = (-1 * BoundingBox.LAT_LIMIT) - 1;

        // assert(relatedRecordIds != null && relatedRecordIds.size() >= 2);

        // nodes that were parsed in the same request referencing this way;
        // either
        // as a create or modify
        Set<Long> modifiedRecordIds = new HashSet<>(relatedRecordIds);
        for (long wayNodeId : relatedRecordIds) {
            assert (nodeCoordsCollection.containsKey(wayNodeId));
            double lat = nodeCoordsCollection.get(wayNodeId).lat;
            double lon = nodeCoordsCollection.get(wayNodeId).lon;
            if (lat < minLat) {
                minLat = lat;
            }
            if (lat > maxLat) {
                maxLat = lat;
            }
            if (lon < minLon) {
                minLon = lon;
            }
            if (lon > maxLon) {
                maxLon = lon;
            }

            modifiedRecordIds.remove(wayNodeId);
        }

        // any way nodes not mentioned in the created/modified in the changeset
        // XML
        // represented by
        // the remainder of the IDs in relatedRecordIds, request must now be
        // retrieved from the
        // database
        List<?> nodeRecords = Element.getElementRecords(getMapId(), ElementType.Node, modifiedRecordIds, conn);
        for (Object record : nodeRecords) {
            CurrentNodes nodeRecord = (CurrentNodes) record;
            double lat = nodeRecord.getLatitude();
            double lon = nodeRecord.getLongitude();
            if (lat < minLat) {
                minLat = lat;
            }
            if (lat > maxLat) {
                maxLat = lat;
            }
            if (lon < minLon) {
                minLon = lon;
            }
            if (lon > maxLon) {
                maxLon = lon;
            }

            assert (modifiedRecordIds.contains(nodeRecord.getId()));
            modifiedRecordIds.remove(nodeRecord.getId());
        }

        assert (modifiedRecordIds.isEmpty());

        return new BoundingBox(minLon, minLat, maxLon, maxLat);
    }

    /**
     * Returns the bounds of this element
     * <p>
     * Any change to a way, including deletion, adds all of the way's nodes to
     * the bbox.
     *
     * @return a bounding box
     * @throws Exception
     */
    @Override
    public BoundingBox getBounds() throws Exception {
        // this is a little kludgy, but...first see if the related record data
        // (waynode data) is left
        // over from the XML parsing (clearTempData clears it out). If it is
        // still here, use it
        // because the way nodes will not have been written to the database yet,
        // so use the cached
        // way node IDs and node coordinate info to construct the bounds
        if ((relatedRecordIds != null) && (!relatedRecordIds.isEmpty())) {
            return getBoundsFromRequestDataAndRemainderFromDatabase();
        }

        // If no temp related record data is present (hasn't been cleared out),
        // the
        // way nodes data
        // for this way must be in the services database and up to date, so get
        // it
        // from there.
        return new BoundingBox(new ArrayList<>(getNodes()));
    }

    /**
     * Populates this element model object based on osm diff data
     *
     * @param xml
     *            xml data to construct the element from
     * @throws Exception
     */
    @Override
    public void fromXml(org.w3c.dom.Node xml) throws Exception {
        log.debug("Parsing way...");

        NamedNodeMap xmlAttributes = xml.getAttributes();

        assert (super.record != null);

        CurrentWays wayRecord = (CurrentWays) super.record;
        wayRecord.setChangesetId(parseChangesetId(xmlAttributes));
        wayRecord.setVersion(parseVersion());
        wayRecord.setTimestamp(parseTimestamp(xmlAttributes));
        wayRecord.setVisible(true);

        if (entityChangeType != EntityChangeType.DELETE) {
            wayRecord.setTags(parseTags(xml));
        }

        super.setRecord(wayRecord);

        // if we're deleting the way, all the way nodes will get deleted
        // automatically...and no new
        // ones need to be parsed
        if (entityChangeType != EntityChangeType.DELETE) {
            parseWayNodesXml(xml);
        }
    }

    @Override
    public void checkAndFailIfUsedByOtherObjects() throws Exception {
        if (!super.getVisible()) {
            throw new OSMAPIAlreadyDeletedException("Way with ID = " + super.getId() + " has been already deleted "
                    + "from map with ID = " + getMapId());
        }

        // From the Rails port of OSM API:
        // rels = Relation.joins(:relation_members).where(:visible => true,
        // :current_relation_members => { :member_type => "Way", :member_id =>
        // id }).
        SQLQuery owningRelationsQuery = new SQLQuery(conn, DbUtils.getConfiguration(getMapId())).distinct()
                .from(currentRelations).join(currentRelationMembers)
                .on(currentRelations.id.eq(currentRelationMembers.relationId))
                .where(currentRelations.visible.eq(true).and(currentRelationMembers.memberType.eq(nwr_enum.way))
                        .and(currentRelationMembers.memberId.eq(super.getId())));

        Set<Long> owningRelationsIds = new TreeSet<>(owningRelationsQuery.list(currentRelationMembers.relationId));

        if (!owningRelationsIds.isEmpty()) {
            throw new OSMAPIPreconditionException(
                    "Node with ID = " + super.getId() + " is still used by other relation(s): "
                            + StringUtils.join(owningRelationsIds) + " from map with ID = " + getMapId());
        }
    }

    /**
     * Returns an XML representation of the element returned in a query; does
     * not add tags; assumes way nodes have already been written to the services
     * db
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
     *            if true, element children are added to the element xml
     * @return an XML element
     * @throws Exception
     */
    @Override
    public org.w3c.dom.Element toXml(org.w3c.dom.Element parentXml, long modifyingUserId,
            String modifyingUserDisplayName, boolean multiLayerUniqueElementIds, boolean addChildren) throws Exception {
        org.w3c.dom.Element element = super.toXml(parentXml, modifyingUserId, modifyingUserDisplayName,
                multiLayerUniqueElementIds, addChildren);
        Document doc = parentXml.getOwnerDocument();

        if (addChildren) {
            List<Long> nodeIds = getNodeIds();
            Set<Long> elementIds = new HashSet<>();

            // way nodes are output in sequence order; list should already be
            // sorted
            // by the query
            for (long nodeId : nodeIds) {
                org.w3c.dom.Element nodeElement = doc.createElement("nd");
                nodeElement.setAttribute("ref", String.valueOf(nodeId));
                element.appendChild(nodeElement);
                elementIds.add(nodeId);
            }

            @SuppressWarnings("unchecked")
            List<Tuple> elementRecords = (List<Tuple>) Element.getElementRecordsWithUserInfo(getMapId(),
                    ElementType.Node, elementIds, conn);

            for (Tuple elementRecord : elementRecords) {
                Element nodeFullElement = ElementFactory.getInstance().create(ElementType.Node, elementRecord, conn,
                        getMapId());
                org.w3c.dom.Element nodeXml = nodeFullElement.toXml(parentXml, modifyingUserId,
                        modifyingUserDisplayName, false, false);
                parentXml.appendChild(nodeXml);
            }
        }

        org.w3c.dom.Element elementWithTags = addTagsXml(element);
        if (elementWithTags == null) {
            return element;
        }

        return elementWithTags;
    }

    private void validateWayNodesSize(NodeList wayNodesXml) throws Exception {
        if (entityChangeType != EntityChangeType.DELETE) {
            CurrentWays wayRecord = (CurrentWays) record;
            long maximumWayNodes = Long.parseLong(HootProperties.getPropertyOrDefault("maximumWayNodes"));

            long numWayNodes = wayNodesXml.getLength();
            if (numWayNodes < 2) {
                throw new Exception("Too few nodes specified for way with ID: " + wayRecord.getId());
            }
            else if (numWayNodes > maximumWayNodes) {
                throw new Exception("Too many nodes specified for way with ID: " + wayRecord.getId());
            }
        }
    }

    private long parseWayNode(org.w3c.dom.Node nodeXml) throws Exception {
        NamedNodeMap nodeXmlAttributes = nodeXml.getAttributes();

        long parsedNodeId = Long.parseLong(nodeXmlAttributes.getNamedItem("ref").getNodeValue());
        wayNodeIdsCache.add(parsedNodeId);

        Coordinates nodeCoords = new Coordinates();

        Map<Long, Element> parsedNodes = parsedElementIdsToElementsByType.get(ElementType.Node);

        // if this is a node created within the same request that is referencing
        // this way, it won't
        // exist in the database, but it will be in the element cache created
        // when parsing the node
        // from the request
        if (parsedNodeId < 0) {
            // assert(parsedNodes.containsKey(parsedNodeId));
            if (!parsedNodes.containsKey(parsedNodeId)) {
                // TODO: add test for this
                throw new Exception(
                        "Created way references new node not found in create request with ID: " + parsedNodeId);
            }
        }

        // The node is referenced somewhere else in this request, so get its
        // info from the request, not
        // the database b/c the database either won't have it or will have
        // outdated info. Only get info
        // from the request if the node is being created/modified, as if it is
        // being deleted, we can
        // just get the info from the database since its coords won't be
        // changing and might not be in
        // the request (not required).
        long actualNodeId;
        if (parsedNodes.containsKey(parsedNodeId)
                && (parsedNodes.get(parsedNodeId).getEntityChangeType() != EntityChangeType.DELETE)) {
            Node node = (Node) parsedElementIdsToElementsByType.get(ElementType.Node).get(parsedNodeId);
            CurrentNodes nodeRecord = (CurrentNodes) node.getRecord();
            actualNodeId = nodeRecord.getId();
            nodeCoords.lat = nodeRecord.getLatitude();
            nodeCoords.lon = nodeRecord.getLongitude();
        }

        // element not referenced in this request, so should already exist in
        // the db and its info be up
        // to date
        else {
            actualNodeId = parsedNodeId;
            CurrentNodes existingNodeRecord = dbNodeCache.get(actualNodeId);

            if (existingNodeRecord == null) {
                throw new Exception("Node with ID: " + actualNodeId + " does not exist for way.");
            }

            if (!existingNodeRecord.getVisible()) {
                throw new Exception("Node with ID: " + actualNodeId + " is not visible for way.");
            }

            nodeCoords.lat = existingNodeRecord.getLatitude();
            nodeCoords.lon = existingNodeRecord.getLongitude();
        }

        assert (actualNodeId > 0);
        nodeCoordsCollection.put(actualNodeId, nodeCoords);

        return actualNodeId;
    }

    private CurrentWayNodes createWayNodeRecord(long actualNodeId, long sequenceId) throws Exception {
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
    private void parseWayNodesXml(org.w3c.dom.Node xml) throws Exception {
        assert (parsedElementIdsToElementsByType != null);

        NodeList wayNodesXml = XPathAPI.selectNodeList(xml, "nd");

        validateWayNodesSize(wayNodesXml);

        relatedRecords = new ArrayList<>();
        relatedRecordIds = new HashSet<>();
        nodeCoordsCollection = new HashMap<>();

        for (int i = 0; i < wayNodesXml.getLength(); i++) {
            org.w3c.dom.Node nodeXml = wayNodesXml.item(i);
            long actualNodeId = parseWayNode(nodeXml);
            relatedRecordIds.add(actualNodeId);
            relatedRecords.add(createWayNodeRecord(actualNodeId, (long) (i + 1)));
        }
    }

    /**
     * Returns the generated table identifier for this element
     *
     * @return a table
     */
    @Override
    public RelationalPathBase<?> getElementTable() {
        return currentWays;
    }

    /**
     * Returns the generated ID field for this element
     *
     * @return a table field
     */
    @Override
    public NumberPath<Long> getElementIdField() {
        return currentWays.id;
    }

    /**
     * Returns the generated visibility field for this element
     *
     * @return a table field
     */
    @Override
    public BooleanPath getElementVisibilityField() {
        return currentWays.visible;
    }

    /**
     * Returns the generated version field for this element
     *
     * @return a table field
     */
    @Override
    public NumberPath<Long> getElementVersionField() {
        return currentWays.version;
    }

    /**
     * Returns the generated changeset ID field for this element
     *
     * @return a table field
     */
    @Override
    public NumberPath<Long> getChangesetIdField() {
        return currentWays.changesetId;
    }

    /**
     * Returns the generated table identifier for records related to this
     * element
     *
     * @return a table
     */
    @Override
    public RelationalPathBase<?> getRelatedRecordTable() {
        return currentWayNodes;
    }

    /**
     * Returns the table field in the related record table that can be joined
     * with the parent element record table
     *
     * @return a table field
     */
    @Override
    public NumberPath<Long> getRelatedRecordJoinField() {
        return currentWayNodes.wayId;
    }

    /**
     * Returns the generated tag field for this element
     *
     * @return a table field
     */
    public SimplePath<Object> getTagField() {
        return currentWays.tags;
    }

    /**
     * OSM related element type (e.g. way nodes for ways, relation members for
     * relations)
     *
     * @return a list of element types
     */
    @Override
    public List<ElementType> getRelatedElementTypes() {
        return Collections.singletonList(ElementType.Node);
    }
}
