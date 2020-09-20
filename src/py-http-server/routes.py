from common.database import *
from flask import Blueprint, redirect, render_template, session, request, make_response, g
import flask
from helpers import rel2table
from handlers import GroupHandler


from common.handlers import GroupHandler
s = ScopedSession()

game = Blueprint('game', __name__, )
pages = Blueprint('pages', __name__,)
tables = Blueprint('tables', __name__, template_folder="templates/tables/")
triggers = Blueprint('triggers',__name__,template_folder="templates/triggers/")

misc = Blueprint('misc', __name__,)


RELATIONS = {
    'cubes': Cube,
    'games': Game,
    'groups': Group,
    'players': Player,
    'answers': Answer,
    'questions': Question,
    'choices': Choice,
    'game_instances': GameInstance
}

# PAGES
@pages.route('/')
@pages.route('/home')
def home_page():
    groups = ScopedSession.query(Group).all()
    return render_template('pages/home.html', groups=groups)

@pages.route('/group')
def group_page():

    if  g.group.state == GroupStates.CREATION:
        p_rand_name = Player.random_name()
        p_rand_avatar = Avatar.random()
        ScopedSession.add(p_rand_avatar)
        ScopedSession.commit()
        return render_template('pages/group.html', is_creation=True, p_rand_name=p_rand_name, p_rand_avatar=p_rand_avatar)

    if g.group.state == GroupStates.IN_GAME:
        return render_template('pages/group.html', gi=g.group.curr_game)



    return render_template('pages/group.html', group=g.group)

@pages.route('/debug')
def debug_page():
    tables = [(t_name.capitalize(), rel2table(t)) for t_name, t in RELATIONS.items()]
    return render_template('pages/debug.html', tables=tables)

@pages.route('/game')
def game_page():
    ans = ScopedSession.query(Answer).filter(Answer.player == GroupHandler.by_session().get_curr_player(), Answer.question == g.group.curr_game.curr_question ).first()
    return render_template('pages/game.html', gi=g.group.curr_game, answer=ans)



# MISC
@misc.route('/rand_name')
def get_rand_name():
    return Player.random_name()

@misc.route('/rand_avatar')
def get_rand_avatar():
    from json import dumps
    avatar = Avatar.random()
    ScopedSession.add(avatar)
    ScopedSession.commit()
    avatar_dic = {
        'avatar_id': avatar.id,
        'avatar_src': avatar.get_image_src()
    }
    return dumps(avatar_dic)






# TRIGGERS
@triggers.route('/login', methods=['POST'])
def login():
    resp = make_response(redirect('/group'))
    print('login')

    t = flask.request.form['auth_token']
    resp.set_cookie('auth-token',t)

    return resp
    # return 'login'

@triggers.route('/logout', methods=['GET'])
def logout():
    resp = make_response(redirect('/'))
    resp.set_cookie('auth-token', '', expires=0)
    return resp

@triggers.route('/new_group', methods=['POST'])
def new_group():
    g_name = request.form['group_name']
    g = GroupHandler.new(name=g_name)

    resp = make_response(redirect('/group'))
    resp.set_cookie('auth-token',g.auth_token)
    return resp

@triggers.route('/end_players_creation', methods=['POST','GET'])
def end_players_creation():

    GroupHandler.by_id(g.group_id).end_player_creation()
    return redirect('/group')

@triggers.route('/new_player', methods=['POST'])
def new_player( ):
    player_name = request.form['player_name']
    player_avatar_id =  request.form['player_avatar_id']

    GroupHandler.by_session().new_player(player_name, player_avatar_id)

    return redirect('/group')

@triggers.route('/new_answer', methods=['GET'])
def new_answer( ):
    ans_num = request.args['num']
    GroupHandler.by_session().new_answer(ans_num)

    return redirect('/game')

@triggers.route('/join_cube', methods=['GET'])
def join_cube():
    cube_id = request.args.get('cube_id')
    gh = GroupHandler.by_session()
    gh.join_cube(cube_id)
    return redirect('/group')

@triggers.route('/leave_cube', methods=['GET'])
def leave_cube():

    GroupHandler.by_session().leave_cube()
    return redirect('/group')



@triggers.route('/next_round', methods=['GET'])
def next_round():
    GroupHandler.by_session().next_round()
    return redirect('/game')

@triggers.route('/start_game', methods=['GET'])
def start_game():
    g_id = request.args['id']
    GroupHandler.by_session().start_game(g_id)
    return redirect('/game')

