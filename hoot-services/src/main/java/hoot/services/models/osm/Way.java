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

import static hoot.services.HootProperties.MAXIMUM_WAY_NODES;
import static hoot.services.models.db.QCurrentNodes.currentNodes;
import static hoot.services.models.db.QCurrentRelationMembers.currentRelationMembers;
import static hoot.services.models.db.QCurrentRelations.currentRelations;
import static hoot.services.models.db.QCurrentWayNodes.currentWayNodes;
import static hoot.services.models.db.QCurrentWays.currentWays;
import static hoot.services.utils.DbUtils.createQuery;
import static org.apache.commons.lang3.StringUtils.join;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.xml.transform.TransformerException;

import org.apache.xpath.XPathAPI;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.NodeList;

import com.querydsl.core.Tuple;
import com.querydsl.core.types.Projections;
import com.querydsl.core.types.dsl.BooleanPath;
import com.querydsl.core.types.dsl.NumberPath;
import com.querydsl.core.types.dsl.SimplePath;
import com.querydsl.sql.RelationalPathBase;

import hoot.services.geo.BoundingBox;
import hoot.services.geo.Coordinates;
import hoot.services.models.db.CurrentNodes;
import hoot.services.models.db.CurrentWayNodes;
import hoot.services.models.db.CurrentWays;
import hoot.services.utils.DbUtils;
import hoot.services.utils.DbUtils.EntityChangeType;


/**
 * Represents the model for an OSM way
 */
public class Way extends Element {
    private static final Logger logger = LoggerFactory.getLogger(Way.class);

    private final List<Long> wayNodeIdsCache = new ArrayList<>();

    // temp collection of way node coordinates used to calculate the way'sbounds
    private Map<Long, Coordinates> nodeCoordsCollection;


    public Way(long mapId) {
        super.elementType = ElementType.Way;
        super.record = new CurrentWays();

        setMapId(mapId);
    }

