$(document).ready(function() {
    $(".led").click(function() {
        console.log("clicked");
        var p = $(this).attr('id'); // get id value (i.e. pin13, pin12, or pin11)
        // send HTTP GET request to the IP address with the parameter "pin" and value "p", then execute the function
        // console.log($.get("http://192.168.4.1/:80/?pin=13"));
        // $.get("http://10.0.0.22:80/pin=13").then(function(data){console.log("request made", data)}).catch(function(err){
        //   console.log(err, "this was error request");
        // }); // execute get request

        $.ajax({
            url: 'http://10.0.0.22:80',
            dataType: 'TEXT',
            crossDomain : true,
            Accept: "/",
            Origin: "http://10.0.0.22",
            success: function(data) {
                console.log('success');
                console.log(data);
            },
            error: function() {
                console.log('something went wrong :-(');
            }
        })
        //
        //     Method : "GET", url : "http://10.0.0.22:80/pin=13", crossDomain : true
        // }).done(function(data) {
        //     console.log("request made with ajax", data)
        // }).catch(function(err) {
        //     console.log(err, "this was error request from ajax");
        // }); // execute get request
    });
});
