import java.lang.Math;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.conf.Configuration;
import java.util.StringTokenizer;
import org.apache.hadoop.mapreduce.TaskAttemptID;
import org.apache.hadoop.mapreduce.MRJobConfig;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.mapreduce.Cluster;

public class CRTMapper extends Mapper<LongWritable, Text, Text, IntWritable>{

  private Text remainer_key = new Text();
  private int dividend,divisor;
  private long remainer;
  private Integer d;
  private Configuration conf;
  private Boolean option;

@Override
  public void run(Context context) throws IOException, InterruptedException {
    conf = context.getConfiguration();
    option = conf.getBoolean("OPTION",true);
    setup(context);
    try {
      while (context.nextKeyValue()) {
        map(context.getCurrentKey(), context.getCurrentValue(), context);
      }
    } finally {
      cleanup(context);
    }
  }

@Override
  protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
    System.out.println("The map key: "+key+" value: "+value);
    StringTokenizer itr = new StringTokenizer(value.toString());
    d = Integer.parseInt(itr.nextToken());
    Integer tmp = Integer.parseInt(itr.nextToken());
    divisor = tmp.intValue();
    remainer = 1;
    remainer_key.set(String.valueOf(divisor));
    while (itr.hasMoreTokens()) {
      tmp = Integer.parseInt(itr.nextToken());
      dividend = tmp.intValue();
      remainer = remainer * (dividend % divisor) % divisor;
    }
  }
@Override
    protected void cleanup(Context context) throws IOException, InterruptedException {
    Cluster cluster = new Cluster(conf);
    Job job = cluster.getJob(context.getJobID());
    TaskAttemptID tid = context.getTaskAttemptID();

    if(option){
      if(d.intValue()==0){
        context.write(remainer_key, new IntWritable((int)remainer));
        System.out.println("Output: "+remainer_key+" "+remainer);
      }
    } else {
      Boolean task = false;
      String tmp = tid.toString();
      if(d.intValue()==1 && tmp.substring(tmp.length()-1).equals("0")){
        System.out.println(tid);
        System.out.println("Failed");
        job.failTask(tid);
      } else {
        System.out.println("The map print:" + remainer_key + " "+remainer);
        context.write(remainer_key, new IntWritable((int)remainer));
      }
    }
  }
}
