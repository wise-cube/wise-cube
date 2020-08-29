from common.database import *
from flask import g

class GroupHandler:
    def __init__(self, group_id=None, auth_token=None):

        try:
           self.group=g.group
        except:
            if group_id is None and auth_token is None:
                raise Exception('cannot retrieve group, supply one argument')

            elif group_id is not None:
                self.id = group_id
                self.group = ScopedSession.query(Group) \
                             .filter( Group.id == self.id) \
                             .first()
                if self.group is None:
                    raise Exception('cannot retrieve group')

            elif auth_token is not None:
                self.group = ScopedSession.query(Group) \
                             .filter( Group.auth_token == auth_token) \
                             .first()
                self.id = self.group.id
                if self.group is None:
                    raise Exception('cannot retrieve group')


    @staticmethod
    def by_auth_token(auth_token):
        return GroupHandler(auth_token=auth_token)

    @staticmethod
    def by_id(group_id):
        return GroupHandler(group_id=group_id)

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
        p = Player(group_id=self.id, name=name)
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

        '''id = Column(Integer, primary_key=True)
        game_id = Column(Integer, ForeignKey('cubes.id'))
        player_id = Column(Integer, ForeignKey('players.id'))
        choice_id = Column(Integer, ForeignKey('cubes.id'))'''

        a=Answer(game_id=self.group.current_game_id, player_id=self.group.current_player_id, choice_id=self.group.current_round.choices[answer_number])
        ScopedSession.add(a)
        ScopedSession.commit()