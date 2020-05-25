
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship, sessionmaker
from sqlalchemy import create_engine, MetaData
from sqlalchemy import Column, Integer, String, ForeignKey
import random
import string

from pprint import pprint
import json

Base = declarative_base()

class Player(Base):
    __tablename__ = 'players'

    id = Column(Integer, primary_key=True)
    name = Column(String)
    auth_token = Column(String)

    cube_id = Column(Integer, ForeignKey('cubes.id'))

    def __repr__(self):
        return "<Player(id='%d', name='%s', auth_token='%s')>" % \
               (self.id, self.name, self.auth_token)

class Cube(Base):
    __tablename__ = 'cubes'

    id = Column(Integer, primary_key=True)
    players = relationship('Player')


    group_id = Column(Integer, ForeignKey('groups.id'))



    def __repr__(self):
        return f"<Cube(id={self.id}, group_id={self.group_id}, players={self.players})>"


class Group(Base):
    __tablename__ = 'groups'

    id = Column(Integer, primary_key=True)
    cubes = relationship("Cube")



    def __repr__(self):
        return "<User(id='%d', name='%s', auth_token='%s')>" % \
               (self.id, self.name, self.auth_token)



class Question(Base):
    __tablename__ = 'questions'

    id = Column(Integer, primary_key=True)
    question_text = Column(String)
    answers = relationship("Answer")

    def __repr__(self):
        return "<Question(id='%d', question_text='%s', answers='%s')>" % \
               (self.id, self.question_text, self.answers)

class Answer(Base):
    __tablename__ = 'answers'

    id = Column(Integer, primary_key=True)
    answer_text = Column(String)
    aux_text = Column(String)
    question_id = Column(Integer, ForeignKey('questions.id'))

    def __repr__(self):
        return "<User(answer='%d', explaination='%s)>" % \
               (self.answer_text, self.aux_text)


class DB:

    def __init__(self):
        self.engine = create_engine('sqlite:///:memory:', echo=True)
        self.session_maker = sessionmaker(bind=self.engine)
        self.session = self.session_maker()
        self.debug = 1
        Base.metadata.create_all(self.engine)

    def seed(self):
        players = []
        cube = Cube()
        players += [Player(name="Pippo")]
        players += [Player(name="Pluto")]
        players += [Player(name="Paperino")]
        players += [Player(name="John")]
        cube.players = players

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
        result = self.session.query(Cube).filter_by(id=player_id).first()
        if (self.debug > 0): print(result)
        return result

    def get_player_by_cube_id(self, cube_id):
        result = self.session.query(Cube).filter_by(cube_id=cube_id).all()
        if (self.debug > 0): print(result)
        return result

    def get_cube_by_id(self, cube_id):
        result = self.session.query(Cube).filter_by(id=cube_id).first()
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


class GameLogic(DB):

    def new_group(self, cube_id):
        tokens = []
        players = []

        cube = self.get_cube_by_id(cube_id)
        for i in range(4):
            t = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(128))
            tokens.append(tokens)
            players.append(Player(auth_token=t, cube_id=cube.id))
        self.session.add_all(players)
        self.session.commit()
        return tokens

    def new_game(self, cube_id):
        tokens = []
        players = []

        cube = self.get_cube_by_id(cube_id)
        for i in range(4):
            t = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(128))
            tokens.append(tokens)
            players.append(Player(auth_token=t, cube_id=cube.id))
        self.session.add_all(players)
        self.session.commit()
        return tokens




game = GameLogic()
game.seed()
print("Players:", game.get_players())
print("Players:", game.new_group(1))
print("Players:", game.get_players())