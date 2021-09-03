#!/bin/bash

rm -r out/
javac -cp "provided.jar:." -sourcepath src -d out/ src/*.java
java -cp "provided.jar:out/" TestSuite Test-SelfHealing && \
java -cp "provided.jar:out/" TestSuite Test-Petrochemical-1 && \
java -cp "provided.jar:out/" TestSuite Test-Petrochemical-2 && \
java -cp "provided.jar:out/" TestSuite Test-Cheerup && \
java -cp "provided.jar:out/" TestSuite Test-Curse && \
java -cp "provided.jar:out/" TestSuite Test-SelfExplosion && \
java -cp "provided.jar:out/" TestSuite Test-Summon && \
java -cp "provided.jar:out/" TestSuite Test-Poison && \
java -cp "provided.jar:out/" TestSuite Test-Poison-2 && \
java -cp "provided.jar:out/" TestSuite Test-Insufficient-MP && \
java -cp "provided.jar:out/" TestSuite Test-OnePunch && \
java -cp "provided.jar:out/" TestSuite only-basic-attack && \
java -cp "provided.jar:out/" TestSuite waterball-to-fireball-1v2 && \
java -cp "provided.jar:out/" TestSuite waterball-to-self-healing-1v2 && \
java -cp "provided.jar:out/" TestSuite waterball-to-petrochemical-2v2 && \
java -cp "provided.jar:out/" TestSuite waterball-to-summon-2v2 && \
java -cp "provided.jar:out/" TestSuite waterball-to-self-explosion-2v2 && \
java -cp "provided.jar:out/" TestSuite waterball-to-cheerup-2v2 && \
java -cp "provided.jar:out/" TestSuite waterball-to-Curse-2v2 && \
java -cp "provided.jar:out/" TestSuite waterball-to-cheerup-2v2 && \
java -cp "provided.jar:out/" TestSuite waterball-to-one-punch-2v2 && \
java -cp "provided.jar:out/" TestSuite waterball-to-one-punch-3v3 && \
java -cp "provided.jar:out/" TestSuite waterball-to-one-punch-3v10 && \
java -cp "provided.jar:out/" TestSuite waterball-to-one-punch-10v10