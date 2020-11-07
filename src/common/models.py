from sqlalchemy.orm import relationship
from sqlalchemy import Column, Integer, String, ForeignKey, Enum
from common.conf import Base, ScopedSession
from common.states import CubeStates, GroupStates
from random import choice
PLAYER_NAMES = ['Mario Speedwagon', 'Petey Cruiser', 'Anna Sthesia', 'Paul Molive', 'Anna Mull', 'Gail Forcewind',
             'Paige Turner', 'Bob Frapples', 'Walter Melon', 'Nick R. Bocker', 'Barb Ackue', 'Buck Kinnear',
             'Greta Life', 'Ira Membrit', 'Shonda Leer', 'Brock Lee', 'Maya Didas', "Rick O'Shea", 'Pete Sariya',
             'Monty Carlo', 'Sal Monella', 'Sue Vaneer', 'Cliff Hanger', 'Barb Dwyer', 'Terry Aki', 'Cory Ander',
             'Robin Banks', 'Jimmy Changa', 'Barry Wine', 'Wilma Mumduya', 'Buster Hyman', 'Poppa Cherry', 'Zack Lee',
             'Don Stairs', 'Saul T. Balls', 'Peter Pants', 'Hal Appeno ', 'Otto Matic', 'Moe Fugga', 'Graham Cracker',
             'Tom Foolery', 'Al Dente', 'Bud Wiser', 'Polly Tech', 'Holly Graham', 'Frank N. Stein', 'Cam L. Toe',
             'Pat Agonia', 'Tara Zona', 'Barry Cade', 'Phil Anthropist ', 'Marvin Gardens', 'Phil Harmonic ',
             'Arty Ficial', 'Will Power', 'Donatella Nobatti', 'Juan Annatoo', 'Stew Gots', 'Anna Rexia', 'Bill Emia',
             'Curt N. Call', 'Max Emum', 'Minnie Mum', 'Bill Yerds', 'Hap E. Birthday', 'Matt Innae', 'Polly Science',
             'Tara Misu', 'Ed U. Cation', 'Gerry Atric', 'Kerry Oaky', 'Midge Itz', 'Gabe Lackmen', 'Mary Christmas',
             'Dan Druff', 'Jim Nasium', 'Angie O. Plasty', 'Ella Vator', 'Sal Vidge', 'Bart Ender', 'Artie Choke',
             'Hans Olo', 'Marge Arin', 'Hugh Briss', 'Gene Poole', 'Ty Tanic', 'Manuel Labor', 'Lynn Guini',
             'Claire Voyant', 'Peg Leg', 'Jack E. Sack', 'Marty Graw', 'Ash Wednesday', 'Olive Yu', 'Gene Jacket',
             'Tom Atoe', 'Doug Out', 'Sharon Needles', 'Beau Tie', 'Serj Protector', 'Marcus Down', 'Warren Peace',
             'Bud Jet', 'Barney Cull', 'Marion Gaze', 'Eric Shun', 'Mal Practice', 'Ed Itorial', 'Rick Shaw',
             'Paul Issy', 'Ben Effit', 'Kat E. Gory', 'Justin Case', 'Louie Z. Ana', 'Aaron Ottix', 'Ty Ballgame',
             'Anne Fibbiyon', 'Barry Cuda', 'John Withawind', 'Joe Thyme', 'Mary Goround', 'Marge Arita',
             'Frank Senbeans', 'Bill Dabear', 'Ray Zindaroof', 'Adam Zapple', 'Lewis N. Clark', 'Matt Schtick',
             'Sue Shee', 'Chris P. Bacon', 'Doug Lee Duckling', 'Mason Protesters', 'Sil Antro', 'Cal Orie',
             'Sara Bellum', 'Al Acart', 'Marv Ellis', 'Evan Shlee', 'Terry Bull', 'Mort Ission', 'Mark Ette',
             'Ken Tucky', 'Louis Ville', 'Colin Oscopy', 'Fred Attchini', 'Al Fredo', 'Penny Tration', 'Reed Iculous',
             'Chip Zinsalsa', 'Matt Uhrafact', 'Jack Dup', 'Mike Roscope', 'Lou Sinclark', 'Faye Daway', 'Javy Cado',
             'Tom Ollie', 'Sam Buca', 'Phil Anderer', 'Sam Owen', 'Mary Achi', 'Ray Cyst', 'Curtis E. Flush',
             'Holland Oats', 'Helen Highwater', 'Eddy Kitt', 'Al Toesacks', 'Sir Kim Scision', 'Elle Bowdrop',
             'Yu Tube', 'Ellis Dee', 'Anna Lytics', 'Sara Bellum', 'Penny Trate', 'Phil Erup', 'Jenna Side',
             'Mary Nara', 'Mick Donalds', 'Amber Alert', 'Vic Tory', 'Bobby Pin', 'Dom Inate', 'Hugh Miliation',
             'Christian Mingle', 'Juan Soponatime', 'Dante Sinferno', 'Ed Zupp', 'Sarah Yevo', 'Jess Thetip',
             'Arthur Itis', 'Faye Sbook', 'Carrie R. Pigeon', 'Rachel Slurs', 'Ty Pryder', 'Cole Slaw', 'Pat Ernity',
             'Deb Utant', 'Luke Warm', 'Travis Tee', 'Clara Fication', 'Paul Itician', 'Deb Utant', 'Moe Thegrass',
             'Carol Sell', 'Scott Schtape', 'Cody Pendant', 'Frank Furter', 'Barry Dalive', 'Mort Adella',
             'Ray Diation', 'Mack Adamia', 'Farrah Moan', 'Theo Retical', 'Eda Torial', "Mae O'Nayse", 'Bella Ruse',
             'Yuri thra', 'Tucker Doubt', 'Cara Larm', 'Abel Body', 'Sal Ami', 'Colin Derr', 'Cathy Derr',
             "Colin O'Scopy", 'Mel Anoma', 'Adam Up', 'Lou Zing', 'Mark Key', 'Sven Gineer', 'Mick Rib', 'Benny Ficial',
             'Genie Inabottle', 'Gene Therapy', 'Reggie Stration', 'Lou Ow', 'Lance Dorporal', 'Lou Tenant',
             'Nick Knack', 'Patty Whack', 'Reuben Sandwich', 'Hugo Slavia', 'Aaron Spacemuseum', 'Petey Atricks',
             'Dan Delion', 'Terry Torial', 'Cal Q. Later', 'Jen Trification', 'Indy Nile', 'Ray Volver', 'Minnie Strone',
             'Gustav Wind', 'Paul Samic', 'Vinny Gret', 'Joyce Tick', 'Cliff Diver', 'Earl E. Riser', 'Cooke Edoh',
             'Jen Youfelct', 'Reanne Carnation', 'Paul Misunday', 'Chris P. Cream', 'Gio Metric', 'Caire Innet',
             'Marsha Mello', 'Manny Petty', 'Val Adictorian', 'Lucy Tania', 'Jaques Amole']


