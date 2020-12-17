from enum import Enum as  EnumClass


class CubeStates(EnumClass):
    DISCONNECTED = 1
    CONNECTED = 2
    #WAITING_GROUP = 3
    #RECONNECTING = 4
    IN_GROUP = 3


    def describe(self):
        return self.value

class GroupStates(EnumClass):
    CREATION = 1
    PAIRING = 2
    IDLE = 3
    IN_GAME = 4
    TERMINATED = 5
    INIT = 6

    def describe(self):
        return self.value


