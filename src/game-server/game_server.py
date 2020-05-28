
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship, sessionmaker
from sqlalchemy import create_engine, MetaData
from sqlalchemy import Column, Integer, String, ForeignKey
from secrets import token_urlsafe  as new_token
import random
import string

from pprint import pprint
import json




Base = declarative_base()

class Player(Base):
    __tablename__ = 'players'

    id = Column(Integer, primary_key=True)
    name = Column(String)
    type = Column(String)
    auth_token = Column(String)
    cube_id = Column(Integer, ForeignKey('cubes.id'))
    score =  Column(Integer)

    answers = relationship('Answer')

    def __repr__(self):
        return "<Player(id='%d', name='%s', auth_token='%s')>" % \
               (self.id, self.name, self.auth_token)

class Answer(Base):
    __tablename__ = 'answers'

    id = Column(Integer, primary_key=True)
    player_id = Column(Integer, ForeignKey('players.id'))
    game_id = Column(Integer, ForeignKey('games.id'))
    cube_id = Column(Integer, ForeignKey('cubes.id'))
    round_id = Column(Integer, ForeignKey('rounds.id'))
    correct = Column(Integer)

    def __repr__(self):
        return "<Answer(id='%d', player_id='%s', game_id='%d', cube_id='%d' round_id='%s',correct='%d')>" % \
               (self.id, self.player_id, self.game_id, self.cube_id, self.round_id, self.correct)

class Cube(Base):
    __tablename__ = 'cubes'

    id = Column(Integer, primary_key=True)
    players = relationship('Player')
    curr_game_id = Column(Integer, ForeignKey('games.id'))
    curr_round_id = Column(Integer, ForeignKey('rounds.id'))


    def join_url(self):
        return f"http://{GameServer.host}/new_player?cube_id={self.id}"

    def __repr__(self):
        return f"<Cube(id={self.id},  players={self.players})>"

class Game(Base):
    __tablename__ = 'games'

    id = Column(Integer, primary_key=True)
    name = Column(String)
    rounds = relationship('Round')

class Round(Base):
    __tablename__ = 'rounds'

    id = Column(Integer, primary_key=True)
    game_id = Column(Integer, ForeignKey('games.id'))

    question_text = Column(String)
    choices = Column(String)
    correct_answer = Column(Integer)



# class GameHandler:
#
#     def __init__(self, game):
#         players = []
#         game = game
#         rounds = game.rounds
#         n_rounds = len(rounds)
#         curr_round = 0
#         curr_question = 0
#         curr_choices = 0
#         curr_answers = []
#         status = "Waiting!"
#
#     def start_round(self, new_round):
#
#
#
#
#     def run(self):
#         while( self.curr_round < self.rounds ):
#             while(self.curr_answers < self.players ):




class DB:

    def __init__(self):
        self.engine = create_engine('sqlite:///wise-cube.db', echo=True)
        self.session_maker = sessionmaker(bind=self.engine)
        self.session = self.session_maker()
        self.debug = 1
        Base.metadata.create_all(self.engine)

    def seed(self):
        if len(self.get_cubes()) == 0:

            players = []
            cube = Cube()
            players += [Player(name="Pippo",auth_token=new_token(32))]
            cube.players = players

            g = Game(name="Julius Caesar")
            r1 = Round(game_id=1, question_text="When did he die ?",choices='''["lion","zebra","gazelle"]''',correct_answer=1)
            r2 = Round(game_id=1, question_text="When did she die ?", choices='''["lion","zebra","gazelle"]''',correct_answer=2)
            g.rounds = [r1, r2]

            self.session.add_all([g, r1, r2])

            self.session.add_all(players)
            self.session.add(cube)
            self.session.commit()

    def get_players(self):
        result = self.session.query(Player).all()
        if (self.debug > 0): print(result)
        return result

    def get_cubes(self):
        result = self.session.query(Cube).all()
        if (self.debug > 0): print(result)
        return result

    def get_player_by_id(self, player_id):
        result = self.session.query(Player).filter_by(id=player_id).first()
        if (self.debug > 0): print(result)
        return result

    def get_round_by_id(self, round_id):
        result = self.session.query(Round).filter_by(id=round_id).first()
        if (self.debug > 0): print(result)
        return result

    def get_player_by_auth_token(self, auth_token):
        result = self.session.query(Player).filter_by(auth_token=auth_token).first()
        if (self.debug > 0): print(result)
        return result

    def get_players_by_cube_id(self, cube_id):
        result = self.session.query(Cube).filter_by(cube_id=cube_id).all()
        if (self.debug > 0): print(result)
        return result

    def get_cube_by_id(self, cube_id):
        result = self.session.query(Cube).filter_by(id=cube_id).first()
        if (self.debug > 0): print(result)
        return result

    def get_game_by_id(self, game_id):
        result = self.session.query(Game).filter_by(id=game_id).first()
        if (self.debug > 0): print(result)
        return result

    def get_answer(self, player_id, game_id, cube_id, round_id):

        result = self.session.query(Answer).filter_by(player_id=player_id,game_id=game_id,cube_id=cube_id,round_id=round_id).first()
        if (self.debug > 0): print(result)
        return result

    def get_answers(self):

        result = self.session.query(Answer).all()
        if (self.debug > 0): print(result)
        return result

    def dump(self):
        meta = MetaData()
        meta.reflect(bind=self.engine)  # http://docs.sqlalchemy.org/en/rel_0_9/core/reflection.html
        result = {}
        for table in meta.sorted_tables:
            result[table.name] = [dict(row) for row in self.engine.execute(table.select())]
        if (self.debug > 0): print(json.dumps(result, indent=2))
        return json.dumps(result)


class GameServer(DB):
    hostname = '192.168.1.200'
    port = 5000
    host = f"{hostname}:{port}"

    def new_cube(self, cube_id):

        cube = Cube()
        self.session.add(cube)
        self.session.commit()
        return cube.id


    def new_player(self, cube_id, player_name, player_type):
        player = Player()
        player.cube_id = cube_id
        player.name = player_name
        player.type = player_type
        player.auth_token = new_token(64)


        cube = self.get_cube_by_id(cube_id)
        print(dir(cube))

        cube.type = player_type

        self.session.add(player)
        self.session.commit()
        return player.id, player.auth_token

    def new_answer(self, player_id, game_id, cube_id, round_id, answer_num ):
        if self.get_answer(player_id, game_id, cube_id, round_id):
            return

        round = self.get_round_by_id(round_id)
        real_answer_num = round.correct_answer
        print('HHhhhhhhhhhhhhhhh')
        print(real_answer_num)
        print(answer_num)
        correct = ( int(real_answer_num) == int(answer_num) )

        ans = Answer(player_id=player_id,game_id=game_id,cube_id=cube_id,round_id=round_id,correct=correct)
        self.session.add(ans)
        self.session.commit()

    def new_game(self, cube_id, game_id):
        cube = self.get_cube_by_id(cube_id)
        cube.curr_game_id = game_id
        cube.curr_round_id = -1

        self.session.commit()

    def start_game(self, cube_id ):
        cube = self.get_cube_by_id(cube_id)
        cube.curr_round_id = 0

        self.session.commit()

    def next_round(self, cube_id ):
        cube = self.get_cube_by_id(cube_id)
        cube.curr_round += 1

        self.session.commit()

    def end_game(self, cube_id):
        cube = self.get_cube_by_id(cube_id)
        cube.curr_game_id = None
        cube.curr_round_id = None

        self.session.commit()





