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

import static hoot.services.models.db.QCurrentNodes.currentNodes;
import static hoot.services.models.db.QCurrentRelationMembers.currentRelationMembers;
import static hoot.services.models.db.QCurrentRelations.currentRelations;
import static hoot.services.models.db.QCurrentWayNodes.currentWayNodes;
import static hoot.services.models.db.QCurrentWays.currentWays;
import static hoot.services.utils.DbUtils.createQuery;

import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.NamedNodeMap;

import com.querydsl.core.types.dsl.BooleanPath;
import com.querydsl.core.types.dsl.NumberPath;
import com.querydsl.core.types.dsl.SimplePath;
import com.querydsl.sql.RelationalPathBase;

import hoot.services.geo.BoundingBox;
import hoot.services.models.db.CurrentNodes;
import hoot.services.utils.DbUtils;
import hoot.services.utils.DbUtils.EntityChangeType;
import hoot.services.utils.GeoUtils;
import hoot.services.utils.QuadTileCalculator;


/**
 * Represents the model for an OSM node
 */
public class Node extends Element {
    private static final Logger logger = LoggerFactory.getLogger(Node.class);

    public Node(Long mapId) {
        super.elementType = ElementType.Node;
        super.record = new CurrentNodes();
        setMapId(mapId);
    }

    public Node(Long mapId, CurrentNodes record) {
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
     */
    @Override
    public BoundingBox getBounds() {
        CurrentNodes nodeRecord;

        if (record != null) {
            nodeRecord = (CurrentNodes) record;
        }
        else {
            nodeRecord = createQuery(getMapId())
                    .select(currentNodes)
                    .from(currentNodes)
                    .where(currentNodes.id.eq(getId()))
                    .fetchOne();
        }

        return new BoundingBox(nodeRecord.getLongitude(), nodeRecord.getLatitude(), nodeRecord.getLongitude(),
                nodeRecord.getLatitude());
    }

    /**
     * Populates the element model object based on osm diff data
     *
     * @param xml
     *            XML data to construct the element from
     */
    @Override
    public void fromXml(org.w3c.dom.Node xml) {
        NamedNodeMap xmlAttributes = xml.getAttributes();

        CurrentNodes nodeRecord = (CurrentNodes) record;

        // set these props at the very beginning, b/c they will be needed
        // regardless of whether following checks fail
        nodeRecord.setChangesetId(parseChangesetId(xmlAttributes));
        nodeRecord.setVersion(parseVersion());
        nodeRecord.setTimestamp(parseTimestamp(xmlAttributes));
        nodeRecord.setVisible(true);

        // Lat/lon are required here on a delete request as well, b/c it keeps
        // from having to do a round trip to the db to get the node lat/long before it is deleted,
        // so that can be used to update the changeset bounds (rails port does it this way).
        double latitude = Double.parseDouble(xmlAttributes.getNamedItem("lat").getNodeValue());
        double longitude = Double.parseDouble(xmlAttributes.getNamedItem("lon").getNodeValue());
        if (!GeoUtils.coordsInWorld(latitude, longitude)) {
            throw new RuntimeException("Coordinates for node with ID: " + getId() + " not within world boundary.");
        }

        // If the node is being deleted, we still need to make sure that the coords passed in match what's on the
        // server, since we'll be relying on them to compute the changeset bounds.
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
    public void checkAndFailIfUsedByOtherObjects() throws OSMAPIAlreadyDeletedException, OSMAPIPreconditionException {
        if (!super.getVisible()) {
            throw new OSMAPIAlreadyDeletedException("Node with ID = " + super.getId() + " has been already deleted "
                    + "from map with ID = " + getMapId());
        }

        // From the Rails port of OSM API:
        // ways = Way.joins(:way_nodes).where(:visible => true, :current_way_nodes => { :node_id => id }).order(:id)
        List<Long> owningWayIds = createQuery(super.getMapId())
                .select(currentWayNodes.wayId)
                .distinct()
                .from(currentWays)
                .join(currentWayNodes).on(currentWays.id.eq(currentWayNodes.wayId))
                .where(currentWays.visible.eq(true).and(currentWayNodes.nodeId.eq(super.getId())))
                .orderBy(currentWayNodes.wayId.asc())
                .fetch();

        if (!owningWayIds.isEmpty()) {
            throw new OSMAPIPreconditionException("Node with ID = " + super.getId() + " is still used by other way(s): "
                    + StringUtils.join(owningWayIds) + " in map with ID = " + getMapId());
        }

        // From the Rails port of OSM API:
        // rels = Relation.joins(:relation_members).where(:visible => true,
        // :current_relation_members => { :member_type => "Node", :member_id => id }).
        List<Long> owningRelationsIds = createQuery(getMapId())
                .select(currentRelationMembers.relationId)
                .distinct()
                .from(currentRelations)
                .join(currentRelationMembers).on(currentRelations.id.eq(currentRelationMembers.relationId))
                .where(currentRelations.visible.eq(true)
                        .and(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.node))
                        .and(currentRelationMembers.memberId.eq(super.getId())))
                .orderBy(currentRelationMembers.relationId.asc())
                .fetch();

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
     */
    @Override
    public org.w3c.dom.Element toXml(org.w3c.dom.Element parentXml, long modifyingUserId,
            String modifyingUserDisplayName, boolean multiLayerUniqueElementIds, boolean addChildren) {

        org.w3c.dom.Element element = super.toXml(parentXml, modifyingUserId, modifyingUserDisplayName,
                multiLayerUniqueElementIds, addChildren);

        CurrentNodes nodeRecord = (CurrentNodes) record;
        if (nodeRecord.getVisible()) {
            element.setAttribute("lat", String.valueOf(nodeRecord.getLatitude()));
            element.setAttribute("lon", String.valueOf(nodeRecord.getLongitude()));
        }

        org.w3c.dom.Element elementWithTags = super.addTagsXml(element);
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
}
