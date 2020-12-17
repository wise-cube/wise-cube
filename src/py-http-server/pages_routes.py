from common.database import *
from flask import Blueprint, redirect, render_template, session, request, make_response, g
import flask
from helpers import rel2table
from handlers import GroupHandler
from common.handlers import GroupHandler

pages = Blueprint('pages', __name__,)

# PAGES
@pages.route('/')
@pages.route('/home')
def home_page():
    groups = ScopedSession.query(Group).all()
    return render_template('pages/home.html', groups=groups)


@pages.route('/group')
def group_page():
    if  g.group.state == GroupStates.INIT:
        p_rand_name = Player.random_name()
        p_rand_avatar = Avatar.random()
        ScopedSession.add(p_rand_avatar)
        ScopedSession.commit()
        return render_template('pages/group.html', is_creation=True, p_rand_name=p_rand_name, p_rand_avatar=p_rand_avatar)

    if g.group.state == GroupStates.CREATION or GroupStates.IDLE:
        g.group.state = GroupStates.IN_GAME

    if g.group.state == GroupStates.IN_GAME:
        return render_template('pages/group.html', gi=g.group.curr_game)

    return render_template('pages/group.html', group=g.group)


@pages.route('/debug')
def debug_page():
    tables = [(t_name.capitalize(), rel2table(t)) for t_name, t in RELATIONS.items()]
    return render_template('pages/debug.html', tables=tables)


@pages.route('/game')
def game_page():
    ans = ScopedSession.query(Answer).filter(Answer.player == GroupHandler.by_session().get_curr_player(),
                                             Answer.question == g.group.curr_game.curr_question).first()
    return render_template('pages/game.html', gi=g.group.curr_game, answer=ans)
