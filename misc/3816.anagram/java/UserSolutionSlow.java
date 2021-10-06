public class UserSolution {
    public int FindAnagram(int l1, String s1, int l2, String s2) {
        int [] aArr = makeAlphabet(s1.toCharArray(), 0, l1);

        int ans = 0;

        for (int i = 0; true; i++) {
            if (l2 < i + l1)
                break;

            int[] bArr = makeAlphabet(s2.toCharArray(), i, i + l1);
            if (isAnagram(aArr, bArr))
                ans++;
        }

        return ans;
    }
     
    private static int[] makeAlphabet (char[] arr, int start, int end){
        int[] alphabet = new int[26];
        for (int i = start; i < end; i++) {
            alphabet[arr[i] - 'a']++; 
        }
        return alphabet;
    }
     
    private static boolean isAnagram(int[] aArr, int[] bArr) {
        for(int i=0; i<aArr.length; i++) {
            if(aArr[i] != bArr[i]) return false;
        }
        return true;
    }
}
