# Výsledková listina

Úkolem je realizovat šablonu třídy, která dokáže sestavovat výsledkové listiny v závislosti na zadaných vstupech - výsledcích zápasů dvojic soupeřů.

Předpokládáme, že máme několik soupeřů. V jednom zápase se utká dvojice soupeřů, výsledek zápasu je nějak popsán (například booleovskou hodnotou, dvojicí počtu bodů, dvojicí časů, ...). Protože chceme mít třídu obecnou, bude výsledek zápasu reprezentován jako generický datový typ. Takto bude do třídy vloženo několik výsledků pro zadané dvojice soupeřů (soupeři jsou jednoznačně identifikováni jménem - řetězcem). Následně chceme zjistit, zda na základě zadaných výsledků lze sestavit jednoznačnou výsledkovou listinu a případně tuto listinu i vypočítat. Trikem je, že při sestavování výsledku chceme vycházet pouze z informací o vítězi/poraženém ze zadaných zápasů.

Výsledková listina je určena jednoznačně, pokud pro každého soupeře _Xi_ na pozici _i_ platí:

- _Xi_ porazil všechny soupeře _Xj_, _i+1 ≤ j ≤ n_. Soupeře porazil buď přímo, nebo zprostředkovaně (_Xi_ porazil _Xa_, _Xa_ porazil _Xb_, ..., _Xz_ porazil _Xj_, _i+1 ≤ a < b < ... < j ≤ n_).
- _Xi_ nevyhrál ani neremizoval se žádným soupeřem _Xk_, _0 ≤ k ≤ i-1_.

Vezměme příklad, kdy bylo zadáno, že _A_ porazí _B_ a _B_ porazí _C_. Pak je zřejmě správná výsledková listina _A B C_. Pokud by ale bylo zadáno, že:

- _A_ porazí _B_
- _B_ porazí _C_
- _C_ porazí _A_

případně:

- _A_ porazí _B_
- _A_ porazí _C_

případně:

- _A_ porazí _B_
- _B_ remizuje s _C_

pak výsledkovou listinu nelze jednoznačně určit (museli bychom dodat informace o dalších zápasech nebo pořadí stanovit i na základě dalších kritérií, to v této úloze nebudeme dělat).

Bylo zmíněno, že zápas je zadaný jako trojice (_soupeř1_, _soupeř2_, _výsledek_), kde _výsledek_ je nějaký datový typ popisující například skóre, časy nebo cokoliv jiného. Výsledek je potřeba převést na informaci o vítězi/remíze/poraženém, to bude mít na starosti porovnávač (komparátor) předaný metodě pro určení výsledkové listiny. Porovnávač je funkce, funktor nebo lambda-výraz, který pro daný výsledek vrací návratovou hodnotu:

- zápornou, pokud _soupeř1_ prohrál,
- nulovou, pokud _soupeř1_ a _soupeř2_ remizovali,
- kladnou, pokud _soupeř1_ vyhrál.

Celkové rozhraní šablony třídy `CContest` bude:

- **Šablona je parametrizovaná generickým parametrem `M_`**: tento typ popisuje výsledek zápasu. Pro datový typ `M_` je garantováno, že je přesouvatelný, kopírovatelný a zrušitelný (`CopyConstructible`, `MoveConstructible`, `CopyAssignable`, `MoveAssignable` a `Destructible`). Další vlastnosti typu nejsou garantované, speciálně, není garantované, že typ `M_` má implicitní konstruktor (nemusí být `DefaultConstructible`).

### Metody třídy `CContest`

- **Implicitní konstruktor**: vytvoří prázdnou instanci `CContest`.

- **`addMatch(contestant1, contestant2, result)`**: Metoda přidá výsledek mezi soupeřem _contestant1_ a _contestant2_. Soupeři jsou zadaní v podobě řetězců. Výsledek _result_ je datového typu `M_`. Metoda si uloží informaci o tomto zápasu do nějakého úložiště ve své instanci. Volání `addMatch` lze řetězit. Metoda vyvolá výjimku `std::logic_error`, pokud byl výsledek zápasu mezi _contestant1_ a _contestant2_ již dříve zadán.

- **`isOrdered(comparator)`**: Metoda rozhodne, zda vyplněné výsledky zápasů vedou k jednoznačné výsledkové listině nebo ne. Návratovou hodnotou je `true` pokud je výsledková listina jednoznačná, `false` pokud ne. Parametrem volání je `comparator` - funkce, funktor nebo lambda výraz, který bude volán při získávání výsledku zápasu (převede `M_` na záporné číslo / nulu / kladné číslo podle popisu výše). Metoda nesmí modifikovat instanci `CContest` a nesmí házet výjimky.

- **`results(comparator)`**: Metoda vypočte výsledkovou listinu. Výsledkem je STL seznam (`std::list`) řetězců se jmény soupeřů v pořadí od prvního (celkového vítěze) k poslednímu. Pokud nelze výsledkovou listinu sestavit (není jednoznačná), metoda vyhodí výjimku `std::logic_error`. Parametr `comparator` má stejný význam jako u metody `isOrdered`.
