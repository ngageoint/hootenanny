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
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

import org.apache.commons.lang3.StringUtils;
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
import hoot.services.exceptions.osm.OSMAPIAlreadyDeletedException;
import hoot.services.exceptions.osm.OSMAPIPreconditionException;
import hoot.services.geo.BoundingBox;
import hoot.services.geo.GeoUtils;
import hoot.services.geo.QuadTileCalculator;


/**
 * Represents the model for an OSM node
 */
public class Node extends Element {
    private static final Logger log = LoggerFactory.getLogger(Node.class);

    public Node(final Long mapId, Connection dbConnection) throws Exception {
        super(dbConnection);
        super.elementType = ElementType.Node;
        super.record = new CurrentNodes();

        setMapId(mapId);
    }

    public Node(Long mapId, Connection dbConnection, CurrentNodes record) throws Exception {
        super(dbConnection);
        super.elementType = ElementType.Node;

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

        super.record = nodeRecord;

        setMapId(mapId);
    }

    /**
     * Returns the bounds of this element
     * <p>
     * Any change to a node, including deletion, adds the node's new location to
     * the bbox.
     *
     * @return a bounding box
     * @throws Exception
     *             if the element has invalid coordinates
     */
    @Override
    public BoundingBox getBounds() throws Exception {
        CurrentNodes nodeRecord;
        if (record != null) {
            nodeRecord = (CurrentNodes) record;
        }
        else {
            nodeRecord = new SQLQuery(conn, DbUtils.getConfiguration(getMapId())).from(currentNodes)
                    .where(currentNodes.id.eq(getId())).singleResult(currentNodes);

        }
        return new BoundingBox(nodeRecord.getLongitude(), nodeRecord.getLatitude(), nodeRecord.getLongitude(),
                nodeRecord.getLatitude());
    }

    /**
     * Returns the nodes specified in the collection of nodes IDs
     *
     * @param mapId
     *            ID of the map the nodes belong to
     * @param nodeIds
     *            a collection of node IDs
     * @param dbConn
     *            JDBC Connection
     * @return a collection of node records
     */
    static List<CurrentNodes> getNodes(long mapId, Set<Long> nodeIds, Connection dbConn) {
        // This seems redundant when compared to Element::getElementRecords

        if (!nodeIds.isEmpty()) {
            return new SQLQuery(dbConn, DbUtils.getConfiguration(mapId)).from(currentNodes)
                    .where(currentNodes.id.in(nodeIds)).list(currentNodes);
        }
        return new ArrayList<>();
    }

    /**
     * Populates the element model object based on osm diff data
     *
     * @param xml
     *            XML data to construct the element from
     * @throws Exception
     */
    @Override
    public void fromXml(org.w3c.dom.Node xml) throws Exception {
        log.debug("Parsing node...");

        NamedNodeMap xmlAttributes = xml.getAttributes();

        assert (record != null);
        CurrentNodes nodeRecord = (CurrentNodes) record;

        // set these props at the very beginning, b/c they will be needed
        // regardless of whether
        // following checks fail
        nodeRecord.setChangesetId(parseChangesetId(xmlAttributes));
        nodeRecord.setVersion(parseVersion());
        nodeRecord.setTimestamp(parseTimestamp(xmlAttributes));
        nodeRecord.setVisible(true);

        // Lat/lon are required here on a delete request as well, b/c it keeps
        // from having to do a
        // round trip to the db to get the node lat/long before it is deleted,
        // so that can be used
        // to update the changeset bounds (rails port does it this way).
        double latitude = Double.parseDouble(xmlAttributes.getNamedItem("lat").getNodeValue());
        double longitude = Double.parseDouble(xmlAttributes.getNamedItem("lon").getNodeValue());
        if (!GeoUtils.coordsInWorld(latitude, longitude)) {
            throw new Exception("Coordinates for node with ID: " + getId() + " not within world boundary.");
        }

        // If the node is being deleted, we still need to make sure that the
        // coords passed in match
        // what's on the server, since we'll be relying on them to compute the
        // changeset bounds.
        nodeRecord.setLatitude(latitude);
        nodeRecord.setLongitude(longitude);

        // no point in updating the tile if we're not deleting
        if (entityChangeType != EntityChangeType.DELETE) {
            nodeRecord.setTile(QuadTileCalculator.tileForPoint(latitude, longitude));
            nodeRecord.setTags(parseTags(xml));
        }

        setRecord(nodeRecord);
    }

