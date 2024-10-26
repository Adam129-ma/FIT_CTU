# KATASTER NEHNUTEĽNOSTÍ

Úlohou je realizovať triedu `CLandRegister`, ktorá bude implementovať databázu katastra nehnuteľností. Tento **zjednodušený kataster** si pamätá zoznam pozemkov a k nim priradených vlastníkov. Každý **pozemok** obsahuje nasledujúce údaje:

- `region` (katastrálne územie, reťazec),
- `id` (číslo pozemku v katastrálnom území, nezáporné číslo),
- `city` (mesto, reťazec),
- `addr` (adresa, reťazec).

Na jednoznačnú identifikáciu pozemku je potrebná buď dvojica údajov `(region, id)`, alebo `(city, addr)`.

**Vlastník** pozemku je identifikovaný reťazcom `owner`. Jeden vlastník môže vlastniť viacero pozemkov, pričom každý pozemok musí mať práve jedného vlastníka. Nové pozemky sú pôvodne vlastnené štátom (vlastník je prázdny reťazec).

## Rozhranie triedy `CLandRegister`:

1. **Konstruktor bez parametrov**: Inicializuje inštanciu triedy ako prázdnu (neobsahuje žiadne záznamy).
2. **Destruktor**: Uvoľňuje prostriedky, ktoré inštancia alokovala.

### Metódy:

- **add(city, addr, region, id)**: Pridá nový pozemok do databázy. Ak už existuje záznam s rovnakou dvojicou `(city, addr)` alebo `(region, id)`, metóda vráti `false`. Inak vráti `true`.
- **del(city, addr)** / **del(region, id)**: Odstráni záznam pozemku z databázy na základe jednoznačnej identifikácie. Ak bol záznam odstránený, metóda vráti `true`, inak `false`.
- **getOwner(city, addr, owner)** / **getOwner(region, id, owner)**: Zistí vlastníka pozemku. Ak pozemok existuje, metóda vráti `true` a nastaví `owner` na meno vlastníka, inak vráti `false`.
- **newOwner(city, addr, owner)** / **newOwner(region, id, owner)**: Nastaví pre pozemok nového vlastníka. Ak pozemok neexistuje alebo má už zadaného vlastníka, metóda vráti `false`.
- **count(owner)**: Vráti počet pozemkov vlastnených zadaným vlastníkom (`owner`).
- **listByAddr()**: Vráti iterátor, ktorým je možné prechádzať všetky pozemky zoradené podľa mesta (`city`) a adresy (`addr`).
- **listByOwner(owner)**: Vráti iterátor pre pozemky vlastnené zadaným vlastníkom (`owner`) zoradené podľa poradia, v akom ich získal.

## Trieda `CIterator`:

Slúži na prechádzanie zoznamu pozemkov.

- **atEnd**: Zisťuje, či iterátor dosiahol koniec zoznamu.
- **next**: Posunie iterátor na ďalší pozemok.
- **city** / **addr** / **region** / **id** / **owner**: Získavajú príslušné informácie o aktuálne vybranom pozemku.
