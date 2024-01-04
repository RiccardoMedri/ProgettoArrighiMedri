CREATE SCHEMA IF NOT EXISTS `tuoDB` DEFAULT CHARACTER SET utf8 ;
USE `tuoDB` ;

CREATE TABLE IF NOT EXISTS `tuoDB`.`tessere` (
  `idtessera` VARCHAR(100) NOT NULL,
  `nome` VARCHAR(100) NOT NULL,
  `cognome` VARCHAR(512) NOT NULL,
  `ruolo` VARCHAR(45) NOT NULL,
  `limite` INT,
  PRIMARY KEY (`idtessera`))
ENGINE = InnoDB;

CREATE TABLE IF NOT EXISTS `tuoDB`.`ingressi` (
  `idtessera` VARCHAR(100) NOT NULL,
  `data` VARCHAR(100) NOT NULL,
  `orario` VARCHAR(512) NOT NULL,
  PRIMARY KEY (`idtessera`, `data`, `orario`))
ENGINE = InnoDB;

CREATE TABLE IF NOT EXISTS `tuoDB`.`IndirizziMAC` (
  `mac` VARCHAR(100) NOT NULL,
  PRIMARY KEY (`mac`))
ENGINE = InnoDB;