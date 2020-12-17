from common.conf import SECRET_KEY
from common.database import *
from flask import g, make_response,redirect

from flask import Flask, request
from os.path import exists
from os import remove
from loguru import logger

from routes import tables,  triggers, game, misc
from pages_routes import pages
from common.handlers import GroupHandler

def db_init():
    DB.migrate()
    DB.clear()
    DB.migrate()
    DB.seed()


app = Flask(__name__, static_folder='static', static_url_path='/')
app.secret_key = SECRET_KEY


@app.before_request
def before_request_func():
    g.group = None
    print("before_request is running!")
    group_auth_token = request.cookies.get('auth-token')
    print(group_auth_token)
    if not group_auth_token :
        print("No group auth-token found")
    else:
        g.group = ScopedSession.query(Group) \
                                  .filter(Group.auth_token == group_auth_token) \
                                  .first()
        try:
            g.gh=GroupHandler(g.group)

        except:
            print("Invalid token")
            resp = make_response(redirect('/'))
            resp.set_cookie('auth-token', '', expires=0)
            return resp






@app.teardown_request
def teardown_request_func(error=None):
    ScopedSession.commit()
    ScopedSession.remove()
    print("teardown_request is running!")
    if error:
        # Log the error
        print(str(error))


@logger.catch
def main():
    db_init()
    app.register_blueprint(tables)
    app.register_blueprint(game)
    app.register_blueprint(pages)
    app.register_blueprint(triggers)
    app.register_blueprint(misc)
    app.run(host='0.0.0.0')



if __name__ == '__main__':
    main()

