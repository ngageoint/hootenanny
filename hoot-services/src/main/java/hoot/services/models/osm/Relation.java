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
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.db.DbUtils.EntityChangeType;
import hoot.services.db.DbUtils.RecordBatchType;

import hoot.services.db2.CurrentNodes;
import hoot.services.db2.CurrentRelationMembers;
import hoot.services.db2.CurrentRelations;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.geo.BoundingBox;
import hoot.services.geo.Coordinates;

import org.apache.commons.lang3.StringUtils;
import org.apache.commons.lang3.reflect.MethodUtils;
import org.apache.xpath.XPathAPI;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.NodeList;

import com.mysema.query.sql.RelationalPathBase;
import com.mysema.query.sql.SQLExpressions;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.types.path.BooleanPath;
import com.mysema.query.types.path.NumberPath;
import com.mysema.query.types.path.SimplePath;

/**
 * Represents the model for an OSM relation
 */
public class Relation extends Element
{
	private static final Logger log = LoggerFactory.getLogger(Relation.class);
	private int maxRecordBatchSize = -1;
	private List<RelationMember> membersCache = new ArrayList<RelationMember>();
	protected static final QCurrentRelationMembers currentRelationMembers = 
		QCurrentRelationMembers.currentRelationMembers;

	public Relation(final long mapId, Connection dbConn)
	{
		super(dbConn);
		elementType = ElementType.Relation;

		this.record = new CurrentRelations();

		try
		{
			setMapId(mapId);
			maxRecordBatchSize = Integer.parseInt(HootProperties.getInstance()
					.getProperty("maxRecordBatchSize",
							HootProperties.getDefault("maxRecordBatchSize")));
		}
		catch (Exception ex)
		{

		}
	}

