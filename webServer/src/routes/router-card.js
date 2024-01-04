const express = require('express');
const handleCard = require('../controller/controller-card');

const router = express.Router();

router.get('/tessera/:id-:mac', handleCard.checkCard);
router.get('/tessera/deleteCard/:id-:mac', handleCard.deleteCard);
router.get("/tessera/nuovaTessera/:id-:nome-:cognome-:ruolo-:mac", handleCard.addNewCard);
router.get('/tessera/limitaAccessi/:idtessera-:limit-:mac', handleCard.limitAcces);
router.get("/timbrature/:nome-:cognome-:mac", handleCard.getData);

module.exports=router; 