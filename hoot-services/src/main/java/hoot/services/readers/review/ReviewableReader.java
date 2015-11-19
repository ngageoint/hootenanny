package hoot.services.readers.review;

import java.sql.Connection;

import hoot.services.db.DbUtils;
import hoot.services.models.review.ReviewableItem;
import hoot.services.models.review.ReviewableStatistics;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ReviewableReader 
{
	@SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewableReader.class);
	
	private ReviewableQueryFactory _fac = new ReviewableQueryFactory();
	
	public ReviewableItem getReviewableItem(final long mapid, final long seqid) throws Exception
	{
		ReviewableItem r = null;
		try(Connection conn = DbUtils.createConnection())
		{
			r = (ReviewableItem)_fac.getReviewableQuery(conn, mapid, seqid).execQuery();
		}
		
		return r;
	}
	
	
	public ReviewableItem getRandomReviewableItem(final long mapid) throws Exception
	{
		ReviewableItem r = null;
		try(Connection conn = DbUtils.createConnection())
		{
			r = (ReviewableItem)_fac.getRandomReviewableQuery(conn, mapid).execQuery();
		}
		
		return r;
	}
	
	
	public ReviewableStatistics getReviewablesStatistics(final long mapid) throws Exception
	{
		ReviewableStatistics r = null;
		try(Connection conn = DbUtils.createConnection())
		{
			r = (ReviewableStatistics)_fac.getReviewableStatisticsQuery(conn, mapid).execQuery();
		}
		
		return r;
	}
}
