package hoot;

import java.io.IOException;
import java.util.LinkedList;
import java.util.List;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapred.FileInputFormat;
import org.apache.hadoop.mapred.InputFormat;
import org.apache.hadoop.mapred.InputSplit;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.JobContext;
import org.apache.hadoop.mapred.RecordReader;
import org.apache.hadoop.mapred.Reporter;

public class PbfInputFormatJava implements InputFormat<FloatWritable, NullWritable>
{
  static
  {
    System.out.println("Initializing...");
    System.out.flush();
    HootHadoop.init();
  }

  static class PipesDummyRecordReader implements RecordReader<FloatWritable, NullWritable>
  {
    float progress = 0.0f;

    public PipesDummyRecordReader(Configuration job, InputSplit split) throws IOException
    {
    }

    public FloatWritable createKey()
    {
      return null;
    }

    public NullWritable createValue()
    {
      return null;
    }

    public synchronized void close() throws IOException
    {
    }

    public synchronized long getPos() throws IOException
    {
      return 0;
    }

    public float getProgress()
    {
      return progress;
    }

    public synchronized boolean next(FloatWritable key, NullWritable value) throws IOException
    {
      progress = key.get();
      return true;
    }
  }

  @Override
  public RecordReader<FloatWritable, NullWritable> getRecordReader(InputSplit genericSplit, JobConf job, Reporter arg2)
    throws IOException
  {
    return new PipesDummyRecordReader(job, genericSplit);
  }

  @Override
  public InputSplit[] getSplits(JobConf job, int numSplits) throws IOException
  {
    String path = FileInputFormat.getInputPaths(job)[0].toString();

    PbfInputFormat pif = new PbfInputFormat(path);

    InputSplit[] result = new InputSplit[pif.getSplitCount()];
    for (int i = 0; i < pif.getSplitCount(); i++)
    {
      PbfInputSplit pis = pif.getSplit(i);
      result[i] = new PbfInputSplitJava(path, pis.getStart(), pis.getLength(), pis.getLocations()
        .split(";"));
    }

    return result;
  }

}
