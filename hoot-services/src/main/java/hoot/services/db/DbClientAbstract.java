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
package hoot.services.db;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.transaction.PlatformTransactionManager;


/**
 * Job classes making database calls should subclass this
 */
public abstract class DbClientAbstract {
    @SuppressWarnings("unused")
    private static final Logger log = LoggerFactory.getLogger(DbClientAbstract.class);

    private ClassPathXmlApplicationContext appContext;
    protected PlatformTransactionManager transactionManager;
    protected boolean simulateFailure = false;
    protected long testDelayMilliseconds = -1;

    public DbClientAbstract() throws Exception {
        baseInit();
    }

    public DbClientAbstract(final boolean simulateFailure, final long testDelayMilliseconds) throws Exception {
        baseInit();
        this.simulateFailure = simulateFailure;
        this.testDelayMilliseconds = testDelayMilliseconds;
    }

    protected void baseInit() throws Exception {
        appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
        transactionManager = appContext.getBean("transactionManager", PlatformTransactionManager.class);
    }
}
