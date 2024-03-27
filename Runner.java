package ie.atu.sw;

import java.util.*;
import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;

public class Runner {
	
	//Default file output location
	private static String outputPath = "./out.txt";
	
	/**
	 * Calculates the cosine similarity between two vectors.
	 * Time Complexity: O(n), where n is the number of dimensions in the vectors.
	 *
	 * @param vectorA The first vector.
	 * @param vectorB The second vector.
	 * @return The cosine similarity between vectorA and vectorB.
	 */
    private static double cosineSimilarity(double[] vectorA, double[] vectorB) {
        double dotProduct = 0.0;
        double normA = 0.0;
        double normB = 0.0;
        for (int i = 0; i < vectorA.length; i++) {
            dotProduct += vectorA[i] * vectorB[i];
            normA += Math.pow(vectorA[i], 2);
            normB += Math.pow(vectorB[i], 2);
        }
        return dotProduct / (Math.sqrt(normA) * Math.sqrt(normB));
    }

    /**
     * Finds the top N most similar words to the given word using cosine similarity.
     * Time Complexity: O(m * d + n * log(n)), where m is the number of words in the embeddings,
     * d is the number of dimensions of the embeddings, and n is the number of top similar words.
     *
     * @param word The word to find similarities for.
     * @param embeddings The map of words to their embedding vectors.
     * @param topN The number of similar words to find.
     * @return A list of the most similar words, along with their similarity scores.
     */
    private static List<Map.Entry<String, Double>> findMostSimilarWords(String word, Map<String, double[]> embeddings, int topN) {
        double[] wordVector = embeddings.get(word.toLowerCase());
        if (wordVector == null) {
            System.out.println("Word not found in embeddings.");
            return Collections.emptyList();
        }

        // Calculate cosine similarity with all other vectors
        PriorityQueue<Map.Entry<String, Double>> similarityQueue = new PriorityQueue<>(Map.Entry.comparingByValue());
        for (Map.Entry<String, double[]> entry : embeddings.entrySet()) {
            double similarity = cosineSimilarity(wordVector, entry.getValue());
            similarityQueue.add(new AbstractMap.SimpleEntry<>(entry.getKey(), similarity));
            if (similarityQueue.size() > topN) {
                similarityQueue.poll();
            }
        }

        // Get the top N similar words
        List<Map.Entry<String, Double>> mostSimilarWords = new ArrayList<>();
        while (!similarityQueue.isEmpty()) {
            mostSimilarWords.add(similarityQueue.poll());
        }
        Collections.reverse(mostSimilarWords);
        return mostSimilarWords;
    }

    /**
     * Finds the least similar words to the given word using cosine similarity.
     * This is the contrasting words finder.
     * Time Complexity: O(m * d + n * log(n)), similar to findMostSimilarWords.
     *
     * @param word The word to find contrasting words for.
     * @param embeddings The map of words to their embedding vectors.
     * @param bottomN The number of least similar words to find.
     * @return A list of the least similar words, along with their (inverted) similarity scores.
     */
    private static List<Map.Entry<String, Double>> findLeastSimilarWords(String word, Map<String, double[]> embeddings, int bottomN) {
        double[] wordVector = embeddings.get(word.toLowerCase());
        if (wordVector == null) {
            System.out.println("Word not found in embeddings.");
            return Collections.emptyList();
        }

        // Calculate cosine similarity with all other vectors and use a priority queue to find the words with the lowest similarity
        PriorityQueue<Map.Entry<String, Double>> similarityQueue = new PriorityQueue<>(bottomN, Map.Entry.comparingByValue());

        for (Map.Entry<String, double[]> entry : embeddings.entrySet()) {
            double similarity = cosineSimilarity(wordVector, entry.getValue());
            // We want the lowest values, so we invert the comparison here by using 1-similarity
            similarityQueue.add(new AbstractMap.SimpleEntry<>(entry.getKey(), 1 - similarity));
            // Keep the queue to the specified size
            if (similarityQueue.size() > bottomN) {
                similarityQueue.poll();
            }
        }

        // Extract the least similar words
        List<Map.Entry<String, Double>> leastSimilarWords = new ArrayList<>();
        while (!similarityQueue.isEmpty()) {
            leastSimilarWords.add(similarityQueue.poll());
        }
        Collections.reverse(leastSimilarWords); // Reverse to get the least similar words at the top of the list
        return leastSimilarWords;
    }

    
    /**
     * Specifies the path to the output file where results will be saved.
     * Time Complexity: O(1) as it's only assigning a value based on user input.
     *
     * @param menu The Menu object for user interaction.
     */
    private static void writeSimilarWordsToFile(List<Map.Entry<String, Double>> similarWords) {
        List<String> lines = new ArrayList<>();
        for (Map.Entry<String, Double> entry : similarWords) {
            lines.add(entry.getKey() + " " + entry.getValue());
        }
        try {
            Files.write(Paths.get(outputPath), lines);
        } catch (IOException e) {
            System.err.println("Error writing to output file: " + e.getMessage());
        }
    }
    
