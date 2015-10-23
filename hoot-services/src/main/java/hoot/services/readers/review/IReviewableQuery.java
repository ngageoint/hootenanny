package hoot.services.readers.review;

import java.sql.SQLException;

import hoot.services.models.review.ReviewQueryMapper;

import com.mysema.query.Tuple;
import com.mysema.query.sql.SQLQuery;

public interface IReviewableQuery {
	public ReviewQueryMapper execQuery()  throws SQLException, Exception;
}
