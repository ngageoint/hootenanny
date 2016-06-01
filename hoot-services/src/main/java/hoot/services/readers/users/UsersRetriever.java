/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.readers.users;

import java.sql.Connection;
import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;

import hoot.services.db.DbUtils;
import hoot.services.db2.QUsers;
import hoot.services.db2.Users;


public class UsersRetriever {
    private static final Logger log = LoggerFactory.getLogger(UsersRetriever.class);
    private Connection _conn;

    public UsersRetriever(final Connection cn) {
        _conn = cn;
    }

    public List<Users> retrieveAll() throws Exception {
        List<Users> res = new ArrayList<>();
        try {
            QUsers users = QUsers.users;
            SQLQuery query = _getAllQuery();
            res = query.list(users);
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }
        return res;
    }

    protected SQLQuery _getAllQuery() throws Exception {
        SQLQuery query = new SQLQuery(this._conn, DbUtils.getConfiguration());
        try {
            QUsers users = QUsers.users;
            query.from(users).orderBy(users.displayName.asc());
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }

        return query;
    }
}