    /**
     * Handles the user interaction for finding the most contrasting words.
     * It prompts the user for a word and the number of contrasting words to find, then displays the results.
     * Time Complexity: O(m * d + n * log(n)), where m is the number of words in the embeddings,
     * d is the number of dimensions of the embeddings, and n is the number of contrasting words requested.
     * This includes the time complexity of finding the contrasting words.
     *
     * @param menu The Menu object for user interaction.
     * @param embeddings The map of words to their embedding vectors.
     */
    private static void handleContrastingWordSearch(Menu menu, Map<String, double[]> embeddings) {
        System.out.print("Enter a word to find the most contrasting words: ");
        String word = menu.getScanner().nextLine();
        System.out.print("Enter the number of contrasting words to find: ");
        int bottomN = Integer.parseInt(menu.getScanner().nextLine());

        List<Map.Entry<String, Double>> leastSimilarWords = findLeastSimilarWords(word, embeddings, bottomN);
        // Output the results
        leastSimilarWords.forEach(entry -> System.out.println(entry.getKey() + " - " + entry.getValue()));
        // Optionally write to a file or other output
    }

    /**
     * Specifies the path to the output file where results will be saved.
     * Time Complexity: O(1) as it's only assigning a value based on user input.
     *
     * @param menu The Menu object for user interaction.
     */
    private static void specifyOutputFile(Menu menu) {
        System.out.print("Enter the new path for the output file (default is ./out.txt): ");
        String path = menu.getScanner().nextLine();
        if (!path.trim().isEmpty()) {
            outputPath = path.trim();
            System.out.println("Output path set to: " + outputPath);
        } else {
            System.out.println("Output path remains: " + outputPath);
        }
    }
    
    /**
     * Handles the user interaction for finding the most similar words.
     * It prompts the user for a word and the number of similar words to find, then displays and writes the results.
     * Time Complexity: O(m * d + n * log(n)), where m is the number of words in the embeddings,
     * d is the number of dimensions of the embeddings, and n is the number of similar words requested.
     * This includes the time complexity of finding the similar words and writing them to a file.
     *
     * @param menu The Menu object for user interaction.
     * @param embeddings The map of words to their embedding vectors.
     */
    private static void handleWordSearch(Menu menu, Map<String, double[]> embeddings) {
    	System.out.print("Enter a word to find similar words: ");
        String word = menu.getScanner().nextLine();
        
        System.out.print("Enter the number of similar words to find (default " + menu.getDefaultNumberOfResults() + "): ");
        String input = menu.getScanner().nextLine();
        
        int topN;
        if (input.isEmpty()) {
            topN = menu.getDefaultNumberOfResults();
        } else {
            topN = Integer.parseInt(input.trim());
        }


        List<Map.Entry<String, Double>> similarWords = findMostSimilarWords(word, embeddings, topN);
        writeSimilarWordsToFile(similarWords); // Write the results to the output file
        similarWords.forEach(entry -> System.out.println(entry.getKey() + " - " + entry.getValue())); // Also print to console
    }
    
