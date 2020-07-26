from database_handler import DatabaseHandler
from models import *
from flask_sqlalchemy_session import current_session

db = DatabaseHandler()

class CubeHandler:

    @staticmethod
    def join_cube(player_id,cube_id):
        s = current_session
        s.query(Player) \
            .filter(Player.id == player_id) \
            .update({Player.cube_id: cube_id})
        s.commit()
        pass

    @staticmethod
    def leave_cube(player_id):
        s = current_session
        s.query(Player) \
            .filter(Player.id == player_id) \
            .update({Player.cube_id: None})
        s.commit()

class GameHandler:
    @staticmethod
    def join_game(player_id, game_id):
        print(type(player_id),type(game_id))
        s = current_session
        c_id = s.query(Player) \
            .filter(Player.id == player_id) \
            .first().cube_id
        s.query(Cube) \
            .filter(Cube.id == str(c_id)) \
            .update({Cube.curr_game_id: game_id})

        print(c_id)
        print(s.query(Cube) \
            .filter(Cube.id == str(c_id)).all())

        s.commit()

    @staticmethod
    def leave_game(player_id):
        s = current_session
        c_id = s.query(Player) \
            .filter(Player.id == player_id) \
            .first().cube_id
        s.query(Cube) \
            .filter(Cube.id == str(c_id)) \
            .update({Cube.curr_game_id: None})
        s.commit()