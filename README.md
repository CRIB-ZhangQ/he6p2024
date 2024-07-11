# crib-artemis-workdir

Artemis work directory for online+offline analysis of CRIB experiment

NOTE: it use cmake version

# Environment

I confirmed it will work well in this Environment

* Almalinux 9.2  
* gcc 11.3.1 
* cmake 3.20.2 
* ROOT (CERN) 6.28/04

# Reference

artemis source: [artemis cmake version (develop branch)](https://www.cns.s.u-tokyo.ac.jp/gitlab/Okawa/crib-art_analysis)

CRIB artemis setting source: [crib-art_analysis](https://www.cns.s.u-tokyo.ac.jp/gitlab/Okawa/crib-art_analysis)

# Example

An example steering file steering/example.tmpl.yaml includes the basic way to describe processors and histogram definitions. Please look at it.

To run the example steering file, please do at the command prompt of artemis.
No special libraries are required.

```console
$ artlogin
$ a
artemis> add steering/example.tmpl.yaml NUM=1000 MAX=10
artemis> res
```

In this run, one thousands of random numbers between 0 and 10 will be generated in a brach named 'random' and two histogram which contain the same histogram difinitions will be created.

# Manual for CRIB experiment 

under preparation
<p><a href="https://okawak.github.io/artemis_crib/" target="_blank">manual</a></p>	
