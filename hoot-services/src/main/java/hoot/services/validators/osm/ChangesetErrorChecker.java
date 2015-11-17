package hoot.services.validators.osm;

import java.sql.Connection;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

import hoot.services.db.DbUtils;
import hoot.services.db.DbUtils.EntityChangeType;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.ElementFactory;
import hoot.services.models.osm.Element.ElementType;

import org.apache.commons.lang3.StringUtils;
import org.apache.xpath.XPathAPI;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

import com.mysema.query.sql.SQLQuery;

/**
 *
 */
public class ChangesetErrorChecker
{
	private static final Logger log = LoggerFactory.getLogger(ChangesetErrorChecker.class);
	
	private static final QCurrentRelationMembers currentRelationMembers = 
	  QCurrentRelationMembers.currentRelationMembers;
	private static final QCurrentWayNodes currentWayNodes = QCurrentWayNodes.currentWayNodes;
	
	private Document changesetDoc;
	private long mapId;
	private Connection dbConn;
	
	public ChangesetErrorChecker(final Document changesetDoc, final long mapId, Connection dbConn)
	{
		this.changesetDoc = changesetDoc;
		this.mapId = mapId;
		this.dbConn = dbConn;
	}
	
	public void checkForVersionErrors() throws Exception
  {
		for (ElementType elementType : ElementType.values())
		{
			if (!elementType.equals(ElementType.Changeset))
			{
				for (EntityChangeType entityChangeType : EntityChangeType.values())
	      {
					final NodeList elementXmlNodes = 
		  			XPathAPI.selectNodeList(
		  				changesetDoc, 
		  				"//osmChange/" + entityChangeType.toString().toLowerCase() + "/" + 
		  				  elementType.toString().toLowerCase());
					Map<Long, Long> elementIdsToVersionsFromChangeset = new HashMap<Long, Long>();
					for (int i = 0; i < elementXmlNodes.getLength(); i++)
					{
						final long id = 
      				Long.parseLong(
    					  elementXmlNodes.item(i).getAttributes().getNamedItem("id").getNodeValue());
      			final long parsedVersion = 
      				Long.parseLong(
    					  elementXmlNodes.item(i).getAttributes().getNamedItem("version").getNodeValue());
						if (entityChangeType.equals(EntityChangeType.CREATE) && parsedVersion != 0)
						{
      				throw new Exception(
      					"Invalid version: " + parsedVersion + " specified for " + elementType.toString() + 
      	          " with ID: " + id + "; expected version 0.");
						}
						else
						{
							if (id > 0)
							{
								elementIdsToVersionsFromChangeset.put(id, parsedVersion);
							}
						}
					}
					
					if (!entityChangeType.equals(EntityChangeType.CREATE))
					{
						final Element prototype = 
							ElementFactory.getInstance().create(mapId, elementType, dbConn);
						final Map<Long, Long> elementIdsToVersionsFromDb =
		  				new SQLQuery(dbConn, DbUtils.getConfiguration(mapId))
		            .from(prototype.getElementTable())
			          .where(prototype.getElementIdField().in(elementIdsToVersionsFromChangeset.keySet()))
						    .map(prototype.getElementIdField(), prototype.getElementVersionField());
						if (!elementIdsToVersionsFromDb.equals(elementIdsToVersionsFromChangeset))
						{
							throw new Exception("Invalid version specified for element(s).");
						}
					}
	      }
			}
		}
  }
	
	public void checkForOwnershipErrors() throws Exception
	{
		checkForRelationOwnershipErrors();
		checkForWayOwnershipErrors();
	}
  
