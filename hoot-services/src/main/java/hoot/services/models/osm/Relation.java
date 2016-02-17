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
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import hoot.services.db.DbUtils;
import hoot.services.db.DbUtils.EntityChangeType;

import hoot.services.db2.CurrentNodes;
import hoot.services.db2.CurrentRelationMembers;
import hoot.services.db2.CurrentRelations;
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
	
	private List<RelationMember> membersCache = new ArrayList<RelationMember>();

	public Relation(final long mapId, Connection dbConn)
	{
		super(dbConn);
		elementType = ElementType.Relation;

		this.record = new CurrentRelations();
	  //TODO: what's the purpose of this catch?
		try
		{
			setMapId(mapId);
		}
		catch (Exception ex)
		{
		}
	}

	public Relation(final long mapId, Connection dbConn, final CurrentRelations record)
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

	  //TODO: what's the purpose of this catch?
		try
		{
			setMapId(mapId);
		}
		catch (Exception ex)
		{
		}
	}

	/**
	 * Populates this element model object based on osm diff data
	 *
	 * @param xml xml data to construct the element from
	 * @throws Exception
	 */
	public void fromXml(final org.w3c.dom.Node xml) throws Exception
	{
		log.debug("Parsing relation...");

		NamedNodeMap xmlAttributes = xml.getAttributes();

		assert(record != null);
		CurrentRelations relationRecord = (CurrentRelations)record;
		relationRecord.setChangesetId(parseChangesetId(xmlAttributes));
		relationRecord.setVersion(parseVersion());
		relationRecord.setTimestamp(parseTimestamp(xmlAttributes));
		relationRecord.setVisible(true);

		if (!entityChangeType.equals(EntityChangeType.DELETE))
		{
			relationRecord.setTags(parseTags(xml));
		}

		setRecord(relationRecord);

		// if we're deleting the relation, all the relation members will get deleted automatically...and
		// no new ones need to be parsed
		if (!entityChangeType.equals(EntityChangeType.DELETE))
		{
			parseMembersXml(xml);
		}
	}

	/**
	 * Returns an XML representation of the element; does not add tags
	 *
	 * @param parentXml XML node this element should be attached under
	 * @param modifyingUserId ID of the user which created this element
	 * @param modifyingUserDisplayName user display name of the user which created this element
	 * @param multiLayerUniqueElementIds if true, IDs are prepended with <map id>_<first letter of the
	 * element type>_; this setting activated is not compatible with standard OSM clients (specific 
	 * to Hootenanny iD)
	 * @param addChildren if true, element children are added to the element xml
	 * @return an XML element
	 * @throws Exception
	 */
	public org.w3c.dom.Element toXml(final org.w3c.dom.Element parentXml,
	    final long modifyingUserId, final String modifyingUserDisplayName,
	    final boolean multiLayerUniqueElementIds, final boolean addChildren)
	    throws Exception
	{
		org.w3c.dom.Element element = 
			super.toXml(
				parentXml, modifyingUserId, modifyingUserDisplayName, multiLayerUniqueElementIds, 
				addChildren);
		Document doc = parentXml.getOwnerDocument();

		if (addChildren)
		{
			final List<CurrentRelationMembers> members = getMembers();
			// output in sequence order; query returns list in the proper order
			for (CurrentRelationMembers member : members)
			{
				org.w3c.dom.Element memberElement = doc.createElement("member");
				memberElement.setAttribute("type", member.getMemberType().toString().toLowerCase());
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
						Map<Long, Element> parsedNodes = 
							parsedElementIdsToElementsByType.get(ElementType.Node);
						if (parsedNodes.containsKey(member.getOldId()))
						{
							Node parsedNode = (Node)parsedNodes.get(member.getOldId());
							coordsToComputeBoundsFrom.add(
								new Coordinates(
							    (Double)MethodUtils.invokeMethod(
							    	parsedNode.getRecord(),
							       "getLatitude", new Object[] {}), 
							     (Double)MethodUtils.invokeMethod(
							    		parsedNode.getRecord(), "getLongitude", new Object[] {})));
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
						Map<Long, Element> parsedWays = 
							parsedElementIdsToElementsByType.get(ElementType.Way);
						if (parsedWays.containsKey(member.getOldId()))
						{
							Way parsedWay = (Way) parsedWays.get(member.getOldId());
							for (long wayNodeId : parsedWay.getWayNodeIdsCache())
							{
								Map<Long, Element> parsedNodes = 
									parsedElementIdsToElementsByType.get(ElementType.Node);
								if (parsedNodes.containsKey(wayNodeId))
								{
									Node parsedNode = (Node) parsedNodes.get(wayNodeId);
									coordsToComputeBoundsFrom.add(
										new Coordinates(
									    (Double)MethodUtils.invokeMethod(
									    	parsedNode.getRecord(),
									       "getLatitude", new Object[] {}), 
									     (Double)MethodUtils.invokeMethod(
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

			dbBounds = 
				getBoundsForNodesAndWays(
					idsOfNodesToRetrieveFromTheDb, idsOfWaysForWhichToRetrieveNodesFromTheDb);
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
		ArrayList<CurrentNodes> nodes = 
			new ArrayList<CurrentNodes>(Node.getNodes(getMapId(), dbNodeIds, conn));
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
		final List<Long> nodeIds = 
			new SQLQuery(conn, DbUtils.getConfiguration(getMapId()))
		  .from(currentRelationMembers)
		  .where(
		    currentRelationMembers.relationId.eq(getId()).and(
		      currentRelationMembers.memberType.eq(DbUtils.nwr_enum.node)))
		  .list(currentRelationMembers.memberId);

		final List<Long> wayIds = 
			new SQLQuery(conn, DbUtils.getConfiguration(getMapId()))
		    .from(currentRelationMembers)
		    .where(
		      currentRelationMembers.relationId.eq(getId()).and(
		       currentRelationMembers.memberType.eq(DbUtils.nwr_enum.way)))
		    .list(currentRelationMembers.memberId);

		return getBoundsForNodesAndWays(new HashSet<Long>(nodeIds), new HashSet<Long>(wayIds));
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
		return 
			new SQLQuery(conn, DbUtils.getConfiguration(getMapId()))
		    .from(currentRelationMembers)
		    .where(currentRelationMembers.relationId.eq(getId()))
		    .orderBy(currentRelationMembers.sequenceId.asc())
		    .list(currentRelationMembers);
	}

	private void checkForCircularReference(final long parsedRelationMemberId) throws Exception
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

	private RelationMember parseMember(final org.w3c.dom.Node nodeXml) throws Exception
	{
		log.debug("Parsing relation member...");
		
		final NamedNodeMap memberXmlAttributes = nodeXml.getAttributes();

		final long parsedMemberId = 
			Long.parseLong(memberXmlAttributes.getNamedItem("ref").getNodeValue());
		long actualMemberId = parsedMemberId;
		final ElementType elementType = 
			Element.elementTypeFromString(memberXmlAttributes.getNamedItem("type").getNodeValue());
		if (elementType == null)
		{
			throw new Exception(
				"Invalid relation member type: " + memberXmlAttributes.getNamedItem("type").getNodeValue());
		}

		if (elementType.equals(ElementType.Relation))
		{
			checkForCircularReference(parsedMemberId);
		}

		final Map<Long, Element> parsedElements = parsedElementIdsToElementsByType.get(elementType);
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
					throw new Exception(
						"Relation with ID: " + parsedMemberId + " does not exist for " + "relation with ID: " + 
					  getId());
				}
			}
			else
			{
				assert(parsedElements.containsKey(parsedMemberId));
			}
		}

		Element memberElement = null;
		
		//TODO: these comments need updating

		// The element is referenced somewhere else in this request, so get its info from the request,
		// not the database, b/c the database either won't have it or will have outdated info for it.
		// Only get info from the request if the element is being created/modified, because if it is
		// being deleted, we can just get the info from the database since the element's bounds won't be
		// changing and its geo info isn't in the request (not required for a delete).
		if (parsedElements.containsKey(parsedMemberId) && 
			  !parsedElements.get(parsedMemberId).getEntityChangeType().equals(EntityChangeType.DELETE))
		{
			memberElement = parsedElements.get(parsedMemberId);
			actualMemberId = memberElement.getId();
		}
		// element not referenced in this request, so should already exist in the db and its info up
		// to date
		else
		{
			memberElement = ElementFactory.getInstance().create(getMapId(), elementType, conn);
		}
		assert (actualMemberId > 0);

		// role is allowed to be empty
		org.w3c.dom.Node roleXmlNode = memberXmlAttributes.getNamedItem("role");
		final String role = roleXmlNode == null ? null : roleXmlNode.getNodeValue();
		RelationMember member = 
			new RelationMember(
				actualMemberId,
		    Element.elementTypeFromString(memberXmlAttributes.getNamedItem("type").getNodeValue()), 
		    role);
		member.setOldId(parsedMemberId);
		return member;
	}

	// relations of size = 0 are allowed; see
	// http://wiki.openstreetmap.org/wiki/Empty_relations
	private void parseMembersXml(final org.w3c.dom.Node xml) throws Exception
	{
		log.debug("Parsing relation members...");
		
		assert (parsedElementIdsToElementsByType != null);
		final NodeList membersXml = XPathAPI.selectNodeList(xml, "member");

		relatedRecords = new ArrayList<Object>();
		relatedRecordIds = new HashSet<Long>();
		for (int i = 0; i < membersXml.getLength(); i++)
		{
			final org.w3c.dom.Node memberXml = membersXml.item(i);
			final RelationMember member = parseMember(memberXml);
			membersCache.add(member);
			relatedRecordIds.add(member.getId());
			relatedRecords.add(
				RelationMember.createRecord(
					member.getId(),
			    new Integer(i + 1), 
			    member.getRole(),
			    Element.elementEnumForElementType(member.getType()), 
			    getId(), 
			    conn));
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
		return 
			Arrays.asList(new ElementType[] { ElementType.Node, ElementType.Way, ElementType.Relation });
	}
}
