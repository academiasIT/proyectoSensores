BEGIN;
--
-- Create model sensorMuestreo
--
CREATE TABLE "appSensores_sensormuestreo" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "fechaMuestreo" date NOT NULL, "origenMuestreo" text NOT NULL);
--
-- Create model sensorpm25
--
CREATE TABLE "appSensores_sensorpm25" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "pm25" real NOT NULL, "idMuestreo_id" integer NOT NULL REFERENCES "appSensores_sensormuestreo" ("id"));
--
-- Create model sensorpm10
--
CREATE TABLE "appSensores_sensorpm10" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "pm10" real NOT NULL, "idMuestreo_id" integer NOT NULL REFERENCES "appSensores_sensormuestreo" ("id"));
--
-- Create model sensorTemperatura
--
CREATE TABLE "appSensores_sensortemperatura" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "temperatura" real NOT NULL, "idMuestreo_id" integer NOT NULL REFERENCES "appSensores_sensormuestreo" ("id"));
--
-- Create model sensorhumedad
--
CREATE TABLE "appSensores_sensorhumedad" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "humedad" real NOT NULL, "idMuestreo_id" integer NOT NULL REFERENCES "appSensores_sensormuestreo" ("id"));
--
-- Create model sensorco
--
CREATE TABLE "appSensores_sensorco" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "co" real NOT NULL, "idMuestreo_id" integer NOT NULL REFERENCES "appSensores_sensormuestreo" ("id"));
--
-- Create model sensoro3
--
CREATE TABLE "appSensores_sensoro3" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "o3" real NOT NULL, "idMuestreo_id" integer NOT NULL REFERENCES "appSensores_sensormuestreo" ("id"));
END;

