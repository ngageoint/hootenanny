
package hoot.services.language;

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

  private Map<String, JoshuaServer> servers = null;
  private Map<String, GenericObjectPool<JoshuaConnection>> connectionPools = new HashMap<String, GenericObjectPool<JoshuaConnection>>();
  private int maxPoolSize = 100;

  public JoshuaConnectionPool(Map<String, JoshuaServer> servers, int maxPoolSize) 
  {
    this.servers = servers;
    this.maxPoolSize = maxPoolSize;
  }

  public synchronized JoshuaConnection borrowObject(String langCode) throws Exception
  {
    if (!servers.containsKey(langCode))
    {
      throw new Exception("No server exists for language code: " + langCode + ".");
    }

    GenericObjectPool<JoshuaConnection> connectionPool = connectionPools.get(langCode);
    if (connectionPool == null)
    {
      logger.error("Creating Joshua connection pool for language code: " + langCode + "...");
      //assuming joshua tcp server is always running local with the hoot services; if it ever needed to be run on another 
      //server, then we'd probably run it as an http server anyway which would involve some refactoring
      //TODO: read from config
      connectionPool = 
        new GenericObjectPool<JoshuaConnection>(
          new PoolableJoshuaConnectionFactory("localhost", servers.get(langCode).getPort(), 10000), maxPoolSize);
      connectionPool.setMaxWait(10000);
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
