
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship
from sqlalchemy import Column, Integer, String, ForeignKey
Base = declarative_base()

class Cube(Base):
    __tablename__ = 'cubes'

    id = Column(Integer, primary_key=True)
    curr_game_id = Column(Integer, ForeignKey('games.id'))
    curr_question_id = Column(Integer, ForeignKey('questions.id'))

    players = relationship('Player', back_populates='cube',lazy="joined")
    curr_game = relationship('Game')



class Player(Base):
    __tablename__ = 'players'

    id = Column(Integer, primary_key=True)
    name = Column(String)
    type = Column(String)
    auth_token = Column(String)
    cube_id = Column(Integer, ForeignKey('cubes.id'))
    score =  Column(Integer)
    avatar = Column(Integer)


    is_authenticated = True
    is_active = True
    is_anonymous = False

    answers = relationship('Answer', backref='player',lazy="joined" )
    cube = relationship('Cube', back_populates='players', innerjoin=True)

    def get_id(self):
        return self.id



class Game(Base):
    __tablename__ = 'games'

    id = Column(Integer, primary_key=True)
    name = Column(String)
    rounds = relationship('Question')


class Question(Base):
    __tablename__ = 'questions'

    id = Column(Integer, primary_key=True)
    game_id = Column(Integer, ForeignKey('games.id'))
    text = Column(String)
    curiosity = Column(String)

    choices = relationship('Choice', backref='question', lazy="joined")

class Answer(Base):
    __tablename__ = 'answers'

    id = Column(Integer, primary_key=True)
    game_id = Column(Integer, ForeignKey('cubes.id') )
    player_id = Column(Integer, ForeignKey('players.id') )
    choice_id = Column(Integer, ForeignKey('cubes.id') )


class Choice(Base):
    __tablename__ = 'choices'

    id = Column(Integer, primary_key=True)
    question_id = Column(Integer, ForeignKey('questions.id'))
    correct = Column(Integer)
    text = Column(String)