    @Override
    public void checkAndFailIfUsedByOtherObjects() throws Exception {
        if (!super.getVisible()) {
            throw new OSMAPIAlreadyDeletedException("Node with ID = " + super.getId() + " has been already deleted "
                    + "from map with ID = " + getMapId());
        }

        // From the Rails port of OSM API:
        // ways = Way.joins(:way_nodes).where(:visible => true,
        // :current_way_nodes => { :node_id => id }).order(:id)
        SQLQuery owningWaysQuery = new SQLQuery(super.getDbConnection(), DbUtils.getConfiguration(super.getMapId()))
                .distinct().from(currentWays).join(currentWayNodes).on(currentWays.id.eq(currentWayNodes.wayId))
                .where(currentWays.visible.eq(true).and(currentWayNodes.nodeId.eq(super.getId())));

        Set<Long> owningWayIds = new TreeSet<>(owningWaysQuery.list(currentWayNodes.wayId));

        if (!owningWayIds.isEmpty()) {
            throw new OSMAPIPreconditionException("Node with ID = " + super.getId() + " is still used by other way(s): "
                    + StringUtils.join(owningWayIds) + " in map with ID = " + getMapId());
        }

        // From the Rails port of OSM API:
        // rels = Relation.joins(:relation_members).where(:visible => true,
        // :current_relation_members => { :member_type => "Node", :member_id =>
        // id }).
        SQLQuery owningRelationsQuery = new SQLQuery(conn, DbUtils.getConfiguration(getMapId())).distinct()
                .from(currentRelations).join(currentRelationMembers)
                .on(currentRelations.id.eq(currentRelationMembers.relationId))
                .where(currentRelations.visible.eq(true)
                        .and(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.node))
                        .and(currentRelationMembers.memberId.eq(super.getId())));

        Set<Long> owningRelationsIds = new TreeSet<>(owningRelationsQuery.list(currentRelationMembers.relationId));

