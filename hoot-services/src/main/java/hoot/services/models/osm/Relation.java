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
import static hoot.services.utils.DbUtils.createQuery;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.xml.transform.TransformerException;

import org.apache.commons.lang3.StringUtils;
import org.apache.commons.lang3.reflect.MethodUtils;
import org.apache.xpath.XPathAPI;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.NodeList;

import com.querydsl.core.types.Projections;
import com.querydsl.core.types.dsl.BooleanPath;
import com.querydsl.core.types.dsl.NumberPath;
import com.querydsl.core.types.dsl.SimplePath;
import com.querydsl.sql.RelationalPathBase;

import hoot.services.geo.BoundingBox;
import hoot.services.geo.Coordinates;
import hoot.services.models.db.CurrentNodes;
import hoot.services.models.db.CurrentRelationMembers;
import hoot.services.models.db.CurrentRelations;
import hoot.services.utils.DbUtils;
import hoot.services.utils.DbUtils.EntityChangeType;


/**
 * Represents the model for an OSM relation
 */
public class Relation extends Element {
    private static final Logger logger = LoggerFactory.getLogger(Relation.class);

    private List<RelationMember> membersCache = new ArrayList<>();

    public Relation(long mapId) {
        super.elementType = ElementType.Relation;
        super.record = new CurrentRelations();
        setMapId(mapId);
    }

    public Relation(long mapId, CurrentRelations record) {
        super.elementType = ElementType.Relation;

        CurrentRelations relationRecord = new CurrentRelations();
        relationRecord.setChangesetId(record.getChangesetId());
        relationRecord.setId(record.getId());
        relationRecord.setTimestamp(record.getTimestamp());
        relationRecord.setVersion(record.getVersion());
        relationRecord.setVisible(record.getVisible());
        relationRecord.setTags(record.getTags());

        super.record = relationRecord;

        setMapId(mapId);
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

        CurrentRelations relationRecord = (CurrentRelations) record;
        relationRecord.setChangesetId(parseChangesetId(xmlAttributes));
        relationRecord.setVersion(parseVersion());
        relationRecord.setTimestamp(parseTimestamp(xmlAttributes));
        relationRecord.setVisible(true);

        if (entityChangeType != EntityChangeType.DELETE) {
            relationRecord.setTags(parseTags(xml));
        }

        setRecord(relationRecord);

        // if we're deleting the relation, all the relation members will get
        // deleted automatically...and no new ones need to be parsed
        if (entityChangeType != EntityChangeType.DELETE) {
            parseMembersXml(xml);
        }
    }

    @Override
    public void checkAndFailIfUsedByOtherObjects() throws OSMAPIAlreadyDeletedException, OSMAPIPreconditionException {
        if (!super.getVisible()) {
            throw new OSMAPIAlreadyDeletedException("Relation with ID = " + super.getId() + " has been already deleted "
                    + "from map with ID = " + getMapId());
        }

        // From the Rails port of OSM API:
        // RelationMember.joins(:relation).find_by("visible = ? AND member_type = 'Relation' and member_id = ? ", true, id)
        List<Long> owningRelationsIds = createQuery(getMapId())
                .select(currentRelationMembers.relationId)
                .distinct()
                .from(currentRelationMembers)
                .join(currentRelations).on(currentRelationMembers.relationId.eq(currentRelations.id))
                .where(currentRelations.visible.eq(true)
                        .and(currentRelationMembers.memberId.eq(super.getId()))
                        .and(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.relation)))
                .orderBy(currentRelationMembers.relationId.asc())
                .fetch();

        if (!owningRelationsIds.isEmpty()) {
            throw new OSMAPIPreconditionException(
                    "Relation with ID = " + super.getId() + " is still used by other relation(s): "
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
            List<CurrentRelationMembers> members = getMembers();
            // output in sequence order; query returns list in the proper order
            for (CurrentRelationMembers member : members) {
                org.w3c.dom.Element memberElement = doc.createElement("member");
                memberElement.setAttribute("type", member.getMemberType().toString().toLowerCase());

                String role = member.getMemberRole();
                if (StringUtils.isEmpty(member.getMemberRole())) {
                    memberElement.setAttribute("role", member.getMemberRole());
                    role = "";
                }

                memberElement.setAttribute("role", role);
                memberElement.setAttribute("ref", String.valueOf(member.getMemberId()));
                element.appendChild(memberElement);
            }
        }

        org.w3c.dom.Element elementWithTags = addTagsXml(element);
        if (elementWithTags == null) {
            return element;
        }

        return elementWithTags;
    }

