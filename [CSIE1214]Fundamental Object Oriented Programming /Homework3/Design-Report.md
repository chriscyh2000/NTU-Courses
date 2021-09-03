# Design-Report.md
## NTU CSIE FOOP Spring 2021
### Software Design :
#### Project Structure:
- **Element :** The super class of all kinds of elements that will exist in the file system. It constructs the common behaviors of all the elements.
- **Dirctory :** To store all the elements within a directory, accept the command from the CLI.
- **File :** Strore the information of a file.
- **Link :** To link a directory to another directory.
- **FileSystemCLI :** To start the interaction with users, accept the command from user and do the corresponding jobs.
#### Explanation of my method design :
- **`toCheck(Element curDir) :`** I create this method in each element in order to check whether an element is a directory or a file. When an element call `toCheck()`, if it is a directory then return itself, otherwise return the input element. Because when the CLI require a directory to return an element within current directory by `getElement()`, it will return the current directory if the current directory can't find the element CLI required. So in the class `FileSystemCLI`, I have to feed a `null` to `test.toCheck` and let `test` return `null` if it is a file in order to avoid misunderstnding whether the returned element is a file or the current directory. This kind of design gives me lots of convenience when I was required to add `Link` into the file system. `Link` will return `null` if the input is `null` when calling method `toCheck()`, so it can be recognize as a file when the command is `cat`. Otherwise it will return the directory it linked to, so it can be recognize as a directory when the command is `cd`.