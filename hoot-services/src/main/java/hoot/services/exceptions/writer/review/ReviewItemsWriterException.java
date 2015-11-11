package hoot.services.exceptions.writer.review;

@SuppressWarnings("serial")
public class ReviewItemsWriterException extends Exception {

	private String sql = "";
	public ReviewItemsWriterException(final String sqlStmt, final String msg) {		
		super(msg);
		sql = sqlStmt;
	}
	
	public final String getSql()
	{
		return sql;
	}
}
