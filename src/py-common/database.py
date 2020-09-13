from common.models import *
from common.conf import SQL_ENGINE, Base, ScopedSession
from secrets import token_urlsafe  as new_token

class DB:

    @staticmethod
    def migrate():
        Base.metadata.create_all(SQL_ENGINE)

    @staticmethod
    def seed():

        group = Group(auth_token=new_token(64),cube_id=1, name='I cchiu bell', state=GroupStates.IN_GAME)

        cube = Cube()
        ScopedSession.add_all((cube,group))
        ScopedSession.commit()

        players = [Player(name=Player.random_name(), group_id=group.id, avatar=Avatar.random()),
                   Player(name=Player.random_name(), group_id=group.id, avatar=Avatar.random()),
                   Player(name=Player.random_name(), group_id=group.id, avatar=Avatar.random())]

        ScopedSession.add_all(players)
        ScopedSession.commit()

        g1 = Game(name="Discobulus and Kouros", intro="dfjbffibaòbjvfnòdjkvnfòneòjnbòdknlvàa")
        q1 = Question(game_id=1, text="What is the original Discobulus statue is made of?",
                   curiosity="The discus-thrower (Gk. discobolus) has become the iconic image of the Olympic Games, and a fantastic representation of the athletic ideal.")
        q2 = Question(game_id=1, text="What Kouros means?",
                   curiosity="Kouros is a perfume for men produced by Yves Saint Laurent.The perfume was introduced in 1981. It was created by perfumer Pierre Bourdon.The perfume was inspired by a trip to Greece Saint Laurent had taken, he was particularly impressed by the kouroi.")

        ScopedSession.add_all((g1,q1,q2))
        ScopedSession.commit()

        group.current_game=g1
        ScopedSession.add(g1)
        ScopedSession.commit()
        to_add = []
        for i,t in enumerate(["Bronze", "Marble", "Plaster", "Wax"]):
            # print('\n'*10)
            # print(t)
            to_add += [Choice(question_id=1, correct=int(i == 1), text=t)]


        for i,t in enumerate(["Old, men, especially of noble rank.",
                              "Young, girl, especially of noble rank." ,
                              "Old, woman, especially of noble rank.",
                              "Youth, boy, especially of noble rank."])    :
            to_add += [Choice( question_id=2, correct=int(i == 4), text=t)]

        print('\n' * 10)


        ScopedSession.add_all(to_add)
        ScopedSession.commit()
        ScopedSession.close()

