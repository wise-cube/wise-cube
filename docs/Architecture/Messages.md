### Messages

Messages are sent over mqtt

A message is a Json string with one mandatory value ( type )  identifying the message type, and other arguments that represent additional information that should be supplied.

Messages can be of 3 types:

- Request
- Response
- Event

They are identified by the suffixes `_req, _resp and _event`, they all have the same structure and use the same protocols, the only exception is that `_req` messages expect a `_resp` message to be published by the CubeHandler on the same topic of the response whereas `_event` messages expect no response



The case of CubeHandler querying the Cube ( sending a request to the cube ) and receiving a response is not covered yet

### Message Types

NEW_GROUP_REQ

```
{
'type': new_group_req
}
```

NEW_GROUP_RESP

```
{
'type': new_group_resp
'group_id': int
'group_token': str(64)
'status': 200 | 404 | 500
}
```

RESUME_GROUP_REQ

```
{
'type': RESUME_GROUP_REQ
'group_token': str(64)
}
```

RESUME_GROUP_RESP

```
{
'type':new_group_resp
'group_id': int
'status': 200 | 404 | 500
}
```

NEW_ANSWER_EVENT

```
{
'type': new_group_event
'answer_id': int
}
```

NEW_GAME_EVENT

```
{
'type': new_game_event
'game_id': int
}
```





​	