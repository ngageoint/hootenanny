
package hoot.services.language;

import java.net.Socket;
import java.io.DataOutputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.SocketException;
import java.io.IOException;

import hoot.services.utils.SocketConnection;

/*
*/
public class JoshuaConnection extends SocketConnection
{
  public JoshuaConnection(Socket socket, int timeout) throws SocketException, IOException
  {
    super(socket, timeout);
    this.reader = new BufferedReader(new InputStreamReader(this.socket.getInputStream(), JoshuaLanguageTranslator.ENCODING));
    this.writer = new DataOutputStream(this.socket.getOutputStream());
  }
}