	public Relation(final long mapId, Connection dbConn,
			final CurrentRelations record)
	{
		super(dbConn);
		elementType = ElementType.Relation;

		CurrentRelations relationRecord = new CurrentRelations();
		relationRecord.setChangesetId(record.getChangesetId());
		relationRecord.setId(record.getId());
		relationRecord.setTimestamp(record.getTimestamp());
		relationRecord.setVersion(record.getVersion());
		relationRecord.setVisible(record.getVisible());
		relationRecord.setTags(record.getTags());
		this.record = relationRecord;

		try
		{
			setMapId(mapId);
			maxRecordBatchSize = Integer.parseInt(HootProperties.getInstance()
					.getProperty("maxRecordBatchSize",
							HootProperties.getDefault("maxRecordBatchSize")));
		}
		catch (Exception ex)
		{

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
		log.debug("Parsing relation...");

		NamedNodeMap xmlAttributes = xml.getAttributes();

		assert (record != null);
		CurrentRelations relationRecord = (CurrentRelations) record;
		relationRecord.setChangesetId(parseChangesetId(xmlAttributes));
		relationRecord.setVersion(parseVersion(xmlAttributes));

		final Set<Long> owningRelationIds = getOwningRelationIds();
		if (entityChangeType.equals(EntityChangeType.DELETE)
				&& owningRelationIds.size() > 0)
		{
			throw new Exception("Relation to be deleted with ID " + getId()
					+ " is still used by " + "relation(s): "
					+ Arrays.toString(owningRelationIds.toArray()));
		}

		relationRecord.setTimestamp(parseTimestamp(xmlAttributes));
		relationRecord.setVisible(true);

		if (!entityChangeType.equals(EntityChangeType.DELETE))
		{
			final java.util.Map<String, String> tags = parseTags(xml);
			relationRecord.setTags(tags);
		}

		setRecord(relationRecord);

		// if we're deleting the relation, all the relation members will get deleted
		// automatically...and
		// no new ones need to be parsed
		if (!entityChangeType.equals(EntityChangeType.DELETE))
		{
			parseMembersXml(xml);
		}
	}

	/**
	 * Returns an XML representation of the element; does not add tags
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
	public org.w3c.dom.Element toXml(final org.w3c.dom.Element parentXml,
			final long modifyingUserId, final String modifyingUserDisplayName,
			final boolean multiLayerUniqueElementIds, final boolean addChildren)
					throws Exception
	{
		org.w3c.dom.Element element = super.toXml(parentXml, modifyingUserId,
				modifyingUserDisplayName, multiLayerUniqueElementIds, addChildren);
		Document doc = parentXml.getOwnerDocument();

		if (addChildren)
		{
			final List<CurrentRelationMembers> members = getMembers();
			// output in sequence order; query returns list in the proper order
			for (CurrentRelationMembers member : members)
			{
				org.w3c.dom.Element memberElement = doc.createElement("member");
				memberElement.setAttribute("type", member.getMemberType().toString()
						.toLowerCase());
				assert (StringUtils.trimToNull(memberElement.getAttribute("type")) != null);
				String role = member.getMemberRole();
				if (StringUtils.isEmpty(member.getMemberRole()))
				{
					memberElement.setAttribute("role", member.getMemberRole());
					role = "";
				}
				memberElement.setAttribute("role", role);
				assert (memberElement.getAttribute("role") != null);
				memberElement.setAttribute("ref", String.valueOf(member.getMemberId()));
				assert (StringUtils.trimToNull(memberElement.getAttribute("ref")) != null);
				element.appendChild(memberElement);
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

	/*
	 * Parses a list of node/way relation members and computes their bounds
	 * 
	 * @param members a list of relation members
	 * @return a bounds
	 * @throws Exception
	 */
	private BoundingBox parseNodesAndWayMembersBounds(List<RelationMember> members)
			throws Exception
	{
		List<Coordinates> coordsToComputeBoundsFrom = new ArrayList<Coordinates>();
		Set<Long> idsOfNodesToRetrieveFromTheDb = new HashSet<Long>();
		Set<Long> idsOfWaysForWhichToRetrieveNodesFromTheDb = new HashSet<Long>();
		BoundingBox bounds = null;
		BoundingBox dbBounds = null;

		// members don't get parsed for a delete request...we'll get the members
		// from the database
		if (!entityChangeType.equals(EntityChangeType.DELETE))
		{
			for (RelationMember member : members)
			{
				if (member.getType().equals(ElementType.Node))
				{
					if (parsedElementIdsToElementsByType != null
							&& parsedElementIdsToElementsByType.size() != 0)
					{
						Map<Long, Element> parsedNodes = parsedElementIdsToElementsByType
								.get(ElementType.Node);
						if (parsedNodes.containsKey(member.getOldId()))
						{
							Node parsedNode = (Node) parsedNodes.get(member.getOldId());
							coordsToComputeBoundsFrom.add(new Coordinates(
									(Double) MethodUtils.invokeMethod(parsedNode.getRecord(),
											"getLatitude", new Object[] {}), (Double) MethodUtils
											.invokeMethod(parsedNode.getRecord(), "getLongitude",
													new Object[] {})));
						}
						else
						{
							idsOfNodesToRetrieveFromTheDb.add(member.getId());
						}
					}
					else
					{
						idsOfNodesToRetrieveFromTheDb.add(member.getId());
					}
				}
				else if (member.getType().equals(ElementType.Way))
				{
					if (parsedElementIdsToElementsByType != null
							&& parsedElementIdsToElementsByType.size() != 0)
					{
						Map<Long, Element> parsedWays = parsedElementIdsToElementsByType
								.get(ElementType.Way);
						if (parsedWays.containsKey(member.getOldId()))
						{
							Way parsedWay = (Way) parsedWays.get(member.getOldId());
							for (long wayNodeId : parsedWay.getWayNodeIdsCache())
							{
								Map<Long, Element> parsedNodes = parsedElementIdsToElementsByType
										.get(ElementType.Node);
								if (parsedNodes.containsKey(wayNodeId))
								{
									Node parsedNode = (Node) parsedNodes.get(wayNodeId);
									coordsToComputeBoundsFrom.add(
										new Coordinates(
											(Double)MethodUtils.invokeMethod(
												parsedNode.getRecord(), "getLatitude", new Object[] {}), 
											(Double) MethodUtils.invokeMethod(
												parsedNode.getRecord(), "getLongitude", new Object[] {})));
								}
								else
								{
									idsOfNodesToRetrieveFromTheDb.add(wayNodeId);
								}
							}
						}
						else
						{
							idsOfWaysForWhichToRetrieveNodesFromTheDb.add(member.getId());
						}
					}
					else
					{
						idsOfWaysForWhichToRetrieveNodesFromTheDb.add(member.getId());
					}
				}
			}

			// compute a bounds for the elements parsed from the request data
			if (coordsToComputeBoundsFrom.size() > 0)
			{
				bounds = new BoundingBox(coordsToComputeBoundsFrom);
			}

			dbBounds = getBoundsForNodesAndWays(idsOfNodesToRetrieveFromTheDb,
					idsOfWaysForWhichToRetrieveNodesFromTheDb);
		}
		else
		{
			dbBounds = getBoundsForNodesAndWays();
		}

		// add to the bounds the bounds calculated for the elements retrieved from
		// the database
		if (dbBounds != null)
		{
			if (bounds == null)
			{
				bounds = new BoundingBox(dbBounds);
			}
			else
			{
				bounds.add(dbBounds);
			}
		}

		return bounds;
	}

	/*
	 * Retrieves geo info for the nodes/ways associated with the input IDs and
	 * then computes the combined bounds for all the elements
	 */
	private BoundingBox getBoundsForNodesAndWays(final Set<Long> dbNodeIds,
			final Set<Long> dbWayIds) throws Exception
	{
		BoundingBox bounds = null;
		ArrayList<CurrentNodes> nodes = new ArrayList<CurrentNodes>(Node.getNodes(
				getMapId(), dbNodeIds, conn));
		nodes.addAll(Way.getNodesForWays(getMapId(), dbWayIds, conn));
		if (nodes.size() > 0)
		{
			BoundingBox nodeBounds = new BoundingBox(nodes);
			bounds = new BoundingBox(nodeBounds);
		}
		return bounds;
	}

	private BoundingBox getBoundsForNodesAndWays() throws Exception
	{
		final List<Long> nodeIds = new SQLQuery(conn,
				DbUtils.getConfiguration(getMapId()))
		.from(Relation.currentRelationMembers)
		.where(
				currentRelationMembers.relationId.eq(getId()).and(
						currentRelationMembers.memberType.eq(DbUtils.nwr_enum.node)))
						.list(currentRelationMembers.memberId);

		final List<Long> wayIds = new SQLQuery(conn,
				DbUtils.getConfiguration(getMapId()))
		.from(Relation.currentRelationMembers)
		.where(
				currentRelationMembers.relationId.eq(getId()).and(
						currentRelationMembers.memberType.eq(DbUtils.nwr_enum.way)))
						.list(currentRelationMembers.memberId);

		return getBoundsForNodesAndWays(new HashSet<Long>(nodeIds),
				new HashSet<Long>(wayIds));
	}

	/**
	 * Returns the bounds of this element
	 *
	 * The following affect a relation's bounds: - adding or removing nodes or
	 * ways from a relation causes them to be added to the changeset bounding box.
	 * - adding a relation member or changing tag values causes all node and way
	 * members to be added to the bounding box.
	 *
	 * @return a bounding box; null if the relation only contains other relations
	 * @throws Exception
	 */
	public BoundingBox getBounds() throws Exception
	{
		BoundingBox bounds = null;
		if (membersCache == null || membersCache.size() == 0)
		{
			membersCache = RelationMember.fromRecords(getMembers());
		}
		BoundingBox nodesAndWaysBounds = parseNodesAndWayMembersBounds(membersCache);
		if (nodesAndWaysBounds != null)
		{
			bounds = new BoundingBox(nodesAndWaysBounds);
		}
		return bounds;
	}

	/*
	 * Retrieves this relation's members from the services database
	 */
	private List<CurrentRelationMembers> getMembers() throws Exception
	{
		return new SQLQuery(conn, DbUtils.getConfiguration(getMapId()))
		.from(currentRelationMembers)
		.where(currentRelationMembers.relationId.eq(getId()))
		.orderBy(currentRelationMembers.sequenceId.asc())
		.list(currentRelationMembers);

	}

	private void validateMembersSize(final NodeList membersXml) throws Exception
	{
		if (!entityChangeType.equals(EntityChangeType.DELETE))
		{
			CurrentRelations relationRecord = (CurrentRelations) record;
			long numMembers = 0;
			if (membersXml != null)
			{
				numMembers = membersXml.getLength();
			}
			if (numMembers < 1)
			{
				throw new Exception("Too few members specified for relation with ID: "
						+ relationRecord.getId());
			}
		}
	}

	private void checkForCircularReference(final long parsedRelationMemberId)
			throws Exception
	{
		long relationId = 0;
		boolean circularRefFound = false;
		if (parsedRelationMemberId > 0 && parsedRelationMemberId == getId())
		{
			relationId = getId();
			circularRefFound = true;
		}
		else if (parsedRelationMemberId < 0 && parsedRelationMemberId == oldId)
		{
			relationId = oldId;
			circularRefFound = true;
		}
		if (circularRefFound)
		{
			throw new Exception("Relation with ID: " + String.valueOf(relationId)
					+ " contains a relation member that references itself.");
		}
	}

	private RelationMember parseMember(final org.w3c.dom.Node nodeXml)
			throws Exception
	{
		final NamedNodeMap memberXmlAttributes = nodeXml.getAttributes();

		final long parsedMemberId = Long.parseLong(memberXmlAttributes
				.getNamedItem("ref").getNodeValue());
		long actualMemberId = parsedMemberId;
		final ElementType elementType = Element
				.elementTypeFromString(memberXmlAttributes.getNamedItem("type")
						.getNodeValue());
		if (elementType == null)
		{
			throw new Exception("Invalid relation member type: "
					+ memberXmlAttributes.getNamedItem("type").getNodeValue());
		}

		if (elementType.equals(ElementType.Relation))
		{
			checkForCircularReference(parsedMemberId);
		}

		final Map<Long, Element> parsedElements = parsedElementIdsToElementsByType
				.get(elementType);
		// if this is an element created within the same request that is referencing
		// this relation, it
		// won't exist in the database, but it will be in the element cache created
		// when parsing the
		// element from the request
		if (parsedMemberId < 0)
		{
			if (elementType.equals(ElementType.Relation))
			{
				if (!parsedElements.containsKey(parsedMemberId))
				{
					throw new Exception("Relation with ID: " + parsedMemberId
							+ " does not exist for " + "relation with ID: " + getId());
				}
			}
			else
			{
				assert (parsedElements.containsKey(parsedMemberId));
			}
		}

		Element memberElement = null;

		// The element is referenced somewhere else in this request, so get its info
		// from the request,
		// not the database b/c the database either won't have it or will have
		// outdated info for it.
		// Only get info from the request if the element is being created/modified,
		// because if it is
		// being deleted, we can just get the info from the database since the
		// element's bounds won't be
		// changing and its geo info isn't in the request (not required).
		if (parsedElements.containsKey(parsedMemberId)
				&& !parsedElements.get(parsedMemberId).getEntityChangeType()
				.equals(EntityChangeType.DELETE))
		{
			memberElement = parsedElements.get(parsedMemberId);
			actualMemberId = memberElement.getId();
		}
		// element not referenced in this request, so should already exist in the db
		// and its info up
		// to date
		else
		{
			Set<Long> elementIds = new HashSet<Long>(
					Arrays.asList(new Long[] { actualMemberId }));
			if (!Element.allElementsExist(getMapId(), elementType, elementIds, conn))
			{
				throw new Exception("Element with ID: " + actualMemberId
						+ " and type: " + elementType.toString()
						+ " does not exist for relation.");
			}
			if (!Element
					.allElementsVisible(getMapId(), elementType, elementIds, conn))
			{
				throw new Exception("Element with ID: " + actualMemberId
						+ " and type: " + elementType.toString()
						+ " is not visible for relation.");
			}
			memberElement = ElementFactory.getInstance().create(getMapId(),
					elementType, conn);
		}
		assert (actualMemberId > 0);

		// role is allowed to be empty
		org.w3c.dom.Node roleXmlNode = memberXmlAttributes.getNamedItem("role");
		final String role = roleXmlNode == null ? null : roleXmlNode.getNodeValue();
		RelationMember member = new RelationMember(actualMemberId,
				Element.elementTypeFromString(memberXmlAttributes.getNamedItem("type")
						.getNodeValue()), role);
		member.setOldId(parsedMemberId);
		return member;
	}

	private void parseMembersXml(final org.w3c.dom.Node xml) throws Exception
	{
		assert (parsedElementIdsToElementsByType != null);
		final NodeList membersXml = XPathAPI.selectNodeList(xml, "member");

		validateMembersSize(membersXml);

		relatedRecords = new ArrayList<Object>();
		relatedRecordIds = new HashSet<Long>();
		for (int i = 0; i < membersXml.getLength(); i++)
		{
			final org.w3c.dom.Node memberXml = membersXml.item(i);

			final RelationMember member = parseMember(memberXml);
			membersCache.add(member);
			relatedRecordIds.add(member.getId());
			relatedRecords.add(RelationMember.createRecord(member.getId(),
					new Integer(i + 1), member.getRole(),
					Element.elementEnumForElementType(member.getType()), getId(), conn));
		}
	}

	/**
	 * Returns the generated table identifier for this element
	 *
	 * @return a table
	 */
	public RelationalPathBase<?> getElementTable()
	{
		return currentRelations;
	}

	/**
	 * Returns the generated ID field for this element
	 *
	 * @return a table field
	 */
	public NumberPath<Long> getElementIdField()
	{
		return currentRelations.id;
	}

	/**
	 * Returns the generated tag field for this element
	 *
	 * @return a table field
	 */
	public SimplePath<Object> getTagField()
	{
		return currentRelations.tags;
	}

	/**
	 * Returns the generated visibility field for this element
	 *
	 * @return a table field
	 */
	public BooleanPath getElementVisibilityField()
	{
		return currentRelations.visible;
	}

	/**
	 * Returns the generated version field for this element
	 *
	 * @return a table field
	 */
	public NumberPath<Long> getElementVersionField()
	{
		return currentRelations.version;
	}

	/**
	 * Returns the generated changeset ID field for this element
	 *
	 * @return a table field
	 */
	public NumberPath<Long> getChangesetIdField()
	{
		return currentRelations.changesetId;
	}

	/**
	 * Returns the generated table identifier for records related to this element
	 *
	 * @return a table
	 */
	public RelationalPathBase<?> getRelatedRecordTable()
	{
		return currentRelationMembers;
	}

	/**
	 * Returns the table field in the related record table that can be joined with
	 * the parent element record table
	 *
	 * @return a table field
	 */
	public NumberPath<Long> getRelatedRecordJoinField()
	{
		return currentRelationMembers.relationId;
	}

	/**
	 * OSM related element type (e.g. way nodes for ways, relation members for
	 * relations)
	 *
	 * @return a list of element types
	 */
	public List<ElementType> getRelatedElementTypes()
	{
		return Arrays.asList(new ElementType[] { ElementType.Node, ElementType.Way,
				ElementType.Relation });
	}

	/*
	 * This method is inefficient but have yet to think of something better.
	 */
	private Set<Long> getMemberIdsByType(final List<RelationMember> members,
			final ElementType elementType)
			{
		Set<Long> memberIds = new HashSet<Long>();
		for (RelationMember member : members)
		{
			if (member.getType().equals(elementType))
			{
				memberIds.add(member.getId());
			}
		}
		return memberIds;
			}

	/*
	 * Adds this relation's members to the services database
	 */
	private void addMembers(final long mapId, final List<RelationMember> members)
			throws Exception
	{
		CurrentRelations relationRecord = (CurrentRelations) record;
		if (members == null || members.size() < 1)
		{
			throw new Exception("Too few members specified for relation with ID: "
					+ relationRecord.getId());
		}
		final Set<Long> nodeIds = getMemberIdsByType(members, ElementType.Node);
		if (!Element.allElementsExist(getMapId(), ElementType.Node, nodeIds, conn))
		{
			throw new Exception(
					"Not all nodes exist specified for relation with ID: "
							+ relationRecord.getId());
		}
		if (!Element
				.allElementsVisible(getMapId(), ElementType.Node, nodeIds, conn))
		{
			throw new Exception("Not all nodes are visible for relation with ID: "
					+ relationRecord.getId());
		}
		final Set<Long> wayIds = getMemberIdsByType(members, ElementType.Way);
		if (!Element.allElementsExist(getMapId(), ElementType.Way, wayIds, conn))
		{
			throw new Exception("Not all ways exist specified for relation with ID: "
					+ relationRecord.getId());
		}
		if (!Element.allElementsVisible(getMapId(), ElementType.Way, wayIds, conn))
		{
			throw new Exception("Not all ways are visible for relation with ID: "
					+ relationRecord.getId());
		}
		final Set<Long> relationIds = getMemberIdsByType(members,
				ElementType.Relation);
		if (!Element.allElementsExist(getMapId(), ElementType.Relation,
				relationIds, conn))
		{
			throw new Exception(
					"Not all relations exist specified for relation with ID: "
							+ relationRecord.getId());
		}
		if (!Element.allElementsVisible(getMapId(), ElementType.Relation,
				relationIds, conn))
		{
			throw new Exception(
					"Not all relations are visible for relation with ID: "
							+ relationRecord.getId());
		}

		List<CurrentRelationMembers> memberRecords = new ArrayList<CurrentRelationMembers>();
		int sequenceCtr = 1;
		for (RelationMember member : members)
		{
			CurrentRelationMembers memberRecord = new CurrentRelationMembers();
			memberRecord.setMemberId(member.getId());
			memberRecord.setMemberRole(member.getRole());
			memberRecord.setMemberType(Element.elementEnumForElementType(member
					.getType()));
			memberRecord.setRelationId(relationRecord.getId());
			memberRecord.setSequenceId(sequenceCtr);
			memberRecords.add(memberRecord);
			sequenceCtr++;
		}

		DbUtils.batchRecords(mapId, memberRecords,
				QCurrentRelationMembers.currentRelationMembers, null,
				RecordBatchType.INSERT, conn, maxRecordBatchSize);
	}

	/**
	 * Inserts a new relation into the services database
	 *
	 * @param changesetId
	 *          corresponding changeset ID for the way to be inserted
	 * @param mapId
	 *          corresponding map ID for the element to be inserted
	 * @param members
	 *          the relation's members
	 * @param tags
	 *          element tags
	 * @param dbConn
	 *          JDBC Connection
	 * @return ID of the newly created element
	 * @throws Exception
	 */
	public static long insertNew(final long changesetId, final long mapId,
			final List<RelationMember> members, final Map<String, String> tags,
			Connection dbConn) throws Exception
	{
		final long nextRelationId = new SQLQuery(dbConn,
				DbUtils.getConfiguration(mapId)).uniqueResult(SQLExpressions.nextval(
						Long.class, "current_relations_id_seq"));

		insertNew(nextRelationId, changesetId, mapId, members, tags, dbConn);
		return nextRelationId;
	}

	/**
	 * Inserts a new relation into the services database with the specified ID;
	 * useful for testing
	 *
	 * @param wayId
	 *          ID to assign to the new way
	 * @param changesetId
	 *          corresponding changeset ID for the element to be inserted
	 * @param mapId
	 *          corresponding map ID for the element to be inserted
	 * @param members
	 *          the relation's members
	 * @param tags
	 *          element tags
	 * @param dbConn
	 *          JDBC Connection
	 * @throws Exception
	 */
	public static void insertNew(final long relId, final long changesetId,
			final long mapId, final List<RelationMember> members,
			final Map<String, String> tags, Connection dbConn) throws Exception
	{
		CurrentRelations relationRecord = new CurrentRelations();
		relationRecord.setChangesetId(changesetId);
		relationRecord.setId(relId);
		final Timestamp now = new Timestamp(Calendar.getInstance()
				.getTimeInMillis());
		relationRecord.setTimestamp(now);
		relationRecord.setVersion(new Long(1));
		relationRecord.setVisible(true);
		if (tags != null && tags.size() > 0)
		{
			relationRecord.setTags(tags);
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

			String sql = "INSERT INTO current_relations_"
					+ mapId
					+ "(\n"
					+ "            id, changeset_id, \"timestamp\", visible, version, tags)\n"
					+ " VALUES(" + relId + "," + changesetId + "," + "CURRENT_TIMESTAMP"
					+ "," + "true" + "," + "1" + "," + strTags +

					")";
			stmt.executeUpdate(sql);
			new Relation(mapId, dbConn, relationRecord).addMembers(mapId, members);

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
