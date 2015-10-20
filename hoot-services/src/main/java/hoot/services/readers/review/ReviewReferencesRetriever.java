package hoot.services.readers.review;

import hoot.services.controllers.osm.MapResource;
import hoot.services.db.DbUtils;
import hoot.services.db2.CurrentRelationMembers;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.ElementInfo;
import hoot.services.review.ReviewUtils;

import java.lang.reflect.InvocationTargetException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;

/**
 * Retrieves element references to reviews for a query element
 */
public class ReviewReferencesRetriever
{
	private static final Logger log = LoggerFactory.getLogger(ReviewReferencesRetriever.class);
	
	private static final QCurrentRelationMembers currentRelationMembers = 
		QCurrentRelationMembers.currentRelationMembers;
	
	private Connection conn;
	
  public ReviewReferencesRetriever(final Connection conn) throws Exception
  {
  	this.conn = conn;
  }
  
  //would like to do this with QueryDSL, rather than straight JDBC, but don't know how to 
  //do the tags part of the query with QueryDSL
  private List<Long> getUnresolvedReviewRelations(final ElementInfo queryElementInfo, 
  	final long mapId) throws SQLException
  {
  	final String currentRelationMembersTableName = "current_relation_members_" + mapId;
		final String currentRelationsTableName = "current_relations_" + mapId;
		String sql = "";
  	sql += "select relation_id from " + currentRelationMembersTableName;
  	sql += " join " + currentRelationsTableName + " on " + currentRelationMembersTableName + 
  	  ".relation_id = " + currentRelationsTableName + ".id";
  	sql += " where " + currentRelationMembersTableName + ".member_id = " + 
  		queryElementInfo.getElementId() + " and " + currentRelationMembersTableName +
  	  ".member_type = '" + 
  		Element.elementTypeFromString(queryElementInfo.getElementType()).toString().toLowerCase() + 
  	  "' and " + currentRelationsTableName + ".tags->'hoot:review:needs' = 'yes'";
  	
  	Statement stmt = null;
  	ResultSet rs = null;
  	List<Long> relationIds = new ArrayList<Long>();
  	try
  	{
  		stmt = conn.createStatement();
  	  rs = stmt.executeQuery(sql);
  		while (rs.next())
  		{
  			relationIds.add(rs.getLong(1));
  		}
  	}
  	finally
  	{
  		if (stmt != null)
  		{
  			stmt.close();
  		}
  		if (rs != null)
  		{
  			rs.close();
  		}
  	}
  	
  	return relationIds;
  }
	
  /**
   * Retrieves all other unresolved element references to reviews for a given element
   * 
   * @param requestingElementInfo element whose review references are to be retrieved
   * @return a list containing all features the input feature needs to be reviewed with
   * @throws Exception 
   * @throws InvocationTargetException 
   * @throws NoSuchMethodException 
   * @throws ClassNotFoundException 
   * @throws IllegalAccessException 
   * @throws InstantiationException 
   */
	public List<ElementInfo> getUnresolvedReferences(final ElementInfo queryElementInfo) 
		throws InstantiationException, IllegalAccessException, ClassNotFoundException, 
		NoSuchMethodException, InvocationTargetException, Exception
  {
		log.debug("requestingElementInfo: " + queryElementInfo.toString());
		List<ElementInfo> references = new ArrayList<ElementInfo>();
		
	  final long mapIdNum = MapResource.validateMap(queryElementInfo.getMapId(), conn);
    assert(mapIdNum != -1);
    
    //check for query element existence
    Set<Long> elementIds = new HashSet<Long>();
		elementIds.add(queryElementInfo.getElementId());
		if (StringUtils.trimToNull(queryElementInfo.getElementType()) == null ||
				!Element.allElementsExist(
				  mapIdNum, Element.elementTypeFromString(queryElementInfo.getElementType()), 
				  elementIds, conn))
		{
			ReviewUtils.handleError(
				new Exception(
					"Element with ID: " + queryElementInfo + " and type: " + 
				  queryElementInfo.getElementType() + " does not exist."), 
					"", 
					false);
		}
		
		//select all unresolved review relation id's from current relation members where member id = 
		//requesting element's member id and the element type = the requesting element type
		final List<Long> unresolvedReviewRelationIds = 
			getUnresolvedReviewRelations(queryElementInfo, mapIdNum);
		
		//select all relation members in the group of remaining unresolved relation id's where the 
		//member's id is not equal to the requesting element's id and the member's type is not = to 
		//the requesting element's type
		final List<CurrentRelationMembers> referencedMembers = 
	    new SQLQuery(conn, DbUtils.getConfiguration(mapIdNum))
	      .from(currentRelationMembers)
	      .where(
	      	currentRelationMembers.relationId.in(unresolvedReviewRelationIds)
	      	  .and(currentRelationMembers.memberId.ne(queryElementInfo.getElementId())
	      	    .or(currentRelationMembers.memberType.ne(
	      	    		  Element.elementEnumFromString(queryElementInfo.getElementType())))))
	      .orderBy(
	      	currentRelationMembers.relationId.asc(), currentRelationMembers.memberId.asc(), 
	      	currentRelationMembers.sequenceId.asc())
	      .list(currentRelationMembers);
		
		//return all elements corresponding to the filtered down set of relation members
		for (CurrentRelationMembers member : referencedMembers)
		{
			references.add(
				new ElementInfo(
					queryElementInfo.getMapId(), 
					member.getMemberId(), 
					Element.elementTypeForElementEnum(member.getMemberType()).toString().toLowerCase()));
		}
		
		return references;
  }
}