class Player(Base):
    __tablename__ = 'players'
    
    id = Column(Integer, primary_key=True)
    group_id = Column(Integer, ForeignKey('groups.id'))
    avatar_id = Column(Integer, ForeignKey('avatars.id'))

    name = Column(String)
    type = Column(String)

    answers = relationship('Answer', back_populates='player')
    group = relationship('Group', back_populates='players')
    avatar = relationship('Avatar', uselist=False)

    def get_score(self):
        return sum([a.points for a in self.answers])

    @staticmethod
    def random_name():
        return choice(PLAYER_NAMES)


class GameInstance(Base):
    __tablename__ = 'game_instances'

    id = Column(Integer, primary_key=True)
    group_id = Column(Integer, ForeignKey('groups.id'))
    game_id = Column(Integer, ForeignKey('games.id'))
    curr_question_id = Column(Integer, ForeignKey('questions.id'))

    round = Column(Integer, default=0)
    player = Column(Integer, default=0)
    players = Column(Integer)
    max_rounds = Column(Integer)

    group = relationship('Group', uselist=False, back_populates='curr_game')
    game = relationship('Game')

    curr_question = relationship('Question')

    @staticmethod
    def new(game_id, group_id):
        group = ScopedSession.query(Group).filter(Group.id == group_id).first()
        game = ScopedSession.query(Game).filter(Game.id == game_id).first()
        n = GameInstance(game_id=game_id, group_id=group_id)
        n.players = ScopedSession.query(Group.players).filter(Group.id == group_id).count()
        n.max_rounds = 1

        ScopedSession.add(n)
        ScopedSession.commit()
        group.game_instance_id = n.id

        return n


