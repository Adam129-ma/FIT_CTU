# Úkol: FIT EXCEL

Úkolem je realizovat třídu (sadu tříd), která bude fungovat jako tabulkový procesor. Tabulkový procesor bude umožňovat operace s buňkami (nastavení, výpočet hodnoty, kopírování), bude umět vypočítávat hodnotu buňky podle vzorce, dokáže detekovat cyklické závislosti mezi buňkami a dokáže obsah tabulky uložit a načíst. Hodnocení se kromě funkčnosti zaměří na vhodný návrh tříd, použití polymorfismu a vhodné využití verzovacího systému.

## Funkcionalita

Funkcionalita je rozdělena na několik částí, hodnocení bude záviset na tom, kolik požadavků dokáže vaše implementace pokrýt. Dále, Vaše implementace nebude muset realizovat vlastní syntaktickou analýzu výrazů, které se v buňkách budou objevovat. Syntaktický analyzátor bude dostupný v testovacím prostředí a je přiložen v dodaném archivu v podobě staticky linkované knihovny.

## Rozhraní tříd

### CSpreadsheet

Vlastní tabulkový procesor, který implementujete. Požadované veřejné rozhraní je popsáno níže, jeho implementace je na Vás.

### CPos

Identifikátor buňky v tabulce. Buňky v tabulce identifikujeme standardním způsobem: neprázdná sekvence písmen A až Z udává sloupec a následuje nezáporné číslo řádky (0 považujeme za platné číslo řádky). Písmena v názvu sloupce mohou být malá i velká (case insensitive). Sloupce jsou značené postupně A, B, C, ..., Z, AA, AB, AC, ..., AZ, BA, BB, ..., ZZZ, AAAA, ... Příklady identifikátorů buněk: A7, PROG7250, ... Pokud si nejste jisti značením, podívejte se na skutečný tabulkový procesor. Takto zadaný identifikátor buňky může být nešikovný pro implementaci, proto je zapouzdřen do třídy CPos, která si jej může převést do vhodnější reprezentace. Implementace převodu a třídy CPos je na Vás.

### CExpressionBuilder

Abstraktní třída, kterou používá dodaný analyzátor výrazů. Pokud se rozhodnete používat dodaný analyzátor, budete muset implementovat podtřídu a její instanci předat analyzátoru.

- `parseExpression()` - funkce v testovacím prostředí, která provede syntaktickou analýzu zadaného výrazu a identifikované části tohoto výrazu předá ke zpracování Vaší podtřídě CExpressionBuilder.

### CValue

Reprezentuje hodnotu buňky. Hodnota buňky je buď nedefinovaná, desetinné číslo, nebo řetězec. Třída CValue je pojmenovaná specializace generické třídy `std::variant`.

### Další

Pro implementaci si zřejmě budete muset vytvořit mnoho svých vlastních tříd a funkcí.

## Výrazy v buňkách

Implementace požaduje, aby buňky tabulky mohly být prázdné (nedefinované), mohly obsahovat číslo, řetězec, nebo vzorec pro výpočet hodnoty. Vzorce mají syntaxi podobnou standardnímu tabulkovému procesoru, jsou ale omezené počtem implementovaných funkcí. Výraz může obsahovat:

- **Číselný literál** - celé nebo desetinné číslo, volitelně s exponentem (např. 15, 2.54, 1e+8, 1.23e-10).
- **Řetězcový literál** - posloupnost znaků zapsaná v uvozovkách. Pokud má řetězec obsahovat uvozovku, je tato uvozovka zdvojena. Řetězcový literál může obsahovat libovolné znaky včetně znaku pro odřádkování.
- **Odkaz na buňku** - odkaz realizován ve standardní notaci (sloupec - posloupnost písmen, řádka - číslo). Například A5, $A5, $A$5, A$5.
- **Rozmezí buněk (range)** - obdélníkové pole buněk, pozice jsou dané levým horním a pravým dolním rohem, oddělovačem je znak dvojtečka (např. A5:X17).
- **Funkce** - volání funkcí a předání parametrů. Implementace požaduje následující funkce:
  - `sum(range)` - sečte hodnoty všech buněk, která se vyhodnotí jako číslo.
  - `count(range)` - spočte všechny buňky, které mají definovanou hodnotu.
  - `min(range)` - projde hodnoty všech buněk, která se vyhodnotí jako číslo a vrátí nejmenší nalezené číslo.
  - `max(range)` - projde hodnoty všech buněk, která se vyhodnotí jako číslo a vrátí největší nalezené číslo.
  - `countval(value, range)` - spočítá buňky, které mají hodnotu stejnou jako vyhodnocený výraz value.
  - `if(cond, ifTrue, ifFalse)` - vyhodnocuje podmínku a vrací ifTrue nebo ifFalse.

