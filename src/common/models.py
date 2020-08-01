from sqlalchemy.orm import relationship
from sqlalchemy import Column, Integer, String, ForeignKey, Enum
from conf import Base
from states import CubeStates


class Group(Base):
    __tablename__ = 'groups'

    id = Column(Integer, primary_key=True)
    cube_id = Column(Integer, ForeignKey('cubes.id'))
    name = Column(String)
    auth_token = Column(String)


    cube = relationship('Cube', back_populates='group')
    players = relationship('Player', back_populates='group')

class Cube(Base):
    __tablename__ = 'cubes'

    id = Column(Integer, primary_key=True)


    state = Column(Enum(CubeStates), default=CubeStates.DISCONNECTED )
    group = relationship('Group', back_populates='cube')



class Player(Base):
    __tablename__ = 'players'

    id = Column(Integer, primary_key=True)
    group_id = Column(Integer, ForeignKey('groups.id'))

    name = Column(String)
    type = Column(String)
    avatar = Column(Integer)

    answers = relationship('Answer', backref='player' )
    group = relationship('Group', back_populates='players' )


    def get_score(self):
        return sum([a.points for a in self.answers])



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
    points = Column(Integer)

    choices = relationship('Choice', back_populates='question')

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

    question = relationship('Question', back_populates='choices')




