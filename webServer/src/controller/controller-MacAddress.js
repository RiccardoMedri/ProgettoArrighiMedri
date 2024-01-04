const connection = require("../utils/db");

module.exports.addMacAddress = async function(req, res) {
    connection.execute( 
        `INSERT INTO IndirizziMAC (mac) VALUES (?)`,
        [req.params.macAddress],
        function(err, results, fields) {
            res.setHeader('Content-Type', 'text/plain');
            if(!err) {
                res.send("Indirizzo MAC aggiunto!");
            }
            else {
                res.send(err);
            }
        }
    )
}

module.exports.removeMacAddress = async function(req, res) {
    connection.execute(
        'SELECT mac from IndirizziMAC WHERE mac=?',                                  //controlla che il mac address ci sia
        [req.params.mac],
        function(err, results, fields) {
            
            if (results.length > 0 && results[0]['mac'] == req.params.mac) {
                connection.execute(
                    `DELETE FROM IndirizziMAC WHERE mac=?`,
                    [req.params.macAddress],
                    function(err, results, fields) {

                        res.setHeader('Content-Type', 'text/plain');
                        if(!err) {
                            res.send("Indirizzo MAC rimosso!");
                        }
                        else {
                            res.send(err);
                        }
                    }
                )
            }    
            else{                                                                    //se non c'è macAddress allora errore
                res.status(404).send('MAC address non riconosciuto')
            } 
        }
    )
}