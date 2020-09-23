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
    IDLE = 2
    IN_GAME = 3
    TERMINATED = 4
    INIT = 5

    def describe(self):
        return self.value


