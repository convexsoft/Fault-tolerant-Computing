#!/bin/bash
mkdir test
for j in {0..4}
do
  echo "The ${j} fail number"
  for i in {1..10}
  do
    echo "The ${i} times testing - 1"
    echo "OPTION=false" > config.properties
    echo "FAILED_NUMBER=${j}" >> config.properties
    cat config.properties
    hadoop fs -rm -r /user/mio/crt/ 2> t.out
    hadoop com.sun.tools.javac.Main CRTMatrixD.java CRTMapper.java CRTReducer.java CRT.java
    jar cf CRT.jar CRT*.class
    hadoop jar CRT.jar CRT /user/mio/crt/input /user/mio/crt/output 2>> t1.out >> t1.out
    hadoop fs -cat /user/mio/crt/output/part-r-00000 >> t1.out
    rm config.properties t.out
    mv t1.out test/${j}_${i}_1.out
    echo "FINISHED 1/2"
    sleep 3

    echo "The ${i} times testing - 2"
    echo "OPTION=true" > config.properties
    echo "FAILED_NUMBER=${j}" >> config.properties
    cat config.properties
    hadoop fs -rm -r /user/mio/crt/ 2> t.out
    hadoop com.sun.tools.javac.Main CRTMatrixD.java CRTMapper.java CRTReducer.java CRT.java
    jar cf CRT.jar CRT*.class
    hadoop jar CRT.jar CRT /user/mio/crt/input /user/mio/crt/output 2>> t1.out >> t1.out
    hadoop fs -cat /user/mio/crt/output/part-r-00000 >> t1.out
    rm config.properties t.out
    mv t1.out test/${j}_${i}_2.out
    echo "FINISHED 2/2"
    sleep 3

  done
done

# python3 TESTING/test.py > t.out