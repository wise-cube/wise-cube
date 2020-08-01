from enum import Enum as  EnumClass


class CubeStates(EnumClass):
    DISCONNECTED = 1
    CONNECTED = 2
    WAITING_GROUP = 3
    RECONNECTING = 4
    IN_GROUP = 5
