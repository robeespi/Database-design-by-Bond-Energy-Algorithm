# Database design by Bond Energy Algorithm

The bond energy algorithm (BEA) was developed to determine how to group data and how to physically place data on RDBMS.

It can be used to cluster attributes based on usage and then perform logical or physical design accordingly. With BEA, the affinity (bond) between database attributes is based on common usage

This repo contains two separate procedures 

AAGenerator.cpp 
CAGenerator.cpp

AAGenerator takes the input of all attributes of a relation, a set of queries and their access frequencies at diﬀerent sites, and produces the output of an aﬃnity matrix AA

CA Generator takes input of an aﬃnity matrix AA and produces a clustered aﬃnity matrix CA.

the Attribute Aﬃnity is measured by the extended Otsuka-Ochiai coefﬁcient (https://en.wikipedia.org/wiki/Yanosuke Otsuka) i. The following equations show the details of the computation, where n is the number of attributes, and m is the number of sites, Aik is the number of times Attribute Ai is accessed by Query qk , considering of all sites. 

![alt text](https://github.com/robeespi/Database-design-by-Bond-Energy-Algorithm/blob/main/bea.jpeg)

