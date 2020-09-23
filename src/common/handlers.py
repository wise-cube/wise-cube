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

    def new_player(self, name, avatar_id):
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
            .filter(Cube.id == cube_id) \
            .update({Cube.state: CubeStates.IN_GROUP})
        s.query(Group) \
            .filter(Group.id == self.group.id) \
            .update({Group.cube_id: cube_id})
        s.commit()

    def leave_cube(self):
        s = ScopedSession()
        if g.group.cube:
            g.group.cube.state = CubeStates.CONNECTED
            g.group.cube = None
            s.add(g.group, g.group.cube)
            s.commit()

    def start_game(self, game_id):
        gi = GameInstance.new(game_id, self.group.id)
        gi.group.state = GroupStates.IN_GAME

        ScopedSession.add(gi)
        ScopedSession.commit()

    def end_game(self):
        self.group.state = GroupStates.IDLE
        self.group.curr_game = None
        ScopedSession.add(self.group)
        ScopedSession.commit()

    def next_round(self):
        gi = self.group.curr_game
        if gi.round == 0:
            gi.round += 1
        else:
            gi.player = (gi.player + 1) % gi.players
            if gi.player == 0:
                gi.round += 1

        gi.curr_question_id = gi.game.questions[gi.player].id

        ScopedSession.add(self.group, gi)
        ScopedSession.commit()

    def to_group_topic(self):
        return f"/to_group/{self.group.id}"

    def get_players(self):
        result = Group.query.filter(Group.id == self.group.id).first()
        print(result)
        return result

    def get_curr_player(self):
        if self.group.players:
            return None

        if self.group.curr_game is None:
            game = Game(name=None, intro=None)
            self.group.curr_game = GameInstance.new(game.id, self.group.id)

        return self.group.players[self.group.curr_game.player]

    def is_curr_player(self, p):
        return self.get_curr_player() == p and self.group.curr_game and self.group.curr_game.round > 0

    def new_answer(self, answer_number):
        answer_number = int(answer_number)

        '''
        id = Column(Integer, primary_key=True)
        game_id = Column(Integer, ForeignKey('cubes.id'))
        player_id = Column(Integer, ForeignKey('players.id'))
        choice_id = Column(Integer, ForeignKey('cubes.id'))
        '''

        choice = self.group.curr_game.curr_question.choices[answer_number]
        c = choice.correct
        gi = self.group.curr_game
        if not ScopedSession.query(Answer) \
                       .filter(Answer.player == GroupHandler.by_session().get_curr_player(),
                               Answer.question_id == gi.curr_question_id).count() > 0:
            a = Answer(game_id=self.group.curr_game.game.id, player=GroupHandler.by_session().get_curr_player(),
                       choice_id=self.group.curr_game.curr_question.choices[answer_number].id,
                       question_id=self.group.curr_game.curr_question.id,
                       points=100 * (int(c)))

            ScopedSession.add(a)
            ScopedSession.commit()
        # self.next_round()

    def is_in_init(self):
        if self.group.state == GroupStates.INIT:
            return 1
        return 0

    def is_in_creation(self):
        if self.group.state == GroupStates.CREATION:
            return 1
        return 0

    def is_in_idle(self):
        if self.group.state == GroupStates.IDLE:
            return 1
        return 0

    def is_in_game(self):
        if self.group.state == GroupStates.IN_GAME:
            return 1
        return 0

    def is_terminated(self):
        if self.group.state == GroupStates.TERMINATED:
            return 1
        return 0

    def change_state(self):
        if self.group.state == GroupStates.INIT:
            self.group.state = GroupStates.CREATION
        return 0
