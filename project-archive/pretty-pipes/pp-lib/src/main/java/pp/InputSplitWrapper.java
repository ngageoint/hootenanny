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
 * @copyright Copyright (C) 2015 Maxar (http://www.maxar.com/)
 */
package pp;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.Writable;
import org.apache.hadoop.mapred.InputSplit;

public class InputSplitWrapper implements InputSplit, Writable
{
  static
  {
    HootHadoop.init();
  }

  pp.InputSplit _is;
  
  InputSplitWrapper()
  {
    
  }
  
  InputSplitWrapper(pp.InputSplit is)
  {
    _is = is;
  }

  @Override
  public long getLength() throws IOException
  {
    return _is.getLength();
  }
  
  public long getStart()
  {
    return _is.getStart();
  }

  @Override
  public String[] getLocations() throws IOException
  {
    return _is.getLocations().split(";");
  }

  @Override
  public void readFields(DataInput in) throws IOException
  {
    String libDeps = in.readUTF();
    Plugins.loadAllLibraries(libDeps);
    String className = in.readUTF();
    _is = pp.InputSplit.create(className);
    int length = in.readInt();
    byte[] data = new byte[length];
    //System.out.printf("Java read data length: %d\n", length);
    in.readFully(data);
    _is.readFields(data);
  }

  @Override
  public void write(DataOutput out) throws IOException
  {
    byte[] data = _is.writeFields();
    out.writeUTF(Plugins.getLoadedLibraries());
    out.writeUTF(_is.getClassName());
    //System.out.printf("Java write data length: %d\n", data.length); 
    out.writeInt(data.length);
    out.write(data);
  }
}
