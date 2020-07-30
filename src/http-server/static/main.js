function getCookie(c_name)
        {
            var i,x,y,ARRcookies=document.cookie.split(";");

            for (i=0;i<ARRcookies.length;i++)
            {
                x=ARRcookies[i].substr(0,ARRcookies[i].indexOf("="));
                y=ARRcookies[i].substr(ARRcookies[i].indexOf("=")+1);
                x=x.replace(/^\s+|\s+$/g,"");
                if (x==c_name)
                {
                    return unescape(y);
                }
             }
        }

        //Sample usage
        var auth_token = getCookie('auth_token');
        var socket = io.connect('/cube/{{ player.cube_id }}');
        var general_socket = io.connect()
        var cube_socket = io.connect('/cube/{{ player.cube_id }}')

        general_socket.on('connect', function() {
             socket.emit('connection', {data: ["{{ player.cube_id }}",'I\'m connected!']});
        });

        general_socket.on('reload', function() {
             document.location.reload()
        });



