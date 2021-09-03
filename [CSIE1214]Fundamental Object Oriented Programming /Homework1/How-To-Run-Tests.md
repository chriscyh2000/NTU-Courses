# How To Run Tests

There are two ways to run the tests: 

1. Run the test via shell script
Run `bash test.sh`
   
2. Run each test manually
1. `cd src`
2. `javac TestSuite.java -d ../out/`
3. `cd ..`
4. `java -cp out/ TestSuite <testcase's name>`
> Replace `<testcase's name>` with the testcase's name you can find under the testcases/ directory. 
> For example `java -cp out/ TestSuite always-play-first-card`