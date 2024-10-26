VELKÁ ČÍSLA

Úkolem je realizovat třídu CBigInt, která bude reprezentovat celá čísla (kladná i záporná) s (téměř) neomezeným rozsahem.

Celá čísla typu int, long long int, ... mají fixní velikost, tedy omezený rozsah. Pokud potřebujeme výpočty ve větším rozsahu hodnot, musíme si pro ně vytvořit vlastní datový typ. Třída implementující tento datový typ bude číslo ukládat ve vnitřní reprezentaci, kterou bude podle potřeby natahovat (alokovat větší prostor). Vaším úkolem je takovou třídu realizovat. Pro zjednodušení implementace jsou následující omezení:

Ukládáme pouze celá čísla (kladná, nulu i záporná). Nezabýváme se desetinnou částí.
Z matematických operací implementujeme pouze sčítání, násobení a porovnávání.
Realizovaná třída tedy musí splňovat následující rozhraní:

konstruktor implicitní
inicializuje objekt, který bude reprezentovat hodnotu 0,
konstruktor s parametrem celého čísla int
inicializuje objekt, reprezentující toto číslo,
konstruktor s parametrem řetězce (ASCIIZ)
inicializuje objekt s hodnotou, jejíž desítková reprezentace je v předávaném řetězci. Pokud je zadaný řetězec neplatný (neobsahuje platné desítkové číslo), konstruktor vyhodí výjimku std::invalid_argument. Výjimka je součástí standardní knihovny, její deklarace je v hlavičkovém souboru stdexcept. Při vyhazování výjimky std::invalid_argument lze jejímu konstruktoru předat řetězec s podrobnějším popisem příčiny chyby, pro tuto úlohu není obsah tohoto řetězce omezen,
kopírující konstruktor
bude implementován, pokud to vnitřní struktury Vaší třídy vyžadují,
destruktor
bude implementován, pokud to vnitřní struktury Vaší třídy vyžadují,
přetížený operátor =
bude umožňovat přiřazení z celého čísla, řetězce a jiné instance CBigInt,
operátor <<
bude umožňovat výstup objektu do C++ streamu. V povinných testech se požaduje výstup v desítkové reprezentaci bez zbytečných úvodních nul. V bonusových testech operátor dále musí spolupracovat se streamem a podle použitého manipulátoru zobrazit výsledek buď desítkově nebo hexadecimálně (osmičkový výpis se netestuje). Hexadecimální výpis používá malá písmena a nezobrazuje zbytečné úvodní nuly.
operátor >>
bude umožňovat načtení ze vstupního streamu (vstup bude v desítkovém zápisu). Čtení se bude chovat stejně jako načítání celých čísel ve standardní knihovně, tedy zastaví na prvním znaku, který již nemůže být platnou součástí čteného čísla.
operátor +
umožní sečíst dvě čísla typu:
CBigInt + CBigInt,
CBigInt + int,
CBigInt + ASCIIZ řetězec,
int + CBigInt a
ASCIIZ řetězec + CBigInt.
operátor +=
umožní k číslu CBigInt přičíst jiné číslo CBigInt, celé číslo nebo číslo v podobě ASCIIZ řetězce.
operátor _
umožní vynásobit dvě čísla ve stejných kombinacích zápisu jako operátor pro sčítání.
operátor _=
umožní číslo typu CBigInt přenásobit jiným číslem CBigInt, celým číslem nebo číslem v podobě ASCIIZ řetězce.
relační operátory (< <=, > >=, == a !=)
umožní porovnávat velká čísla mezi sebou, porovnání si opět musí poradit se všemi kombinacemi jako sčítání a násobení.
