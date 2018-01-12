# reverse

Reverse is a simple utility used for expediting reverse TCP/UDP tunnel creation. A reverse tunnel can be beneficial if access to a service is prevented by lack of exposure in a NAT setup.

Reverse runs as a client and a server.

The server must be accessible by the client, as this is not a bi-directional NAT bypass technique\*. Once the client connection establishes, a simple reverse tunnel is formed and a random port on the server is bound. Any connection to this port will be duplicated over the revrse tunnel and sent to the destination service. This has the side-effect of the client showing no direct inbound connections caused by the outbound tunnel.

*\* If you are interested in bi-directional communication with both clients behind a NAT, check out Samy Kamkar's [pwnat](https://samy.pl/pwnat/) project.*

## Traffic/Transport

Traffic can be transported over the following protocols/encryption schemes:

 - TCP Raw
 - TCP XOR
 - UDP Raw
 - UDP XOR
 

#### Raw
With a raw connection, the data is duplicated exactly as it is received by the client. This provides no connection obscurity whatsoever, and all protocols 


## Example

**Reverse Server** is running on public IP *70.35.17.5:8000* and the **Reverse Client** is running on a client sitting behind a NAT with the public IP *200.100.50.25* and a private IP scheme of *192.168.1.0/24*. The owner of **Reverse Server** would like to access an SSH server running on *192.168.1.3:22* tunneled through a device with the IP *192.168.1.7*.


 1. **Reverse Client** on private IP *192.168.1.7* establishes an outbound connection to **Reverse Server** *70.35.17.5:8000* with the destination service *192.168.1.3:22*.
 2. **Reverse Server** receives the connection. A random port, 38057, is opened on the server's loopback or public interface.
 3. Any connection to **Reverse Server**'s port 38057 will create a new connection from *192.168.1.7* to *192.168.1.3:22*.
