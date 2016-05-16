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
    private static final Logger log = LoggerFactory.getLogger(UserSaver.class);

    private Connection _conn;

    public UserSaver(final Connection cn) {
        _conn = cn;
    }

    public Users getOrSaveByEmail(final String userEmail) throws Exception {
        Users ret = null;

        try {
            QUsers users = QUsers.users;

            ret = (new SQLQuery(_conn, DbUtils.getConfiguration())).from(users)
                    .where(users.email.equalsIgnoreCase(userEmail)).singleResult(users);
            // none then create
            if (ret == null) {
                long nCreated = insert(userEmail);
                if (nCreated > 0) {

                    ret = (new SQLQuery(_conn, DbUtils.getConfiguration())).from(users)
                            .where(users.email.equalsIgnoreCase(userEmail)).singleResult(users);
                }
            }
        }
        catch (Exception ex) {
            log.error("Failed to save user by email: " + ex.getMessage());
            throw ex;
        }
        return ret;
    }

    public long insert(final String email) throws Exception {
        long nInserted = 0;
        try {
            SQLInsertClause cl = _createInsertClause(email);
            if (cl != null) {
                nInserted = cl.execute();
            }
            else {
                throw new Exception("Invalid insert clause.");
            }

        }
        catch (Exception ex) {
            String err = ex.getMessage();
            log.error(err);
            throw ex;
        }
        return nInserted;
    }

    protected SQLInsertClause _createInsertClause(final String email) throws Exception {
        SQLInsertClause cl = null;
        try {
            Configuration configuration = DbUtils.getConfiguration();
            QUsers users = QUsers.users;
            cl = new SQLInsertClause(_conn, configuration, users).columns(users.email, users.displayName).values(email,
                    email);
        }
        catch (Exception ex) {
            log.error(ex.getMessage());
            throw ex;
        }
        return cl;
    }
}
