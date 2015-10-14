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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.job;

import java.sql.Connection;
import java.sql.Timestamp;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.db2.QMaps;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.ModelDaoUtils;
import javax.ws.rs.Consumes;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionDefinition;
import org.springframework.transaction.TransactionStatus;
import org.springframework.transaction.support.DefaultTransactionDefinition;

/**
 * Non-WPS service endpoint for the conflated data review process
 *
 * @todo Unfortunately, not having these default values as attributes in the methods makes
  validation impossible, but having them as attributes renders their config values useless.
  Need to come up with a better way to handle default values.  Is there some way to populate the
  attribute values directly from a file?
 */
@Path("/review")
public class ReviewResource
{
  private static final Logger log = LoggerFactory.getLogger(ReviewResource.class);

  //These parameters are passed in by the unit tests only.  With better unit test coverage,
  //these params could probably go away.
  public static long testDelayMilliseconds = 0;
  public static boolean simulateFailure = false;
  //TODO: see #6270
  public static String reviewRecordWriter = "reviewPrepareDbWriter2";

  private ClassPathXmlApplicationContext appContext;
  private PlatformTransactionManager transactionManager;

  public ReviewResource() throws Exception
  {
    log.debug("Reading application settings...");
    appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
    log.debug("Initializing transaction manager...");
    transactionManager = appContext.getBean("transactionManager", PlatformTransactionManager.class);
  }


 
}