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
import hoot.services.db2.CurrentNodes;
import hoot.services.db2.QCurrentNodes;
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
	protected static final QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
	
	private Document changesetDoc;
	private long mapId;
	private Connection dbConn;
	
	public ChangesetErrorChecker(final Document changesetDoc, final long mapId, Connection dbConn)
	{
		this.changesetDoc = changesetDoc;
		this.mapId = mapId;
		this.dbConn = dbConn;
	}
	
	/**
	 * 
	 * 
	 * @throws Exception
	 */
	public void checkForVersionErrors() throws Exception
  {
		log.debug("Checking for element version errors...");
		
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
						if (entityChangeType.equals(EntityChangeType.CREATE))
						{
							if (parsedVersion != 0)
							{
								throw new Exception(
		      				"Invalid version: " + parsedVersion + " specified for created " + 
								  elementType.toString() + " with ID: " + id + "; expected version 0.");
							}
						}
						else
						{
							elementIdsToVersionsFromChangeset.put(id, parsedVersion);
						}
					}
					
					if (!entityChangeType.equals(EntityChangeType.CREATE) && 
							elementIdsToVersionsFromChangeset.size() > 0)
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
	
	/**
	 * 
	 * 
	 * @throws Exception
	 */
	public void checkForOwnershipErrors() throws Exception
	{
		log.debug("Checking for child element ownership errors...");
		checkForRelationOwnershipErrors();
		checkForWayOwnershipErrors();
	}
  
	/*
	 * 
	 */
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
	  		if (deletedElementIds.size() > 0)
	  		{
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
  }
  
  /*
   * 
   */
  private void checkForWayOwnershipErrors() throws Exception
  {
  	final NodeList deletedNodeIdXmlNodes = 
			XPathAPI.selectNodeList(changesetDoc, "//osmChange/delete/node/@id");
		Set<Long> deletedNodeIds = new HashSet<Long>();
		for (int i = 0; i < deletedNodeIdXmlNodes.getLength(); i++)
		{
			deletedNodeIds.add(Long.parseLong(deletedNodeIdXmlNodes.item(i).getNodeValue()));
		}
		if (deletedNodeIds.size() > 0)
		{
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
  }
  
  /**
   * 
   * 
   * @throws Exception
   * @todo is this check actually necessary?
   */
  public void checkForElementVisibilityErrors() throws Exception
  {
  	log.debug("Checking for element visibility errors...");
  	
    //if a child element is referenced and is invisible, then fail.  elements are created visible 
  	//by default with a create changeset, which is why we can skip checking negative id's (elements 
  	//being created).  we're also skipping checking anything in the delete sections, b/c since the
  	//elements are being deleted, they're visibility no longer is important.
  	
  	for (ElementType elementType : ElementType.values())
  	{
  		if (!elementType.equals(ElementType.Changeset))
  		{
				Set<Long> relationMemberIds = new HashSet<Long>();
  			//I know there's a way to use the '|' operator in the xpath query to make this simpler...
  			//its just not working
  			for (EntityChangeType entityChangeType : EntityChangeType.values())
  			{
  				if (!entityChangeType.equals(EntityChangeType.DELETE))
  				{
  					final NodeList relationMemberIdXmlNodes = 
				  	  XPathAPI.selectNodeList(
				  	  	changesetDoc, 
				  	  	"//osmChange/" + entityChangeType.toString().toLowerCase() + 
				  	  	  "/relation/member[@type = \"" + elementType.toString().toLowerCase() + "\"]");
						for (int i = 0; i < relationMemberIdXmlNodes.getLength(); i++)
						{
							//don't need to check for empty id here, b/c previous checking would have already
							//errored out for it
							final long id = 
								Long.parseLong(
									relationMemberIdXmlNodes.item(i).getAttributes().getNamedItem("ref").getNodeValue());
							if (id > 0)
							{
								relationMemberIds.add(id);	
							}
						}
  				}
					
  			}
  			
				if (!Element.allElementsVisible(mapId, elementType, relationMemberIds, dbConn))
				{
					throw new Exception(elementType.toString() + " member(s) aren't visible for relation.");
				}
  		}
  	}
  	
  	Set<Long> wayNodeIds = new HashSet<Long>();
  	for (EntityChangeType entityChangeType : EntityChangeType.values())
		{
  		if (!entityChangeType.equals(EntityChangeType.DELETE))
  		{
  			final NodeList wayNodeIdXmlNodes = 
		  	  XPathAPI.selectNodeList(
		  	  	changesetDoc, "//osmChange/" + entityChangeType.toString().toLowerCase() + "/way/nd");
				for (int i = 0; i < wayNodeIdXmlNodes.getLength(); i++)
				{
				  //don't need to check for empty id here, b/c previous checking would have already
					//errored out for it
					final long id = 
						Long.parseLong(
							wayNodeIdXmlNodes.item(i).getAttributes().getNamedItem("ref").getNodeValue());
					if (id > 0)
					{
						wayNodeIds.add(id);	
					}
				}
  		}
		}
		if (!Element.allElementsVisible(mapId, ElementType.Node, wayNodeIds, dbConn))
		{
			throw new Exception("Way node(s) aren't visible for way.");
		}
  }
  
  /**
   * 
   * 
   * @return
   * @throws Exception
   */
  public Map<Long, CurrentNodes> checkForElementExistenceErrors() throws Exception
  {
  	log.debug("Checking for element existence errors...");
  	
    //if an element is referenced (besides in its own create change) and doesn't exist in the db, 
  	//then fail
  	
  	Map<ElementType, Set<Long>> elementTypesToElementIds = new HashMap<ElementType, Set<Long>>();
  	for (ElementType elementType : ElementType.values())
  	{
  		if (!elementType.equals(ElementType.Changeset))
  		{
  			elementTypesToElementIds.put(elementType, new HashSet<Long>());
  		}
  	}
  	
  	final String emptyIdErrorMsg = "Element in changeset has empty ID.";
  	
  	//check relation members
  	for (ElementType elementType : ElementType.values())
  	{
  		if (!elementType.equals(ElementType.Changeset))
  		{
  			final NodeList relationMemberIdXmlNodes = 
		  	  XPathAPI.selectNodeList(
		  	  	changesetDoc, 
		  	  	"//osmChange/*/relation/member[@type = '" + elementType.toString().toLowerCase() + "']");
  			for (int i = 0; i < relationMemberIdXmlNodes.getLength(); i++)
				{
  				long id;
  				try
  				{
  					//log.debug(
  						//relationMemberIdXmlNodes.item(i).getAttributes().getNamedItem("ref").getNodeValue());
  					id = 
  					  Long.parseLong(
  						  relationMemberIdXmlNodes.item(i).getAttributes().getNamedItem("ref").getNodeValue());
  				}
  				catch (NumberFormatException e)
  				{
  					throw new Exception(emptyIdErrorMsg);
  				}
  				catch (NullPointerException e)
  				{
  					throw new Exception(emptyIdErrorMsg);
  				}
					if (id > 0)
					{
						elementTypesToElementIds.get(elementType).add(id);
					}
				}
  		}
  	}
  	
  	//check way nodes
  	final NodeList wayNodeIdXmlNodes = 
  		XPathAPI.selectNodeList(changesetDoc, "//osmChange/*/way/nd/@ref");
		for (int i = 0; i < wayNodeIdXmlNodes.getLength(); i++)
		{
			long id;
			try
			{
				id = Long.parseLong(wayNodeIdXmlNodes.item(i).getNodeValue());
			}
			catch (NumberFormatException e)
			{
				throw new Exception(emptyIdErrorMsg);
			}
			catch (NullPointerException e)
			{
				throw new Exception(emptyIdErrorMsg);
			}
			if (id > 0)
			{
				elementTypesToElementIds.get(ElementType.Node).add(id);
			}
		}
		
		//check top level elements
		for (EntityChangeType entityChangeType : EntityChangeType.values())
		{
			if (!entityChangeType.equals(EntityChangeType.CREATE))
			{
				
				for (ElementType elementType : ElementType.values())
		  	{
		  		if (!elementType.equals(ElementType.Changeset))
		  		{
		  			final NodeList elementIdXmlNodes = 
	  		  	  XPathAPI.selectNodeList(
	  		  	  	changesetDoc, 
	  		  	  	"//osmChange/" + entityChangeType.toString().toLowerCase() + "/" + 
	  		  	  	elementType.toString().toLowerCase() + "/@id");
	  				//log.debug(String.valueOf(nodeIdXmlNodes.getLength()));
	  				for (int i = 0; i < elementIdXmlNodes.getLength(); i++)
	  				{
	  					long id;
	  					try
	  					{
	  						id = Long.parseLong(elementIdXmlNodes.item(i).getNodeValue());
	  					}
	  					catch (NumberFormatException e)
	    				{
	    					throw new Exception(emptyIdErrorMsg);
	    				}
	    				catch (NullPointerException e)
	    				{
	    					throw new Exception(emptyIdErrorMsg);
	    				}
	  					if (id > 0)
	  					{
	  						elementTypesToElementIds.get(elementType).add(id);
	  					}
	  				}
		  		}
		  	}
			}
		}
		
		for (ElementType elementType : ElementType.values())
  	{
  		if (!elementType.equals(ElementType.Changeset))
  		{
  			if (!Element.allElementsExist(
  					  mapId, elementType, elementTypesToElementIds.get(elementType), dbConn))
  			{
  				//TODO: list the id's and types of the elements that don't exist
  				throw new Exception("Element(s) being referenced don't exist.");
  			}
  		}
  	}
		
		if (elementTypesToElementIds.get(ElementType.Node).size() > 0)
		{
			return
				new SQLQuery(dbConn, DbUtils.getConfiguration(mapId))
	        .from(currentNodes)
	        .where(currentNodes.id.in(elementTypesToElementIds.get(ElementType.Node)))
			    .map(currentNodes.id, currentNodes);
		}
		return null;
  }
}
