package hoot;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.Writable;
import org.apache.hadoop.mapred.InputSplit;

public class PbfInputSplitJava implements InputSplit, Writable
{
  static
  {
    HootHadoop.init();
  }

  private long _start, _length;
  private String[] _locations;
  private String _path;
  
  PbfInputSplitJava()
  {
    
  }
  
  PbfInputSplitJava(String path, long start, long length, String[] locations)
  {
    _path = path;
    _start = start;
    _length = length;
    _locations = locations;
  }

  @Override
  public long getLength() throws IOException
  {
    return _length;
  }
  
  public long getStart()
  {
    return _start;
  }

  @Override
  public String[] getLocations() throws IOException
  {
    return _locations;
  }

  @Override
  public void readFields(DataInput in) throws IOException
  {
    _start = in.readLong();
    _length = in.readLong();
    _path = in.readUTF();    
  }

  @Override
  public void write(DataOutput out) throws IOException
  {
    out.writeLong(_start);
    out.writeLong(_length);
    out.writeUTF(_path);    
  }

}
