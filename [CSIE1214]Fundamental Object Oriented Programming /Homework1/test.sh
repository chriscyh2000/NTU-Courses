#!/bin/bash

rm -r out/
cd src
javac -cp . TestSuite.java -d ../out/
cd ..
java -cp out/ TestSuite always-play-first-card && \
java -cp out/ TestSuite normal-no-error-play1 && \
java -cp out/ TestSuite normal-no-error-play2 && \
java -cp out/ TestSuite illegal-actions && \
java -cp out/ TestSuite straight && \
java -cp out/ TestSuite fullhouse