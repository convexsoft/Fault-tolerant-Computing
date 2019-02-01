import java.util.*;
import java.lang.*;
import java.io.*;

public class CRTMatrixA {
  int t,n,l,cnt,count,prefer;
  int[] data;
  int[] prime;
  List<List<Integer>> groups = new ArrayList<List<Integer>>(n);
  List<List<List<Integer>>> answers = new ArrayList<List<List<Integer>>>();
  public CRTMatrixA(int l, int n, int t,int[] prime){
    this.l = l;
    this.n = n;
    this.t = t;
    this.cnt = (t+1)*l;
    this.data = new int[cnt];
    for(int i=0;i<cnt;i++)
      this.data[i] = (i+1);
    this.count = 0;
    this.prime = prime;
    this.prefer = (int)((t+1)*l/n);
  }
  public int[][] run(){
    init();
    dfs(0);
    return findAns();
  }
  void init(){
      for (int i = 0; i < n; i++) {
        List<Integer> group = new ArrayList<Integer>();
        groups.add(group);
      }
  }
  void dfs(int pos){
    if(pos > cnt-1){
      for(int i=0;i<n;i++)
        if(0 == groups.get(i).size())
          return;
    boolean flag = true;
    for(int i=0;i<n;i++){
      Set<Integer> numbers = new HashSet<>();
      Iterator itr = groups.get(i).iterator();
      while(itr.hasNext()){
        int tmpnum = ((Integer)itr.next()).intValue()%l;
        Integer num = new Integer(tmpnum);
        if(numbers.contains(num))
          flag = false;
        else
          numbers.add(num);
      }
    }
    if(flag){
      List<List<Integer>> tmpgroups = new ArrayList<List<Integer>>(n);
      for(int i=0;i<n;i++){
        List<Integer> tmpgroup = new ArrayList<Integer>();
        if(groups.get(i).size()>prefer+1 || groups.get(i).size()<prefer)
          return;
        for(int j=0;j<groups.get(i).size();j++)
          tmpgroup.add(groups.get(i).get(j));
        tmpgroups.add(tmpgroup);
      }
      answers.add(tmpgroups);
      count++;
    }
    return;
  }
    groups.get(0).add(data[pos]);
    dfs(pos+1);
    for(int i=1;i<n;i++){
      if(groups.get(i-1).size()>1){
        groups.get(i-1).remove(groups.get(i-1).size()-1);
        groups.get(i).add(data[pos]);
        dfs(pos+1);
      if(i==n-1)
        groups.get(i).remove(groups.get(i).size()-1);
      }
      else{
        groups.get(i-1).remove(groups.get(i-1).size()-1);
        break;
      }
    }
  }
   int[][] findAns(){
    // System.out.println("CNT IS :" + count);
    int mxcnt = l;
    int ans = 0;
    int tmpmxcnt;
    int mxnum = 1;
    int tmpmxnum;
    for(int i=0;i<l;i++)
      mxnum *= prime[l-1];
    for(int i=0;i<count;i++){
      tmpmxcnt = 0;
      tmpmxnum = 0;
      // System.out.println("------------");
      for(int j=0;j<n;j++){
        int temp = 1;
        tmpmxcnt = Math.max(tmpmxcnt,answers.get(i).get(j).size());
        for(int k=0;k<answers.get(i).get(j).size();k++){
           // System.out.print(answers.get(i).get(j).get(k)+" ");
          temp *= prime[(answers.get(i).get(j).get(k)-1)%l];
        }
        // System.out.println();
        tmpmxnum = Math.max(tmpmxnum,temp);
      }
      if(mxcnt>=tmpmxcnt && mxnum>=tmpmxnum){
        mxcnt = tmpmxcnt;
        mxnum = tmpmxnum;
        ans = i;
      }
    }
    int[][] matrixA = new int[n][l];
    for(int i=0;i<n;i++){
      for(int j=0;j<answers.get(ans).get(i).size();j++){
        int tmp = (((Integer)answers.get(ans).get(i).get(j)).intValue()-1)%l;
        matrixA[i][tmp] = 1;
      }
    }
    return matrixA;
  }
}