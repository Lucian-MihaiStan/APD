import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Set;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Class for a space explorer.
 */
public class SpaceExplorer extends Thread {
    private static ConcurrentHashMap<Integer, Integer> discovered = null;
    private CommunicationChannel channel;
    private final int hashCount;

    /**
     * Creates a {@code SpaceExplorer} object.
     *
     * @param hashCount
     *            number of times that a space explorer repeats the hash operation
     *            when decoding
     * @param discovered
     *            set containing the IDs of the discovered solar systems
     * @param channel
     *            communication channel between the space explorers and the
     *            headquarters
     */
    SpaceExplorer(Integer hashCount, Set<Integer> discovered, CommunicationChannel channel) {
        this.hashCount 	= hashCount;
        this.channel 	= channel;

        if (SpaceExplorer.discovered == null) {
            SpaceExplorer.discovered = new ConcurrentHashMap<>();

            for (Integer node : discovered) {
                SpaceExplorer.discovered.put(node, 0);
            }
        }
    }

    /**
     * Thanks to the way the communication channel is implemented, an explorer only has to take a
     * message, check if the child node is undiscovered, and if so, decode the given frequency and
     * send it back to HQ.
     */
    @Override
    public void run() {
        Message message;
        String crtData;
        Integer childIsExplored;
        int crtChild;

        while (true) {
            message     = channel.getMessageHeadQuarterChannel();
            crtData     = message.getData();
            crtChild    = message.getCurrentSolarSystem();

            // The galaxy has been fully explored
            if (crtData.equals("EXIT")) {
                break;
            }

            // If the child node is undiscovered, add it to the set (map) of discovered nodes
            childIsExplored = discovered.putIfAbsent(crtChild, 0);

            // If the child were undiscovered, calculate its frequency and send it to HQ
            if (childIsExplored == null) {
                message.setData(encryptMultipleTimes(crtData, hashCount));
                channel.putMessageSpaceExplorerChannel(message);
            }
        }
    }

    /**
     * Applies a hash function to a string for a given number of times (i.e.,
     * decodes a frequency).
     *
     * @param input
     *            string to he hashed multiple times
     * @param count
     *            number of times that the string is hashed
     * @return hashed string (i.e., decoded frequency)
     */
    private String encryptMultipleTimes(String input, Integer count) {
        String hashed = input;

        for (int i = 0; i < count; ++i) {
            hashed = encryptThisString(hashed);
        }

        return hashed;
    }

    /**
     * Applies a hash function to a string (to be used multiple times when decoding
     * a frequency).
     *
     * @param input
     *            string to be hashed
     * @return hashed string
     */
    private static String encryptThisString(String input) {
        try {
            MessageDigest md        = MessageDigest.getInstance("SHA-256");
            byte[] messageDigest    = md.digest(input.getBytes(StandardCharsets.UTF_8));

            // Convert to string
            StringBuilder hexString = new StringBuilder();

            for (byte crtByte : messageDigest) {
                String hex = Integer.toHexString(0xff & crtByte);

                if (hex.length() == 1) {
                    hexString.append('0');
                }

                hexString.append(hex);
            }
            return hexString.toString();

        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException(e);
        }
    }
}
