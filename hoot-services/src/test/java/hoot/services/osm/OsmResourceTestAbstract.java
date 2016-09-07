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
package hoot.services.osm;

import javax.ws.rs.core.Application;

import org.glassfish.jersey.test.JerseyTest;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.runner.RunWith;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.test.annotation.Rollback;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionStatus;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.context.support.AnnotationConfigWebApplicationContext;

import hoot.services.ApplicationContextUtils;
import hoot.services.HootServicesJerseyApplication;
import hoot.services.HootServicesSpringConfig;
import hoot.services.utils.MapUtils;


/*
 * Base class for tests that need to read/write OSM data to the services database
 */

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringConfig.class, loader = AnnotationConfigContextLoader.class)
@Transactional
@Rollback
public abstract class OsmResourceTestAbstract extends JerseyTest {
    private static final Logger logger = LoggerFactory.getLogger(OsmResourceTestAbstract.class);

    protected static long userId = -1;
    protected static long mapId = -1;

    public OsmResourceTestAbstract(String... controllerGroup) {
        super();
    }

    @BeforeClass
    public static void beforeClass() throws Exception {
        AnnotationConfigWebApplicationContext appContext = null;
        ApplicationContextUtils acu = null;

        try {
            appContext = new AnnotationConfigWebApplicationContext();
            appContext.register(HootServicesSpringConfig.class);
            appContext.refresh();

            acu = new ApplicationContextUtils();
            acu.setApplicationContext(appContext);

            PlatformTransactionManager txManager = appContext.getBean("transactionManager", PlatformTransactionManager.class);
            TransactionStatus ts = txManager.getTransaction(null);

            userId = MapUtils.insertUser();
            mapId = MapUtils.insertMap(userId);
            OsmTestUtils.setUserId(userId);
            OsmTestUtils.setMapId(mapId);

            txManager.commit(ts);
        }
        finally {
            if (appContext != null) {
                appContext.destroy();
            }
            if (acu != null) {
                acu.setApplicationContext(null);
            }
        }
    }

    @AfterClass
    public static void afterClass() throws Exception {
        AnnotationConfigWebApplicationContext appContext = null;
        ApplicationContextUtils acu = null;

        try {
            appContext = new AnnotationConfigWebApplicationContext();
            appContext.register(HootServicesSpringConfig.class);
            appContext.refresh();

            acu = new ApplicationContextUtils();
            acu.setApplicationContext(appContext);

            PlatformTransactionManager txManager = appContext.getBean("transactionManager", PlatformTransactionManager.class);
            TransactionStatus ts = txManager.getTransaction(null);

            MapUtils.deleteOSMRecord(mapId);

            appContext.destroy();
            acu.setApplicationContext(null);
            txManager.commit(ts);
        }
        finally {
            if (appContext != null) {
                appContext.destroy();
            }
            if (acu != null) {
                acu.setApplicationContext(null);
            }
        }
    }

    @Before
    public void beforeTest() throws Exception {}

    @After
    public void afterTest() throws Exception {}

    @Override
    protected Application configure() {
        return new HootServicesJerseyApplication();
    }
}
