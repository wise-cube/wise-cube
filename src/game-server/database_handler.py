from models import *
from conf import ENGINE
from flask_sqlalchemy_session import current_session
from secrets import token_urlsafe  as new_token

from conf import Session

class DatabaseHandler:
    @staticmethod
    def migration():
        Base.metadata.create_all(ENGINE)
    @staticmethod
    def seed():
        s = Session()
        s.commit()
        to_add = []

        cube = Cube()

        players = [Player(name="Pippo",auth_token=new_token(32), cube_id=cube.id, avatar=1),Player(name="Paperino",auth_token=new_token(32),  cube_id=cube.id,avatar=2),Player(name="Pluto",auth_token=new_token(32), cube_id=cube.id,avatar=3)]
        to_add += [cube]
        to_add += players

        g1 = Game(name="Discobulus and Kouros")
        q1 = Question(game_id=1, text="What is the original Discobulus statue is made of?",
                   curiosity="The discus-thrower (Gk. discobolus) has become the iconic image of the Olympic Games, and a fantastic representation of the athletic ideal.")
        q2 = Question(game_id=1, text="What Kouros means?",
                   curiosity="Kouros is a perfume for men produced by Yves Saint Laurent.The perfume was introduced in 1981. It was created by perfumer Pierre Bourdon.The perfume was inspired by a trip to Greece Saint Laurent had taken, he was particularly impressed by the kouroi.")

        to_add += [g1,q1,q2]
        for i,t in enumerate(["Bronze", "Marble", "Plaster", "Wax"]):
            # print('\n'*10)
            # print(t)
            to_add += [Choice(question_id=1, correct=(i == 1), text=t)]


        for i,t in enumerate(["Old, men, especially of noble rank.",
                              "Young, girl, especially of noble rank." ,
                              "Old, woman, especially of noble rank.",
                              "Youth, boy, especially of noble rank."])    :
            to_add += [Choice( question_id=2, correct=(i == 4), text=t)]

        print('\n' * 10)

        print(to_add)
        s.add_all(to_add)
        s.commit()


            # g2 = Game(name="Athena and Nike")
            # r1 = Question(game_id=1, question_text="When the most famous copy of the Athena was rediscovery?", choices='''["1997", "1797" , "1610", "1893"]''',
            #            correct_answer=2 , curiosity= "Upon rediscovery, it was purchased by Vincenzo Pacetti, who added the peak of the helmet, the straight forearm, the hands, the feet, the snakes, and a section of the cloak, and polished the overall surface. He then sold it to the French Directory, which transported it to Rome, where it was soon seized by Neapolitan armies when they briefly held the city from November to December 1798 during the opening stages of the War of the Second Coalition. France then regained it in the Treaty of Florence, and it was displayed at the Louvre from December 1803 onwards.")
            # r2 = Question(game_id=1, question_text="Where is exposed the original Nike of Samothrace?", choices='''[ "National Archaeological Museum, Athens" , "Louvre Museum, Paris", "Uffizzi gallery, Florence", "MoMa, New York"]''',
            #            correct_answer=2, curiosity="The Nike company takes its name from Nike, the Greek goddess of victory, the Nike logo represent the statue's wing.")
            # g2.rounds = [r1, r2]
            #
            # self.session.add_all([g2, r1, r2])
            # self.session.commit()
            #
            # g3 = Game(name="Lion Gate")
            # r1 = Question(game_id=1, question_text="In which ancient town was build the Lion Gate?", choices='''["Jerusalem", "Mycenae", "Delphi", "Athens"]''',
            #            correct_answer=2, curiosity="In the same archaelogical site of Mycenae has been discovered the famouse Mask of Agamemnon (gold funeral mask).The mask, displayed in the National Archaeological Museum of Athens, has been described by Cathy Gere as the 'Mona Lisa of prehistory' ")
            #
            # g3.rounds = [r1]
            #
            # self.session.add_all([g3, r1])
            #
            # self.session.add_all(players)
            # self.session.add(cube)
            # self.session.commit()

    @staticmethod
    def get_players():
        result = current_session.query(Player).all()
        return result

    @staticmethod
    def get_cubes():
        result = current_session.query(Cube).all()
        return result

    @staticmethod
    def get_questions():
        result = current_session.query(Question).all()
        return result

    @staticmethod
    def get_answers():
        result = current_session.query(Answer).all()
        return result

    @staticmethod
    def get_games():
        result = current_session.query(Game).all()
        return result

    @staticmethod
    def get_choices():
        result = current_session.query(Choice).all()
        return result

    @staticmethod
    def get_player_by_id(player_id):
        s = current_session
        result = s.query(Player).filter_by(id=player_id).first()
        return result

    @staticmethod
    def get_cube_by_id( cube_id):
        s = current_session
        result = s.query(Cube).filter_by(id=cube_id).first()
        return result

    @staticmethod
    def get_game_by_id( game_id):
        s = current_session
        result = s.query(Game).filter_by(id=game_id).first()
        return result

    @staticmethod
    def get_player_by_auth_token( auth_token):
        s = current_session
        result = s.query(Player).filter_by(auth_token=auth_token).first()
        return result



    #
    #
    #
    #
    # def get_round_by_id(self, round_id):
    #     result = self.session.query(Round).filter_by(id=round_id).first()
    #     if (self.debug > 0): print(result)
    #     return result
    #
    #
    #
    # def get_players_by_cube_id(self, cube_id):
    #     result = self.session.query(Cube).filter_by(cube_id=cube_id).all()
    #     if (self.debug > 0): print(result)
    #     return result
    #
    #
    #
    # def get_answer(self, player_id, game_id, cube_id, round_id):
    #
    #     result = self.session.query(Answer).filter_by(player_id=player_id,game_id=game_id,cube_id=cube_id,round_id=round_id).first()
    #     if (self.debug > 0): print(result)
    #     return result
    #
    #
    #
    # def new_cube(self, cube_id):
    #
    #     cube = Cube()
    #     self.session.add(cube)
    #     self.session.commit()
    #     return cube.id
    #
    # def new_player(self, cube_id, player_name, player_type):
    #     player = Player()
    #     player.cube_id = cube_id
    #     player.name = player_name
    #     player.type = player_type
    #     player.auth_token = new_token(64)
    #
    #     cube = self.get_cube_by_id(cube_id)
    #     print(dir(cube))
    #
    #     cube.type = player_type
    #
    #     self.session.add(player)
    #     self.session.commit()
    #     return player.id, player.auth_token
    #
    # def new_answer(self, player_id, game_id, cube_id, round_id, answer_num):
    #     if self.get_answer(player_id, game_id, cube_id, round_id):
    #         return
    #
    #     round = self.get_round_by_id(round_id)
    #     real_answer_num = round.correct_answer
    #     correct = (int(real_answer_num) == int(answer_num))
    #
    #     ans = Answer(player_id=player_id, game_id=game_id, cube_id=cube_id, round_id=round_id, correct=correct)
    #     self.session.add(ans)
    #     self.session.commit()
    #
    # def new_game(self, cube_id, game_id):
    #     cube = self.get_cube_by_id(cube_id)
    #     cube.curr_game_id = game_id
    #     cube.curr_round_id = -1
    #
    #     self.session.commit()
    #
    # def start_game(self, cube_id):
    #     cube = self.get_cube_by_id(cube_id)
    #     cube.curr_round_id = 0
    #
    #     self.session.commit()
    #
    # def next_round(self, cube_id):
    #     cube = self.get_cube_by_id(cube_id)
    #     cube.curr_round += 1
    #
    #     self.session.commit()
    #
    # def end_game(self, cube_id):
    #     cube = self.get_cube_by_id(cube_id)
    #     cube.curr_game_id = None
    #     cube.curr_round_id = None
    #
    #     self.session.commit()