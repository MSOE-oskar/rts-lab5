package edu.msoe.swe4211.core.network;

public interface iNetworkControllerStatusObserver {
    public void updateConnectionStatus(boolean currentConnectedStatus);
}
