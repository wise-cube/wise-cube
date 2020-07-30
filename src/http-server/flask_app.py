from common import *
from views import *
from common import SESSION as Session

from flask import Flask
from os.path import exists



def db_init():
    if not exists('wise-cube.db'):
        DatabaseHandler.migration()
        DatabaseHandler.seed()

App = Flask(__name__, static_folder='static', static_url_path='/')
App.secret_key = SECRET_KEY


ScopedSession = flask_scoped_session(Session, App)


@logger.catch
def main():


    login_manager.init_app(App)

    App.register_blueprint(game)
    App.register_blueprint(pages)
    App.register_blueprint(tables)
    App.register_blueprint(triggers)
    App.run()


main()

