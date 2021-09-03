#!/bin/bash

rm -r out/
javac -cp . -sourcepath src -d out/ src/*.java
java -cp out/ TestSuite cd-ls-mkdir &&\
java -cp out/ TestSuite cd-ls-mkdir-rm &&\
java -cp out/ TestSuite cd-ls-mkdir-rm-touch &&\
java -cp out/ TestSuite cd-ls-mkdir-rm-touch-error &&\
java -cp out/ TestSuite all &&\
java -cp out/ TestSuite bonus