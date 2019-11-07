/*
 * DO NOT MODIFY THIS FILE! IT WILL BE AUTOMATICALLY OVERWRITTEN BY THE CHECKER!
 */
/**
 * Class for a message exchanged between space explorers and headquarters.
 */
class Message {
    private int parentSolarSystem;
    private int currentSolarSystem;
    private String data;

    /*
     * DO NOT MODIFY THIS FILE! IT WILL BE AUTOMATICALLY OVERWRITTEN BY THE CHECKER!
     */
    /**
     * Creates a {@code Message} object to be sent from an HQ to a space explorer.
     *
     * @param currentSolarSystem
     *            the current solar system
     * @param data
     *            data contained in the message (i.e., undecoded frequency)
     */
    Message(int currentSolarSystem, String data) {
        this.currentSolarSystem = currentSolarSystem;
        this.data = data;
    }

    /*
     * DO NOT MODIFY THIS FILE! IT WILL BE AUTOMATICALLY OVERWRITTEN BY THE CHECKER!
     */
    /**
     * Gets the parent solar system from the message.
     *
     * @return the parent solar system ID
     */
    int getParentSolarSystem() {
        return parentSolarSystem;
    }

    /*
     * DO NOT MODIFY THIS FILE! IT WILL BE AUTOMATICALLY OVERWRITTEN BY THE CHECKER!
     */
    /**
     * Sets the parent solar system in the message.
     *
     * @param parentSolarSystem
     *            the parent solar system ID to be set
     */
    void setParentSolarSystem(int parentSolarSystem) {
        this.parentSolarSystem = parentSolarSystem;
    }

    /*
     * DO NOT MODIFY THIS FILE! IT WILL BE AUTOMATICALLY OVERWRITTEN BY THE CHECKER!
     */

    /**
     * Gets the current solar system from the message.
     *
     * @return the current solar system ID
     */
    int getCurrentSolarSystem() {
        return currentSolarSystem;
    }

    /*
     * DO NOT MODIFY THIS FILE! IT WILL BE AUTOMATICALLY OVERWRITTEN BY THE CHECKER!
     */
    /**
     * Gets the data in the message.
     *
     * @return the data in the message
     */
    String getData() {
        return data;
    }

    /*
     * DO NOT MODIFY THIS FILE! IT WILL BE AUTOMATICALLY OVERWRITTEN BY THE CHECKER!
     */
    /**
     * Sets the data in the message.
     *
     * @param data
     *            the data to be set in the message
     */
    void setData(String data) {
        this.data = data;
    }
}

/*
 * DO NOT MODIFY THIS FILE! IT WILL BE AUTOMATICALLY OVERWRITTEN BY THE CHECKER!
 */
