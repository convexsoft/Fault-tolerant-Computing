import java.util.*;
import java.lang.*;
import java.io.*;

public class CRTMatrixD {
  int n,l,t;
  int[] prime;
  int[][] ans;
  int[] multy;
  PriorityQueue<Entry> q = new PriorityQueue<>();
  Queue<Entry> tmpq = new LinkedList<Entry>();
  public CRTMatrixD(int l, int n, int t,int[] prime){
    this.l = l;
    this.n = n;
    this.t = t;
    this.prime = prime;
    ans = new int[n][l];
  }

  class Entry implements Comparable<Entry> {
    private int key;
    private int value;
    public Entry(int key, int value) {
        this.key = key;
        this.value = value;
    }
    public int getKey(){
      return this.key;
    }
    @Override
    public int compareTo(Entry other) {
        return this.getKey()-other.getKey();
    }
  }

  private void findAns(){
    for(int i=0;i<n;i++){
      q.add(new Entry(1,i));
    }
    Entry tmp;
    for(int i=l-1;i>=0;i--){
      // System.out.println();
      for(int j=0;j<t+1;j++){
        tmp = q.poll();
        // System.out.println("test "+tmp.key+" "+tmp.value);
        tmp.key = tmp.key*prime[i];
        ans[tmp.value][i]=1;
        tmpq.add(tmp);
      }
      for(int j=0;j<t+1;j++)
        q.add(tmpq.poll());
    }
  }
  public int[][] run(){
    findAns();
    return ans;
  }
}