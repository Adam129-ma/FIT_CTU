# Odvod

## Zadanie

Vytvoriť server a klienta, ktorí komunikujú prostredníctvom socketov. Server spracováva prichádzajúce správy od klienta, spravuje stav robota a reaguje na rôzne príkazy, ako je pohyb a otáčanie.

## Riešenie

### 1. Server (`server.py`)

- **Inicializácia:** Server vytvorí socket, ktorý počúva na predpísanej adrese a porte.
- **Prijímanie pripojení:** Keď sa klient pripojí, server vytvorí nový thread pre spracovanie tohto spojenia.
- **Robot:** Každý robot má svoj vlastný stav, vrátane pozície a smeru. Server spracováva príkazy na pohyb a otáčanie robota.
- **Autorizácia:** Server overuje používateľské meno a kľúč od klienta. V prípade úspechu odpovedá server s potvrdením.
- **Spracovanie príkazov:** Server spracováva príkazy od klienta, ako sú pohyb, otáčanie, a potvrdenie úspešného vykonania príkazu.

### 2. Klient (`client.py`)

- **Pripojenie na server:** Klient sa pripojí k serveru a odosiela používateľské meno a ID kľúča.
- **Odosielanie správ:** Klient odosiela príkazy serveru a čaká na odpoveď.
- **Spracovanie odpovedí:** Klient spracováva správy od servera, pričom kontroluje, či dostal potvrdenie alebo chybu.

## Implementácia

- **Server:**

  - Vytvorenie triedy `Server`, ktorá spravuje socket a vlákna pre klientské pripojenia.
  - Metódy na spracovanie príkazov ako `handle_connection`, `authorize`, `send_move` a ďalšie.
  - Udržiavanie stavu robota prostredníctvom triedy `Robot`.

- **Klient:**
  - Funkcie na pripojenie k serveru, odosielanie používateľských údajov a spracovanie odpovedí.
  - Rôzne správy sa odosielajú a prijímajú vo formáte, ktorý server očakáva.

## Záver

Tento projekt demonštruje základnú komunikáciu medzi klientom a serverom pomocou socketov v Pythone. Riešenie je modulárne a umožňuje jednoduché rozšírenie o ďalšie funkcie, ako napríklad spracovanie rôznych príkazov alebo rozšírenie logiky robota.