### Operátory

- **Binární operátory**: `^`, `*`, `/`, `-` (umocňování, násobení, dělení a odčítání).
- **Operátor +**: sčítání/zřetězení.
- **Unární operátor**: ke změně znaménka.
- **Relační operátory**: `<`, `<=`, `>`, `>=`, `<>`, `=`.

### Priorita operátorů

Pořadí od nejvyšší priority:

1. Volání funkce,
2. Umocnění `^`, levá asociativita,
3. Unární `-`, pravá asociativita,
4. Násobení `*` a dělení `/`, levá asociativita,
5. Sčítání/zřetězení `+` a odčítání `-`, levá asociativita,
6. Relační operátory `<`, `<=`, `>`, `>=`, levá asociativita,
7. Operátory rovnost/nerovnost `<>` a `=`, levá asociativita.

## Třída CPos

Třída reprezentuje identifikátor buňky. Testovací prostředí používá pouze konstruktor této třídy, kterému předá řetězec s názvem buňky (např. "A7"). Implementace tento řetězec zpracuje, zkontroluje a uloží do členských proměnných. Pokud konstruktor dostane neplatný identifikátor buňky, vyhodí výjimku `std::invalid_argument`. Vnitřní realizace třídy a její další rozhraní je na Vás.

## Funkce parseExpression() a třída CExpressionBuilder

Funkce `parseExpression(expr, builder)` představuje rozhraní dodaného analyzátoru výrazů. Parametrem volání je řetězec s výrazem `expr` a instance `builder`, jejíž rozhraní bude voláno během analýzy výrazu. Funkce nevrací žádnou hodnotu, zpracovaný výraz si bude v sobě ukládat dodaná instance builder. Funkce výraz buď zpracuje bez chyb, nebo vyhodí výjimku `std::invalid_argument` s popisem chyby. Pro správné použití funkce je nutné dodat vlastní builder.

## Abstraktní třída CExpressionBuilder

- `opAdd()` - aplikace operátoru sčítání/zřetězení (+).
- `opSub()` - aplikace operátoru odčítání (-).
- `opMul()` - aplikace operátoru násobení (\*).
- `opDiv()` - aplikace operátoru dělení (/).
- `opPow()` - aplikace operátoru umocnění (^).
- `opNeg()` - aplikace operátoru unární mínus (-).
- `opEq()` - aplikace operátoru porovnání na rovnost (=).
- `opNe()` - aplikace operátoru porovnání na nerovnost (<>).
- `opLt()` - aplikace operátoru porovnání menší než (<).
- `opLe()` - aplikace operátoru porovnání menší nebo rovno (<=).
- `opGt()` - aplikace operátoru porovnání větší než (>).
- `opGe()` - aplikace operátoru porovnání větší nebo rovno (>=).
- `valNumber(num)` - vyhodnotí číslo a aplikuje ho.
- `valString(str)` - vyhodnotí řetězec a aplikuje ho.
- `cellRef(cell)` - aplikace odkazu na buňku.
- `rangeRef(range)` - aplikace odkazu na rozsah buněk.
- `funcCall(fnName, paramCnt)` - aplikace volání funkce.

## Třída CSpreadsheet

### Konstruktor

Konstruktor třídy CSpreadsheet vytvoří prázdnou tabulku.

### Kopírovaní/přesouvací konstruktor/operátor=

Implementace se zaměří na správné kopírovací operace a převod celého obsahu.

### Metody

- `save(os)` - Uloží obsah tabulky do zadaného výstupního streamu.
- `load(is)` - Načte obsah tabulky z příslušného vstupního streamu.
- `setCell(pos, contents)` - Uloží obsah do určené buňky.
- `getValue(pos)` - Vypočítá a vrátí hodnotu z určené buňky.
- `copyRect(dstCell, srcCell, w, h)` - Zkopíruje čtverec buněk z oblasti.
- `capabilities()` - Vrátí vlastnosti implementace.

## Hodnocení

Hodnocení se zaměří na:

- Funkčnost Vaší implementace.
- Vhodnost návrhu tříd.
- Použití polymorfismu.
- Efektivní použití verzovacího systému.
