from flask import Flask, render_template, request, abort, redirect, make_response
import game_server
from flask_socketio import SocketIO
import json
from flask_qrcode import QRcode

flask_server = Flask(__name__, static_folder="static/")
db = game_server.GameServer()

socketio = SocketIO(flask_server)

flask_server.config['SERVER_NAME'] = db.host
QRcode(flask_server)
# flask_server.config['SECRET_KEY'] = 'secret!'

#
# @flask_server.route('/')
# @flask_server.route('/guide/<title>/<txt_q>', methods=['GET', 'POST'])
# @flask_server.route('/game/<question>/<a>/<b>/<c>/<d>/<txt_a>', methods=['GET', 'POST'])
# def homepage():
#     if request.method == 'GET':
#         ty = request.args.get('t')
#         title="prova titolo"
#         txt_q="prova testo"
#         txt_a="prova testo risp"
#         a="prova a"
#         b="prova b"
#         c="prova d"
#         d="prova D"
#         question="prova domanda"
#         if ty == "txt":
#             return render_template("guide.html", title=title, txt_q=txt_q)
#         else:
#             return render_template("game.html", question=question, a=a, b=b, c=c, d=d, txt_a=txt_a)

@flask_server.route('/')
@flask_server.route('/home')
def home_page():
    if not 'auth_token' in request.cookies.keys():
        return redirect('/cubes')
    else:
        auth_token = request.cookies.get('auth_token')
        player = db.get_player_by_auth_token(auth_token)
        return render_template('home.html',player=player)


@flask_server.route('/players')
def players_page():
    players = db.get_players()
    return render_template("players.html", host=db.host, players=players)

@flask_server.route('/player')
def player_page():
    auth_token = request.args.get('auth_token')
    if not auth_token:
        auth_token = request.cookies.get('auth_token')
    if auth_token:
        player = db.get_player_by_auth_token(auth_token)
        resp = make_response(render_template("player.html", host=db.host, player=player))
        resp.set_cookie('auth_token', auth_token)
        return resp
    else:
        return redirect('/new_player')


@flask_server.route('/cubes')
def cubes_page():
    cubes = db.get_cubes()
    return render_template("cubes.html", host=db.host, cubes=cubes)

@flask_server.route('/new_player',methods=['GET', 'POST'])
def new_player_page():
    cube_id = request.args.get('cube_id')
    if not cube_id:
        cube_id = request.form.get('cube_id')
    if cube_id:
        if request.method == 'GET':
            return render_template('new_player.html', cube_id=cube_id)

        elif request.method == 'POST':
            name = request.form.get('name')
            kind = request.form.get('type')
            _, auth_token = db.new_player(cube_id, name, kind)
            return redirect(f"/player?auth_token={auth_token}")

    else:
        print(request.args)
        return abort(500)

@flask_server.route('/new_game')
def new_game_trigger():
    cube_id=request.args.get('cube_id')
    game_id=request.args.get('game_id')
    db.new_game(cube_id=cube_id, game_id=game_id)
    return redirect('/cubes')



@flask_server.route('/start_game')
def start_game_trigger():
    cube_id=request.args.get('cube_id')
    db.start_game(cube_id=cube_id)
    return redirect('/cubes')

@flask_server.route('/new_answer')
def new_answer_trigger():

    answer_num = request.args.get('ans')
    auth_token = request.cookies.get('auth_token')
    player = db.get_player_by_auth_token(auth_token)
    cube = db.get_cube_by_id(player.cube_id)
    game = db.get_game_by_id(cube.curr_game_id)
    db.new_answer(player_id=player.id, game_id=cube.curr_game_id, cube_id=cube.id, round_id=game.rounds[cube.curr_round_id].id, answer_num=answer_num)
    return redirect('/game')

@flask_server.route('/next_round')
def next_round_trigger():
    cube_id=request.args.get('cube_id')
    cube = db.get_cube_by_id(cube_id)
    game = db.get_game_by_id(cube.curr_game_id)
    rounds = game.rounds
    if cube.curr_round_id < len(rounds):
        cube.curr_round_id += 1
        return redirect('/cubes')
    else:
        return redirect('/cubes')


@flask_server.route('/end_game')
@flask_server.route('/stop_game')
def stop_game_trigger():
    cube_id=request.args.get('cube_id')
    db.end_game(cube_id=cube_id)
    return redirect('/cubes')

@flask_server.route('/logout')
def logout_page():
    resp = make_response(redirect('/'))
    resp.set_cookie('auth_token', '', expires=0)
    return resp

@flask_server.route('/game')
def game_page():
    auth_token = request.cookies.get('auth_token')
    player = db.get_player_by_auth_token(auth_token)
    cube = db.get_cube_by_id(player.cube_id)
    game = db.get_game_by_id(cube.curr_game_id)
    if game:


        round = game.rounds[cube.curr_round_id]
        choices = json.loads(round.choices)

        answer = db.get_answer(player_id=player.id, game_id=cube.curr_game_id, cube_id=cube.id, round_id=round.id)

        print(answer)
        print(db.get_answers())
        print(player.id,cube.curr_game_id,cube.id,round.id)

        if not answer:
            answer_text = "None"
        elif not answer.correct:
            answer_text="Wrong"
        else:
            answer_text="Right"
        return render_template('game.htm', game=game, cube=cube, choices=choices, answer=answer_text, curiosity=round.curiosity, question= round.question_text)

    else:
        answer_text = "None"
        return render_template('game.htm',cube=cube)

# @socketio.on('message')
# def handle_message(message):
#     print('received message: ' + message)
#
# @socketio.on('json')
# def handle_json(json):
#     print('received json: ' + str(json))

@socketio.on('connection')
def handle_my_custom_event(json):
    print('received json: ' + str(json))

    socketio.emit("")




if __name__ == '__main__':


    flask_server.run()
