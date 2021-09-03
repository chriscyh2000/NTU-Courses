# How To Run Tests

There are two ways to run the tests: 

1. Run the test via shell script
Run `bash test.sh` or `sh test.sh`
   
2. Run each test manually
1. `javac -cp . -sourcepath src -d out/ src/*.java`
2. `java -cp ".:out/" TestSuite <testcase's name>`
> Replace `<testcase's name>` with the testcase's name you can find under the testcases/ directory. 
> For example `java -cp ".:out/" TestSuite cd-ls-mkdir`


# Understand the testcases

We provided a good tool to help you replay the testcase in the CLI mode.
So that you can understand the testcase more easily.

Run `bash replay.sh <testcase's name>` or `sh replay.sh <testcase's name>` 