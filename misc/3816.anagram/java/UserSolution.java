public class UserSolution {     
    public int FindAnagram(int l1, String str1, int l2, String str2) {
        int ans = 0;
        int[] counts = new int[26];

        for (int i = 0; i < l1; i++) {
            counts[str1.charAt(i) - 'a']++;
        }

        int zero = 0;
        for (int i = 0; i < 26; i++) {
            if (counts[i] == 0) {
                zero++;
            }
        }

        for (int i = 0; i < l2; i++) {
            char a = str2.charAt(i);

            if (counts[a - 'a'] == 0) {
                zero--;
            }
            counts[a - 'a']--;
            if (counts[a - 'a'] == 0) {
                zero++;
            }


            if (i >= l1) {
                char b = str2.charAt(i - l1);
                if (counts[b - 'a'] == 0) {
                    zero--;
                }
                counts[b - 'a']++;
                if (counts[b - 'a'] == 0) {
                    zero++;
                }
            }


            if (zero == 26) {
                ans++;
            }
        }

        return ans;
    }
}
