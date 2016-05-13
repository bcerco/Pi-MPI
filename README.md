# Pi-MPI

Pi-MPI is a Message Passing Interface for a Raspberry Pi Beowulf Cluster.  It is
currently written in C, however, there are plans to implement it in other languages
such as Go.  Although it was designed and tested on a Pi cluster, it is not limited
to running on a Pi cluster.  It could be run on any cluster provided that the 
participating machines are running the Linux Kernel.

## How it works

There are two main parts to the system: pmpid and pmpiexec.  The former is the
service that is run at all times on each node in the cluster.  The latter is
how user programs are executed on the cluster.  

### pmpid

While running on each node, pmpid listens on a socket and waits until a connection
is made.  This first connection is made in order to receive the source code 
from the master that has just executed pmpiexec.  Once the source code is 
received, pmpid compiles and executes it.  It also listens on another socket
for connections made in order to send messages between nodes.

### pmpiexec

Running a parallel program on the cluster is done like so:
```sh
pmpiexec my_parallel.c 3
```
The 3 in the above command indicates how many nodes this program should run on,
not including the master.  In other words, if you've got a 4 node cluster and want
to execute code on the whole cluster then the number would be 3.  

If the program compiles, pmpiexec will distribute the source code to the other
nodes participating in the execution.  It will also execute the program that it 
just compiled on the master.

## How you can run it

Right now there is a shell script, setup.sh, which you can use to compile
pmpid and pmpiexec.  You'll need to compile pmpid on each node and then run it
in the background.  
```sh
./setup.h pmpid
./pmpid &
```
You will only need pmpiexec on the master node.
```sh
./setup.sh pmpiexec
```

You will also need "hosts.txt" on each node.  It is the list of the IP
addresses of each node in the cluster with the master IP being the first one 
in the list.  Additionally, each node needs its own "node_add.txt" file which 
contains the IP address of that node.
