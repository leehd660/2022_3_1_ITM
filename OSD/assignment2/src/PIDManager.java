public interface PIDManager {
    //the range of allowable PIDs (inclusive)
    public static final int MIN_PID = 4;
    public static final int MAX_PID = 128;

//    Return a claid PID or -1 if none are available
    public int getPID();

//    Return a valid PID, possibly blocking the calling process until one is available.
    public int getPIDWait() throws InterruptedException;

//    Release the pid
//    Throw an IllegalArgumentException if the pid is outside of the rande of PID values.
    public void releasePID(int pid);
}
