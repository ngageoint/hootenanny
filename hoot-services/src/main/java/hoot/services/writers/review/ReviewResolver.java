package hoot.services.writers.review;

import hoot.services.models.osm.Changeset;

import java.sql.Connection;
import java.sql.Statement;
import java.util.HashMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * 
 */
public class ReviewResolver
{
  private static final Logger log = LoggerFactory.getLogger(ReviewResolver.class);
  
  private Connection conn;
	
  public ReviewResolver(final Connection conn) throws Exception
  {
    this.conn = conn;
  }
  
  /**
   * Resolves all reviews for a given map
   * 
   * @param mapId ID of the map owning the review data
   * @param userId user ID associated with the review data
   * @return the ID of the changeset used to resolve the reviews
   * @throws Exception
   */
  public long setAllReviewsResolved(final long mapId, final long userId) throws Exception
  {
  	//create a changeset
    java.util.Map<String, String> changesetTags = new HashMap<String, String>();
    changesetTags.put("bot", "yes");
    changesetTags.put("created_by", "hootenanny"); 
  	final long changesetId = Changeset.createChangeset(mapId, userId, changesetTags, conn);
  	Changeset.closeChangeset(mapId, changesetId, conn);
  	
  	/*
  	 * - mark all review relations belonging to the map as resolved
  	 * - update the changeset id for each review relation
  	 * - increment the version for each review relation
  	 */
  	String sql = "";
  	sql += "update current_relations_" + mapId;
  	sql += " set tags = tags || ('hoot:review:needs' => 'no'),";
  	sql += " changeset_id = " + changesetId + ",";
  	sql += " version = version + 1";
  	sql += " where tags->'type' = 'review'";
  	
  	Statement stmt = null;
  	try
  	{
  		stmt = conn.createStatement();
  		final int numRecordsUpdated = stmt.executeUpdate(sql);
  		log.debug(numRecordsUpdated + " records updated.");
  	}
  	finally
  	{
  		if (stmt != null)
  		{
  			stmt.close();
  		}
  	}
  	
  	return changesetId;
  }
}
