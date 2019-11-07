import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Semaphore;

/**
 * Class that implements the channel used by headquarters and space explorers to communicate.
 */
public class CommunicationChannel {
    private List<Message> explorerToHQ;
    private List<Message> HQToExplorer;

    private ConcurrentHashMap<Long, Integer> parents;

    private Semaphore fullExplorerToHQ;
    private Semaphore fullHQToExplorer;
    /**
     * Creates a {@code CommunicationChannel} object.
     */
    public CommunicationChannel() {
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
    public void putMessageSpaceExplorerChannel(Message message) {
        explorerToHQ.add(message);
        fullExplorerToHQ.release();
    }

    /**
     * Gets a message from the space explorer channel (i.e., where space explorers write to and
     * headquarters read from).
     *
     * @return message from the space explorer channel
     */
    public Message getMessageSpaceExplorerChannel() {
        try {
            fullExplorerToHQ.acquire();
        } catch(InterruptedException e) {
            e.printStackTrace();
        }

        return explorerToHQ.remove(0);
    }

    /**
     * Puts a message on the headquarters channel (i.e., where headquarters write to and
     * space explorers read from).
     *
     * @param message
     *            message to be put on the channel
     */
    public void putMessageHeadQuarterChannel(Message message) {
//        System.out.println("[CHANNEL]: to " + message.getCurrentSolarSystem());

        if (message.getData().equals("EXIT")) {
            HQToExplorer.add(message);
            fullHQToExplorer.release();
            return;
        }

        long tid = Thread.currentThread().getId();

        if (message.getData().equals("END")) {
//            System.out.println("received END");
            parents.remove(tid);
            return;
        }

        Integer lastParent = parents.get(tid);
//        System.out.println("lastParent = " + lastParent);

        if (lastParent == null) {
//            System.out.println("putting lastParent in map");
            parents.put(tid, message.getCurrentSolarSystem());
        } else if (message.getCurrentSolarSystem() != lastParent){
//            System.out.println("sending to explorer");
            HQToExplorer.add(new Message(
                lastParent,
                message.getCurrentSolarSystem(),
                message.getData())
            );
            fullHQToExplorer.release();
        }

//        System.out.println();
    }

    /**
     * Gets a message from the headquarters channel (i.e., where headquarters write to and
     * space explorer read from).
     *
     * @return message from the header quarter channel
     */
    public Message getMessageHeadQuarterChannel() {
        try {
            fullHQToExplorer.acquire();
        } catch(InterruptedException e) {
            e.printStackTrace();
        }

        return HQToExplorer.remove(0);
    }
}