        if (!owningRelationsIds.isEmpty()) {
            throw new OSMAPIPreconditionException(
                    "Node with ID = " + super.getId() + " is still used by other relation(s): "
                            + StringUtils.join(owningRelationsIds) + " in map with ID = " + getMapId());
        }
    }

    /**
     * Returns an XML representation of the element; does not add tags
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
     *            ignored by Node
     * @return an XML node
     * @throws Exception
     */
    @Override
    public org.w3c.dom.Element toXml(org.w3c.dom.Element parentXml, long modifyingUserId,
            String modifyingUserDisplayName, boolean multiLayerUniqueElementIds, boolean addChildren) throws Exception {
        org.w3c.dom.Element element = super.toXml(parentXml, modifyingUserId, modifyingUserDisplayName,
                multiLayerUniqueElementIds, addChildren);
        CurrentNodes nodeRecord = (CurrentNodes) record;
        if (nodeRecord.getVisible()) {
            element.setAttribute("lat", String.valueOf(nodeRecord.getLatitude()));
            element.setAttribute("lon", String.valueOf(nodeRecord.getLongitude()));
        }

        org.w3c.dom.Element elementWithTags = addTagsXml(element);
        if (elementWithTags == null) {
            return element;
        }

        return elementWithTags;
    }

    /**
     * Returns the generated table identifier for this element
     */
    @Override
    public RelationalPathBase<?> getElementTable() {
        return currentNodes;
    }

    /**
     * Returns the generated ID field for this element
     *
     * @return a table field
     */
    @Override
    public NumberPath<Long> getElementIdField() {
        return currentNodes.id;
    }

    /**
     * Returns the generated visibility field for this element
     *
     * @return a table field
     */
    @Override
    public BooleanPath getElementVisibilityField() {
        return currentNodes.visible;
    }

    /**
     * Returns the generated version field for this element
     *
     * @return a table field
     */
    @Override
    public NumberPath<Long> getElementVersionField() {
        return currentNodes.version;
    }

    /**
     * Returns the generated changeset ID field for this element
     *
     * @return a table field
     */
    @Override
    public NumberPath<Long> getChangesetIdField() {
        return currentNodes.changesetId;
    }

    /**
     * Returns the generated tag field for this element
     *
     * @return a table field
     */
    public SimplePath<Object> getTagField() {
        return currentNodes.tags;
    }

    /**
     * OSM related element type (e.g. way nodes for ways, relation members for
     * relations)
     *
     * @return a list of element types
     */
    @Override
    public List<ElementType> getRelatedElementTypes() {
        return null;
    }

    /**
     * Returns the generated table identifier for records related to this
     * element
     *
     * @return a table
     */
    @Override
    public RelationalPathBase<?> getRelatedRecordTable() {
        return null;
    }

    /**
     * Returns the table field in the related record table that can be joined
     * with the parent element record table
     *
     * @return a table field
     */
    @Override
    public NumberPath<Long> getRelatedRecordJoinField() {
        return null;
    }

    /**
     * Returns the table field in the related record table containing the map ID
     *
     * @return a table field
     */
    public NumberPath<Long> getRelatedRecordMapIdField() {
        return null;
    }

    /**
     * Inserts a new node into the services database
     *
     * @param changesetId
     *            corresponding changeset ID for the node to be inserted
     * @param mapId
     *            corresponding map ID for the node to be inserted
     * @param latitude
     *            latitude coordinate for the node to be inserted
     * @param longitude
     *            longitude coordinate for the node to be inserted
     * @param tags
     *            element tags
     * @param conn
     *            JDBC Connection
     * @return ID of the newly created node
     * @throws Exception
     */
    public static long insertNew(long changesetId, long mapId, double latitude, double longitude,
            java.util.Map<String, String> tags, Connection conn) throws Exception {
        long nextNodeId = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .uniqueResult(SQLExpressions.nextval(Long.class, "current_nodes_id_seq"));
        insertNew(nextNodeId, changesetId, mapId, latitude, longitude, tags, conn);
        return nextNodeId;
    }

    /**
     * Inserts a new node into the services database with the specified ID;
     * useful for testing
     *
     * @param nodeId
     *            ID to assign to the new node
     * @param changesetId
     *            corresponding changeset ID for the node to be inserted
     * @param mapId
     *            corresponding map ID for the node to be inserted
     * @param latitude
     *            latitude coordinate for the node to be inserted
     * @param longitude
     *            longitude coordinate for the node to be inserted
     * @param tags
     *            element tags
     * @param conn
     *            JDBC Connection
     * @throws Exception
     */
    public static void insertNew(long nodeId, long changesetId, long mapId, double latitude, double longitude,
            java.util.Map<String, String> tags, Connection conn) throws Exception {
        // querydsl does not support hstore so using jdbc

        String strKv = "";
        if (tags != null) {
            Iterator it = tags.entrySet().iterator();
            while (it.hasNext()) {
                Map.Entry pairs = (Map.Entry) it.next();
                String key = "\"" + pairs.getKey() + "\"";
                String val = "\"" + pairs.getValue() + "\"";
                if (strKv.length() > 0) {
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
        try {
            Class.forName(POSTGRESQL_DRIVER);

            stmt = conn.createStatement();

            String sql = "INSERT INTO current_nodes_" + mapId + "(\n"
                    + "            id, latitude, longitude, changeset_id,  visible, \"timestamp\", tile, version, tags)\n"
                    + " VALUES(" + nodeId + "," + latitude + "," + longitude + "," + changesetId + "," + "true" + ","
                    + "CURRENT_TIMESTAMP" + "," + QuadTileCalculator.tileForPoint(latitude, longitude) + "," + "1" + ","
                    + strTags +

                    ")";
            stmt.executeUpdate(sql);
        }
        catch (Exception e) {
            throw new Exception("Error inserting node.");
        }
        finally {
            if (stmt != null) {
                stmt.close();
            }
        }
    }
}