    /*
     * Parses a list of node/way relation members and computes their bounds
     *
     * @param members a list of relation members
     * 
     * @return a bounds
     * 
     * @throws Exception
     */
    private BoundingBox parseNodesAndWayMembersBounds(List<RelationMember> members) {
        BoundingBox bounds = null;
        BoundingBox dbBounds;

        // members don't get parsed for a delete request...we'll get the members from the database
        if (entityChangeType != EntityChangeType.DELETE) {
            Set<Long> idsOfNodesToRetrieveFromTheDb = new HashSet<>();
            Set<Long> idsOfWaysForWhichToRetrieveNodesFromTheDb = new HashSet<>();
            List<Coordinates> coordsToComputeBoundsFrom = new ArrayList<>();
            for (RelationMember member : members) {
                if (member.getType() == ElementType.Node) {
                    if ((parsedElementIdsToElementsByType != null) && (!parsedElementIdsToElementsByType.isEmpty())) {
                        Map<Long, Element> parsedNodes = parsedElementIdsToElementsByType.get(ElementType.Node);
                        if (parsedNodes.containsKey(member.getOldId())) {
                            Node parsedNode = (Node) parsedNodes.get(member.getOldId());
                            try {
                                coordsToComputeBoundsFrom.add(new Coordinates(
                                        (Double) MethodUtils.invokeMethod(parsedNode.getRecord(), "getLatitude"),
                                        (Double) MethodUtils.invokeMethod(parsedNode.getRecord(), "getLongitude")));
                            }
                            catch (Exception e) {
                                throw new RuntimeException("Error invoking getLatitude() or getLongitude()!", e);
                            }
                        }
                        else {
                            idsOfNodesToRetrieveFromTheDb.add(member.getId());
                        }
                    }
                    else {
                        idsOfNodesToRetrieveFromTheDb.add(member.getId());
                    }
                }
                else if (member.getType() == ElementType.Way) {
                    if ((parsedElementIdsToElementsByType != null) && !parsedElementIdsToElementsByType.isEmpty()) {
                        Map<Long, Element> parsedWays = parsedElementIdsToElementsByType.get(ElementType.Way);
                        if (parsedWays.containsKey(member.getOldId())) {
                            Way parsedWay = (Way) parsedWays.get(member.getOldId());
                            for (long wayNodeId : parsedWay.getWayNodeIdsCache()) {
                                Map<Long, Element> parsedNodes = parsedElementIdsToElementsByType.get(ElementType.Node);
                                if (parsedNodes.containsKey(wayNodeId)) {
                                    Node parsedNode = (Node) parsedNodes.get(wayNodeId);
                                    try {
                                        coordsToComputeBoundsFrom.add(new Coordinates(
                                                (Double) MethodUtils.invokeMethod(parsedNode.getRecord(), "getLatitude"),
                                                (Double) MethodUtils.invokeMethod(parsedNode.getRecord(), "getLongitude")));
                                    }
                                    catch (Exception e) {
                                        throw new RuntimeException("Error invoking getLatitude() or getLongitude()!", e);
                                    }
                                }
                                else {
                                    idsOfNodesToRetrieveFromTheDb.add(wayNodeId);
                                }
                            }
                        }
                        else {
                            idsOfWaysForWhichToRetrieveNodesFromTheDb.add(member.getId());
                        }
                    }
                    else {
                        idsOfWaysForWhichToRetrieveNodesFromTheDb.add(member.getId());
                    }
                }
            }

            // compute a bounds for the elements parsed from the request data
            if (!coordsToComputeBoundsFrom.isEmpty()) {
                bounds = new BoundingBox(new HashSet<>(coordsToComputeBoundsFrom));
            }

            dbBounds = getBoundsForNodesAndWays(idsOfNodesToRetrieveFromTheDb,
                    idsOfWaysForWhichToRetrieveNodesFromTheDb);
        }
        else {
            dbBounds = getBoundsForNodesAndWays();
        }

        // add to the bounds the bounds calculated for the elements retrieved from the database
        if (dbBounds != null) {
            if (bounds == null) {
                bounds = new BoundingBox(dbBounds);
            }
            else {
                bounds.add(dbBounds);
            }
        }

        return bounds;
    }

    /*
     * Retrieves geo info for the nodes/ways associated with the input IDs and
     * then computes the combined bounds for all the elements
     */
    private BoundingBox getBoundsForNodesAndWays(Set<Long> dbNodeIds, Set<Long> dbWayIds) {
        List<CurrentNodes> nodes = new ArrayList<>();

        if (!dbNodeIds.isEmpty()) {
            nodes = createQuery(getMapId())
                    .select(Projections.bean(CurrentNodes.class, currentNodes.latitude, currentNodes.longitude))
                    .from(currentNodes)
                    .where(currentNodes.id.in(dbNodeIds))
                    .fetch();
        }

        if (!dbWayIds.isEmpty()) {
            nodes.addAll(createQuery(getMapId())
                    .select(Projections.bean(CurrentNodes.class, currentNodes.latitude, currentNodes.longitude))
                    .from(currentWayNodes)
                    .join(currentNodes).on(currentWayNodes.nodeId.eq(currentNodes.id))
                    .where(currentWayNodes.wayId.in(dbWayIds))
                    .fetch());
        }

        BoundingBox bounds = null;
        if (!nodes.isEmpty()) {
            BoundingBox nodeBounds = new BoundingBox(nodes);
            bounds = new BoundingBox(nodeBounds);
        }

        return bounds;
    }

