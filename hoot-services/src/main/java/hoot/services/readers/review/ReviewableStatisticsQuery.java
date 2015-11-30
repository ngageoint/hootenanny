package hoot.services.readers.review;

import hoot.services.models.review.ReviewQueryMapper;
import hoot.services.models.review.ReviewableStatistics;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * 
 */
public class ReviewableStatisticsQuery extends ReviewableQueryBase implements IReviewableQuery  
{
	@SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewableStatisticsQuery.class);
	
	public ReviewableStatisticsQuery(final Connection c, final long mapid)
	{
		super(c,mapid);
	}
	
	protected long getTotalReviewablesCount()  throws SQLException, Exception
	{
  	long recordCount = 0;
 
		try(
				Statement stmt = getConnection().createStatement();
				ResultSet rs = stmt.executeQuery(_getTotalReviewableCountQueryString())
				)
		{
			while(rs.next())
      {
				recordCount = rs.getLong("totalcnt");         
      }

		}
	
		return recordCount;

	}
	
	protected long getRemainingReviewablesCount()  throws SQLException, Exception
	{
  	long recordCount = 0;
  	 
		try(
				Statement stmt = getConnection().createStatement();
				ResultSet rs = stmt.executeQuery(_getUnreviewedCountQueryString())
				)
		{
			while(rs.next())
      {
				recordCount = rs.getLong("remaining");         
      }

		}

		return recordCount;
	}

	
	public ReviewQueryMapper execQuery()  throws SQLException, Exception
	{
		long nTotal = getTotalReviewablesCount();
		long nUnReviewed = getRemainingReviewablesCount();
		
		ReviewableStatistics ret = new  ReviewableStatistics(nTotal, nUnReviewed);
		return ret;
	}
	
	protected String _getTotalReviewableCountQueryString()
	{
		return "select count(*) as totalcnt from current_relations_" + getMapId() + 
				" where tags->'type' = 'review'";
	}
	
	protected String _getUnreviewedCountQueryString()
	{
		return "select count(*) as remaining from current_relations_" + getMapId() + 
				" where tags->'hoot:review:needs' = 'yes'";
	}
	
}
