import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Semaphore;

/**
 * Class that implements the channel used by headquarters and space explorers to communicate.
 */
class CommunicationChannel {
    // Buffer for the communication between the explorers and the HQs
    private List<Message> explorerToHQ;

    // Buffer for the communication between the HQs and the explorers
    private List<Message> HQToExplorer;

    // Contains all nodes that are still sending child nodes.
    private ConcurrentHashMap<Long, Integer> parents;

    // Used to signal when the 2 previous buffers are empty.
    private Semaphore fullExplorerToHQ;
    private Semaphore fullHQToExplorer;

    /**
     * Creates a {@code CommunicationChannel} object.
     */
    CommunicationChannel() {
        explorerToHQ 		= Collections.synchronizedList(new LinkedList<>());
        HQToExplorer 		= Collections.synchronizedList(new LinkedList<>());

        fullExplorerToHQ 	= new Semaphore(0);
        fullHQToExplorer 	= new Semaphore(0);

        parents             = new ConcurrentHashMap<>();
    }

    /**
     * Puts a message on the space explorer channel (i.e., where space explorers write to and
     * headquarters read from).
     *
     * @param message
     *            message to be put on the channel
     */
    void putMessageSpaceExplorerChannel(Message message) {
        explorerToHQ.add(message);
        fullExplorerToHQ.release();
    }

    /**
     * Gets a message from the space explorer channel (i.e., where space explorers write to and
     * headquarters read from).
     *
     * @return message from the space explorer channel
     */
    Message getMessageSpaceExplorerChannel() {
        try {
            // Signal that an element is taken
            fullExplorerToHQ.acquire();
        } catch (InterruptedException e) {
            // If an exception is thrown, then return an invalid message
            return null;
        }

        return explorerToHQ.remove(0);
    }

    /**
     * Puts a message on the headquarters channel (i.e., where headquarters write to and
     * space explorers read from).
     *
     * Distinguishes between nodes coming from different HQs by their threadIDs.
     * If no prent has been received for a given threadID, then the first message that thread sends
     * is the parent. Subsequently, all second messages (the parents) are ignored and the the
     * message added to the buffer contains both the parent and the child nodes, together with the
     * frequencies.
     *
     * @param message
     *            message to be put on the channel
     */
    void putMessageHeadQuarterChannel(Message message) {
        String crtData = message.getData();

        // "EXIT" is the signal for the explorers to finish, so it is simply forwarded to them
        if (crtData.equals("EXIT")) {
            HQToExplorer.add(message);
            fullHQToExplorer.release();
            return;
        }

        // The id of the thread using the method
        long tid = Thread.currentThread().getId();

        // The HQ has finished sending parents and children
        if (crtData.equals("END")) {
            // Thus, the current parent is no longer relevant
            parents.remove(tid);
            return;
        }

        int crtSolarSystem = message.getCurrentSolarSystem();

        // Use the previously received parent for this thread
        Integer lastParent = parents.get(tid);

        if (lastParent == null) {
            // If there is no such parent node, add it to the map
            parents.put(tid, crtSolarSystem);
        } else if (crtSolarSystem != lastParent) {
            message.setParentSolarSystem(lastParent);
            HQToExplorer.add(message);
            fullHQToExplorer.release();
        }
    }

    /**
     * Gets a message from the headquarters channel (i.e., where headquarters write to and
     * space explorer read from).
     *
     * @return message from the header quarter channel
     */
    Message getMessageHeadQuarterChannel() {
        try {
            // Signal that an element is taken
            fullHQToExplorer.acquire();
        } catch (InterruptedException e) {
            // If an exception is thrown, then return an invalid message
            return null;
        }

        return HQToExplorer.remove(0);
    }
}
