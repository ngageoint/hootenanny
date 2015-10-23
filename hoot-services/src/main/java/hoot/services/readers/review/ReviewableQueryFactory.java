package hoot.services.readers.review;

import java.sql.Connection;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ReviewableQueryFactory {
	private static final Logger log = LoggerFactory.getLogger(ReviewableQueryFactory.class);
	
	public IReviewableQuery getNextReviewableQuery(final Connection c, final long mapid, final long seqid)
	{
		return new NextReviewableQuery(c, mapid, seqid);
	}
	
	public IReviewableQuery getRandomReviewableQuery(final Connection c, final long mapid)
	{
		return new RandomReviewableQuery(c, mapid);
	}
	
	public IReviewableQuery getReviewableStatisticsQuery(final Connection c, final long mapid)
	{
		return new ReviewableStatisticsQuery(c, mapid);
	}
	
	
}
