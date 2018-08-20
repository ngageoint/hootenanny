package hoot.services.utils;

import java.io.IOException;
import java.net.Socket;
import java.net.SocketException;
import java.io.Closeable;
import java.io.Reader;
import java.io.OutputStream;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

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