@triggers.route('/end_game', methods=['GET'])
def end_game():
    GroupHandler.by_session().end_game()
    return redirect('/group')

# @game.route('/player')
# def player_page():
#     auth_token = request.args.get('auth_token')
#     if not auth_token:
#         auth_token = request.cookies.get('auth_token')
#     if auth_token:
#         player = DatabaseHandler.get_player_by_auth_token(auth_token)
#         resp = make_response(render_template("player.html",player=player))
#         resp.set_cookie('auth_token', auth_token)
#         return resp
#     else:
#         return redirect('/new_player')

#
# @game.route('/cubes')
# def cubes_page():
#     cubes = db.get_cubes()
#     return render_template("cubes.htm", cubes=cubes)
#
# @game.route('/new_player', methods=['GET', 'POST'])
# def new_player_page():
#     cube_id = request.args.get('cube_id')
#     if not cube_id:
#         cube_id = request.form.get('cube_id')
#     if cube_id:
#         if request.method == 'GET':
#             return render_template('partials/new_player.html', cube_id=cube_id)
#
#         elif request.method == 'POST':
#             name = request.form.get('name')
#             kind = request.form.get('type')
#             _, auth_token = db.new_player(cube_id, name, kind)
#             return redirect(f"/player?auth_token={auth_token}")
#
#     else:
#         print(request.args)
#         return abort(500)
#
# @game.route('/new_game')
# def new_game_trigger():
#     cube_id=request.args.get('cube_id')
#     game_id=request.args.get('game_id')
#     db.new_game(cube_id=cube_id, game_id=game_id)
#     return redirect('/cubes')
#
#
#
# @game.route('/start_game')
# def start_game_trigger():
#     cube_id=request.args.get('cube_id')
#     db.start_game(cube_id=cube_id)
#     return redirect('/cubes')
#
# @game.route('/new_answer')
# def new_answer_trigger():
#
#     answer_num = request.args.get('ans')
#     auth_token = request.cookies.get('auth_token')
#     player = db.get_player_by_auth_token(auth_token)
#     cube = db.get_cube_by_id(player.cube_id)
#     game = db.get_game_by_id(cube.curr_game_id)
#     db.new_answer(player_id=player.id, game_id=cube.curr_game_id, cube_id=cube.id, round_id=game.rounds[cube.curr_round_id].id, answer_num=answer_num)
#     return redirect('/game')
#
# @game.route('/next_round')
# def next_round_trigger():
#     cube_id=request.args.get('cube_id')
#     cube = db.get_cube_by_id(cube_id)
#     game = db.get_game_by_id(cube.curr_game_id)
#     rounds = game.rounds
#     if cube.curr_round_id < len(rounds):
#         cube.curr_round_id += 1
#         return redirect('/cubes')
#     else:
#         return redirect('/cubes')
#
#
# @game.route('/end_game')
# @game.route('/stop_game')
# def stop_game_trigger():
#     cube_id=request.args.get('cube_id')
#     db.end_game(cube_id=cube_id)
#     return redirect('/cubes')
#
# @game.route('/game')
# def game_page():
#     auth_token = request.cookies.get('auth_token')
#     player = db.get_player_by_auth_token(auth_token)
#     cube = db.get_cube_by_id(player.cube_id)
#     game = db.get_game_by_id(cube.curr_game_id)
#     if game:
#
#
#         round = game.rounds[cube.curr_round_id]
#         choices = json.loads(round.choices)
#
#         answer = db.get_answer(player_id=player.id, game_id=cube.curr_game_id, cube_id=cube.id, round_id=round.id)
#
#         print(answer)
#         print(db.get_answers())
#         print(player.id,cube.curr_game_id,cube.id,round.id)
#
#         if not answer:
#             answer_text = "None"
#         elif not answer.correct:
#             answer_text="Wrong"
#         else:
#             answer_text="Right"
#         return render_template('partials/game.htm', game=game, cube=cube, choices=choices, answer=answer_text, curiosity=round.curiosity, question= round.question_text)
#
#     else:
#         answer_text = "None"
#         return render_template('partials/game.htm', cube=cube)
#
# # @socketio.on('message')
# # def handle_message(message):
# #     print('received message: ' + message)
# #
# # @socketio.on('json')
# # def handle_json(json):
# #     print('received json: ' + str(json))
#
# # @socketio.on('connection')
# # def handle_my_custom_event(json):
# #     print('received json: ' + str(json))
# #
# #     socketio.emit("")
# #
# #
