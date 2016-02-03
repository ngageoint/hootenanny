package hoot.services.readers.users;

import hoot.services.db.DbUtils;
import hoot.services.db2.QUsers;
import hoot.services.db2.Users;

import java.sql.Connection;
import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;

public class UsersRetriever {
	 private static final Logger log = LoggerFactory.getLogger(UsersRetriever.class);
	 private Connection _conn;
	 
	 public UsersRetriever(final Connection cn) {
		 _conn = cn;
	 }
	 
	 public List<Users> retrieveAll() throws Exception
	 {
		 List<Users> res = new ArrayList<>();
		 try
			{
			 QUsers users = QUsers.users;
				SQLQuery query = _getAllQuery();
				res = query.list(users);
			}
			catch (Exception ex)
			{
				log.error(ex.getMessage());
				throw ex;
			}
			return res;
	 }
	 
	 protected SQLQuery _getAllQuery() throws Exception
		{
			SQLQuery query = new SQLQuery(this._conn, DbUtils.getConfiguration());
			try
			{
				QUsers users = QUsers.users;
				query.from(users)
				.orderBy(users.displayName.asc());
			}
			catch (Exception ex)
			{
				log.error(ex.getMessage());
				throw ex;
			}
			
			return query;
		}
}
