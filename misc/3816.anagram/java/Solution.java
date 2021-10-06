import java.util.Scanner;

public class Solution {
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        Scanner sc = new Scanner(System.in);

        for (int TestCase = sc.nextInt(), tc = 1; tc <= TestCase; tc = tc+1) {
            String S1 = sc.next();
            String S2 = sc.next();

            int L1 = S1.length();
            int L2 = S2.length();

            int answer = new UserSolution().FindAnagram(L1, S1, L2, S2);

            System.out.println("#"+ tc + " " + answer);
        }
    }

}