  private void checkForRelationOwnershipErrors() throws Exception
  {
  	for (ElementType elementType : ElementType.values())
  	{
  		if (!elementType.equals(ElementType.Changeset))
  		{
  			final NodeList deletedElementIdXmlNodes = 
	  			XPathAPI.selectNodeList(
	  				changesetDoc, "//osmChange/delete/" + elementType.toString().toLowerCase() + "/@id");
	  		List<Long> deletedElementIds = new ArrayList<Long>();
	  		for (int i = 0; i < deletedElementIdXmlNodes.getLength(); i++)
	  		{
	  			deletedElementIds.add(Long.parseLong(deletedElementIdXmlNodes.item(i).getNodeValue()));
	  		}
	  		final Set<Long> owningRelationIds =
	  			new TreeSet<Long>(
	  				new SQLQuery(dbConn, DbUtils.getConfiguration(mapId))
	          .from(currentRelationMembers)
		        .where(
			        currentRelationMembers.memberId.in(deletedElementIds)
		            .and(currentRelationMembers.memberType.eq(
		        	    Element.elementEnumForElementType(elementType))))
		        .orderBy(currentRelationMembers.relationId.asc())
					  .list(currentRelationMembers.relationId));
	  		if (owningRelationIds.size() > 0)
	  		{
	  			throw new Exception(
	  				"Elements(s) to be deleted of type + " + elementType.toString() + " still used by " +
	  			  "other relation(s): " + StringUtils.join(owningRelationIds));
	  		}
  		}
  	}
  }
  
  private void checkForWayOwnershipErrors() throws Exception
  {
  	final NodeList deletedNodeIdXmlNodes = 
			XPathAPI.selectNodeList(changesetDoc, "//osmChange/delete/node/@id");
		Set<Long> deletedNodeIds = new HashSet<Long>();
		for (int i = 0; i < deletedNodeIdXmlNodes.getLength(); i++)
		{
			deletedNodeIds.add(Long.parseLong(deletedNodeIdXmlNodes.item(i).getNodeValue()));
		}
		final Set<Long> owningWayIds =
			new TreeSet<Long>(
				new SQLQuery(dbConn, DbUtils.getConfiguration(mapId))
          .from(currentWayNodes)
          .where(currentWayNodes.nodeId.in(deletedNodeIds))
          .orderBy(currentWayNodes.wayId.asc())
			    .list(currentWayNodes.wayId));
		if (owningWayIds.size() > 0)
		{
			throw new Exception(
				"Node(s) to be deleted still used by other way(s): " + StringUtils.join(owningWayIds));
		}
  }
  
  //TODO
  /*private void checkForCoordinateErrors()
  {
  	//way (type not equal to delete) and node
  	
  }*/
  
  public void checkForChildElementExistAndVisibilityErrors() throws Exception
  {
    //if a child element is referenced with a non-negative id and doesn't exist in the db, then fail
  	for (ElementType elementType : ElementType.values())
  	{
  		if (!elementType.equals(ElementType.Changeset))
  		{
  			final NodeList relationMemberIdXmlNodes = 
		  	  XPathAPI.selectNodeList(
		  	  	changesetDoc, 
		  	  	"//osmChange/create|update/relation/member[type = \"" + 
		  	  	  elementType.toString().toLowerCase() + "\"]/@id");
				Set<Long> relationMemberIds = new HashSet<Long>();
				for (int i = 0; i < relationMemberIdXmlNodes.getLength(); i++)
				{
					final long id = Long.parseLong(relationMemberIdXmlNodes.item(i).getNodeValue());
					if (id > 0)
					{
						relationMemberIds.add(id);
					}		
				}
				if (!Element.allElementsExist(mapId, elementType, relationMemberIds, dbConn))
				{
					throw new Exception(elementType.toString() + " member(s) don't exist for relation.");
				}
				if (!Element.allElementsVisible(mapId, elementType, relationMemberIds, dbConn))
				{
					throw new Exception(elementType.toString() + " member(s) aren't visible for relation.");
				}
  		}
  	}
  	
  	final NodeList wayNodeIdXmlNodes = 
  	  XPathAPI.selectNodeList(changesetDoc, "//osmChange/create|update/way/nd/@ref");
  	Set<Long> wayNodeIds = new HashSet<Long>();
		for (int i = 0; i < wayNodeIdXmlNodes.getLength(); i++)
		{
			final long id = Long.parseLong(wayNodeIdXmlNodes.item(i).getNodeValue());
			if (id > 0)
			{
				wayNodeIds.add(id);
			}		
		}
		if (!Element.allElementsExist(mapId, ElementType.Node, wayNodeIds, dbConn))
		{
			throw new Exception("Way node(s) don't exist for way.");
		}
		if (!Element.allElementsVisible(mapId, ElementType.Node, wayNodeIds, dbConn))
		{
			throw new Exception("Way node(s) aren't visible for way.");
		}
  }
}
