package hoot.services.readers.review;

import java.sql.Connection;

import hoot.services.db.DbUtils;
import hoot.services.geo.BoundingBox;
import hoot.services.models.review.AllReviewableItems;
import hoot.services.models.review.ReviewableItem;
import hoot.services.models.review.ReviewableStatistics;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * 
 */
public class ReviewableReader 
{
	@SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewableReader.class);
	
	private ReviewableQueryFactory _fac = new ReviewableQueryFactory();
	private Connection _conn;
	
	public ReviewableReader(final Connection  conn)
	{
		_conn = conn;
	}
	
	public ReviewableItem getReviewableItem(final long mapid, final long seqid) throws Exception
	{
		ReviewableItem r = null;
		r = (ReviewableItem)_fac.getReviewableQuery(_conn, mapid, seqid).execQuery();
		return r;
	}
	
	public ReviewableItem getRandomReviewableItem(final long mapid) throws Exception
	{
		ReviewableItem r = null;
		
		r = (ReviewableItem)_fac.getRandomReviewableQuery(_conn, mapid).execQuery();
		
		return r;
	}

	public ReviewableStatistics getReviewablesStatistics(final long mapid) throws Exception
	{
		ReviewableStatistics r = null;
		r = (ReviewableStatistics)_fac.getReviewableStatisticsQuery(_conn, mapid).execQuery();
		
		return r;
	}
	
	public AllReviewableItems getAllReviewableItems(final long mapid, final BoundingBox bbox) throws Exception
	{
		AllReviewableItems r = null;
		r = (AllReviewableItems)_fac.getAllReviewableItemsQuery(_conn, mapid, bbox).execQuery();
		
		return r;
	}
}
