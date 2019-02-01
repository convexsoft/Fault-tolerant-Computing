import java.util.*;
import java.lang.*;
import java.io.*;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;

import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
// import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.chain.ChainMapper;
// import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.fs.FileSystem;
// import java.io.OutputStreamWriter;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

public class CRT extends Configured implements Tool {
  static Boolean OPTION;
  // static final int[] PRIMES = {2,3,5,7,11,13,17,19,23,29,31,37};
  static final int[] PRIMES = {23,29,31,37,41,43,47,53,59,61,67};
  static final int NUMBER = 11;
  static int FAILED_NUMBER;
  static int x1;
  static int x2;

  static final Logger logger = LogManager.getLogger();
  static Configuration conf = new Configuration();

  public static void addProperty() throws IOException{
    Properties prop = new Properties();
    InputStream input = null;
    input = new FileInputStream("config.properties");
    // load a properties file
    prop.load(input);
    // get the property value and print it out
    OPTION = Boolean.valueOf(prop.getProperty("OPTION"));
    FAILED_NUMBER = Integer.valueOf(prop.getProperty("FAILED_NUMBER"));
    long mx = 1;
    for(int i=0;i<PRIMES.length;i++)
      mx = mx * PRIMES[i];
    long tmp = (long)(Math.random()*50)+1;
    x1 = (int)(Math.sqrt(mx)-tmp);
    tmp = (long)(Math.random()*50)+1;
    x2 = (int)(Math.sqrt(mx)-tmp);
    // System.out.println("X1 and X2 is: "+x1+" "+x2);
    System.out.println("The First Number="+x1);
    System.out.println("The Second Number="+x2);
    // System.out.println("X1 and X2 is: "+x1+" "+x2);
  }

  private static int[] failNode(){
    int n = NUMBER;
    int[] ans = new int[n];
    int[] rand_number = new int[n];
    Random rand = new Random();
    for(int i=0;i<n;i++){
      rand_number[i] = i;
      ans[i] = 0;
    }
    for(int i=0;i<FAILED_NUMBER;i++){
      int pos = rand.nextInt(n-i);
      // swap(rand[n-i],rand[pos]);
      int tmp = rand_number[pos];
      rand_number[pos] = rand_number[n-i-1];
      rand_number[n-i-1] = tmp;
      ans[tmp] = 1;
    }
    return ans;
  }

  public static void init() throws IOException {
    int[] fail_node = failNode();
    int LEN = PRIMES.length;
    int[][] matrixA = new int[NUMBER][LEN];
    // If with coding, the maximal mapper number equals to number, otherwise the maximal mapper number equals to the prime's number
    int max_num = OPTION ? NUMBER : LEN;

    if(OPTION){
    // CRTMatrixA mtx = new CRTMatrixA(LEN,NUMBER,FAILED_NUMBER,PRIMES);
      CRTMatrixD mtx = new CRTMatrixD(LEN,NUMBER,FAILED_NUMBER,PRIMES);
      matrixA = mtx.run();
    }

    String tmpoutput="hdfs://localhost:9000/user/mio/crt/input/data";
    String output;
    FileSystem Phdfs = FileSystem.get(new Configuration());
    String fileStr = "";

    for(int i=0;i<max_num;i++){
      fileStr = (fail_node[i]==1) ? "1 " : "0 "; // 1 means the node will fail
      if(OPTION){
        int multi = 1;
        String tstr = "";
        for(int j=0;j<LEN;j++){
          if(matrixA[i][j]==1){
            multi *= PRIMES[j];
            tstr += String.valueOf(PRIMES[j])+" ";
          }
        }
        conf.set(String.valueOf(multi),tstr);
        fileStr += String.valueOf(multi)+" "+String.valueOf(x1)+" "+String.valueOf(x2);
        System.out.println("Config: "+tstr);
      } else {
        fileStr += String.valueOf(PRIMES[i])+" "+String.valueOf(x1)+" "+String.valueOf(x2);
      }
      output = tmpoutput + String.valueOf(i) +".txt";
        Path fname=new Path(output);
        BufferedWriter out=new BufferedWriter(new OutputStreamWriter(Phdfs.create(fname,true)));
        System.out.println(fileStr);
        out.write(fileStr);
        out.close();
    }
  }

  @Override
  public int run(String[] args) throws Exception {
    Job job = Job.getInstance(conf, "crt");
    job.setJarByClass(CRT.class);

    job.setMapperClass(CRTMapper.class);
    job.setMapOutputKeyClass(Text.class);
    job.setMapOutputValueClass(IntWritable.class);

    job.setReducerClass(CRTReducer.class);
    job.setNumReduceTasks(1);
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(LongWritable.class);

    TextInputFormat.addInputPath(job, new Path(args[0]));
    FileOutputFormat.setOutputPath(job, new Path(args[1]));

    return job.waitForCompletion(true) ? 0 : 1;
  }

  public static void main(String[] args) throws Exception {
    addProperty();
    conf.setBoolean("OPTION",OPTION);
    System.out.println("Config is: "+OPTION+" "+FAILED_NUMBER);
    init();
    long start = new Date().getTime();
    int exitCode = ToolRunner.run(new CRT(),args);
    long end = new Date().getTime();
    System.out.println("Whole time="+(end-start));
    System.exit(exitCode);
  }
}