    /**
     * Prompts the user to specify the path to the embeddings file and parses it.
     * Time Complexity: O(L) for reading the file, where L is the number of lines in the file.
     * Parsing complexity is O(m * d), where m is the number of lines and d is the number of dimensions.
     *
     * @param menu The Menu object for user interaction.
     * @return A map of words to their embedding vectors.
     */
    private static Map<String, double[]> specifyEmbeddingFile(Menu menu) {
        System.out.print("Enter the path to the embedding file: ");
        String filePath = menu.getScanner().nextLine();
        File file = new File(filePath);

        if (!file.exists() || !file.isFile()) {
            System.out.println("Error: Invalid file path.");
            return null;
        } else {
            long totalLines = 0;
            try {
                // Count the total number of lines first
                totalLines = Files.lines(Paths.get(filePath)).count();
            } catch (IOException e) {
                System.err.println("Error counting lines in file: " + e.getMessage());
                return null;
            }

            // Now call the parse method with the total lines count
            Map<String, double[]> embeddings = parseEmbeddingsFile(filePath, menu, totalLines);
            return embeddings;
        }
    }

    /**
     * Parses the embeddings file and stores each word and its vector in a map.
     * Time Complexity: O(m * d), where m is the number of lines and d is the number of dimensions.
     *
     * @param filePath The file path to the embeddings file.
     * @param menu The Menu object for user interaction.
     * @param totalLines The total number of lines in the file, used for the progress bar.
     * @return A map of words to their embedding vectors.
     */
    private static Map<String, double[]> parseEmbeddingsFile(String filePath, Menu menu, long totalLines) {
        Map<String, double[]> embeddings = new HashMap<>();
        long currentLine = 0;

        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] parts = line.split(",\\s*"); // Split on comma followed by zero or more spaces
                if (parts.length < 2) {
                    System.out.println("Skipping line: " + line); // For debugging
                    continue; // Skip if line doesn't contain at least two parts
                }

                String word = parts[0];
                double[] vector = new double[parts.length - 1];
                for (int i = 1; i < parts.length; i++) {
                    vector[i - 1] = Double.parseDouble(parts[i]);
                }
                embeddings.put(word, vector); // Use the word as it is

                currentLine++;
                // Update progress and ensure it doesn't exceed 100%
                int progress = (int) (currentLine * 100 / totalLines);
                menu.printProgress(Math.min(progress, 100), 100);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println("Loaded " + embeddings.size() + " embeddings.");
        return embeddings;
    }

    //Main
    public static void main(String[] args) {
        Menu menu = new Menu();
        menu.displayMenuHeader();
        boolean quit = false;
        Map<String, double[]> embeddings = null;

        while (!quit) {
            menu.displayMenuOptions();
            int choice = menu.getUserChoice();
            switch (choice) {
                case 1:
                    embeddings = specifyEmbeddingFile(menu);
                    break;
                case 2:
                    // Handle specifying the output file
                	specifyOutputFile(menu);
                    break;
                case 3:
                    if (embeddings != null) {
                        handleWordSearch(menu, embeddings);
                    } else {
                        System.out.println("Please load the embeddings file first.");
                    }
                    break;
                case 4:
                    menu.configureDefaultNumberOfResults();
                    break;
                case 5:
                    if (embeddings != null) {
                        handleContrastingWordSearch(menu, embeddings);
                    } else {
                        System.out.println("Please load the embeddings file first.");
                    }
                    break;
                case 6:
                    quit = true;
                    break;
                default:
                    System.out.println("Invalid option. Please try again.");
                    break;
            }
        }

        menu.closeScanner();
        System.out.println(ConsoleColour.RESET);
    }

}
