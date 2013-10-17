import java.io.*;
import java.util.StringTokenizer;

public class Template implements Runnable {

    void solve() {

    }

    BufferedReader br;
    StringTokenizer st = null;

    PrintWriter out;

    @Override
    public void run() {
        try {
            br = new BufferedReader(new InputStreamReader(new FileInputStream("file.txt")));
            // or
            br = new BufferedReader(new InputStreamReader(System.in));

            out = new PrintWriter(System.out);

            solve();

            out.flush();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            System.exit(1);
        }
    }

    String nextString() throws IOException{
        while (st == null || !st.hasMoreTokens()) {
            String s = br.readLine();
            st = new StringTokenizer(s);
        }
        return st.nextToken();
    }

    int nextInt() throws IOException {
        return Integer.parseInt(nextString());
    }

    double nextDouble() throws IOException {
        return Double.parseDouble(nextString());
    }

    public static void main(String[] args) {
        new Thread(null, new Template(), "", 1<<26).start(); //64MB
    }

}
