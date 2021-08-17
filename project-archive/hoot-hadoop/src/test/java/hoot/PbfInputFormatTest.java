package hoot;

import java.io.IOException;

import junit.framework.TestCase;

import org.apache.hadoop.fs.BlockLocation;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;

public class PbfInputFormatTest extends TestCase
{
  public void setUp()
  {
    HootHadoop.init();
  }
  
  public void testBasics() throws IOException
  {
    String localIn = TestUtils.composeInputDir(this);
    Path hdfsIn = TestUtils.composeInputHdfs(this);
    FileSystem fs = Utils.getFileSystem();
    
    fs.mkdirs(hdfsIn);
    Path splits = new Path(localIn + "../../SmallSplits.pbf");
    fs.copyFromLocalFile(splits, hdfsIn);
    
    BlockLocation[] bl = fs.getFileBlockLocations(fs.getFileStatus(splits), 0, 1);
    // if you're testing on a cluster with multiple hosts this may fail
    String host = bl[0].getHosts()[0];

    PbfInputFormat uut = new PbfInputFormat(hdfsIn.toString() + "/SmallSplits.pbf");
    assertEquals(12, uut.getSplitCount());
    assertEquals(1125, uut.getSplit(0).getStart());
    assertEquals(1123, uut.getSplit(0).getLength());
    assertEquals(2248, uut.getSplit(1).getStart());
    assertEquals(1123, uut.getSplit(1).getLength());
    assertEquals(3371, uut.getSplit(2).getStart());
    assertEquals(1123, uut.getSplit(2).getLength());
    assertEquals(4494, uut.getSplit(3).getStart());
    assertEquals(1123, uut.getSplit(3).getLength());
    assertEquals(5617, uut.getSplit(4).getStart());
    assertEquals(1123, uut.getSplit(4).getLength());
    assertEquals(6740, uut.getSplit(5).getStart());
    assertEquals(1123, uut.getSplit(5).getLength());
    assertEquals(7863, uut.getSplit(6).getStart());
    assertEquals(1123, uut.getSplit(6).getLength());
    assertEquals(8986, uut.getSplit(7).getStart());
    assertEquals(1124, uut.getSplit(7).getLength());
    assertEquals(10110, uut.getSplit(8).getStart());
    assertEquals(1124, uut.getSplit(8).getLength());
    assertEquals(11234, uut.getSplit(9).getStart());
    assertEquals(1127, uut.getSplit(9).getLength());
    assertEquals(12361, uut.getSplit(10).getStart());
    assertEquals(1127, uut.getSplit(10).getLength());
    assertEquals(13488, uut.getSplit(11).getStart());
    assertEquals(1127, uut.getSplit(11).getLength());

    for (int i = 0; i < 12; i++)
    {
      assertEquals(host, uut.getSplit(i).getLocations());
    }
  }
}
