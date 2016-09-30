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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.osm;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Timestamp;

import javax.ws.rs.core.Application;

import org.apache.commons.dbcp.BasicDataSource;
import org.glassfish.jersey.test.JerseyTest;
import org.junit.After;
import org.junit.Before;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.AnnotationConfigApplicationContext;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionStatus;

import hoot.services.HootServicesJerseyTestApplication;
import hoot.services.HootServicesSpringTestConfig;
import hoot.services.utils.MapUtils;


/*
 * Base class for tests that need to read/write OSM data to the services database
 */

public abstract class OSMResourceTestAbstract extends JerseyTest {

    protected long userId;
    protected long mapId;

    private ApplicationContext applicationContext;

    private PlatformTransactionManager txManager;

    private TransactionStatus transactionStatus;


    public OSMResourceTestAbstract(String... controllerGroup) {}

    @Before
    public void beforeTest() throws Exception {
        if (this.txManager == null) {
            this.txManager = applicationContext.getBean("transactionManager", PlatformTransactionManager.class);
        }
        this.transactionStatus = txManager.getTransaction(null);

        userId = MapUtils.insertUser();
        mapId = MapUtils.insertMap(userId);
        OSMTestUtils.setUserId(userId);
        OSMTestUtils.setMapId(mapId);
    }

    @After
    public void afterTest() throws Exception {
        txManager.rollback(transactionStatus);
        this.transactionStatus = null;
    }

    @Override
    protected Application configure() {
        this.applicationContext = new AnnotationConfigApplicationContext(HootServicesSpringTestConfig.class);
        return new HootServicesJerseyTestApplication(this.applicationContext);
    }

    protected Timestamp getCurrentDBTime() throws Exception {
        BasicDataSource dbcpDatasource = applicationContext.getBean("dataSource", BasicDataSource.class);

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
