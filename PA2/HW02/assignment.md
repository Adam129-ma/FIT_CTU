# VEĽKÉ ČÍSLA

Úlohou je realizovať triedu `CBigInt`, ktorá bude reprezentovať celé čísla (kladné aj záporné) s takmer neobmedzeným rozsahom.

Celé čísla typu `int`, `long long int`, atď. majú fixnú veľkosť a teda obmedzený rozsah. Ak potrebujeme pracovať s číslami s väčším rozsahom hodnôt, musíme vytvoriť vlastný dátový typ. Trieda implementujúca tento dátový typ bude ukladať číslo vo vnútornej reprezentácii, ktorú podľa potreby rozšíri (alokuje väčší priestor).

## Obmedzenia implementácie

- Trieda `CBigInt` ukladá iba celé čísla (kladné, nulu aj záporné).
- Z matematických operácií implementujeme len **sčítanie**, **násobenie** a **porovnávanie**.

## Rozhranie triedy `CBigInt`

### Konstruktory

1. **Konstruktor implicitný**: Inicializuje objekt s hodnotou `0`.
2. **Konstruktor s parametrom celého čísla `int`**: Inicializuje objekt reprezentujúci dané číslo.
3. **Konstruktor s parametrom reťazca (ASCIIZ)**: Inicializuje objekt s hodnotou v desiatkovej reprezentácii uvedenej v reťazci. Ak je reťazec neplatný (neobsahuje platné desiatkové číslo), konstruktor vyhodí výnimku `std::invalid_argument`.

### Ďalšie základné funkcie

4. **Kopírovací konstruktor**: Bude implementovaný podľa potreby vnútorných štruktúr triedy.
5. **Destruktor**: Bude implementovaný podľa potreby vnútorných štruktúr triedy.

### Operátory

6. **Priradenie (`=`)**: Umožňuje priradenie z celého čísla, reťazca a inej inštancie `CBigInt`.
7. **Operátor `<<`**: Umožňuje výstup objektu do C++ streamu v desiatkovej reprezentácii bez úvodných núl. V bonusových testoch musí operátor spolupracovať so streamom a podľa použitého manipulátora zobraziť výsledok buď desiatkovo alebo hexadecimálne (používa malé písmená a nezobrazuje úvodné nuly).
8. **Operátor `>>`**: Umožňuje načítanie zo vstupného streamu v desiatkovom zápise.
9. **Operátor `+`**: Umožňuje sčítanie v kombináciách:
   - `CBigInt + CBigInt`
   - `CBigInt + int`
   - `CBigInt + ASCIIZ reťazec`
   - `int + CBigInt`
   - `ASCIIZ reťazec + CBigInt`
10. **Operátor `+=`**: Umožňuje pripočítať k číslu `CBigInt` iné číslo `CBigInt`, celé číslo alebo ASCIIZ reťazec.
11. **Operátor `*`**: Umožňuje násobenie v rovnakých kombináciách ako operátor pre sčítanie.
12. **Operátor `*=`**: Umožňuje prenasobiť číslo `CBigInt` iným číslom `CBigInt`, celým číslom alebo ASCIIZ reťazcom.
13. **Relačné operátory** (`<`, `<=`, `>`, `>=`, `==`, `!=`): Umožňujú porovnávať veľké čísla medzi sebou vo všetkých kombináciách ako sčítanie a násobenie.
