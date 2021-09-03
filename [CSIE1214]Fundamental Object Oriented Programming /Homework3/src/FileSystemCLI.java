import static utils.Inputs.in;

public class FileSystemCLI {
    private Directory root;  // TODO
    private Directory currentDir;
    public FileSystemCLI(){
        root = new Directory("/", "/", null);
        root.setParentDir(root);
        currentDir = root;
    }
    public void start() {
        while (true) {
            printCurrentPath();
            String command = in.nextLine();
            if (command.equals("exit")) {
                break;
            }
            executeCommand(command);
        }
    }

    private void printCurrentPath() {
        System.out.println("Current path: " + currentDir.getPath());
    }

    public void executeCommand(String command) {
        String[] segments = command.split("\\s");

        try {
            String commandName = segments[0];
            performCommand(commandName, segments);
        } catch (Exception err) {
            System.err.println("(Debug) Error: " + err.getMessage());
            System.out.println("Illegal command.");
        }
    }

    private void performCommand(String commandName, String[] segments) {
        switch (commandName) {
            case "cd":
                changeDirectory(segments[1]);
                break;
            case "mkdir":
                makeDirectory(segments[1]);
                break;
            case "touch":
                touch(segments[1], segments[2]);
                break;
            case "rm":
                remove(segments[1]);
                break;
            case "cat":
                concatenate(segments[1]);
                break;
            case "ls":
                list();
                break;
            case "search":
                search(segments[1]);
                break;
            case "ln":
                link(segments[1], segments[2]);
                break;
        }
    }

    private void changeDirectory(String directoryName) {
        // TODO
        Element test = currentDir.getElement(directoryName);
        Directory cur;
        if(test.toCheck(currentDir) == currentDir && test != currentDir){
            System.out.println("Illegal command.");
            return;
        }
        cur = (Directory) test.toCheck(currentDir);
        currentDir = cur;
    }

    private void makeDirectory(String directoryName) {
        // TODO
        Directory toCreate = new Directory();
        currentDir.addNewElement(directoryName, "", toCreate);
    }

    private void touch(String fileName, String content) {
        // TODO
        File toCreate = new File();
        currentDir.addNewElement(fileName, content, toCreate);
    }

    private void remove(String childName) {
        // TODO
        currentDir.removeElement(childName);
    }

    private void concatenate(String fileName) {
        // TODO
        Element test = currentDir.getElement(fileName);
        File cur;
        if(test.toCheck(null) != null && test != currentDir){
            System.out.println("Illegal command.");
            return;
        }
        else if(test == currentDir) return;
        cur = (File)test;
        System.out.println(cur.getContent());
    }

    private void list() {
        // TODO
        currentDir.listAllElement();
    }

    private void link(String targetName, String linkName) {
        // TODO
        Link toCreate = new Link();
        currentDir.addNewElement(linkName, targetName, toCreate);
        Link l = (Link) currentDir.getElement(linkName);
        if(l.toCheck(currentDir) == currentDir) currentDir.removeElement(linkName);
    }

    private void search(String keyword) {
        // TODO
        currentDir.search(keyword);
    }

}
