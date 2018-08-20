package hoot.services.language;

import java.net.Socket;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.utils.PoolableSocketConnectionFactory;

public class PoolableJoshuaConnectionFactory extends PoolableSocketConnectionFactory
{
  private static final Logger logger = LoggerFactory.getLogger(PoolableJoshuaConnectionFactory.class);

  public PoolableJoshuaConnectionFactory(String host, int port, int timeout)
  {
    super(host, port, timeout);
  }

  public Object makeObject() throws Exception 
  {
    logger.trace("Making new connection...");
    return new JoshuaConnection(new Socket(host, port), timeout);
  }
}
