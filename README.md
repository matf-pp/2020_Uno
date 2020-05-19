# UNO!

### Opis teme
UNO! je kartaška igra slična Mau Mau, sa posebno odštampanim kartama i specifičnim pravilima. Predviđena je za 2-10 igrača. Ideja projekta je simulacija uzastopnih partija između četiri igrača.

#### Pravila igre
*Cilj:* Svi igrači počinju partiju sa po 6 karata u ruci. Cilj jedne partije je izbaciti sve karte iz ruke. Igrač koji to prvi uradi je pobednik, i osvaja poene u zavisnosti od toga koliko i kojih karata je ostalo u rukama ostalih igrača. Igrač koji prvi skupi 500 poena je pobednik celokupne igre.

*Koncept partije:* Kao što je već rečeno, igrači počinju sa po 6 karata u ruci. Svaki igrač ima svoj potez u kojem on može ali ne mora da odigra neku kartu. Kada igrač odigra neku kartu, on je izbacuje na talon, koga čini špil karata okrenutih nagore. Karta koja je na vrhu talona određuje koju kartu igrač može baciti u svom potezu. Postoji i špil za izvlačenje sa kog igrač vuče ako, recimo, nema šta da odigra u svom potezu. Karte imaju specijalne efekte koji drastično utiču na tok partije.

*Karte:* UNO! karte su specifične i čine srž ove igre. Postoje 4 boje karata: crvena, žuta, zelena i plava. Karte koje su u nekoj od tih boja mogu biti brojevi, "+2", "Skip" ili "Reverse" karte.
- Brojevi su karte obeležene nekom od cifara 0-9 i nemaju specijalan efekat.
- Kada igrač odigra "+2" kartu, sledeći igrač preskače svoj potez i vuče dve karte.
- Kada igrač odigra "Skip" kartu, sledeći igrač preskače svoj potez.
- Kada igrač odigra "Reverse" kartu, redosled u kojem se igra se obrće (iz smera kazaljke na satu u obrnuti ili suprotno).

Postoje i tzv. *džokeri*: "Wild" i "Wild +4" karte. Ove karte su crne boje, i imaju drugačije efekte:
- Kada igrač odigra "Wild" kartu, on menja boju karte na talonu u neku od prethodno navedene četiri.
- "Wild +4" karta ima isti efekat kao "Wild" karta, uz dodatan efekat da sledeći igrač vuče 4 karte i preskače potez.

Sve karte imaju svoju vrednost u poenima koja odgovara jačini njihovog efekta:
- Karta sa brojem vredi onoliko koliko iznosi taj broj (npr. karta "3" vredi 3 poena).
- "+2", "Skip" i "Reverse" karte vrede 20 poena
- "Wild" i "Wild +4" karte vrede 50 poena

*Potez:* U svakom potezu igrač na koga je došao red treba da odigra neku kartu. Igrač može da izbaci kartu iz ruke ukoliko se ona poklapa po boji ili znaku sa kartom na talonu, ili je u pitanju crna karta (koju uvek može da odigra). Ako igrač ima neku validnu kartu u ruci i odluči se da je izbaci, prvo se izvrši odgovarajući efekat te karte, a zatim igra sledeći igrač po redu. Ako igrač nema kartu koju može da odigra, ili eventualno ima, ali želi da je zadrži u ruci, on mora da vuče jednu kartu. U tom slučaju on može da odigra samo tu kartu (ukoliko želi i ukoliko to pravila dozvoljavaju).

#### Naša zamisao projekta

Ideja projekta jeste napraviti simulaciju partija između 4 igrača koji igraju relativno pametno.
"UNO!" nije igra u kojoj se može tačno odrediti koji je najbolji potez u datom trenutku. Stoga, potrebno je od svih različitih ideja za strategije opredeliti se za kombinaciju onih koje najviše utiču na pozitivan ishod konkretnog igrača. Iako nije lako odrediti ovu kombinaciju, mi smo se opredelili za neke konkretne strategije i načine razmišljanja koje smo smatrali prirodnim.

Strategije koje smo implementirali su sledeće:
- S obzirom na to da karte koje vrede više daju više poena protivniku u slučaju poraza, igrač, ukoliko se opredelio da igra broj, igraće što je moguće veći broj. Drugim rečima, igrač uvek igra prvo veće brojeve, pa onda manje.
- S tim u vezi, igrač će pokušati da prvo izbaci karte sa većim vrednostima pre onih sa manjim.
- S druge strane, ako je moguće, pokušaće da sačuva karte sa najboljim efektima za kraj partije.
- Ako igrač menja boju, uzeće u razmatranje:
  - koliko karata ima u svakoj od boja;
  - koliko poena ima u svakoj od boja;
  - koliko efekata ima u svakoj od boja.
- Igrač će uvek posmatrati koliko karata u ruci imaju ostali igrači i regulisati svoj način igranja prema tome.
- Igrač će analizirati koliko je i on sam blizu pobede, pa će odgovarajuće igrati.

Ideja je da korisnik učestvuje kao posmatrač, odnosno on nije jedan od igrača, već posmatra uzastupne partije i ima pregled toka cele igre. Dakle, njegov uticaj na izvršavanje programa je minimalan.

### Programski jezik
Projekat je napisan u celosti u programskom jeziku C++.

### Potrebni alati
Za prevođenje izvornog koda potreban je C++ kompilator g++, a za pokretanje izvršnog fajla nisu potrebni nikakvi dodatni alati.

### Prevođenje i pokretanje programa
Kod se prevodi kompilatorom g++, koristeći C++ standard 11:

<code>g++ test.cpp -std=c++11 -o UNO</code>

Izvršni fajl se pokreće iz terminala:

<code>./UNO</code>

### Operativni sistem
Program je namenjen za izvršavanje na operativnom sistemu Ubuntu.

### Autori
- Aleksa Trbović,  217/2017 aleksa.trbovic@gmail.com

- Robert Doža,  33/2017 dozarobert98@gmail.com
