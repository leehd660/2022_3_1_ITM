import java.io.BufferedReader;
import java.io.File;
import java.io.InputStreamReader;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class SimpleShell {
    private static String nowPath;
    private static List<String> historyList;
    private static String absPath;

    public static void main(String[] args) throws java.io.IOException {
        //System.getProperty("user.dir") : 현재 디렉토리의 경로를 String으로 반환
        nowPath = System.getProperty("user.dir");
        absPath = System.getProperty("user.home");
        historyList = new ArrayList<>();

        BufferedReader console = new BufferedReader(new InputStreamReader(System.in));

        //we bread out with <control><C>
        while (true) {
            //read what the user entered
            System.out.print("jsh>");
            String commandLine = console.readLine();

            instruction(commandLine);
        }
    }

    static void instruction(String commandLine) {
        // #4,#5 split string value of command Line, and store each value in ArrayList
        List<String> cmdList = new ArrayList<>();
        for (String s : commandLine.split(" ")) {
            cmdList.add(s);
        }

        // if the user entered a return, just loop again
        if (commandLine.equals("")) {
            ;
        }
        //exit case
        else if (commandLine.equals("exit") || commandLine.equals("quit")) {
            System.out.println("Goodbye.");
            System.exit(0);
        }
        // 'history' function -> historyList에 추가됨
        else if (cmdList.get(0).equals("history")) {
            if (cmdList.size()==1) {
                historyList.add(commandLine);
                for (int i = 1; i <= historyList.size(); i++) {
                    System.out.println(i + "\t" + historyList.get(i - 1));
                }
            }
            else {
                //숫자 예외처리
                try {
                    int index = Integer.parseInt(cmdList.get(1));
                    historyList.add(commandLine);
                    if (index <= 0) {
                        System.out.println("error : your input value of Integer is smaller than history size.");
                    }
                    else if (index > historyList.size()+1) {
                        for (int i = 1; i <= historyList.size(); i++) {
                            System.out.println(i + "\t" + historyList.get(i - 1));
                        }
                    }
                    else {
                        for (int i=index-1; i>=0; i--) {
                            System.out.println(historyList.size()-i + "\t" + historyList.get(historyList.size()-1-i));
                        }
                    }
                }catch (NumberFormatException e) {
                    //첫글자가 !로 시작하는 경우 (history 이어서)
                    if (cmdList.get(1).charAt(0) == 33) {
                        // '!!' function -> historyList에 추가가 안됨 / 바로 전 명령어 실행
                        if (cmdList.get(1).charAt(1) == 33) {
                            if (historyList.isEmpty()) { // historyList가 비어있는 경우
                                System.out.println("There was no previous command.");
                            }
                            else {
                                commandLine = historyList.get(historyList.size() - 1);
                                System.out.println(commandLine);
                                instruction(commandLine);
                            }
                        }
                        // '!{int value}' -> historyList에 (int value)번째 명령어 추가됨.
                        else {
                            try {
                                int index = Integer.parseInt(cmdList.get(1).substring(1));
                                if (index > historyList.size()+1 || index <= 0) {
                                    System.out.println("error : your input value of Integer is bigger/smaller than history size.");
                                }
                                else {
                                    System.out.println(historyList.get(index-1));
                                    instruction(historyList.get(index-1));
                                }
                            }catch (NumberFormatException ne) {
                                System.out.println("error : invalid input value.");
                            }
                        }
                    }
                    else {
                        System.out.println("error : invalid input value.");
                    }
                }
            }

        }

        // function of 'cd'
        else if (cmdList.get(0).equals("cd")) {
            // cd로 시작하는 명령어는 전부 들어감
            historyList.add(commandLine);
            if (cmdList.size() == 1) {
                nowPath = absPath;
            }
            else {
                if (cmdList.get(1).equals("..")) {
                    if (!nowPath.equals("/")) {
                        Path parentPath = Paths.get(nowPath).getParent();
                        nowPath = parentPath.toString();
                    }
                }
                else if (cmdList.get(1).equals("/home")) {
                    nowPath = Paths.get(absPath).getParent().toString();
                }
                else if (cmdList.get(1).equals("leehd60")) {
                    nowPath = absPath;
                }
                else {
                    //특정 폴더 위치로 가기
                    String certainPath = cmdList.get(1);
                    File fullFile = new File(certainPath);
                    if (fullFile.exists() && fullFile.isDirectory()) { //full 파일을 입력한 것이면,
                        nowPath = fullFile.toString();
                    }
                    else if (fullFile.exists() && !fullFile.isDirectory()) {
                        System.out.println("Not a directory");
                    }
                    else {
                        //하위 폴더명 살펴보기
                        File underFile = new File(nowPath + "/"+ certainPath);
                        if (underFile.exists() && underFile.isDirectory()) {
                            nowPath = underFile.toString();
                        }
                        else if (underFile.exists() && !underFile.isDirectory()) {
                            System.out.println("Not a directory");
                        }
                        else {
                            System.out.println("invalid value in your path.");
                        }
                    }
                }
            }
        }

        else {
            // #6, create ProcessBuilder object
            ProcessBuilder pb = new ProcessBuilder(cmdList);

            try {
                pb.directory(new File(nowPath));

                // Process start
                Process process = pb.start();

                //obtain the input stream
                BufferedReader bf = new BufferedReader(new InputStreamReader(process.getInputStream()));

                //read the output of the process
                String line;
                while ((line = bf.readLine()) != null) {
                    System.out.println(line);
                }

            } catch (Exception e) {
                System.out.println("invalid value in your input.");
                return;
            }

            //예외처리된거 제외하고 실행된 코드들만 historyList에 저장함.
            historyList.add(commandLine);
        }
    }
}