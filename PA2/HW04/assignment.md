# Evidence Výpočetní Techniky

Úkolem je navrhnout a implementovat sadu tříd, které budou simulovat evidenci počítačového vybavení firmy. Konkrétně budeme ukládat informace o sítích (`CNetwork`), počítačích (`CComputer`), jejich procesorech (`CCPU`), pamětech (`CMemory`) a discích (`CDisk`).

Úkol je zaměřen na návrh tříd, kde bude využito dědičnosti, polymorfismu a abstraktních metod. Pokud jsou tyto OOP prostředky použité racionálně, není implementace příliš dlouhá. Naopak, pokud provedete návrh špatně, bude se vám kód opakovat a implementační soubor bude velký. Zkuste identifikovat základní třídu a vhodně z ní děděním odvoďte podtřídy.

## Hlavní třídy a rozhraní

### 1. CNetwork

- Reprezentuje síť, která obsahuje počítače.
- **Konstruktor**: Přijímá název sítě.
- **Destruktor**, kopírovací konstruktor, operátor `=`.
- **Metoda** `addComputer`: Přidá počítač do sítě.
- **Metoda** `findComputer`: Vyhledá počítač podle jména, vrátí ukazatel na objekt nebo neplatný ukazatel, pokud není nalezen.
- **Operátor `<<`** pro výpis: Zobrazuje seznam počítačů a komponent v přidaném pořadí.

### 2. CComputer

- Reprezentuje jednotlivý počítač s přidělenými adresami a komponentami.
- **Konstruktor**: Přijímá jméno počítače.
- **Destruktor**, kopírovací konstruktor, operátor `=`.
- **Metoda** `addComponent`: Přidává komponentu do počítače.
- **Metoda** `addAddress`: Přidává adresu (řetězec) k počítači.
- **Operátor `<<`** pro výpis: Zobrazuje adresy a komponenty počítače. Nejprve adresy a následně komponenty, vše v pořadí přidávání.

### 3. CCPU

- Reprezentuje procesor (CPU).
- **Konstruktor**: Přijímá počet jader a frekvenci v MHz.
- **Destruktor**, kopírovací konstruktor, operátor `=`.

### 4. CMemory

- Reprezentuje paměť RAM.
- **Konstruktor**: Přijímá velikost paměti v MiB.
- **Destruktor**, kopírovací konstruktor, operátor `=`.

### 5. CDisk

- Reprezentuje úložiště.
- **Konstruktor**: Přijímá typ disku (symbolická konstanta `SSD` nebo `MAGNETIC`, deklarovaná v této třídě) a velikost v GiB.
- **Destruktor**, kopírovací konstruktor, operátor `=`.
- **Metoda** `addPartition`: Přidává parcely na disku se specifikací velikosti (GiB) a identifikace (řetězec). Parcely jsou zobrazovány v pořadí přidávání.
