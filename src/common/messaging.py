import json

class Message:
    types = [
	        'new_cube_req', # no args
            'new_cube_resp'  # no args
	        'resume_cube_req', # { cube_id }
			'new_game_event',
			'new_answer_event',
			'click_ok_event',
            'click_ko_event',
			'shake_event',
			'throw_event',
			'result_event']

    def __init__(self, type, **kwargs ):
        self.type = type
        self.args = kwargs
        self.req = False
        self.resp = False

        if self.type not in self.types:
            raise ValueError('Invalid type')

        if "_req" in self.type:
            self.req = True

        if "_resp" in self.type:
            self.resp = True

    @staticmethod
    def parse(msg):
        args = json.loads(msg)
        return Message(args['type'], **args )

    def serialize(self):
        args = {'type':self.type, **self.args }
        return json.dumps(args).encode('utf8')

    def __str__(self):
        return str(self.type, json.dumps(self.args))


class Response:
    def __init__(self, ok, **kwargs):
        self.ok = ok
        self.args = kwargs

    @staticmethod
    def parse(msg):
        args = json.loads(msg)
        ok = args.pop('ok')
        return Message(ok, **args)

    def serialize(self):
        args = {'ok' :self.ok, **self.args}
        return json.dumps(args).encode('utf8')

    def __str__(self):
        return str(b'Ok: ? ', self.ok, json.dumps(self.args))