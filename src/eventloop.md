Overview
========

High Priority Thread
--------------------
Double Audio Buffer
+-----------------+-----------------+
| Receive Input 1 | Receive Input 2 |
| Process Input 2 | Process Input 1 |
+-----------------+-----------------+

Triple Audio Buffer
+-----------------+-----------------+-----------------+
| Receive Input 1 | Receive Input 2 | Receive Input 3 |
| Process Input 2 | Process Input 1 | Process Input 2 |
+-----------------+-----------------+-----------------+

Process Function
> Write Input to Buffer
> Advance Write Pointer
