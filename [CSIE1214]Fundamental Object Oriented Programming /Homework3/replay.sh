#!/bin/bash

rm -r out/
javac -cp . -sourcepath src -d out/ src/*.java
java -cp out/ TestcaseReplayer $1