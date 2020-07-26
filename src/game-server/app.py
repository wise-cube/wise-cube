
from flask_socketio import SocketIO
from flask_qrcode import QRcode
from flask_login import LoginManager
from flask_sqlalchemy_session import current_session

from os.path import exists
from views import game, tables, triggers, pages
from models import *
from database_handler import DatabaseHandler

from loguru import logger
from celery import Celery

from conf import *



socketio = SocketIO(FlaskApp)
qrcode = QRcode(FlaskApp)
login_manager = LoginManager()


# FlaskApp.config['CELERY_BROKER_URL'] = 'redis://localhost:6379/0'
# FlaskApp.config['CELERY_RESULT_BACKEND'] = 'redis://localhost:6379/0'
#
# celery = Celery(FlaskApp.name, broker=FlaskApp.config['CELERY_BROKER_URL'])
# celery.conf.update(FlaskApp.config)

@login_manager.user_loader
def user_loader(id):
    return current_session.query(Player).filter(Player.id == id).first()

@login_manager.request_loader
def request_loader(request):
    t = request.cookies.get('auth_token')
    return  current_session.query(Player).filter(Player.auth_token == t).first()

@logger.catch
def main():
    if not exists('wise-cube.db'):
        DatabaseHandler.migration()
        DatabaseHandler.seed()

    login_manager.init_app(FlaskApp)

    FlaskApp.register_blueprint(game)
    FlaskApp.register_blueprint(pages)
    FlaskApp.register_blueprint(tables)
    FlaskApp.register_blueprint(triggers)
    FlaskApp.run()


main()

