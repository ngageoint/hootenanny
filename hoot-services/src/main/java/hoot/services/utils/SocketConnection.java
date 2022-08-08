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
 * @copyright Copyright (C) 2018, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.utils;

import java.io.IOException;
import java.net.Socket;
import java.net.SocketException;
import java.io.Closeable;
import java.io.Reader;
import java.io.OutputStream;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * A class encapsulating a readable/writable socket
 */
public abstract class SocketConnection implements Closeable
{
  private static final Logger logger = LoggerFactory.getLogger(SocketConnection.class);

  protected Socket socket;
  protected Reader reader;
  protected OutputStream writer;
  protected int timeout = 5000;

  public SocketConnection(Socket socket, int timeout) throws SocketException
  {
    this.socket = socket;
    this.socket.setSoTimeout(timeout);
  }

  public Socket getSocket() { return socket; }
  public Reader getReader() { return reader; }
  public OutputStream getWriter() { return writer; }

  /**
   * Closes the socket and all of its resources
   */
  public void close() throws IOException, SocketException 
  {
    logger.trace("Closing connection...");
    if (reader != null) 
    {
      reader.close();
    }
    if (writer != null) 
    {
      writer.close();
    }
    if (socket != null) 
    {
      socket.close();
    }
  }
}
