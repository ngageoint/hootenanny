package hoot.services.readers.review;

import java.sql.SQLException;

import hoot.services.models.review.ReviewQueryMapper;

/**
 *
 */
public interface IReviewableQuery 
{
	public ReviewQueryMapper execQuery()  throws SQLException, Exception;
}
