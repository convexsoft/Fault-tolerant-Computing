// javac matrixTest.java CRTMatrixD.java CRTMatrixA.java
// javac Tmp.java CRTMatrixB.java
// javac Tmp.java CRTMatrixD.java
// java -classpath . matrixTest
import java.util.Arrays;

public class matrixTest{

  private static int[] prime = {3,5,7};
  private static int l,n,t;

  private static int[] count(int[][] ans){
    int[] num = new int[ans.length];
    for(int i=0;i<ans.length;i++){
      int tmp = 0,ttmp=1;
      for(int j=0;j<ans[i].length;j++){
        if(ans[i][j]==1)
          tmp = tmp | ttmp;
        ttmp = ttmp<<1;
      }
      num[i] = tmp;
    }
    Arrays.sort(num);
    for(int i=0;i<ans.length;i++)
      System.out.print(num[i]+" ");
    System.out.println();
    return num;
  }

  private static int[] testA(){
    CRTMatrixA matrix = new CRTMatrixA(l,n,t,prime);
    int[][] ans = matrix.run();
    for(int i=0;i<ans.length;i++){
      for(int j=0;j<ans[i].length;j++)
        System.out.print(ans[i][j]+" ");
      System.out.println();
    }
    return count(ans);
  }

  private static int[] testD(){
    CRTMatrixD matrix = new CRTMatrixD(l,n,t,prime);
    int[][] ans = matrix.run();
    for(int i=0;i<ans.length;i++){
      for(int j=0;j<ans[i].length;j++)
        System.out.print(ans[i][j]+" ");
      System.out.println();
    }
    return count(ans);
  }

  private static void compare(){
    int[] a1 = testA();
    int[] a2 = testD();
    boolean flag = true;
    for(int i=0;i<l;i++)
      if(a1[i]!=a2[i]){
        flag = false;
        break;
      }
    if(!flag){
      System.out.print(" The difference "+l+" "+n+" "+t+" ");
      for(int i=0;i<l;i++)
        System.out.print(prime[i]+" ");
      System.out.println();

    }
  }

  public static void main(String[] args){
    l = Integer.parseInt(args[0]);
    n = Integer.parseInt(args[1]);
    t = Integer.parseInt(args[2]);
    prime = new int[l];
    for(int i=3;i<l+3;i++){
      prime[i-3] = Integer.parseInt(args[i]);
    }
    compare();
  }
}