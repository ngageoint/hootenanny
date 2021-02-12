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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.utils;

import java.net.Socket;
import java.io.Closeable;

import org.apache.commons.pool.PoolableObjectFactory;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Abstract class for dealing with creating poolable socket objects
 * 
 * @see Apache Commons Pool
 */
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
