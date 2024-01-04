const mysql = require('mysql2');

const connection = mysql.createConnection({
    host: 'localhost',
    user: 'pythonUser',
    password: 'pythonPWD',
    database: 'tuoDB'
  });
  
  module.exports = connection;