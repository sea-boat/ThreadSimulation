# ThreadSimulation
Java thread simulation in jvm.

```
javac -h ./ com/seaboat/Thread.java

g++ -fPIC -c -std=c++0x com_seaboat_Thread.cpp -I /usr/java/jdk1.8.0_111/include/ -I /usr/java/jdk1.8.0_111/include/linux/

g++ -shared com_seaboat_Thread.o -o com_seaboat_Thread.so

java com.seaboat.Thread
```
