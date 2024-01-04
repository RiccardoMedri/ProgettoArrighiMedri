const express = require('express');
const macAddress = require('../controller/controller-MacAddress');

const router = express.Router();

router.get('/macAddress/nuovoMacAddress/:macAddress', macAddress.addMacAddress);
router.get('/macAddress/deleteMacAddress/:mac-:macAddress', macAddress.removeMacAddress);

module.exports=router;