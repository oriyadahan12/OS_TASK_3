# Sync and Kosaraju server

## Description

In this project to build a server of the KOSARJU algorithm in addition
We were required to realize 2 design patterns
1.PROACTOR
2.REACTOR

## running 
How to runnig?
1.in terminal go to the desired folder (`cd question_i` : 1<=i<=10).
2.write `make`.
3.write `./qi`: 1<=i<=10.
4.if the question requires a server and client connection.
Repeat step three in one terminal and in the others terminals(clients) .
work with Netcat program with the command `nc localhost 9034` for connect.

after that you can use with the following commands(in clients) :

1.`Newgraph n m <edge1> ... <edgeM> `
`n` means number of vertex (starting from 1).
`m` means number of edges (starting from 1).
for example :edge = 1 2 means that there is an edge between vertex 1 to vertex 2
after that the graph printed as list of neighborhoods.

2.`Kosaraju` - printed the SCC (Strongly Connected Component).

3.`Newedge u v` - add edge between u and v after that prined if sucsses .

4.`Removeedge u v` - remove edge between u and v after that prined if sucsses .
