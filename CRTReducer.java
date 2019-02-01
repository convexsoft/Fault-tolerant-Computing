import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;
import java.util.*;
import org.apache.hadoop.conf.Configuration;

public class CRTReducer extends Reducer<Text,IntWritable,Text,LongWritable> {
  private LongWritable result = new LongWritable();
  private Text test = new Text("Result: ");
  private Queue<Integer> divisor_queue = new LinkedList<Integer>();
  private Queue<Integer> remainer_queue = new LinkedList<Integer>();
  Set<Integer> primes = new HashSet<>();
  public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException {

    int remainer = 1;
    int divisor = Integer.parseInt(key.toString());
    for (IntWritable value : values) {
      remainer = value.get();
    }
    Configuration conf = context.getConfiguration();
    if(conf.getBoolean("OPTION",true)){
      String[] primestr = conf.get(key.toString()).split(" ");
      int[] prime = new int[primestr.length];
      for(int i=0;i<primestr.length;i++)
        prime[i] = Integer.parseInt(primestr[i]);
      for(int i=0;i<primestr.length;i++){
        Integer tmp = new Integer(prime[i]);
        if(primes.contains(tmp))
          continue;
        else{
          primes.add(tmp);
          divisor_queue.add(prime[i]);
          remainer_queue.add(remainer%prime[i]);
        }
      }
    } else {
      Integer tmp = new Integer(divisor);
      if(!primes.contains(tmp)){
        primes.add(tmp);
        divisor_queue.add(divisor);
        remainer_queue.add(remainer);
      }
    }
  }

  private void residue_int(){
    int len = divisor_queue.size();
    int radix[] = new int[len];
    int value[] = new int[len];
    int invradix[][] = new int[len][len];

    Integer tmp_radix[] = divisor_queue.toArray(new Integer[0]);
    Integer tmp_value[] = remainer_queue.toArray(new Integer[0]);

    for(int i=0;i<len;i++){
      radix[i] = tmp_radix[i].intValue();
      value[i] = tmp_value[i].intValue();
    }

    for(int mult_inv_of = 0; mult_inv_of<len-1; mult_inv_of++)
      for(int with_respect_to = mult_inv_of+1; with_respect_to<len; with_respect_to++){
        int a, b, temp, u, v;
        for(a=radix[mult_inv_of], b=radix[with_respect_to],u=0,v=1; a!=0; ){
          temp = v;
          v = u - (b/a) * v;
          u = temp;
          temp = b;
          b = a;
          a = temp % a;
        }
        if(u<0) u = u + radix[with_respect_to];
        invradix[mult_inv_of][with_respect_to] = u;
      }
      long v[] = new long[len];
      int n;
      for(int x=0; x<len; x++)
        for(v[x] = value[x], n=0; n<x; n++){
          if(v[x]>=v[n])
            v[x] = ((v[x]-v[n]) * invradix[n][x]) % radix[x];
          else
            v[x] = radix[x] - (((v[n]-v[x]) * invradix[n][x]) % radix[x]);
        }
      long res = 0;
      for(int x=len-1; x>0; x--)
        res = (res + v[x]) * radix[x-1];
      res = res + v[0];
      result.set(res);
  }

@Override
  protected void cleanup(Context context) throws IOException, InterruptedException {
    residue_int();
    context.write(test, result);
  }
}
