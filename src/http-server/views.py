from fl


game = Blueprint('game', __name__, )
pages = Blueprint('pages', __name__,)
tables = Blueprint('tables',__name__, template_folder="tables/")
triggers = Blueprint('buttons',__name__)




@tables.route('/tables/players')
def players_table():
    players = Player
    return render_template("tables/players.htm", players=players)

@tables.route('/tables/cubes')
def cubes_table():
    cubes = DatabaseHandler.get_cubes()
    return render_template("tables/cubes.htm", cubes=cubes)

@tables.route('/tables/games')
def games_table():
    games = DatabaseHandler.get_cubes()
    return render_template("tables/games.htm", games=games)

@tables.route('/tables/choices')
def choices_table():
    choices = DatabaseHandler.get_choices()
    return render_template("tables/choices.htm", choices=choices)

@pages.route('/')
@pages.route('/home')
def home():
    player = None
    auth_token = request.args.get('auth_token')
    if auth_token:
        player = DatabaseHandler.get_player_by_auth_token(auth_token)
    return render_template('home.html', player=player)


@pages.route('/debug')
def debug():

    c = DatabaseHandler.get_cubes()
    p = DatabaseHandler.get_players()
    g = DatabaseHandler.get_games()
    q = DatabaseHandler.get_questions()
    a = DatabaseHandler.get_answers()
    cc= DatabaseHandler.get_choices()
    return render_template('debug.html', cubes=c, players=p, games=g, answers=a, questions=q, choices=cc)
    # if not 'auth_token' in request.cookies.keys():
    #     return redirect('/cubes')
    # else:
    #     auth_token = request.cookies.get('auth_token')
    #     player = db.get_player_by_auth_token(auth_token)
    #     return render_template('home.html',player=player)


@triggers.route('/login', methods=['POST'])
def login():
    t = flask.request.form['auth_token']
    p = DatabaseHandler.get_player_by_auth_token(t)
    login_user(p)
    return redirect('/debug')

@triggers.route('/logout', methods=['POST'])
def logout():
    logout_user()
    return redirect('/debug')


@triggers.route('/join_cube', methods=['POST'])
def join_cube():
    player_id = current_user.id
    cube_id = request.form['cube_id']
    CubeHandler.join_cube(player_id, cube_id)
    return redirect('/debug')


@triggers.route('/join_game', methods=['POST'])
def join_game():
    player_id = current_user.id
    game_id = request.form['game_id']
    GameHandler.join_game(player_id, game_id)
    return redirect('/debug')

@triggers.route('/leave_cube', methods=['POST'])
def leave_cube():
    player_id = current_user.id
    CubeHandler.leave_cube(player_id)
    return redirect('/debug')

@triggers.route('/leave_game', methods=['POST'])
def leave_game():
    player_id = current_user.id
    GameHandler.leave_game(player_id)
    return redirect('/debug')










@game.route('/player')
def player_page():
    auth_token = request.args.get('auth_token')
    if not auth_token:
        auth_token = request.cookies.get('auth_token')
    if auth_token:
        player = DatabaseHandler.get_player_by_auth_token(auth_token)
        resp = make_response(render_template("player.html",player=player))
        resp.set_cookie('auth_token', auth_token)
        return resp
    else:
        return redirect('/new_player')

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