    public Way(long mapId, CurrentWays record) {
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

    List<Long> getWayNodeIdsCache() {
        return wayNodeIdsCache;
    }

    /*
     * Returns the IDs of the nodes associated with this way
     *
     * This is a List, rather than a Set, since the same node ID can be used for
     * the first and last node ID in the way nodes sequence for closed polygons.
     */
    private List<Long> getNodeIds() {
        return createQuery(getMapId())
                .select(currentWayNodes.nodeId)
                .from(currentWayNodes)
                .where(currentWayNodes.wayId.eq(getId()))
                .orderBy(currentWayNodes.sequenceId.asc())
                .fetch();
    }

    /*
     * First calculates the bounds for all nodes belonging to this way that were
     * referenced explicitly in the changeset upload request. Then calculates
     * the bounds for all the way's nodes not mentioned in the request after
     * retrieving them from the services database. The bounds returned is a sum
     * of the two calculated bounds.
     */
    private BoundingBox getBoundsFromRequestDataAndRemainderFromDatabase() {
        double minLon = BoundingBox.LON_LIMIT + 1;
        double minLat = BoundingBox.LAT_LIMIT + 1;
        double maxLon = (-1 * BoundingBox.LON_LIMIT) - 1;
        double maxLat = (-1 * BoundingBox.LAT_LIMIT) - 1;

        // nodes that were parsed in the same request referencing this way;
        // either as a create or modify
        Set<Long> modifiedRecordIds = new HashSet<>(relatedRecordIds);
        for (long wayNodeId : relatedRecordIds) {
            double lat = nodeCoordsCollection.get(wayNodeId).getLat();
            double lon = nodeCoordsCollection.get(wayNodeId).getLon();
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
        // XML represented by the remainder of the IDs in relatedRecordIds, request must now be
        // retrieved from the database
        List<?> nodeRecords = Element.getElementRecords(getMapId(), ElementType.Node, modifiedRecordIds);
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

            modifiedRecordIds.remove(nodeRecord.getId());
        }

        return new BoundingBox(minLon, minLat, maxLon, maxLat);
    }

    /**
     * Returns the bounds of this element
     * <p>
     * Any change to a way, including deletion, adds all of the way's nodes to
     * the bbox.
     *
     * @return a bounding box
     */
    @Override
    public BoundingBox getBounds() {
        // this is a little kludgy, but...first see if the related record data
        // (waynode data) is left over from the XML parsing (clearTempData clears it out). If it is
        // still here, use it because the way nodes will not have been written to the database yet,
        // so use the cached way node IDs and node coordinate info to construct the bounds
        if ((relatedRecordIds != null) && (!relatedRecordIds.isEmpty())) {
            return getBoundsFromRequestDataAndRemainderFromDatabase();
        }

        // Returns all nodes associated with this way
        List<CurrentNodes> nodes = createQuery(getMapId())
                .select(Projections.bean(CurrentNodes.class, currentNodes.latitude, currentNodes.longitude))
                .from(currentWayNodes)
                .join(currentNodes).on(currentWayNodes.nodeId.eq(currentNodes.id))
                .where(currentWayNodes.wayId.eq(getId()))
                .orderBy(currentWayNodes.sequenceId.asc())
                .fetch();

        // If no temp related record data is present (hasn't been cleared out),
        // the way nodes data for this way must be in the services database and up to date, so get it from there.
        return new BoundingBox(nodes);
    }

    /**
     * Populates this element model object based on osm diff data
     *
     * @param xml
     *            xml data to construct the element from
     */
    @Override
    public void fromXml(org.w3c.dom.Node xml) {
        NamedNodeMap xmlAttributes = xml.getAttributes();

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
        // automatically...and no new ones need to be parsed
        if (entityChangeType != EntityChangeType.DELETE) {
            parseWayNodesXml(xml);
        }
    }

    @Override
    public void checkAndFailIfUsedByOtherObjects() throws OSMAPIAlreadyDeletedException, OSMAPIPreconditionException {
        if (!super.getVisible()) {
            throw new OSMAPIAlreadyDeletedException("Way with ID = " + super.getId() + " has been already deleted "
                    + "from map with ID = " + getMapId());
        }

        // From the Rails port of OSM API:
        // rels = Relation.joins(:relation_members).where(:visible => true,
        // :current_relation_members => { :member_type => "Way", :member_id => id }).
        List<Long> owningRelationsIds = createQuery(getMapId())
                .select(currentRelationMembers.relationId)
                .distinct()
                .from(currentRelations, currentRelationMembers)
                .join(currentRelationMembers).on(currentRelations.id.eq(currentRelationMembers.relationId))
                .where(currentRelations.visible.eq(true)
                        .and(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.way))
                        .and(currentRelationMembers.memberId.eq(super.getId())))
                .orderBy(currentRelationMembers.relationId.asc())
                .fetch();

        if (!owningRelationsIds.isEmpty()) {
            throw new OSMAPIPreconditionException(
                    "Node with ID = " + super.getId() + " is still used by other relation(s): "
                            + join(owningRelationsIds) + " from map with ID = " + getMapId());
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
     */
    @Override
    public org.w3c.dom.Element toXml(org.w3c.dom.Element parentXml, long modifyingUserId,
            String modifyingUserDisplayName, boolean multiLayerUniqueElementIds, boolean addChildren) {
        org.w3c.dom.Element element = super.toXml(parentXml, modifyingUserId, modifyingUserDisplayName,
                multiLayerUniqueElementIds, addChildren);
        Document doc = parentXml.getOwnerDocument();

        if (addChildren) {
            List<Long> nodeIds = getNodeIds();
            Set<Long> elementIds = new HashSet<>();

            // way nodes are output in sequence order; list should already be sorted by the query
            for (long nodeId : nodeIds) {
                org.w3c.dom.Element nodeElement = doc.createElement("nd");
                nodeElement.setAttribute("ref", String.valueOf(nodeId));
                element.appendChild(nodeElement);
                elementIds.add(nodeId);
            }

            List<Tuple> elementRecords = (List<Tuple>) Element.getElementRecordsWithUserInfo(getMapId(),
                    ElementType.Node, elementIds);

            for (Tuple elementRecord : elementRecords) {
                Element nodeFullElement = ElementFactory.create(ElementType.Node, elementRecord, getMapId());
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

    private void validateWayNodesSize(NodeList wayNodesXml) {
        if (entityChangeType != EntityChangeType.DELETE) {
            CurrentWays wayRecord = (CurrentWays) record;
            long maximumWayNodes = Long.parseLong(MAXIMUM_WAY_NODES);

            long numWayNodes = wayNodesXml.getLength();
            if (numWayNodes < 2) {
                throw new IllegalArgumentException("Too few nodes specified for way with ID: " + wayRecord.getId());
            }
            else if (numWayNodes > maximumWayNodes) {
                throw new IllegalArgumentException("Too many nodes specified for way with ID: " + wayRecord.getId());
            }
        }
    }

    private long parseWayNode(org.w3c.dom.Node nodeXml) {
        NamedNodeMap nodeXmlAttributes = nodeXml.getAttributes();

        long parsedNodeId = Long.parseLong(nodeXmlAttributes.getNamedItem("ref").getNodeValue());
        wayNodeIdsCache.add(parsedNodeId);

        Coordinates nodeCoords = new Coordinates();

        Map<Long, Element> parsedNodes = parsedElementIdsToElementsByType.get(ElementType.Node);

        // if this is a node created within the same request that is referencing
        // this way, it won't exist in the database, but it will be in the element cache created
        // when parsing the node from the request
        if (parsedNodeId < 0) {
            if (!parsedNodes.containsKey(parsedNodeId)) {
                // TODO: add test for this
                throw new IllegalArgumentException("Created way references new node not " +
                        "found in create request with ID: " + parsedNodeId);
            }
        }

        // The node is referenced somewhere else in this request, so get its
        // info from the request, not the database b/c the database either won't have it or will have
        // outdated info. Only get info from the request if the node is being created/modified, as if it is
        // being deleted, we can just get the info from the database since its coords won't be
        // changing and might not be in the request (not required).
        long actualNodeId;
        if (parsedNodes.containsKey(parsedNodeId)
                && (parsedNodes.get(parsedNodeId).getEntityChangeType() != EntityChangeType.DELETE)) {
            Node node = (Node) parsedElementIdsToElementsByType.get(ElementType.Node).get(parsedNodeId);
            CurrentNodes nodeRecord = (CurrentNodes) node.getRecord();
            actualNodeId = nodeRecord.getId();
            nodeCoords.setLat(nodeRecord.getLatitude());
            nodeCoords.setLon(nodeRecord.getLongitude());
        }
        else {
            // element not referenced in this request, so should already exist in
            // the db and its info be up to date

            actualNodeId = parsedNodeId;
            CurrentNodes existingNodeRecord = dbNodeCache.get(actualNodeId);

            if (existingNodeRecord == null) {
                throw new IllegalStateException("Node with ID: " + actualNodeId + " does not exist for way.");
            }

            if (!existingNodeRecord.getVisible()) {
                throw new IllegalStateException("Node with ID: " + actualNodeId + " is not visible for way.");
            }

            nodeCoords.setLat(existingNodeRecord.getLatitude());
            nodeCoords.setLon(existingNodeRecord.getLongitude());
        }

        nodeCoordsCollection.put(actualNodeId, nodeCoords);

        return actualNodeId;
    }

    private CurrentWayNodes createWayNodeRecord(long actualNodeId, long sequenceId) {
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
    private void parseWayNodesXml(org.w3c.dom.Node xml) {
        NodeList wayNodesXml;
        try {
            wayNodesXml = XPathAPI.selectNodeList(xml, "nd");
        }
        catch (TransformerException e) {
            throw new RuntimeException("Error selecting XML node 'nd'!", e);
        }

        validateWayNodesSize(wayNodesXml);

        relatedRecords = new ArrayList<>();
        relatedRecordIds = new HashSet<>();
        nodeCoordsCollection = new HashMap<>();

        for (int i = 0; i < wayNodesXml.getLength(); i++) {
            org.w3c.dom.Node nodeXml = wayNodesXml.item(i);
            long actualNodeId = parseWayNode(nodeXml);
            relatedRecordIds.add(actualNodeId);
            relatedRecords.add(createWayNodeRecord(actualNodeId, (i + 1)));
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
