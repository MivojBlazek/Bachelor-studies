-- 2. část - SQL skript pro vytvoření objektů schématu databáze
--
-- Zadání - Evidence přestupků
--
-- @author Michal Blažek <xblaze38>
-- @author Tomáš Řezníček <xrezni33>


-- V databázi jsme pro převod reprezentace generalizace/specializace využili metodu tabulky pro nadtyp + pro podtypy s primárním klíčem nadtypu.
-- Tato metoda byla zvolena, abychom nemuseli kopírovat atributy jedné tabulky do dalších a tím ušetříme nějaké místo v paměti.

DROP TABLE Spachal CASCADE CONSTRAINTS;
DROP TABLE Prestupek CASCADE CONSTRAINTS;
DROP TABLE Ridic_ridi_motorove_vozidlo CASCADE CONSTRAINTS;
DROP TABLE Ridic CASCADE CONSTRAINTS;
DROP TABLE Osoba CASCADE CONSTRAINTS;
DROP TABLE Firma CASCADE CONSTRAINTS;
DROP TABLE Kradene CASCADE CONSTRAINTS;
DROP TABLE Motorove_vozidlo CASCADE CONSTRAINTS;
DROP TABLE Vlastnik CASCADE CONSTRAINTS;

CREATE TABLE Prestupek
(
    ID_prestupku            INT GENERATED AS IDENTITY PRIMARY KEY,
    Nazev                   VARCHAR(255) NOT NULL,
    Maximalni_vyse_pokuty   NUMBER(9,2) NOT NULL,
    Minimalni_vyse_pokuty   NUMBER(9,2),
    Popis                   VARCHAR(255) NOT NULL,
    CHECK (Minimalni_vyse_pokuty IS NULL OR Minimalni_vyse_pokuty <= Maximalni_vyse_pokuty)
);

CREATE TABLE Vlastnik
(
    ID_vlastnika    INT GENERATED AS IDENTITY PRIMARY KEY
);

CREATE TABLE Osoba
(
    ID_vlastnika    INT,
    Rodne_cislo     VARCHAR(11) PRIMARY KEY,
    Prijmeni        VARCHAR(255) NOT NULL,
    Jmeno           VARCHAR(255) NOT NULL,
    Trvaly_pobyt    VARCHAR(255) NOT NULL,
    Telefonni_cislo NUMBER(9),
    CHECK (LENGTH(TO_CHAR(Telefonni_cislo)) = 9 OR Telefonni_cislo IS NULL),
    CHECK (REGEXP_LIKE(Rodne_cislo, '^\d{2}[0156]\d[0-3]\d/\d{3,4}$')),
    FOREIGN KEY (ID_vlastnika) REFERENCES Vlastnik(ID_vlastnika) ON DELETE SET NULL
);

CREATE TABLE Firma
(
    ID_vlastnika    INT,
    ICO             NUMBER(8) PRIMARY KEY,
    Nazev           VARCHAR(255) NOT NULL,
    Sidlo           VARCHAR(255) NOT NULL,
    Telefonni_cislo NUMBER(9),
    CHECK (LENGTH(TO_CHAR(Telefonni_cislo)) = 9 OR Telefonni_cislo IS NULL),
    CHECK (LENGTH(ICO) = 8 AND SUBSTR(ICO, 8, 1) =
        (MOD((11 - MOD(
            8 * TO_NUMBER(SUBSTR(ICO, 1, 1)) + 7 * TO_NUMBER(SUBSTR(ICO, 2, 1)) + 6 * TO_NUMBER(SUBSTR(ICO, 3, 1)) + 5 * TO_NUMBER(SUBSTR(ICO, 4, 1)) +
            4 * TO_NUMBER(SUBSTR(ICO, 5, 1)) + 3 * TO_NUMBER(SUBSTR(ICO, 6, 1)) + 2 * TO_NUMBER(SUBSTR(ICO, 7, 1)), 11)
        ), 10))
    ),
    FOREIGN KEY (ID_vlastnika) REFERENCES Vlastnik(ID_vlastnika) ON DELETE CASCADE
);

CREATE TABLE Ridic
(
    Rodne_cislo         VARCHAR(11) PRIMARY KEY,
    Ridicske_opravneni  VARCHAR(255),
    FOREIGN KEY (Rodne_cislo) REFERENCES Osoba(Rodne_cislo) ON DELETE CASCADE
);

CREATE TABLE Spachal
(
    Rodne_cislo_ridice  VARCHAR(11),
    ID_prestupku        INT,
    Datum_spachani      DATE NOT NULL,
    Popis               VARCHAR(255) NOT NULL,
    Vyse_pokuty         NUMBER(9,2) NOT NULL,
    CHECK (Vyse_pokuty > 0),
    PRIMARY KEY (Rodne_cislo_ridice, ID_prestupku),
    FOREIGN KEY (Rodne_cislo_ridice) REFERENCES Ridic(Rodne_cislo) ON DELETE CASCADE,
    FOREIGN KEY (ID_prestupku) REFERENCES Prestupek(ID_prestupku) ON DELETE CASCADE
);

