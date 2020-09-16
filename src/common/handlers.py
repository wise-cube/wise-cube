from flask import g
from .conf import *
from .models import *
from .states import CubeStates, GroupStates
from .database import *


class CubeHandler:
    def __init__(self, cube):
        if not cube:
            raise Exception('cube can\'t be none')
        self.cube = cube

    @staticmethod
    def by_id(cube_id):
        cube = ScopedSession.query(Cube) \
            .filter(Cube.id == cube_id) \
            .first()

        return CubeHandler(cube)

    def connected(self):
        self.cube.state = CubeStates.CONNECTED
        ScopedSession.add(self.cube.state)
        ScopedSession.commit()

    def disconnected(self):
        self.cube.state = CubeStates.DISCONNECTED
        ScopedSession.add(self.cube.state)
        ScopedSession.commit()

    def assign_group(self, group):
        self.cube.state = CubeStates.IN_GROUP
        self.cube.group = group
        ScopedSession.add(self.cube.state)
        ScopedSession.commit()


class GroupHandler:
    def __init__(self, group):
        if not group:
            raise Exception('group can\'t be none')
        self.group = group

    @staticmethod
    def by_id(group_id):
        group = ScopedSession.query(Group) \
            .filter(Group.id == group_id) \
            .first()

        return GroupHandler(group)

    @staticmethod
    def by_auth_token(auth_token):
        group = ScopedSession.query(Group) \
            .filter(Group.auth_token == auth_token) \
            .first()
        return GroupHandler(group)

    @staticmethod
    def by_session():
        group = None
        try:
            group = g.group
        except:
            pass

        return GroupHandler(group)


    @staticmethod
    def new(name=None):
        g = Group()
        g.auth_token = new_token(64)

        if name is not None:
            g.name = name

        ScopedSession.add(g)
        ScopedSession.commit()
        return g

    def new_player(self, name, avatar_id=None):
        p = Player(group_id=self.group.id, name=name)
        if avatar_id is not None:
            p.avatar_id = avatar_id

        ScopedSession.add(p)
        ScopedSession.commit()
        # TODO
        # clean up all avatars not assigned to a player
        return p

    def end_player_creation(self):
        self.group.state = GroupStates.IDLE
        ScopedSession.add(self.group)
        ScopedSession.commit()

    def join_cube(self, cube_id):
        s = ScopedSession
        s.query(Cube) \
            .filter(Group.id == self.id) \
            .update({Group.cube_id: cube_id})
        s.commit()

    def leave_cube(self):
        s = ScopedSession
        s.query(Cube) \
            .filter(Group.id == self.id) \
            .update({Group.cube_id: None})
        s.commit()

    def start_game(self, game_id):
        self.group.state= GroupStates.IN_GAME
        self.group.current_game_id= game_id
        self.group.current_round = 0
        ScopedSession.add(self.group)
        ScopedSession.commit()

    def end_game(self):
        self.group.state = GroupStates.IDLE
        self.group.current_game_id = None
        self.group.current_round = 0
        ScopedSession.add(self.group)
        ScopedSession.commit()

    def next_round(self):
        player_number= self.group.current_round%len(self.group.players)
        self.group.current_player=self.group.players[player_number]
        self.group.current_round=+1
        ScopedSession.add(self.group)
        ScopedSession.commit()

    def new_answer(self, answer_number):

        '''
        id = Column(Integer, primary_key=True)
        game_id = Column(Integer, ForeignKey('cubes.id'))
        player_id = Column(Integer, ForeignKey('players.id'))
        choice_id = Column(Integer, ForeignKey('cubes.id'))
        '''

        a = Answer(game_id=self.group.current_game_id, player_id=self.group.current_player_id, choice_id=self.group.current_round.choices[answer_number])
        ScopedSession.add(a)
        ScopedSession.commit()