class Group(Base):
    __tablename__ = 'groups'
    query = ScopedSession.query_property()

    id = Column(Integer, primary_key=True)
    cube_id = Column(Integer, ForeignKey('cubes.id'))

    name = Column(String)
    auth_token = Column(String)

    state = Column(Enum(GroupStates), default=GroupStates.INIT)

    curr_game = relationship('GameInstance', uselist=False, back_populates='group')
    # current_round= Column(Integer, default=0)
    # current_player= relationship('Player', foreign_keys=[current_player_id])

    cube = relationship('Cube', uselist=False, back_populates='group')
    players = relationship('Player', back_populates='group')

    def get_av_cubes(self):
        return ScopedSession.query(Cube) \
            .filter(Cube.state == CubeStates.CONNECTED) \
            .all()

    # def get_rem_question(self):
    #     return ScopedSession.query(Question)\
    #                         .filter(Question.game_id == self.game_id)\
    #                         .filter(~Question.id.in_(ScopedSession.query(Question.id) \
    #                                                  .join(Answer, Answer.question_id==self.curr_question )\
    #                                                  .filter(Player.in_(self.get_players()))
    #
    #                                                  ))
    #


class Cube(Base):
    __tablename__ = 'cubes'

    id = Column(Integer, primary_key=True)

    state = Column(Enum(CubeStates), default=CubeStates.DISCONNECTED )
    group = relationship('Group', back_populates='cube', uselist=False, )


class Game(Base):
    __tablename__ = 'games'

    id = Column(Integer, primary_key=True)
    name = Column(String)
    questions = relationship('Question')
    intro = Column(String)


class Question(Base):
    __tablename__ = 'questions'

    id = Column(Integer, primary_key=True)
    game_id = Column(Integer, ForeignKey('games.id'))
    text = Column(String)
    intro = Column(String)
    curiosity = Column(String)
    points = Column(Integer)

    choices = relationship('Choice', back_populates='question')


class Answer(Base):
    __tablename__ = 'answers'

    id = Column(Integer, primary_key=True)
    game_id = Column(Integer, ForeignKey('cubes.id') )
    player_id = Column(Integer, ForeignKey('players.id') )
    choice_id = Column(Integer, ForeignKey('choices.id') )
    question_id = Column(Integer, ForeignKey('questions.id'))
    points = Column(Integer)
    player = relationship('Player', back_populates='answers')
    question = relationship('Question')
    choice = relationship('Choice')


class Choice(Base):
    __tablename__ = 'choices'

    id = Column(Integer, primary_key=True)
    question_id = Column(Integer, ForeignKey('questions.id'))

    correct = Column(Integer)
    text = Column(String)

    question = relationship('Question', back_populates='choices')


class Avatar(Base):
    __tablename__ = 'avatars'

    OPTIONS = {
        'CLOTH': ['BlazerShirt', 'Hoodie','ShirtScoopNeck','ShirtVNeck'],
        'SKIN_COLOR': ['Light', 'Tanned', 'Brown', 'Black'],
        'HAIR': ['NoHair', 'Hat', 'LongHairCurly', 'ShortHairSides']
    }

    id = Column(Integer, primary_key=True)

    cloth = Column(String, default=(OPTIONS['CLOTH'].pop()))
    skin_color = Column(String, default=(OPTIONS['SKIN_COLOR'].pop()))
    hair = Column(String, default=(OPTIONS['HAIR'].pop()))

    def get_image_src(self):
        return 'https://avataaars.io/?avatarStyle=Transparent&' \
               f'topType={self.hair}&' \
               'accessoriesType=Blank&' \
               'hairColor=BrownDark&' \
               'facialHairType=Blank&' \
               f'clotheType={self.cloth}&' \
               'eyeType=Default&' \
               'eyebrowType=Default&' \
               'mouthType=Default&' \
               f'skinColor={self.skin_color}'

    @staticmethod
    def random():
        c = choice(Avatar.OPTIONS['CLOTH'])
        sk = choice(Avatar.OPTIONS['SKIN_COLOR'])
        hair = choice(Avatar.OPTIONS['HAIR'])
        return Avatar(cloth=c, skin_color=sk, hair=hair)

