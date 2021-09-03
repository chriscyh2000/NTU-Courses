# How To Run Tests

There are two ways to run the tests: 

1. Run the test via shell script
Run `bash test.sh` or `sh test.sh`
   
2. Run each test manually
1. `javac -cp "provided.jar:." -sourcepath src -d out/ src/*.java`
4. `java -cp "provided.jar:out/" TestSuite <testcase's name>`
> Replace `<testcase's name>` with the testcase's name you can find under the testcases/ directory. 
> For example `java -cp "provided.jar:out/" TestSuite only-basic-attack`