import java.util.Random;

public class Main {
    public static void main(String[] args) {
        SkipListSet<Integer> skipList = new SkipListSet<>();
        Random rand = new Random(42); // fixed seed for reproducibility

        int numElements = 1_000_000;
        int searchSamples = 10_000;

        System.out.println("Stress test with " + numElements + " elements...");
        
        // Insert
        long start = System.currentTimeMillis();
        for (int i = 0; i < numElements; i++) {
            skipList.add(rand.nextInt(Integer.MAX_VALUE));
        }
        long end = System.currentTimeMillis();
        System.out.println("Insertion time: " + (end - start) + " ms");
        System.out.println("SkipList size: " + skipList.size());

        // Search
        start = System.currentTimeMillis();
        int found = 0;
        for (int i = 0; i < searchSamples; i++) {
            if (skipList.contains(rand.nextInt(Integer.MAX_VALUE))) {
                found++;
            }
        }
        end = System.currentTimeMillis();
        System.out.println("Search time (" + searchSamples + " samples): " + (end - start) + " ms");
        System.out.println("Found: " + found);

        // Delete
        start = System.currentTimeMillis();
        int removed = 0;
        for (int i = 0; i < searchSamples; i++) {
            if (skipList.remove(rand.nextInt(Integer.MAX_VALUE))) {
                removed++;
            }
        }
        end = System.currentTimeMillis();
        System.out.println("Removal time (" + searchSamples + " samples): " + (end - start) + " ms");
        System.out.println("Successfully removed: " + removed);
    }
}
