# TCPClient
A simple client developed in Qt with C++ for transferring network packets to a TCP server by following a specific protocol. This specific client connects to a given IP address on a temporary port, receives a new port as response and then re-connects with the new port. The server will then ping the client a random amount of times, and the client's job is to respond by returning a pong. The client eventually abort the connection.

### Protocol:
| Offset | Size    | Description |
| --- | --- | --- |
| 0 | 2 bytes | Total package size in bytes |
| 2 | 1 byte | Message ID (see below) |
| 3 | - | Data request |

Message IDs:
```
0x01 - Request a port
0x02 - Receive a port,
0x03 - Receive a ping from the server,
0x04 - Return a pong to the server,
0x05 - Abort connection
```

### Preview:
![](img/screenshot.png?raw=true)
