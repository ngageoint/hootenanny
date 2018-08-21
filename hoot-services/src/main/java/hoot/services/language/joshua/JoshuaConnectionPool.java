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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

package hoot.services.language;

import static hoot.services.HootProperties.*;

import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

import java.net.Socket;

import org.apache.commons.pool.impl.GenericObjectPool;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/*
*/
public class JoshuaConnectionPool
{
  private static final Logger logger = LoggerFactory.getLogger(JoshuaConnectionPool.class);

  private Map<String, JoshuaServiceInfo> services = null;
  private Map<String, GenericObjectPool<JoshuaConnection>> connectionPools = new HashMap<String, GenericObjectPool<JoshuaConnection>>();
  private int maxPoolSize = 100;

  public JoshuaConnectionPool(Map<String, JoshuaServiceInfo> services, int maxPoolSize) 
  {
    this.services = services;
    this.maxPoolSize = maxPoolSize;
  }

  public synchronized JoshuaConnection borrowObject(String langCode) throws Exception
  {
    if (!services.containsKey(langCode))
    {
      throw new Exception("No server exists for language code: " + langCode + ".");
    }

    GenericObjectPool<JoshuaConnection> connectionPool = connectionPools.get(langCode);
    if (connectionPool == null)
    {
      logger.error("Creating Joshua connection pool for language code: " + langCode + "...");
      //assuming joshua tcp server is always running local with the hoot services; if it ever needed to be run on another 
      //server, then we'd probably run it as an http server anyway which would involve some refactoring
      connectionPool = 
        new GenericObjectPool<JoshuaConnection>(
          new PoolableJoshuaConnectionFactory(
            "localhost", services.get(langCode).getPort(), Integer.parseInt(JOSHUA_CONNECTION_TIMEOUT)), maxPoolSize);
      connectionPool.setMaxWait(Integer.parseInt(JOSHUA_CONNECTION_MAX_WAIT));
      connectionPool.setTestOnBorrow(true);
      connectionPool.setWhenExhaustedAction(GenericObjectPool.WHEN_EXHAUSTED_BLOCK);
      connectionPools.put(langCode, connectionPool);
    }

    JoshuaConnection connection = null;
    try
    {
      connection = (JoshuaConnection)connectionPool.borrowObject();
    }
    catch (Exception e)
    {
      throw new Exception("Error retrieving connection from pool for language code: " + langCode + ".  error: " + e.getMessage());
    }
    return connection;
  }

  public synchronized void returnObject(String langCode, JoshuaConnection connection) throws Exception
  {
    connectionPools.get(langCode).returnObject(connection);
  }

  public synchronized void close() throws Exception
  {
    logger.error("Closing Joshua connection pool...");

    boolean errorEncountered = false;
    List<String> errorLangCodes = new ArrayList<String>();
    String errorMsgBase = "Error closing connection pool for language code: ";
    for (Map.Entry<String, GenericObjectPool<JoshuaConnection>> poolEntry : connectionPools.entrySet())
    {
      String langCode = poolEntry.getKey();
      GenericObjectPool<JoshuaConnection> pool = poolEntry.getValue();
      String errorMsg = errorMsgBase + langCode + ".";
      try
      {
        pool.clear();
        pool.close();
      }
      catch (Exception e)
      { 
        errorEncountered = true;
        errorLangCodes.add(langCode);
        logger.error(errorMsg);
      }
    }

    if (errorEncountered)
    {
      throw new Exception(errorMsgBase + String.join(",", errorLangCodes.toArray(new String[]{})));
    }
  }
}
