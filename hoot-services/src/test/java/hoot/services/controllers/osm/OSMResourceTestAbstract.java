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
package hoot.services.controllers.osm;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Timestamp;

import org.apache.commons.dbcp.BasicDataSource;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionStatus;

import hoot.services.testsupport.HootServicesJerseyTestAbstract;
import hoot.services.testsupport.MapUtils;


/*
 * Base class for tests that need to read/write OSM data to the services database
 */

public abstract class OSMResourceTestAbstract extends HootServicesJerseyTestAbstract {

    private static PlatformTransactionManager txManager;
    private static TransactionStatus transactionStatus;
    private static BasicDataSource dbcpDatasource;

    protected long userId;
    protected long mapId;

    static {
        txManager = applicationContext.getBean("transactionManager", PlatformTransactionManager.class);
        dbcpDatasource = applicationContext.getBean("dataSource", BasicDataSource.class);
    }

    public OSMResourceTestAbstract(String... controllerGroup) {}


    @BeforeClass
    public static void beforeClass() {}

    @AfterClass
    public static void afterClass() {}

    @Before
    public void beforeTest() throws Exception {
        transactionStatus = txManager.getTransaction(null);

        userId = MapUtils.insertUser();
        mapId = MapUtils.insertMap(userId);
        OSMTestUtils.setUserId(userId);
        OSMTestUtils.setMapId(mapId);
    }

    @After
    public void afterTest() throws Exception {
        txManager.rollback(transactionStatus);
        transactionStatus = null;
    }

    protected Timestamp getCurrentDBTime() throws Exception {
        try (Connection conn = dbcpDatasource.getConnection()) {
            String sql = "SELECT now()";
            try (PreparedStatement stmt = conn.prepareStatement(sql)) {
                ResultSet rs = stmt.executeQuery();
                rs.next();
                return rs.getTimestamp(1);
            }
        }
    }
}