    private BoundingBox getBoundsForNodesAndWays() {
        List<Long> nodeIds = createQuery(getMapId())
                .select(currentRelationMembers.memberId)
                .from(currentRelationMembers)
                .where(currentRelationMembers.relationId.eq(getId())
                        .and(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.node)))
                .fetch();

        List<Long> wayIds = createQuery(getMapId())
                .select(currentRelationMembers.memberId)
                .from(currentRelationMembers)
                .where(currentRelationMembers.relationId.eq(getId())
                        .and(currentRelationMembers.memberType.eq(DbUtils.nwr_enum.way)))
                .fetch();

        return getBoundsForNodesAndWays(new HashSet<>(nodeIds), new HashSet<>(wayIds));
    }

    /**
     * Returns the bounds of this element
     * <p>
     * The following affect a relation's bounds: - adding or removing nodes or
     * ways from a relation causes them to be added to the changeset bounding
     * box. - adding a relation member or changing tag values causes all node
     * and way members to be added to the bounding box.
     *
     * @return a bounding box; null if the relation only contains other relations
     */
    @Override
    public BoundingBox getBounds() {
        if ((membersCache == null) || (membersCache.isEmpty())) {
            membersCache = fromRecords(getMembers());
        }

        BoundingBox nodesAndWaysBounds = parseNodesAndWayMembersBounds(membersCache);

        BoundingBox bounds = null;
        if (nodesAndWaysBounds != null) {
            bounds = new BoundingBox(nodesAndWaysBounds);
        }

        return bounds;
    }

    /*
     * Retrieves this relation's members from the services database
     */
    private List<CurrentRelationMembers> getMembers() {
        return createQuery(getMapId())
                .select(currentRelationMembers)
                .from(currentRelationMembers)
                .where(currentRelationMembers.relationId.eq(getId()))
                .orderBy(currentRelationMembers.sequenceId.asc())
                .fetch();
    }

    private void checkForCircularReference(long parsedRelationMemberId) {
        long relationId = 0;
        boolean circularRefFound = false;

        if ((parsedRelationMemberId > 0) && (parsedRelationMemberId == getId())) {
            relationId = getId();
            circularRefFound = true;
        }
        else if ((parsedRelationMemberId < 0) && (parsedRelationMemberId == oldId)) {
            relationId = oldId;
            circularRefFound = true;
        }

        if (circularRefFound) {
            throw new IllegalStateException("Relation with ID: " + relationId +
                    " contains a relation member that references itself.");
        }
    }

    private RelationMember parseMember(org.w3c.dom.Node nodeXml) {
        NamedNodeMap memberXmlAttributes = nodeXml.getAttributes();

        long parsedMemberId = Long.parseLong(memberXmlAttributes.getNamedItem("ref").getNodeValue());

        ElementType elementType = Element.elementTypeFromString(memberXmlAttributes.getNamedItem("type").getNodeValue());

        if (elementType == null) {
            throw new IllegalArgumentException("Invalid relation member type: " +
                    memberXmlAttributes.getNamedItem("type").getNodeValue());
        }

        if (elementType == ElementType.Relation) {
            checkForCircularReference(parsedMemberId);
        }

        Map<Long, Element> parsedElements = parsedElementIdsToElementsByType.get(elementType);

        // if this is an element created within the same request that is
        // referencing this relation, it won't exist in the database, but it will be in the element cache
        // created when parsing the element from the request
        if (parsedMemberId < 0) {
            if (elementType == ElementType.Relation) {
                if (!parsedElements.containsKey(parsedMemberId)) {
                    throw new IllegalStateException("Relation with ID: " + parsedMemberId + " does not exist for "
                            + "relation with ID: " + getId());
                }
            }
        }

        Element memberElement;

        // TODO: these comments need updating

        // The element is referenced somewhere else in this request, so get its
        // info from the request, not the database, b/c the database either won't have it or will have
        // outdated info for it. Only get info from the request if the element is being
        // created/modified, because if it is being deleted, we can just get the info from the database since the
        // element's bounds won't be changing and its geo info isn't in the request (not required for a delete).
        long actualMemberId = parsedMemberId;
        if (parsedElements.containsKey(parsedMemberId)
                && (parsedElements.get(parsedMemberId).getEntityChangeType() != EntityChangeType.DELETE)) {
            memberElement = parsedElements.get(parsedMemberId);
            actualMemberId = memberElement.getId();
        }
        // element not referenced in this request, so should already exist in
        // the db and its info up to date
        else {
            memberElement = ElementFactory.create(getMapId(), elementType);
        }

        // role is allowed to be empty
        org.w3c.dom.Node roleXmlNode = memberXmlAttributes.getNamedItem("role");
        String role = (roleXmlNode == null) ? null : roleXmlNode.getNodeValue();
        RelationMember member = new RelationMember(actualMemberId,
                Element.elementTypeFromString(memberXmlAttributes.getNamedItem("type").getNodeValue()), role);
        member.setOldId(parsedMemberId);

        return member;
    }

    // relations of size = 0 are allowed; see http://wiki.openstreetmap.org/wiki/Empty_relations
    private void parseMembersXml(org.w3c.dom.Node xml) {
        NodeList membersXml = null;
        try {
            membersXml = XPathAPI.selectNodeList(xml, "member");
        }
        catch (TransformerException e) {
            throw new RuntimeException("Error accessing 'member' node using XPathAPI!", e);
        }

        relatedRecords = new ArrayList<>();
        relatedRecordIds = new HashSet<>();

        for (int i = 0; i < membersXml.getLength(); i++) {
            org.w3c.dom.Node memberXml = membersXml.item(i);
            RelationMember member = parseMember(memberXml);
            membersCache.add(member);
            relatedRecordIds.add(member.getId());
            relatedRecords.add(createRecord(member.getId(), i + 1, member.getRole(),
                    Element.elementEnumForElementType(member.getType()), getId()));
        }
    }

    /**
     * Creates a relation member database record
     *
     * @param id
     *            the member's ID
     * @param sequenceId
     *            the member's sequence ordering (1..n)
     * @param role
     *            the member's role
     * @param elementType
     *            the member's element type
     * @param relationId
     *            the owning relation ID
     * @return a relation member database record
     */
    private static CurrentRelationMembers createRecord(long id, int sequenceId, String role, Object elementType,
            long relationId) {
        CurrentRelationMembers memberRecord = new CurrentRelationMembers();
        memberRecord.setMemberId(id);
        memberRecord.setSequenceId(sequenceId);
        memberRecord.setMemberRole(StringUtils.isEmpty(role) ? "" : role);
        memberRecord.setMemberType(elementType);
        memberRecord.setRelationId(relationId);
        return memberRecord;
    }

    /**
     * Transforms a list of relation member database records into a list of
     * RelationMember objects
     *
     * @param records
     *            a list of relation member database records
     * @return a list of RelationMember objects
     */
    private static List<RelationMember> fromRecords(List<CurrentRelationMembers> records) {
        List<RelationMember> members = new ArrayList<>();
        for (CurrentRelationMembers memberRecord : records) {
            members.add(new RelationMember(memberRecord));
        }
        return members;
    }

    /**
     * Returns the generated table identifier for this element
     *
     * @return a table
     */
    @Override
    public RelationalPathBase<?> getElementTable() {
        return currentRelations;
    }

    /**
     * Returns the generated ID field for this element
     *
     * @return a table field
     */
    @Override
    public NumberPath<Long> getElementIdField() {
        return currentRelations.id;
    }

    /**
     * Returns the generated tag field for this element
     *
     * @return a table field
     */
    public SimplePath<Object> getTagField() {
        return currentRelations.tags;
    }

    /**
     * Returns the generated visibility field for this element
     *
     * @return a table field
     */
    @Override
    public BooleanPath getElementVisibilityField() {
        return currentRelations.visible;
    }

    /**
     * Returns the generated version field for this element
     *
     * @return a table field
     */
    @Override
    public NumberPath<Long> getElementVersionField() {
        return currentRelations.version;
    }

    /**
     * Returns the generated changeset ID field for this element
     *
     * @return a table field
     */
    @Override
    public NumberPath<Long> getChangesetIdField() {
        return currentRelations.changesetId;
    }

    /**
     * Returns the generated table identifier for records related to this
     * element
     *
     * @return a table
     */
    @Override
    public RelationalPathBase<?> getRelatedRecordTable() {
        return currentRelationMembers;
    }

    /**
     * Returns the table field in the related record table that can be joined
     * with the parent element record table
     *
     * @return a table field
     */
    @Override
    public NumberPath<Long> getRelatedRecordJoinField() {
        return currentRelationMembers.relationId;
    }

    /**
     * OSM related element type (e.g. way nodes for ways, relation members for
     * relations)
     *
     * @return a list of element types
     */
    @Override
    public List<ElementType> getRelatedElementTypes() {
        return Arrays.asList(ElementType.Node, ElementType.Way, ElementType.Relation);
    }
}
