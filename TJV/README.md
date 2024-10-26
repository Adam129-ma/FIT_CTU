# BI TJV

## Dealership Management Application

### Business Operations
Každý dealer musí mať originálne osobné číslo. Pri pridávaní nového dealera sa vždy posiela požiadavok API na získanie všetkých obsadených čísel. Ak je dané číslo voľné, pošle sa požiadavok API na jeho uloženie do databázy. Ak nie, klient sa nepridá a užívateľovi sa vráti spätná väzba.

### Komplexný Dotaz
Vyberte všetkých dealerov, ktorí predali/nepredali dané auto.

![image](/uploads/c45455e151ce6a0fb60799f5733b4c52/image.png)

## Spúšťanie

Aplikácia sa skladá z dvoch častí:
- Server: [link na server](https://gitlab.fit.cvut.cz/mackaada/BI-TJV/-/tree/master/cardatabase)
- Klient: [link na klienta](https://gitlab.fit.cvut.cz/mackaada/BI-TJV/-/tree/master/client-frontend)

Pre spustenie sú potrebné nasledujúce nástroje:
- Docker
- Gradle



