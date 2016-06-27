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
package hoot.services.writers.user;


import java.sql.Connection;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.Configuration;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLInsertClause;

import hoot.services.db.DbUtils;
import hoot.services.db2.QUsers;
import hoot.services.db2.Users;


public class UserSaver {
    private static final Logger logger = LoggerFactory.getLogger(UserSaver.class);

    private final Connection conn;

    public UserSaver(Connection cn) {
        conn = cn;
    }

    public Users getOrSaveByEmail(String userEmail) {
        Users ret = (new SQLQuery(conn, DbUtils.getConfiguration()))
                .from(QUsers.users)
                .where(QUsers.users.email.equalsIgnoreCase(userEmail)).singleResult(QUsers.users);

        // none then create
        if (ret == null) {
            long nCreated = insert(userEmail);
            if (nCreated > 0) {
                ret = (new SQLQuery(conn, DbUtils.getConfiguration()))
                        .from(QUsers.users)
                        .where(QUsers.users.email.equalsIgnoreCase(userEmail)).singleResult(QUsers.users);
            }
        }

        return ret;
    }

    public long insert(String email) {
        SQLInsertClause cl = createInsertClause(email);
        long nInserted = cl.execute();
        return nInserted;
    }

    protected SQLInsertClause createInsertClause(String email) {
        Configuration configuration = DbUtils.getConfiguration();
        SQLInsertClause cl = new SQLInsertClause(conn, configuration, QUsers.users).
                                      columns(QUsers.users.email, QUsers.users.displayName).values(email, email);
        return cl;
    }
}
