package hoot.services.utils;

import java.net.Socket;
import java.io.Closeable;

import org.apache.commons.pool.PoolableObjectFactory;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public abstract class PoolableSocketConnectionFactory implements PoolableObjectFactory
{
  private static final Logger logger = LoggerFactory.getLogger(PoolableSocketConnectionFactory.class);

  protected String host;
  protected int port;
  protected int timeout;

  public PoolableSocketConnectionFactory(String host, int port, int timeout)
  {
    this.host = host;
    this.port = port;
    this.timeout = timeout;
  }

  public void destroyObject(Object obj) throws Exception 
  {
    if (obj instanceof Closeable) 
    {
      logger.trace("Destroying object...");
      ((Closeable)obj).close();
    }
  }

  public boolean validateObject(Object obj) 
  {
    boolean isValid = false;
    if (obj instanceof SocketConnection) 
    {
      isValid = !((SocketConnection)obj).getSocket().isClosed();
    }
    logger.trace("Object is valid: " + isValid);
    return isValid;
  }

  public void activateObject(Object obj) throws Exception {}

  public void passivateObject(Object obj) throws Exception {}
}
