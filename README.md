# 1. Database design by Bond Energy Algorithm

The bond energy algorithm (BEA) was developed to determine how to group data and how to physically place data on RDBMS.

It can be used to cluster attributes based on usage and then perform logical or physical design accordingly. With BEA, the affinity (bond) between database attributes is based on common usage

This repo contains two separate procedures 

*  AAGenerator.cpp 
*  CAGenerator.cpp

AAGenerator takes the input of all attributes of a relation, a set of queries and their access frequencies at diﬀerent sites, and produces the output of an aﬃnity matrix AA

CA Generator takes input of an aﬃnity matrix AA and produces a clustered aﬃnity matrix CA.

the Attribute Aﬃnity is measured by the extended Otsuka-Ochiai coefﬁcient (https://en.wikipedia.org/wiki/Yanosuke Otsuka) i. The following equations show the details of the computation, where n is the number of attributes, and m is the number of sites, Aik is the number of times Attribute Ai is accessed by Query qk , considering of all sites. 

![alt text](https://github.com/robeespi/Database-design-by-Bond-Energy-Algorithm/blob/main/bea.jpeg)

# 2. Code Structure Overview

AAGenerator.cpp 

1. Accept three input text ﬁles in the order of Attributes (att), Queries (query) and Access Frequencies (acc) as command line arguments

2. Read the features of the relation (att)

3. Read the queries (qs)
    
4. Get attribute usage values (att,qs)
    
5. Read Access frequency matrix ACC
    
6. Filling the attribute affinity matrix AA (usage, ACC, size.att, qs.size)
    
7. Output: Printing out the required attribute aﬃnity matrix (AA). 
    

CAGenerator.cpp

1.Accept  input affinity matrix (aa) as a command line argument

2.Read the Affinity Matrix AA

3.Calculate the Clustered Affinity Matrix CA by Bond Energy Algorithm (calculating the bond and contribution of a placement as 2 separated functions)

4.Printing out Matrix CA
