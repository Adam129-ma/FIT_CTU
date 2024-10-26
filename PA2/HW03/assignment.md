# DATABÁZE STUDENTŮ

Úlohou je realizovať triedy, ktoré implementujú správu študentov na študijnom oddelení.

Na študijnom oddelení sa vedie agenda študentov, kde je každý študent reprezentovaný svojím menom (priezvisko a krstné meno, prípadne viac mien oddelených aspoň jednou medzerou), dátumom narodenia a rokom nástupu na fakultu. Implementácia umožňuje pridávať, odoberať a vyhľadávať študentov v databáze.

## Požiadavky na funkčnosť

- **Unikátnosť**: Trojica údajov (meno + dátum narodenia + rok nástupu) je jedinečná.
- **Filtrovanie a radenie**: Možnosť filtrovať študentov podľa mena, dátumu narodenia a roku nástupu a radiť podľa zadaných kritérií.
- **Našeptávač**: Pre zadanú časť mena vyhľadá všetkých študentov zodpovedajúcich zadanému vzoru.

## Rozhranie implementácie

### Trieda `CDate`

Táto trieda reprezentuje dátum a bude slúžiť pre ukladanie dátumu narodenia a filtrov.

- **Konstruktor `(y, m, d)`**: Inicializuje objekt zadaným rokom, mesiacom a dňom.
- **Operátor `<=>`**: Porovnáva inštanciu `this` s inštanciou `date`, vráti jednu z hodnôt `std::strong_ordering`.
- **Operátor `<<`**: Umožňuje výstup dátumu do streamu (používa sa hlavne na testovanie a ladenie).

> Poznámka: Táto trieda je implementovaná v testovacom prostredí a nie je potrebné ju meniť.

### Výčtový typ `ESortKey`

Identifikuje kritérium radenia, s hodnotami:

- **NAME**: Radenie podľa mena študenta.
- **BIRTH_DATE**: Radenie podľa dátumu narodenia.
- **ENROLL_YEAR**: Radenie podľa roku nástupu na fakultu.

> Poznámka: Tento typ je deklarovaný v testovacom prostredí.

### Trieda `CStudent`

Reprezentuje jedného študenta.

- **Konstruktor `(name, dateOfBirth, enrollYear)`**: Inicializuje inštanciu s danými údajmi.
- **Operátor `==`**: Porovná dve inštancie `CStudent` podľa mena, dátumu narodenia a roku nástupu (citlivý na veľkosť písmen a poradie mien).
- **Operátor `!=`**: Porovná dve inštancie `CStudent`, či sa líšia (negácia operátora `==`).

### Trieda `CFilter`

Reprezentuje kritériá pre filtrovanie študentov.

- **Konstruktor**: Inicializuje prázdny filter.
- **bornBefore(date)**: Pridá kritérium pre narodenie pred zadaným dátumom.
- **bornAfter(date)**: Pridá kritérium pre narodenie po zadanom dátume.
- **enrolledBefore(year)**: Pridá kritérium pre zápis pred zadaným rokom.
- **enrolledAfter(year)**: Pridá kritérium pre zápis po zadanom roku.
- **name(pattern)**: Pridá kritérium na meno. Porovnanie je case-insensitive a nezávislé na poradí slov.

### Trieda `CSort`

Reprezentuje kritériá pre radenie študentov vo výsledku vyhľadávania.

- **Konstruktor**: Inicializuje prázdny zoznam kritérií radenia.
- **addKey(key, ascending)**: Pridá kritérium radenia podľa výčtového typu `ESortKey` s hodnotou `true` pre vzostupné alebo `false` pre zostupné radenie.

### Trieda `CStudyDept`

Reprezentuje databázu študentov s nasledujúcim rozhraním:

- **Konstruktor**: Inicializuje prázdnu databázu.
- **addStudent(student)**: Pridá študenta do databázy, vracia `true` pre úspech a `false` pre duplicitu.
- **delStudent(student)**: Odstráni študenta z databázy, vracia `true` pre úspech a `false` pre neexistujúceho študenta.
- **search(filter, sortOpt)**: Vyhľadá študentov podľa kritérií filtra `filter` a vráti ich zoradených podľa `sortOpt`.
- **suggest(name)**: Našeptávač, ktorý pre zadaný reťazec `name` vráti všetkých registrovaných študentov, ktorí zodpovedajú časti mena.

> Poznámka: Implementácia môže obsahovať ďalšie metódy a členské premenné podľa potreby.
