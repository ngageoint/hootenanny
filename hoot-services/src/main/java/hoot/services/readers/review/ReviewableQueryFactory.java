package hoot.services.readers.review;

import hoot.services.geo.BoundingBox;

import java.sql.Connection;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 *
 */
public class ReviewableQueryFactory 
{
	@SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewableQueryFactory.class);
	
	public IReviewableQuery getReviewableQuery(final Connection c, final long mapid, final long seqid)
	{
		return new ReviewableQuery(c, mapid, seqid);
	}
	
	public IReviewableQuery getRandomReviewableQuery(final Connection c, final long mapid)
	{
		return new RandomReviewableQuery(c, mapid);
	}
	
	public IReviewableQuery getReviewableStatisticsQuery(final Connection c, final long mapid)
	{
		return new ReviewableStatisticsQuery(c, mapid);
	}
	
	public IReviewableQuery getAllReviewableItemsQuery(final Connection c, final long mapid, final BoundingBox bbox)
	{
		return new AllReviewableItemsQuery(c, mapid, bbox);
	}
}