CREATE TABLE Motorove_vozidlo
(
    VIN                         VARCHAR(50) PRIMARY KEY NOT NULL,
    Znacka                      VARCHAR(255) NOT NULL,
    SPZ                         VARCHAR(8),
    Rok_vyroby                  NUMBER(4) NOT NULL,
    Ridicska_skupina            VARCHAR(2) NOT NULL,
    ID_vlastnika                INT,
    CHECK (Rok_vyroby > 1886),
    CHECK (Ridicska_skupina IN ('AM', 'A1', 'A2', 'A', 'B', 'B1', 'C', 'C1', 'D', 'D1', 'T')),
    CHECK (REGEXP_LIKE(VIN, '^[A-HJ-NPR-Z0-9]+$')),
    CHECK (LENGTH(SPZ) <= 8),
    CHECK (REGEXP_LIKE(SPZ, '^[A-Za-z0-9]+$')),
    CHECK (REGEXP_LIKE(SPZ, '[0-9]') AND REGEXP_LIKE(SPZ, '[A-Za-z]')),
    FOREIGN KEY (ID_vlastnika)  REFERENCES Vlastnik(ID_vlastnika) ON DELETE CASCADE
);

CREATE TABLE Ridic_ridi_motorove_vozidlo
(
    Rodne_cislo VARCHAR(11),
    VIN VARCHAR(50),
    PRIMARY KEY (Rodne_cislo, VIN),
    FOREIGN KEY (Rodne_cislo) REFERENCES Ridic(Rodne_cislo) ON DELETE CASCADE,
    FOREIGN KEY (VIN) REFERENCES Motorove_vozidlo(VIN) ON DELETE CASCADE
);

CREATE TABLE Kradene
(
    VIN                 VARCHAR(50) PRIMARY KEY,
    Datum_kradeze       DATE NOT NULL,
    Misto_kradeze       VARCHAR(255),
    CHECK (Datum_kradeze > DATE '1886-01-29'),
    FOREIGN KEY (VIN) REFERENCES Motorove_vozidlo(VIN) ON DELETE CASCADE
);

INSERT INTO Vlastnik (ID_vlastnika)
VALUES (DEFAULT);
INSERT INTO Vlastnik (ID_vlastnika)
VALUES (DEFAULT);
INSERT INTO Vlastnik (ID_vlastnika)
VALUES (DEFAULT);

INSERT INTO Firma (ID_vlastnika, ICO, Nazev, Sidlo, Telefonni_cislo) VALUES
(1, 25596641, 'Redhat', 'Brno', NULL);
INSERT INTO Firma (ID_vlastnika, ICO, Nazev, Sidlo, Telefonni_cislo) VALUES
(3, 15596648, 'FirmaVPraze', 'Praha', 123456789);

INSERT INTO Osoba (ID_vlastnika, Rodne_cislo, Prijmeni, Jmeno, Trvaly_pobyt, Telefonni_cislo)
VALUES (2, '120430/0789', 'Novak', 'Petr', 'Praha', 111222333);
INSERT INTO Osoba (              Rodne_cislo, Prijmeni, Jmeno, Trvaly_pobyt, Telefonni_cislo)
VALUES (   '915624/0321', 'Svobodova', 'Anna', 'Brno', NULL);

INSERT INTO Ridic (Rodne_cislo, Ridicske_opravneni) VALUES
('120430/0789', 'B');
INSERT INTO Ridic (Rodne_cislo, Ridicske_opravneni) VALUES
('915624/0321', 'A2');

INSERT INTO Motorove_vozidlo (VIN, Znacka, SPZ, Rok_vyroby, Ridicska_skupina, ID_vlastnika) VALUES
('ABC123', 'Kawasaki', 'ABC1234', 2020, 'A2', 3);
INSERT INTO Motorove_vozidlo (VIN, Znacka, SPZ, Rok_vyroby, Ridicska_skupina,  ID_vlastnika) VALUES
('XYZ789', 'Volkswagen', 'XYZ9876', 2005, 'B', 2);

INSERT INTO Ridic_ridi_motorove_vozidlo (Rodne_cislo, VIN) VALUES
('120430/0789', 'ABC123');
INSERT INTO Ridic_ridi_motorove_vozidlo (Rodne_cislo, VIN) VALUES
('915624/0321', 'XYZ789');

INSERT INTO Prestupek (Nazev, Maximalni_vyse_pokuty, Minimalni_vyse_pokuty, Popis) VALUES
('Překročení rychlosti', 100000.00, 5000.00, 'Ve městě minimálně o 5 km/h. Mimo město minimálně o 10 km/h.');
INSERT INTO Prestupek (Nazev, Maximalni_vyse_pokuty, Minimalni_vyse_pokuty, Popis) VALUES
('Zákaz parkování', 5000.00, NULL, 'Zóna A a B není pro návštěvníky a zóna C je pouze od 6:00 do 17:00.');

INSERT INTO Spachal (Rodne_cislo_ridice, ID_prestupku, Datum_spachani, Popis, Vyse_pokuty) VALUES
('915624/0321', 1, TO_DATE('2024-03-22', 'YYYY-MM-DD'), 'Překročení rychlosti', 6969.69);

INSERT INTO Kradene (VIN, Datum_kradeze, Misto_kradeze) VALUES
('ABC123', TO_DATE('2024-03-23', 'YYYY-MM-DD'), 'Ostrava');

COMMIT;
