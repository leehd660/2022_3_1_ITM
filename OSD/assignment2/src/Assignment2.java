import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.*;

import static java.lang.Thread.sleep;

public class Assignment2 {
    private static long startTime;
    private static int threadNum;
    private static int totalTime;
    private static int lifeTime;
    private static Deque<Integer> usedList;
    private static Deque<Integer> unUsedList;
    static int count;
    static Random random;
    static Manager pidManager;

    public static void main(String[] args) throws IOException {
        BufferedReader bf = new BufferedReader(new InputStreamReader(System.in));
        Scanner scanner = new Scanner(System.in);
        random = new Random();

        // 예외 처리해주기
        while (true) {
            System.out.print("input the number of thread > ");
            String strThreadNum = bf.readLine();
            try {
                threadNum = Integer.parseInt(strThreadNum);
                if (threadNum >= 1) break;
            } catch (NumberFormatException e) { }
            System.out.println("Invalid input value. please retype.");
        }
        while (true) {
            System.out.print("input total program time > ");
            String strTotalTime = bf.readLine();
            try {
                totalTime = Integer.parseInt(strTotalTime);
                if (totalTime >= 1) break;
            } catch (NumberFormatException e) { }
            System.out.println("Invalid input value. please retype.");
        }
        while (true) {
            System.out.print("input lifetime of thread (millisecond)> ");
            String strLifeTime = bf.readLine();
            try {
                lifeTime = Integer.parseInt(strLifeTime);
                if (lifeTime >= 0) break;
            } catch (NumberFormatException e) { }
            System.out.println("Invalid input value. please retype.");
        }

        // PID가 4~128번까지 사용가능한 true상태로 만들어 놓는다.
        unUsedList = new ArrayDeque<>();
        usedList = new ArrayDeque<>();
        for(int i=PIDManager.MIN_PID; i<=PIDManager.MAX_PID; i++) {
            unUsedList.addLast(i);
        }

        pidManager = new Manager();
        // 예외 처리
        int managerMethod = 0;
        System.out.print("What kind of method will you use to find the PID? \n" + "1 : getPID(), 2 : getPIDWait()\n");
        while (true) {
            System.out.print("Enter either 1 or 2 as int value. > ");
            String strMethod = bf.readLine();
            try {
                managerMethod = Integer.parseInt(strMethod);
                if (managerMethod == 1 || managerMethod == 2) break;
            } catch (NumberFormatException e) { }
            System.out.println("Invalid input value. please retype.");
        }

        System.out.println("Test program is initialized with "+ threadNum +" thread and " + totalTime + " seconds, with the life time " + lifeTime + " seconds of each thread");

        startTime = System.currentTimeMillis()/1000;
        count = 0;

        Timer timer = new Timer();
        int finalManagerMethod = managerMethod;
        TimerTask task=new TimerTask(){
            @Override
            public synchronized void run() {
                if(System.currentTimeMillis()/1000 - startTime <= totalTime){
                    if (count < threadNum) {
                        int pid=-1;
                        if (finalManagerMethod == 1) {
                            pid = pidManager.getPID();
                        }
                        else {
                            try {
                                pid = pidManager.getPIDWait();
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }
                        if (pid != -1) {
                            Thread makeThread = new Thread(new MyRunnable(pid));
                            makeThread.start();
                            count++;
                        }
                        else {
//                            System.out.println("@@@@@@@@@@@@@ -1 occurred @@@@@@@@@@@@@@");
                        }
                    }
                }
                else{
                    timer.cancel();
                    System.out.println(totalTime + " seconds has passed... Program ends");
                    System.exit(0);
                }
            }
        };
        timer.schedule(task, 0, 100); // 일단은 10초 내로 랜덤하게 나온다고 가정

    }

    static class Manager implements PIDManager {

        @Override
        public synchronized int getPID() {
            int pid;
            if (!unUsedList.isEmpty()) {
                pid = unUsedList.peekFirst();
                usedList.addLast(unUsedList.pollFirst());
            }
            else {
                pid = -1;
            }
            return pid;
        }

        @Override
        public synchronized int getPIDWait() throws InterruptedException {
            int pid = 0;
            while (unUsedList.isEmpty()) {
                try {
                    wait();
                }catch (InterruptedException ie) {}
            }
            pid = unUsedList.peekFirst();
            usedList.addLast(unUsedList.pollFirst());
            return pid;
        }

        @Override
        public synchronized void releasePID(int pid) {
            unUsedList.addLast(usedList.pollFirst());
            notify();
        }
    }


    static class MyRunnable implements Runnable {
        int num;

        public MyRunnable() {
            this.num = 0;
        }

        public MyRunnable(int num) {
            this.num = num;
        }

        @Override
        public synchronized void run() {

            long nowTime = System.currentTimeMillis()/1000; // 지금 시간 초로 만들기
            System.out.println("Thread " + this.num + " created at Second " + String.valueOf((nowTime-startTime)));
            try{
                sleep(lifeTime*1000);
            }catch (InterruptedException e){
                e.printStackTrace();
            }
            System.out.println("Thread " + this.num + " destroyed at Second " + String.valueOf((nowTime-startTime+lifeTime)));
            pidManager.releasePID(usedList.peekFirst());
        }
    }
}


